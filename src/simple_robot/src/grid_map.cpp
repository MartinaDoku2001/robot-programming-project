
#include "grid_map.h"
#include "grid.h"
#include <map_utils.h>
using namespace std;



GridMap::GridMap(int rows, int cols, float resolution) : 
    Grid_<uint8_t>(rows, cols),
    resolution(resolution) {
    mapping.resize(rows, cols, resolution);
}

void GridMap::loadFromImage(const char* filename, float resolution) {
    cerr << "Loading [" << filename << "] with resolution " << resolution << endl;
    cv::Mat image = cv::imread(filename);
    if (image.rows == 0) {
        cerr << "Error: could not load image [" << filename << "]" << endl;
        exit(1);
    }
    cv::Mat loaded_image;
    cv::cvtColor(image, loaded_image, cv::COLOR_BGR2GRAY);
    int size = loaded_image.rows * loaded_image.cols;
    loaded_image.resize(loaded_image.rows, loaded_image.cols);
    mapping.resize(loaded_image.rows, loaded_image.cols, resolution);
    cerr << "transform:" << endl;
    cerr << mapping.world_to_grid.matrix() << endl;

    memcpy(&cells[0], loaded_image.data, size);
}

void GridMap::draw(Canvas& canvas) const {
    drawGrid(canvas, *this);
}
