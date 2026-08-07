#include "common.h"
#include "heatmap.h"
#include "heatmap_colormap.h"
#include "renderer.h"

void init_heatmap(heatmap_t* heatmap, unsigned char* heatmap_data, unsigned int width_in_tiles, unsigned int height_in_tiles) {
	heatmap->enable_heatmap = false;
	heatmap->apply_gradient_smoothing = false;

	set_heatmap_data(heatmap, heatmap_data, width_in_tiles, height_in_tiles);
	// Set Up Heatmap ColorMap
	reset_heatmap_colormap_default_values(&heatmap->heatmap_colormap, 0);
	generate_heatmap_color_lut(&heatmap->heatmap_colormap);
	renderer_set_heatmap_colormap_lut_texture(heatmap);
}

void init_test_heatmap(heatmap_t* heatmap) {
	// static unsigned char test_data[] = {
	// 	255, 125, 255,   0,   0,
	// 	  0, 255,   0, 255, 125,
	// 	255, 127, 255,   0, 255,
	// 	  0, 255,   0, 255,   0,
	// 	255,   0, 255,   0, 255,
	// };
	// init_heatmap(heatmap, test_data, 5, 5);
	static unsigned char test_data_2[] =
	{ 135, 11, 215, 193, 190, 163, 89, 55, 163, 52, 57, 18, 252, 195, 201, 233, 131, 99, 20, 115, 195, 32, 46, 52, 253, 109, 202, 46, 13, 5, 230, 237, 92, 85, 191, 94, 55, 241, 160, 73, 218, 226, 180, 89, 73, 17, 73, 71, 53, 87, 216, 224, 108, 93, 155, 88, 248, 54, 134, 236, 152,
55, 205, 75, 124, 130, 246, 187, 178, 122, 211, 50, 215, 49, 178, 110, 184, 44, 224, 105, 9, 157, 189, 38, 192, 168, 189, 2, 68, 212, 125, 146, 177, 34, 149, 38, 57, 161, 31, 90, 194, 205, 61, 84, 128, 57, 194, 253, 137, 79, 243, 117, 172, 253, 205, 253, 199, 109, 179, 40, 144,
233, 26, 43, 20, 143, 191, 59, 73, 183, 76, 177, 6, 165, 150, 103, 70, 119, 36, 42, 19, 55, 114, 217, 71, 248, 28, 247, 198, 68, 225, 151, 88, 99, 147, 36, 154, 207, 60, 220, 234, 74, 179, 65, 169, 135, 84, 93, 27, 130, 237, 21, 68, 132, 92, 171, 51, 188, 251, 144, 235,
193, 47, 211, 209, 116, 4, 146, 255, 18, 24, 23, 112, 205, 52, 168, 14, 164, 50, 217, 176, 18, 194, 167, 244, 174, 101, 157, 82, 200, 147, 220, 67, 97, 86, 190, 196, 85, 74, 235, 0, 101, 185, 0, 219, 198, 83, 2, 207, 239, 32, 158, 231, 158, 43, 90, 53, 76, 14, 126, 239,
231, 1, 20, 151, 176, 5, 77, 142, 234, 98, 176, 118, 209, 210, 162, 154, 137, 122, 89, 222, 41, 11, 206, 110, 171, 42, 216, 179, 137, 29, 98, 85, 115, 216, 152, 118, 37, 95, 90, 232, 167, 110, 253, 91, 71, 103, 219, 158, 94, 85, 203, 107, 214, 157, 157, 97, 172, 153, 25, 37,
204, 103, 217, 215, 137, 175, 64, 128, 220, 128, 116, 82, 170, 36, 152, 52, 118, 249, 210, 218, 125, 237, 3, 0, 59, 196, 51, 164, 98, 196, 218, 86, 152, 169, 194, 57, 159, 25, 94, 232, 142, 158, 158, 63, 131, 27, 65, 202, 228, 14, 57, 151, 173, 236, 216, 193, 59, 224, 171, 6,
56, 191, 155, 121, 50, 234, 40, 88, 102, 170, 248, 128, 217, 122, 1, 134, 5, 124, 19, 249, 219, 45, 212, 106, 188, 244, 169, 125, 18, 165, 1, 153, 1, 199, 12, 181, 51, 82, 78,

	};
	init_heatmap(heatmap, test_data_2, 20, 20);
}

void set_heatmap_data(heatmap_t *heatmap, unsigned char *heatmap_data, unsigned int width_in_tiles, unsigned int height_in_tiles) {
	heatmap->heatmap_data = heatmap_data;
	heatmap->width_in_tiles = width_in_tiles;
	heatmap->height_in_tiles = height_in_tiles;
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
