/* create the header file fo rthe robot element*/
#pragma once
#include <eigen3/Eigen/Dense>
#include <cmath>
#include <iostream>

class Robot {
public:
    // Constructor
    Robot(float x = 0.0, float y = 0.0);

    // Getter
    Eigen::Vector2f getPosition() const;

    // Setter
    void setPosition(float x, float y);

    // Function to move the robot
    void step(float angle, float step_size);

private:
    Eigen::Vector2f position_; // Position of the robot
};

