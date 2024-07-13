/* create the header file fo rthe robot element*/
#pragma once
#include "map_utils.h"
#include "grid_map.h"
#include <eigen3/Eigen/Dense>
#include <cmath>
#include <iostream>

class Robot {
public:
    /**
     * @brief Constructor for the Robot class.
     * 
     * This constructor initializes the position of the robot.
     */
    
    Robot(float x = 0.0, float y = 0.0);

    // Getter
    Eigen::Vector2f getPosition() const;

    // Setter
    void setPosition(float x, float y);

    // Function to move the robot
    void step(float angle, float step_size);

    void draw(Canvas& canvas, const GridMap& map, uint8_t color, int radius);

private:
    Eigen::Vector2f position_; // Position of the robot
};

