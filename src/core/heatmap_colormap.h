#ifndef SLIDESCAPE_HEATMAP_COLORMAP_H
#define SLIDESCAPE_HEATMAP_COLORMAP_H
#include <stdbool.h>
#include <stddef.h>

// heatmap_colormap_t is used for defining colors associated with the grayscale/single channel values
// Multiple color_stop_t are used to generate a color gradient.
// The full color_gradient (colormap_lut_texture) will be built up by generating linear gradients between color_stops
// For example, between a color_stop with stop_point 0.2f and a color_stop with stop_point 0.4f 20% of the full gradient will be determined
// Color stops are dynamically adjustable and can be removed/add at will, but at least 2 stops should be present to properly generate the colormap
// If there is no color stop with stop_point 0.0f is present the color_stop with the smallest stop_point value will be used instead
// to determine the start value of the gradient. The same is true for the end (stop_point 1.0f and the largest stop_point value)

typedef struct color_stop_t {
    int id;           // Usable to track color_stops (E.G. for GUI), heatmap_colormap_t will normally automatically sort by stop_point
    float color[4];	  // RGBA, each value should be between 0.0f and 255.0f
    float stop_point; // value should be between 0.0f and 1.0f
} color_stop_t;

typedef struct heatmap_colormap_t {
    unsigned int colormap_lut_texture;

    color_stop_t* color_stops;
    size_t color_stop_count;
    size_t color_stop_capacity;

    unsigned char color_lut[255][4];

    bool needs_update; // If any changes have occurred to the color_stops, this value should be set to true
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