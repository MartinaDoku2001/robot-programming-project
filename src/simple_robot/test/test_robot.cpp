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
    Robot robot(-40.0, -27.0);
    int key = -1;

    //update the robot position
    float angle = M_PI / 4; // 45 degrees in radians
    float step_size = 0.5;


    while(1) {
        grid_map.draw(canvas);
        
        
        
        switch (key) {
            case 82:  // Up Arrow
                angle = M_PI/2;
                break;
            case 84:   // Down Arrow
                angle = -M_PI/2; 
                break;
            case 81:   // Left Arrow
                angle = M_PI ; 
                break;
            case 83:  // Right Arrow
                angle = 0; 
                break;
            case -1:
                // No key pressed
                break;
            default:
                std::cout << "Other key: " << key << "\n";
        }

        robot.step(angle, step_size);

        //check if the new robot position is colliding with an obstacle
        float x = robot.getPosition()[0];
        float y = robot.getPosition()[1];
        std::cout << "Robot position: " << robot.getPosition().transpose() << std::endl;
        bool hit = grid_map.is_colliding(x,y);
        
        if (hit) {
            std::cout << "Robot is colliding with an obstacle\n";
            //move the robot back to the previous position
            robot.step(angle + M_PI, step_size);
        }


        // Draw the robot on the canvas
        robot.draw(canvas, grid_map, 127, 5);
    
        // Display the canvas
        key = showCanvas(canvas, 0);


    }
    return 0;
}
