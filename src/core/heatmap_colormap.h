#ifndef SLIDESCAPE_HEATMAP_COLORMAP_H
#define SLIDESCAPE_HEATMAP_COLORMAP_H
#include <stdbool.h>
#include <stddef.h>

typedef struct color_stop_t {
    int id;
    float color[4];	  // RGBA, each value should be between 0.0f and 255.0f
    float stop_point; // value should be between 0.0f and 1.0f
} color_stop_t;

typedef struct heatmap_colormap_t {
    unsigned int colormap_lut_texture;

    color_stop_t* color_stops;
    size_t color_stop_count;
    size_t color_stop_capacity;

    unsigned char color_lut[255][4];

    bool needs_update;
} heatmap_colormap_t;

// Function Prototypes
void add_color_stop(heatmap_colormap_t* heatmap_colormap, float red, float green, float blue, float alpha, float stop_point);
void remove_color_stop_by_id(heatmap_colormap_t* heatmap_colormap, int color_stop_id);
void update_color_stop_by_id(heatmap_colormap_t* heatmap_colormap, int color_stop_id, float red, float green, float blue, float alpha, float stop_point);

void reset_heatmap_colormap_default_values(heatmap_colormap_t* heatmap_colormap, int color_scheme_id);

void generate_heatmap_color_lut(heatmap_colormap_t* heatmap_colormap);

color_stop_t* get_color_stop_by_id(heatmap_colormap_t* heatmap_colormap, int id);
void reset_color_stop_ids(heatmap_colormap_t* heatmap_colormap);

#endif //SLIDESCAPE_HEATMAP_COLORMAP_H