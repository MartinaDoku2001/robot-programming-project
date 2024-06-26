/* implementation of the code for robots */

#include "../include/robot.h"

// Constructor
Robot::Robot(Position position,  Velocity velocity) {
    this->position = position;
    this->velocity = velocity;
}

// Destructor
Robot::~Robot() {
}

// Move the robot
void Robot::move(Position position) {
    this->position = position;
}

// Get the robot's position
Position Robot::getPosition() {
    return this->position;
}

// Get the robot's speed
Velocity Robot::getVelocity() {
    return this->velocity;
}

