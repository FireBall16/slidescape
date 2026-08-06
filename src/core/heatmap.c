#include "common.h"
#include "heatmap.h"
#include "heatmap_colormap.h"
#include "renderer.h"

void init_heatmap(heatmap_t* heatmap, unsigned char* heatmap_data, unsigned int width_in_tiles, unsigned int height_in_tiles) {
	heatmap->apply_gradient_smoothing = false;

	set_heatmap_data(heatmap, heatmap_data, width_in_tiles, height_in_tiles);
	// Set Up Heatmap ColorMap
	reset_heatmap_colormap_default_values(&heatmap->heatmap_colormap, 0);
	generate_heatmap_color_lut(&heatmap->heatmap_colormap);
	renderer_set_heatmap_colormap_lut_texture(heatmap);
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

void update_heatmap_colors(heatmap_t* heatmap) {
	generate_heatmap_color_lut(&heatmap->heatmap_colormap);
	renderer_set_heatmap_colormap_lut_texture(heatmap);
	heatmap->heatmap_colormap.needs_update = false;
}
