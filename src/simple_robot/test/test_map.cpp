#include <iostream>
#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Geometry>
#include "../include/grid_map.h"

using namespace std; 

int main() {
    GridMap map(100, 100, 0.1);
    map.loadFromImage("map.png", 0.1);
    Canvas canvas;
    drawGrid(canvas, map);
    showCanvas(canvas, 0);
    return 0;
}