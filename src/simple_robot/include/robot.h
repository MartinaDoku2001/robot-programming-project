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
     * 
     * @param x The x-coordinate of the robot (world coordinates).
     * @param y The y-coordinate of the robot (world coordinates).
     */
    Robot(float x = 0.0, float y = 0.0);

    /**
     * @brief Getter for the position of the robot.
     * 
     * This function returns the position of the robot.
     * 
     * @return The position of the robot.
     */
    Eigen::Vector2f getPosition() const;

    /**
     * @brief Setter for the position of the robot.
     * 
     * This function sets the position of the robot.
     * 
     * @param x The x-coordinate of the robot (world coordinates).
     * @param y The y-coordinate of the robot (world coordinates).
     */
    void setPosition(float x, float y);

    /**
     * @brief Function to move the robot.
     * 
     * This function moves the robot based on the given angle and step size.
     * 
     * @param angle The angle to move the robot in radians.
     * @param step_size The step size to move the robot.
     */
    void step(float angle, float step_size);

    /**
     * @brief Function to draw the robot on the canvas.
     * 
     * This function draws the robot on the canvas.
     * 
     * @param canvas The canvas to draw on.
     * @param map The grid map to draw on.
     * @param color The color of the robot.
     * @param radius The radius of the robot.
     */
    void draw(Canvas& canvas, const GridMap& map, uint8_t color, int radius);

private:
    Eigen::Vector2f position_; // Position of the robot
    float orientation_; // Orientation of the robot
};

