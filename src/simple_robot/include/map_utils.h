#pragma once
#include <opencv4/opencv2/opencv.hpp>
#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Geometry>
#include "grid.h"

using Canvas = cv::Mat; // we define the canvas as a cv matrix
using Eigen::Vector2i;


/* define drawing functions */
void drawGrid(Canvas& canvas, const Grid_<uint8_t>& src); 

void drawLine(Canvas& canvas, const Vector2i& start, const Vector2i& end, uint8_t color);

void drawCircle(Canvas& canvas, const Vector2i& center, int radius, uint8_t color);

void drawCircle(Canvas& canvas, const Vector2i& center, int radius, uint8_t color_red, uint8_t color_green, uint8_t color_blue);

int showCanvas( Canvas& canvas, int timeout_ms);

void closeCanvas();

void refreshCanvas(Canvas& canvas);

/**
 * @brief RFunction to display the values on the canvas.
 * 
 * This function displays the values of the distance of the closest obstacle in the laser range (given as a parameter)
 * and the position of the robot on the canvas.
 * 
 * @param canvas The canvas to display the values on.
 * @param distance The distance of the closest obstacle in the laser range.
 * @param robot_position The position of the robot.
 * 
 */
void displayValuesOnCanvas( Canvas& canvas, float distance, Eigen::Vector2f robot_position);


/**
 * @brief RFunction to display the values on the canvas.
 * 
 * This function displays the values of the distance of the closest obstacle in the laser range (given as a parameter)
 * and the position of the robot on the canvas.
 * 
 * @param canvas The canvas to display the values on.
 * @param distance The distance of the closest obstacle in the laser range.
 * @param robot_position The position of the robot.
 * @param goal_position The position of the goal.
 * @param laser_active The state of the laser.
 * @param goal_reached The state of the goal.
 * @param time_elapsed The time elapsed since the start of the program.
 * 
 */
void displayValuesOnCanvas(Canvas& canvas, float distance, Eigen::Vector2f robot_position, 
    Eigen::Vector2f goal_position, bool laser_active, bool goal_reached,  float time_elapsed);

/**
 * @brief Represents a grid map.
 * 
 * GridMap is a class that represents a grid map and contains
 * the tools to work with it
 */
struct GridMapping {

    Eigen::Transform<float, 2, Eigen::Affine> world_to_grid, grid_to_world; // transformation matrices
    Eigen::Matrix2f world_to_grid_d, grid_to_world_d;

    /*create two resize functions:  
    1- maps coordinates so that row/2 and col/2 are the center of world
    2- maps coordinates so that the upper left corner of the grid is at the given position
    */

   /**
    * @brief Resizes the grid map to the given dimensions.
    * 
    * This function resizes the grid map to the given dimensions.
    * The center of the grid map is kept at the same position.
    * 
    * @param rows The new number of rows.
    * @param cols The new number of columns.
    * @param resolution The resolution of the grid map.
    */
    void resize(int rows, int cols, float resolution);


    /**
     * @brief Resizes the grid map to the given dimensions.
     * 
     * This function resizes the grid map to the given dimensions.
     * The upper left corner of the grid map is set to the given position.
     * 
     * @param res The resolution of the grid map.
     * @param upper_left The position of the upper left corner of the grid map.
     * 
     */
    void resize(float res, const Eigen::Vector2f& upper_left);


    /**
     * @brief Converts a world point to grid coordinates.
     * 
     * This function converts a world point to grid coordinates.
     * 
     * @param world_point The world point to convert.
     * 
     * @return The grid coordinates of the world point.
     */
    inline Eigen::Vector2f world2grid(const Eigen::Vector2f& world_point)const {// get the grid coordinates of a world point
        return world_to_grid * world_point;
    }

    /**
     * @brief Converts a grid point to world coordinates.
     * 
     * This function converts a grid point to world coordinates.
     * 
     * @param grid_point The grid point to convert.
     * 
     * @return The world coordinates of the grid point.
     */
    inline Eigen::Vector2f grid2world(const Eigen::Vector2f& grid_point)const {/
        return grid_to_world * grid_point;
    }


    /**
     * @brief Get the derivative of the world to grid transformation.
     * 
     */
    inline const Eigen::Matrix2f& world2gridDerivative()const { 
        return world_to_grid_d;
    }

    /**
     * @brief Get the derivative of the grid to world transformation.
     * 
     */
    inline const Eigen::Matrix2f& grid2worldDerivative()const { 
        return grid_to_world_d;
    }
};


