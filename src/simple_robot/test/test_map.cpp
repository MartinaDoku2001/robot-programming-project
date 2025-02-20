#include <iostream>
#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Geometry>
#include "../include/grid_map.h"

// Ensure DATA_DIR is defined
#define DATA_DIR "/home/martina/rp_project/robot-programming-project/src/simple_robot/data"
using namespace std;

int main() {
    std::cout << "Starting the map testing" << std::endl;

    std::string imagePath =  std::string(DATA_DIR) + "/map.png";
    float resolution = 0.1;
    
    GridMap grid_map(0,0, 0.1);
    grid_map.loadFromImage(imagePath, resolution);
    Canvas canvas;
    Vector2f center(0.,0.);
    
    float alpha=0;
    while(1) {
    grid_map.draw(canvas);
    Vector2f direction;
    direction[0]=cos(alpha);
    direction[1]=sin(alpha);
    Vector2f dest;
    bool hit = grid_map.scanRay(dest, center, direction, 30);
    cerr << "hit: " << hit << endl;
    cerr << "origin: " << grid_map.gm.world2grid(center) << endl;
    cerr << "endpoint: " << grid_map.gm.world2grid(dest) << endl;
    drawLine(canvas,
             grid_map.gm.world2grid(center).cast<int>(),
             grid_map.gm.world2grid(dest).cast<int>(), 127);
    drawCircle(canvas, grid_map.gm.world2grid(center).cast<int>(), 3, 127);
    
    showCanvas(canvas, 0);
    
    alpha+=0.01;
    cerr << "alpha: " << alpha << endl;
  }
}






