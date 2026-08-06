#include "heatmap_color_scheme.h"

static const color_stop_t RED_BLUE_STOPS[] =
{
    { 0, 255.0f, 0.0f, 0.0f, 255.0f, 0.0f },
    { 1, 125.0f, 0.0f, 125.0f, 125.0f, 0.5f },
    { 2, 0.0f, 0.0f, 255.0f, 255.0f, 0.8f },
    { 3, 0.0f, 125.0f, 255.0f, 255.0f, 1.0f }
};

static const color_stop_t VIRIDIS_STOPS[] =
{
    {0, {68.0f, 1.0f, 84.0f, 255.0f}, 0.0f},
    {1, {33.0f, 145.0f, 141.0f, 255.0f}, 0.5f},
    {2, {253.0f, 231.0f, 37.0f, 255.0f}, 1.0f}
};

static const color_stop_t GRAYSCALE_STOPS[] =
{
    {0, {0.0f, 0.0f, 0.0f, 255.0f}, 0.0f},
    {1, {255.0f, 255.0f, 255.0f, 255.0f}, 1.0f}
};

static const color_stop_t HOT_STOPS[] =
{
    {0, {0.0f, 0.0f, 0.0f, 255.0f}, 0.0f},
    {1, {255.0f, 0.0f, 0.0f, 255.0f}, 0.3f},
    {2, {255.0f, 255.0f, 0.0f, 255.0f}, 0.6f},
    {3, {255.0f, 255.0f, 255.0f, 255.0f}, 1.0f}
};

const heatmap_color_scheme COLOR_SCHEMES[] =
{
    {
        "Red-Blue",
        RED_BLUE_STOPS,
        sizeof(RED_BLUE_STOPS) / sizeof(RED_BLUE_STOPS[0])
    },
    {
        "Viridis",
        VIRIDIS_STOPS,
        sizeof(VIRIDIS_STOPS) / sizeof(VIRIDIS_STOPS[0])
    },
    {
        "Grayscale",
        GRAYSCALE_STOPS,
        sizeof(GRAYSCALE_STOPS) / sizeof(GRAYSCALE_STOPS[0])
    },
    {
        "Hot",
        HOT_STOPS,
        sizeof(HOT_STOPS) / sizeof(HOT_STOPS[0])
    }
};

const size_t COLOR_SCHEME_COUNT = sizeof(COLOR_SCHEMES) / sizeof(COLOR_SCHEMES[0]);