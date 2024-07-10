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

    std::cout << "Initial position: (" << robot.getPosition()[0] << ", " << robot.getPosition()[1] << ")" << std::endl;

    float angle = M_PI / 4; // 45 degrees in radians
    float step_size = 1.0;

    robot.step(angle, step_size);

    std::cout << "New position: (" << robot.getPosition()[0] << ", " << robot.getPosition()[1] << ")" << std::endl;

    // Draw the robot on the canvas
    drawRobot(canvas, robot, 10, 255);

    // Display the canvas
    cv::imshow("Robot Canvas", canvas);
    cv::waitKey(0);

    return 0;
}

