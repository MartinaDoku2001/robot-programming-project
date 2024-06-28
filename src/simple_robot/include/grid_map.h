#pragma once
#include <cstdint>

#include "grid.h"
#include "map_utils.h"

using namespace std;

struct GridMap: public Grid_<uint8_t> {
    GridMapping mapping;
    float resolution; //meters * pixel

    GridMap(int rows, int cols, float resolution);

    void loadFromImage(std::string filename, float resolution);// load the map from an image file

    void draw(Canvas& canvas) const; // draw the map on a canvas
};