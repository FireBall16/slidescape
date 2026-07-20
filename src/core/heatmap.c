#include "heatmap.h"
#include <stdbool.h>

#include "renderer.h"

void init_heatmap(heatmap_t* heatmap, unsigned char* heatmap_data, unsigned int width_in_tiles, unsigned int height_in_tiles) {
    heatmap->apply_gradient_smoothing = false;

    set_heatmap_data(heatmap, heatmap_data, width_in_tiles, height_in_tiles);
    renderer_set_heatmap_texture(heatmap);
}

void set_heatmap_data(heatmap_t *heatmap, unsigned char *heatmap_data, unsigned int width_in_tiles, unsigned int height_in_tiles) {
    heatmap->heatmap_data = heatmap_data;
    heatmap->width_in_tiles = width_in_tiles;
    heatmap->height_in_tiles = height_in_tiles;
}

void set_heatmap_apply_gradient_smoothing(heatmap_t* heatmap, bool apply_gradient_smoothing) {
    heatmap->apply_gradient_smoothing = apply_gradient_smoothing;
}