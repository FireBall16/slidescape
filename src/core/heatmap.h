#ifndef SLIDESCAPE_HEATMAP_H
#define SLIDESCAPE_HEATMAP_H

#include <stdbool.h>

#include "heatmap_colormap.h"

// The heatmap is a 2d data structure/texture that is mapped to a single quad
// The heatmap consists of "tiles", where each texel is one tile.
// The data for these tiles is stored in heatmap_data, width_in_tiles and height_in_tiles determines how many tiles there are per row/column

// The actual heatmap (stored in heatmap_data) is a "grayscale"/single channel heatmap. It uses heatmap_colormap to associate colors to a given tile value.
// For each tile heatmap_data takes one byte/char to store a value between 0 and 255.
// Value 1 will be used as the lowest possible value and 255 as the highest. Heatmap attention scores will be mapped to these values.
// Value 0 is reserved as for empty tiles, and will not be given any color (tile will have RGBA value of [0,0,0,0])

// Heatmap data and its used colors (heatmap_colormap) can be updated separately

typedef struct heatmap_t {
    unsigned int heatmap_texture; // texture containing the (grayscale/single channel) heatmap

    unsigned int width_in_tiles;
    unsigned int height_in_tiles;
    unsigned char *heatmap_data;

    bool enable_heatmap;
    bool apply_gradient_smoothing;
    bool data_initialized; // whether the heatmap_data has been set before or not
    heatmap_colormap_t heatmap_colormap;
} heatmap_t;

void init_heatmap(heatmap_t* heatmap, unsigned char* heatmap_data, unsigned int width_in_tiles, unsigned int height_in_tiles);
void update_heatmap(heatmap_t* heatmap, unsigned char* heatmap_data, unsigned int width_in_tiles, unsigned int height_in_tiles);

void init_test_heatmap(heatmap_t* heatmap);

void set_heatmap_data(heatmap_t* heatmap, unsigned char* heatmap_data, unsigned int width, unsigned int height);
void set_enable_heatmap(heatmap_t* heatmap, bool enable_heatmap);
void set_heatmap_apply_gradient_smoothing(heatmap_t* heatmap, bool apply_gradient_smoothing);

void update_heatmap_colors(heatmap_t* heatmap);

#endif //SLIDESCAPE_HEATMAP_H
