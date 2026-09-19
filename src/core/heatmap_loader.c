#include "heatmap_loader.h"

#include <stdio.h>

#include "common.h"
#include "json.h"
#include "platform.h"

static int extract_json_string_from_file(const char* filename, char **result_buffer, long *result_length);
static int get_tile_width_height(struct json_value_s* json_root, int * width, int *height);
static int get_slide_width_height(struct json_value_s* json_root, int *width, int *height);
static struct json_object_element_s* find_json_field_by_name(struct json_value_s* json_root, const char *field_name);
static int build_heatmap_from_attention_json(struct json_value_s* json_root, int tile_width, int tile_height, int slide_width_in_tiles,
    unsigned char* heatmap_data);
static int get_values_from_attention_array_element(struct json_array_s* attention_entry_sub_array, int *x_coord, int *y_coord, float *normalized_attention_score);
static int parse_and_set_heatmap_JSON(heatmap_t *heatmap, char *buffer, long length);

static int extract_json_string_from_file(const char* filename, char **result_buffer, long *result_length) {
    mem_t* file = platform_read_entire_file(filename);
    if (!file) {
        printf ("ERROR: heatmap_loader: failed to load file\n");
        return -1;
    }

    *result_length = (long)file->len;
    *result_buffer = malloc(*result_length);
    memcpy(*result_buffer, file->data, *result_length);

    return 0;
}

// Note: only searches the current layer, will not look at nested layers
static struct json_object_element_s* find_json_field_by_name(struct json_value_s* json_root, const char *field_name) {
    struct json_object_s* root_object = (struct json_object_s*)json_root->payload;
    struct json_object_element_s* field= root_object->start;

    while (field != NULL) {
        struct json_string_s* element_name = field->name;
        if (strcmp(element_name->string, field_name) == 0) {
            return field;
        }
        field = field->next;
    }
    return NULL;
}

static int get_tile_width_height(struct json_value_s* json_root, int * width, int *height) {
    struct json_object_element_s* field_tile_definition = find_json_field_by_name(json_root, "tile_definition");
    if (field_tile_definition) {
        struct json_object_element_s* field_tile_width = find_json_field_by_name(field_tile_definition->value, "width");
        if (field_tile_width) {
            struct json_value_s* field_value = field_tile_width->value;
            struct json_number_s* field_number = (struct json_number_s*)field_value->payload;
            *width = strtol(field_number->number, NULL, 10);
        } else {
            printf("ERROR: heatmap_loader: field [width] not found in JSON\n");
        }
        struct json_object_element_s* field_tile_height = find_json_field_by_name(field_tile_definition->value, "height");
        if (field_tile_height) {
            struct json_value_s* field_value = field_tile_height->value;
            struct json_number_s* field_number = (struct json_number_s*)field_value->payload;
            *height = strtol(field_number->number, NULL, 10);
        } else {
            printf("ERROR: heatmap_loader: field [height] not found in JSON\n");
        }
    } else {
        printf("ERROR: heatmap_loader: field [tile_definition] not found in JSON\n");
        return -1;
    }
    return 0;
}

static int get_slide_width_height(struct json_value_s* json_root, int *width, int *height) {
    struct json_object_element_s* field_slide = find_json_field_by_name(json_root, "slide");
    if (!field_slide) {
        printf("ERROR: heatmap_loader: field [slide] not found in JSON\n");
        return -1;
    }
    struct json_object_element_s* field_slide_bounds = find_json_field_by_name(field_slide->value, "bounds");

    if (field_slide_bounds) {
        struct json_object_element_s* field_tile_width = find_json_field_by_name(field_slide_bounds->value, "width");
        if (field_tile_width) {
            struct json_value_s* field_value = field_tile_width->value;
            struct json_number_s* field_number = (struct json_number_s*)field_value->payload;
            *width = strtol(field_number->number, NULL, 10);
        } else {
            printf("ERROR: heatmap_loader: field [width] not found in JSON\n");
        }
        struct json_object_element_s* field_tile_height = find_json_field_by_name(field_slide_bounds->value, "height");
        if (field_tile_height) {
            struct json_value_s* field_value = field_tile_height->value;
            struct json_number_s* field_number = (struct json_number_s*)field_value->payload;
            *height = strtol(field_number->number, NULL, 10);
        } else {
            printf("ERROR: heatmap_loader: field [height] not found in JSON\n");
        }
    } else {
        printf("ERROR: heatmap_loader: field [bounds] not found in JSON\n");
        return -1;
    }
    return 0;
}

static int get_values_from_attention_array_element(struct json_array_s* attention_entry_sub_array, int *x_coord, int *y_coord, float *normalized_attention_score) {
    int element_counter = 0;
    struct json_array_element_s* sub_array_element = attention_entry_sub_array->start;

    struct json_number_s* sub_array_value_number = (struct json_number_s*)sub_array_element->value->payload;
    *x_coord = strtol(sub_array_value_number->number, NULL, 10);

    sub_array_element = sub_array_element->next;
    element_counter++;

    if (sub_array_element == NULL) {
        printf("ERROR: heatmap_loader: get_values_from_attention_array_element could not retrieve [y coordinate] from JSON\n");
        return -1;
    }
    sub_array_value_number = (struct json_number_s*)sub_array_element->value->payload;
    *y_coord = strtol(sub_array_value_number->number, NULL, 10);

    while (element_counter < 3) {
        sub_array_element = sub_array_element->next;
        element_counter++;
        if (sub_array_element == NULL) {
            printf("ERROR: heatmap_loader: get_values_from_attention_array_element could not [retrieve normalized attention score] from JSON\n");
            return -1;
        }
    }

    sub_array_value_number = (struct json_number_s*)sub_array_element->value->payload;
    *normalized_attention_score = strtof(sub_array_value_number->number, NULL);

    return 0;
}

static int build_heatmap_from_attention_json(struct json_value_s* json_root, int tile_width, int tile_height, int slide_width_in_tiles,
    unsigned char* heatmap_data) {
    if (heatmap_data == NULL) {
        printf("ERROR: heatmap_loader: heatmap_data is not initialized\n");
        return -1;
    }

    struct json_object_element_s* field_attention = find_json_field_by_name(json_root, "attention");
    if (!field_attention) {
        printf("ERROR: heatmap_loader: field [attention] not found in JSON\n");
        return -1;
    }
    struct json_array_s* field_attention_array = field_attention->value->payload;
    struct json_array_element_s* field_attention_array_element = field_attention_array->start;

    int target_tile_x = -1, target_tile_y = -1;
    float target_attention_score = -1.0f;

    int loop_counter = 0;
    while (field_attention_array_element != NULL) {
        const int status = get_values_from_attention_array_element(
            field_attention_array_element->value->payload,
            &target_tile_x, &target_tile_y, &target_attention_score);
        if (status != 0) {
            printf("ERROR: heatmap_loader: get_values_from_attention_array_element failed\n");
            return -1;
        }
        // int target_position = target_tile_x + target_tile_y * tile_width;
        heatmap_data[target_tile_x/tile_width + target_tile_y/tile_height * slide_width_in_tiles] = (char)roundf(target_attention_score * 254 + 1);
        field_attention_array_element = field_attention_array_element->next;
        loop_counter++;
    }
    printf("building heatmap: DONE, loops: %d\n", loop_counter);

    return 0;
}

// Parses the heatmap and sets the retrieved values for the passed heatmap
static int parse_and_set_heatmap_JSON(heatmap_t *heatmap, char *buffer, long length) {
    int tile_width, tile_height;
    int slide_width, slide_height;
    if (!buffer) {
        printf("ERROR: heatmap_loader: buffer is null\n");
        return -1;
    }
    // load metadata
    struct json_value_s* root = json_parse(buffer, length);

    int status = get_tile_width_height(root, &tile_width, &tile_height);
    if (status == 0) {
        printf("tile_definition width: %d, height: %d\n", tile_width, tile_height);
    } else {
        printf("ERROR: heatmap_loader: failed retrieving metadata\n");
        return -1;
    }

    status = get_slide_width_height(root, &slide_width, &slide_height);
    if (status == 0) {
        printf("slide width: %d, height: %d\n", slide_width, slide_height);
    } else {
        printf("ERROR: heatmap_loader: failed retrieving metadata\n");
        return -1;
    }

    // Load heatmap data
    if (tile_width <= 0 || tile_height <= 0 || slide_width <= 0 || slide_height <= 0) {
        printf("ERROR: heatmap_loader: slide or tile width too small\n");
        return -1;
    }

    int width_in_tiles = slide_width / tile_width;
    int height_in_tiles = slide_height / tile_height;

    unsigned char* heatmap_data = calloc(width_in_tiles * height_in_tiles, sizeof(unsigned char));
    if (heatmap_data == NULL) {
        printf("ERROR: heatmap_loader: calloc failed\n");
        return -1;
    }

    if (build_heatmap_from_attention_json(root, tile_width, tile_height, width_in_tiles, heatmap_data) != 0) {
        printf("ERROR: heatmap_loader: build_heatmap_from_attention_json failed\n");
        return -1;
    }
    update_heatmap(heatmap, heatmap_data, width_in_tiles, height_in_tiles);
    return 0;
}

int load_heatmap_from_JSON(heatmap_t* heatmap, const char *filename) {
    char *buffer;
    long length;

    if(extract_json_string_from_file(filename, &buffer, &length) != 0) {
        printf("ERROR: heatmap_loader: failed to extract data from JSON file\n");
        return -1;
    }
    if (parse_and_set_heatmap_JSON(heatmap, buffer, length) != 0) {
        printf("ERROR: heatmap_loader: failed to parse JSON file\n");
    }
    return 0;
}