#ifndef SLIDESCAPE_HEATMAP_H
#define SLIDESCAPE_HEATMAP_H

#include <stdbool.h>
#include "linmath.h"

typedef struct heatmap_t{
    unsigned int heatmap_texture;

    unsigned int width_in_tiles;
    unsigned int height_in_tiles;
    unsigned char *heatmap_data;

    bool apply_gradient_smoothing;
}heatmap_t;

void init_heatmap(heatmap_t* heatmap, unsigned char* heatmap_data, unsigned int width_in_tiles, unsigned int height_in_tiles);

void set_heatmap_data(heatmap_t* heatmap, unsigned char* heatmap_data, unsigned int width, unsigned int height);

//void set_heatmap_texture(unsigned char heatmap_data[], unsigned int width, unsigned int height);

void set_heatmap_projection_view_matrix(mat4x4 projection_view_matrix);

void set_heatmap_apply_gradient_smoothing(heatmap_t* heatmap, bool apply_gradient_smoothing);

#endif //SLIDESCAPE_HEATMAP_H