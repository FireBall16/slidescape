#ifndef SLIDESCAPE_HEATMAP_LOADER_H
#define SLIDESCAPE_HEATMAP_LOADER_H

#ifdef __cplusplus
extern "C" {
#endif
#include "heatmap.h"

void load_heatmap_from_JSON(heatmap_t* heatmap, const char *filename);

#ifdef __cplusplus
}
#endif

#endif //SLIDESCAPE_HEATMAP_LOADER_H