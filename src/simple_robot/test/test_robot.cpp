#include <iostream>
#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Geometry>
#include "../include/grid_map.h"
#include "../include/robot.h"


// Ensure DATA_DIR is defined
#define DATA_DIR "/home/martina/rp_project/robot-programming-project/src/simple_robot/data"
using namespace std;


int main() {
    std::string imagePath =  std::string(DATA_DIR) + "/map.png";
    float resolution = 0.1;
    
    GridMap grid_map(0,0, 0.1);
    grid_map.loadFromImage(imagePath, resolution);
    Canvas canvas;
    Robot robot(0.0, 0.0);

    while(1) {
        grid_map.draw(canvas);
        
        
        //update the robot position
        float angle = M_PI / 4; // 45 degrees in radians
        float step_size = 4.0;

        robot.step(angle, step_size);

        //print the new position of the robot
        std::cout << "Robot position: " << robot.getPosition().transpose() << std::endl;


        // Draw the robot on the canvas
        robot.draw(canvas, grid_map, 127, 10);
    
        // Display the canvas
        showCanvas(canvas, 0);
    }
    return 0;
}
