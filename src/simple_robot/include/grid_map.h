#pragma once
#include <cstdint>

#include "grid.h"
#include "map_utils.h"

struct GridMap: public Grid_<uint8_t> {
    GridMapping mapping;
    float resolution; //meters * pixel

    GridMap(int rows, int cols, float resolution);

    void loadFromImage(const char* filename, float resolution);// load the map from an image file

    void draw(Canvas& canvas) const; // draw the map on a canvas
};