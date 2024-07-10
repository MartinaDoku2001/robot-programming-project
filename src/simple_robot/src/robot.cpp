/* implementation of the code for robots */

#include "../include/robot.h"

// Constructor
Robot::Robot(float x, float y) : position_(x, y) {}

// Getter
Eigen::Vector2f Robot::getPosition() const {
    return position_;
}

// Setter
void Robot::setPosition(float x, float y) {
    position_[0] = x;
    position_[1] = y;
}

// Function to move the robot
void Robot::step(float angle, float step_size) {
    // Update the position based on the angle and step size
    position_[0] += step_size * cos(angle);
    position_[1] += step_size * sin(angle);
}

