#include "common.h"
#include "heatmap.h"

void init_heatmap(heatmap_t* heatmap, unsigned char* heatmap_data, unsigned int width_in_tiles, unsigned int height_in_tiles) {
	heatmap->apply_gradient_smoothing = false;

	set_heatmap_data(heatmap, heatmap_data, width_in_tiles, height_in_tiles);
}

void init_test_heatmap(heatmap_t* heatmap) {
	static unsigned char test_data[] = {
		255, 125, 255,   0,   0,
		  0, 255,   0, 255, 125,
		255, 127, 255,   0, 255,
		  0, 255,   0, 255,   0,
		255,   0, 255,   0, 255,
	};
	init_heatmap(heatmap, test_data, 5, 5);
}

void set_heatmap_data(heatmap_t *heatmap, unsigned char *heatmap_data, unsigned int width_in_tiles, unsigned int height_in_tiles) {
	heatmap->heatmap_data = heatmap_data;
	heatmap->width_in_tiles = width_in_tiles;
	heatmap->height_in_tiles = height_in_tiles;
}

void set_heatmap_apply_gradient_smoothing(heatmap_t* heatmap, bool apply_gradient_smoothing) {
	heatmap->apply_gradient_smoothing = apply_gradient_smoothing;
}
