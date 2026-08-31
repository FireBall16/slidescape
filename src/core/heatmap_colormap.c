#include "heatmap_colormap.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "heatmap_color_scheme.h"

static void set_color_lut_sequence(heatmap_colormap_t* heatmap_colormap, const color_stop_t* first_color_stop, const color_stop_t* second_color_stop);
static int compare_ids(const void* a, const void* b);
static int find_free_id(heatmap_colormap_t* heatmap_colormap);
static int find_color_stop_index_by_id(heatmap_colormap_t* heatmap_colormap, int id);

void add_color_stop(heatmap_colormap_t* heatmap_colormap, float red, float green, float blue, float alpha, float stop_point) {

    int free_id = find_free_id(heatmap_colormap);
    color_stop_t new_color_stop = { free_id, red, green,blue,alpha, stop_point };

    // Add extra memory
    size_t new_capacity = heatmap_colormap->color_stop_capacity;
    if (heatmap_colormap->color_stop_count >= heatmap_colormap->color_stop_capacity) {
        if (new_capacity == 0) {
            new_capacity = 1;
        }
        else {
            new_capacity *= 2;
        }
        void* temp = realloc(heatmap_colormap->color_stops, sizeof(color_stop_t) * new_capacity);
        if (temp == NULL) {
            console_print_error("add_color_stop(): failed to realloc memory\n");
            return;
        }
        heatmap_colormap->color_stops = (color_stop_t*)temp;
        heatmap_colormap->color_stop_capacity = new_capacity;
    }

    // Find insertion point
    int insert_point = 0;
    while (insert_point < heatmap_colormap->color_stop_count) {
        if (heatmap_colormap->color_stops[insert_point].stop_point <= new_color_stop.stop_point) {
            insert_point++;
        } else {
            break;
        }
    }

    // Move over endpoints with larger index than the insertion point
    if (insert_point < heatmap_colormap->color_stop_count) {
        memmove(
            heatmap_colormap->color_stops + insert_point + 1,
            heatmap_colormap->color_stops + insert_point,
            sizeof(color_stop_t) * (heatmap_colormap->color_stop_count - insert_point + 1)); // heatmap_colormap->color_stop_count  is not yet increased, thus +1
    }

    // Update heatmap_colormap members
    heatmap_colormap->color_stop_count++;
    heatmap_colormap->color_stops[insert_point] = new_color_stop;
    heatmap_colormap->needs_update = true;
};

void remove_color_stop_by_id(heatmap_colormap_t* heatmap_colormap, const int color_stop_id) {
    int color_stop_index = find_color_stop_index_by_id(heatmap_colormap, color_stop_id);
    if (color_stop_index == -1) {
        console_print_error("remove_color_stop_by_id(): failed to find color stop with given id: %d\n", color_stop_id);
        return;
    }

    if (color_stop_index < heatmap_colormap->color_stop_count && color_stop_index >= 0) {
        memmove(
            heatmap_colormap->color_stops + color_stop_index,
            heatmap_colormap->color_stops + color_stop_index + 1,
            sizeof(color_stop_t) * (heatmap_colormap->color_stop_count - color_stop_index));

        size_t new_capacity = heatmap_colormap->color_stop_capacity;
        if (new_capacity >= 2 * (heatmap_colormap->color_stop_count - 1)) {
            new_capacity = new_capacity / 2;
            void* temp = realloc(heatmap_colormap->color_stops, sizeof(color_stop_t) * new_capacity);
            if (temp == NULL) {
                console_print_error("remove_color_stop_by_id(): failed to realloc memory\n");
            }
            else {
                heatmap_colormap->color_stops = (color_stop_t*)temp;
                heatmap_colormap->color_stop_capacity = new_capacity;
            }
        }
        // Update heatmap_colormap members
        heatmap_colormap->color_stop_count--;
        heatmap_colormap->needs_update = true;
    }
}

void update_color_stop_by_id(heatmap_colormap_t* heatmap_colormap, int color_stop_id, float red, float green, float blue, float alpha, float stop_point) {
    int color_stop_index = find_color_stop_index_by_id(heatmap_colormap, color_stop_id);
    if (color_stop_index == -1) {
        console_print_error("update_color_stop_by_id(): failed to find color_stop with given id: %d\n", color_stop_id);
        return;
    }

    // check if color_stops need sorting when setting the new_color_stop values
    unsigned int insert_point = color_stop_index;
    color_stop_t new_color_stop = { heatmap_colormap->color_stops[color_stop_index].id, red, green, blue, alpha, stop_point };
    if (heatmap_colormap->color_stops[color_stop_index].stop_point != new_color_stop.stop_point) {
        color_stop_t old_color_stop = heatmap_colormap->color_stops[color_stop_index];

        if (new_color_stop.stop_point > old_color_stop.stop_point
            && color_stop_index < heatmap_colormap->color_stop_count - 1) {

            if (new_color_stop.stop_point > heatmap_colormap->color_stops[color_stop_index + 1].stop_point) {

                while (insert_point < heatmap_colormap->color_stop_count - 1) {
                    if (heatmap_colormap->color_stops[insert_point + 1].stop_point > new_color_stop.stop_point) {
                        break;
                    }
                    insert_point++;
                }

                if (insert_point < heatmap_colormap->color_stop_count) {
                    memmove(
                        heatmap_colormap->color_stops + color_stop_index,
                        heatmap_colormap->color_stops + color_stop_index + 1,
                        sizeof(color_stop_t) * (insert_point - color_stop_index)); // heatmap_colormap->color_stop_count  is not yet increased, thus +1
                }
            }
        }
        else if (new_color_stop.stop_point < old_color_stop.stop_point
            && color_stop_index > 0) {
            if (new_color_stop.stop_point < heatmap_colormap->color_stops[color_stop_index - 1].stop_point) {

                while (insert_point > 0) {
                    if (heatmap_colormap->color_stops[insert_point - 1].stop_point < new_color_stop.stop_point) {
                        break;
                    }
                    insert_point--;
                }
                memmove(
                    heatmap_colormap->color_stops + insert_point + 1,
                    heatmap_colormap->color_stops + insert_point,
                    sizeof(color_stop_t) * (color_stop_index - insert_point)); // heatmap_colormap->color_stop_count  is not yet increased, thus +1
            }
        }
    }
    heatmap_colormap->color_stops[insert_point] = new_color_stop;
    heatmap_colormap->needs_update = true;
}

void reset_heatmap_colormap_default_values(heatmap_colormap_t* heatmap_colormap, int color_scheme_id)
{
    if (color_scheme_id > COLOR_SCHEME_COUNT || color_scheme_id < 0) {
        console_print_error("reset_heatmap_colormap_default_values(): failed to find color_stop with given id: %d\n", color_scheme_id);
        return;
    }
    const heatmap_color_scheme new_color_scheme = COLOR_SCHEMES[color_scheme_id];

    size_t new_capacity = 1;
    while (new_capacity < new_color_scheme.color_stops_count) {
        new_capacity *= 2;
    }

    void* temp = realloc(heatmap_colormap->color_stops, sizeof(color_stop_t) * new_capacity);
    if (temp == NULL) {
        console_print_error("reset_heatmap_colormap_default_values(): failed to realloc memory\n");
        return;
    }
    heatmap_colormap->color_stop_capacity = new_capacity;
    heatmap_colormap->color_stops = (color_stop_t*)temp;
    heatmap_colormap->color_stop_count = new_color_scheme.color_stops_count;

    for (int i = 0; i < new_color_scheme.color_stops_count; i++) {
        heatmap_colormap->color_stops[i] = new_color_scheme.color_stops[i];
    }

    heatmap_colormap->needs_update = true;
}

void generate_heatmap_color_lut(heatmap_colormap_t* heatmap_colormap) {
    if (heatmap_colormap->color_stop_count <= 0) {
        console_print_error("generate_heatmap_color_lut(): heatmap_colormap has no color_stops (color_stop_count <= 0)\n");
        return;
    }

    if (heatmap_colormap->color_stops[0].stop_point > 0.0) {
        color_stop_t color_stop = heatmap_colormap->color_stops[0];
        color_stop.stop_point = 0.0f;
        set_color_lut_sequence(heatmap_colormap, &color_stop, &heatmap_colormap->color_stops[0]);
    }
    for (int i = 0; i < heatmap_colormap->color_stop_count - 1; i++) {
        set_color_lut_sequence(heatmap_colormap, &heatmap_colormap->color_stops[i], &heatmap_colormap->color_stops[i + 1]);
    }

    if (heatmap_colormap->color_stops[heatmap_colormap->color_stop_count - 1].stop_point < 1.0) {
        color_stop_t color_stop = heatmap_colormap->color_stops[heatmap_colormap->color_stop_count - 1];
        color_stop.stop_point = 1.0f;
        set_color_lut_sequence(heatmap_colormap, &heatmap_colormap->color_stops[heatmap_colormap->color_stop_count - 1], &color_stop);
    }
}

color_stop_t* get_color_stop_by_id(heatmap_colormap_t* heatmap_colormap, int id) {
    for (int i = 0; i < heatmap_colormap->color_stop_count; i++) {
        if (heatmap_colormap->color_stops[i].id == id) {
            return &heatmap_colormap->color_stops[i];
        }
    }
    return NULL;
}

void reset_color_stop_ids(heatmap_colormap_t* heatmap_colormap) {
    for (int i = 0; i < heatmap_colormap->color_stop_count; i++) {
        heatmap_colormap->color_stops[i].id = i;
    }
}

// Static helper functions
static void set_color_lut_sequence(heatmap_colormap_t* heatmap_colormap, const color_stop_t* first_color_stop, const color_stop_t* second_color_stop) {
    const size_t color_lut_entry_size = sizeof(heatmap_colormap->color_lut[0]);
    unsigned int const max_color_lut_index = (sizeof(heatmap_colormap->color_lut) / color_lut_entry_size) - 1;

    unsigned int first_color_lut_index = (unsigned int)roundf(first_color_stop->stop_point * (float)max_color_lut_index);
    unsigned int second_color_lut_index = (unsigned int)roundf(second_color_stop->stop_point * (float)max_color_lut_index);

    for (unsigned int lut_index = first_color_lut_index; lut_index <= second_color_lut_index; lut_index++) {
        float color_percentage = (float)(lut_index - first_color_lut_index) / (float)(second_color_lut_index - first_color_lut_index);

        for (unsigned int color_index = 0; color_index < color_lut_entry_size; color_index++) {
            heatmap_colormap->color_lut[lut_index][color_index] =
                (unsigned char)(
                    ((1.0f - color_percentage) * first_color_stop->color[color_index]) + (color_percentage * second_color_stop->color[color_index])
                    );
        }
    }

    // Some issues might arise when the color_stops have the same stop_point,
    // most prevalent at stop_point = 1.0f where the alpha will be 0 and thus the max values will be filtered;
    // This makes sure that the specific value will be set correctly
    if (first_color_lut_index == second_color_lut_index) {
        for (unsigned int color_index = 0; color_index < color_lut_entry_size; color_index++) {
            heatmap_colormap->color_lut[first_color_lut_index][color_index] =
                (unsigned char)(
                    first_color_stop->color[color_index]);
        }
    }
}

static int compare_ids(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

static int find_free_id(heatmap_colormap_t* heatmap_colormap) {
    int* ids = malloc(sizeof(int) * heatmap_colormap->color_stop_count);
    if (ids == NULL) {
        return -1;
    }

    for (int i = 0; i < heatmap_colormap->color_stop_count; i++) {
        ids[i] = heatmap_colormap->color_stops[i].id;
    }

    qsort(ids, heatmap_colormap->color_stop_count, sizeof(ids[0]), compare_ids);

    int free_id = 0;
    for (int i = 0; i < heatmap_colormap->color_stop_count; i++) {
        if (free_id == ids[i]) {
            free_id++;
        }
    }

    free(ids);
    return free_id;
}

static int find_color_stop_index_by_id(heatmap_colormap_t* heatmap_colormap, int id) {
    for (unsigned int i = 0; i < heatmap_colormap->color_stop_count; i++) {
        if (heatmap_colormap->color_stops[i].id == id) {
            return (int)i;
        }
    }
    return -1;
}