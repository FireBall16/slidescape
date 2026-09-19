#include "common.h"
#include "heatmap.h"
#include "heatmap_colormap.h"
#include "renderer.h"


// It is allowed and likely to have heatmap_data == NULL with width_in_tiles == 0 and height_in_tiles == 0
// These variables can be used to directly initialize an actual heatmap on start or when having multiple scenes
void init_heatmap(heatmap_t* heatmap, unsigned char* heatmap_data, unsigned int width_in_tiles, unsigned int height_in_tiles) {
	heatmap->enable_heatmap = false;
	heatmap->apply_gradient_smoothing = false;

	set_heatmap_data(heatmap, heatmap_data, width_in_tiles, height_in_tiles);
	// Set Up Heatmap ColorMap
	reset_heatmap_colormap_default_values(&heatmap->heatmap_colormap, 0);
	generate_heatmap_color_lut(&heatmap->heatmap_colormap);
	renderer_set_heatmap_colormap_lut_texture(heatmap);
	heatmap->data_initialized = true;
}

void update_heatmap(heatmap_t* heatmap, unsigned char* heatmap_data, unsigned int width_in_tiles, unsigned int height_in_tiles) {
	set_heatmap_data(heatmap, heatmap_data, width_in_tiles, height_in_tiles);
	generate_heatmap_color_lut(&heatmap->heatmap_colormap);
	renderer_set_heatmap_texture(heatmap);
	renderer_set_heatmap_colormap_lut_texture(heatmap);
}

void set_heatmap_data(heatmap_t *heatmap, unsigned char *heatmap_data, unsigned int width_in_tiles, unsigned int height_in_tiles) {
	if (heatmap->heatmap_data != NULL && heatmap->data_initialized == true) {
		free(heatmap->heatmap_data);
		heatmap->heatmap_data = NULL;
	}

	heatmap->width_in_tiles = width_in_tiles;
	heatmap->height_in_tiles = height_in_tiles;
	heatmap->heatmap_data = heatmap_data;
}

void set_enable_heatmap(heatmap_t* heatmap, bool enable_heatmap) {
	heatmap->enable_heatmap = enable_heatmap;
}

void set_heatmap_apply_gradient_smoothing(heatmap_t* heatmap, bool apply_gradient_smoothing) {
	heatmap->apply_gradient_smoothing = apply_gradient_smoothing;
}

void update_heatmap_colors(heatmap_t* heatmap) {
	generate_heatmap_color_lut(&heatmap->heatmap_colormap);
	renderer_set_heatmap_colormap_lut_texture(heatmap);
	heatmap->heatmap_colormap.needs_update = false;
}
