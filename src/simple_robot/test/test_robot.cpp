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
    int key = -1;

    //update the robot position
    float angle = M_PI / 4; // 45 degrees in radians
    float step_size = 4.0;


    while(1) {
        grid_map.draw(canvas);
        
        
        
        switch (key) {
            case 82: case 2490368:  // Up Arrow
                angle = M_PI/2;
                std::cout << "Up arrow pressed\n";
                break;
            case 84: case 2621440:  // Down Arrow
                angle = -M_PI/2; 
                std::cout << "Down arrow pressed\n";
                break;
            case 81: case 2424832:  // Left Arrow
                angle = M_PI ; 
                std::cout << "Left arrow pressed\n";
                break;
            case 83: case 2555904:  // Right Arrow
                angle = 0; // -90 degrees
                std::cout << "Right arrow pressed\n";
                break;
            case -1:
                // No key pressed
                break;
            default:
                std::cout << "Other key: " << key << "\n";
        }

        robot.step(angle, step_size);

        //print the new position of the robot
        std::cout << "Robot position: " << robot.getPosition().transpose() << std::endl;


        // Draw the robot on the canvas
        robot.draw(canvas, grid_map, 127, 10);
    
        // Display the canvas
        key = showCanvas(canvas, 0);

        std::cout << "key: " << key << std::endl;

    }
    return 0;
}
