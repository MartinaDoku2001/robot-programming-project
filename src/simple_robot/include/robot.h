/* create the header file fo rthe robot element*/

#include "utils.h"

class Robot {
    public:
        // Constructor
        Robot(Position position, Velocity velocity);
        // Destructor
        ~Robot();
        // Move the robot
        void move(Position position);
        // Get the robot's position
        Position getPosition();
        // Get the robot's speed
        Velocity getVelocity();
    private:
        // The robot's position
        Position position;
        // The robot's speed
        Velocity velocity;
};