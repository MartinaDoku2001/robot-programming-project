/* implementation of the code for robots */

#include "../include/robot.h"
#include "../include/map_utils.h"
#include "../include/grid_map.h"
#include <cmath>
#include <iostream>


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
    orientation_ = angle;
}

void Robot::draw(Canvas& canvas, const GridMap& map, uint8_t color, int radius) {
    // Draw the robot on the canvas
    drawCircle(canvas, map.gm.world2grid(position_).cast<int>(), radius, color);
}

