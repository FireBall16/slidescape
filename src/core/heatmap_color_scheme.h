#ifndef SLIDESCAPE_HEATMAP_COLOR_SCHEME_H
#define SLIDESCAPE_HEATMAP_COLOR_SCHEME_H
#include "heatmap_colormap.h"

typedef struct heatmap_color_scheme {
    const char* name;
    const color_stop_t* color_stops;
    size_t color_stops_count;
} heatmap_color_scheme;

extern const heatmap_color_scheme COLOR_SCHEMES[];

extern const size_t COLOR_SCHEME_COUNT;

#endif //SLIDESCAPE_HEATMAP_COLOR_SCHEME_H