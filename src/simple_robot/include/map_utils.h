#pragma once
#include <opencv2/opencv.hpp>
#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Geometry>

#include "grid.h"

using Canvas = cv::Mat; // we define the canvas as a cv matrix
using Eigen::Vector2i;


/* define drawing functions */
void drawGrid(Canvas& canvas, const Grid_<uint8_t>& src); 

void drawLine(Canvas& canvas, const Vector2i& start, const Vector2i& end, uint8_t color);

void drawCircle(Canvas& canvas, const Vector2i& center, int radius, uint8_t color);

int showCanvas( Canvas& canvas, int timeout_ms);



/* tools for mapping the grid */
struct GridMapping {

    Eigen::Transform<float, 2, Eigen::Affine> world_to_grid, grid_to_world; // transformation matrices
    Eigen::Matrix2f world_to_grid_d, grid_to_world_d;

    /*create two resize functions:  
    1- maps coordinates so that row/2 and col/2 are the center of world
    2- maps coordinates so that the upper left corner of the grid is at the given position
    */
    void resize(int rows, int cols, float resolution);

    void resize(float res, const Eigen::Vector2f& upper_left);

    inline Eigen::Vector2f world2grid(const Eigen::Vector2f& world_point)const {// get the grid coordinates of a world point
        return world_to_grid * world_point;
    }

    inline Eigen::Vector2f grid2world(const Eigen::Vector2f& grid_point)const {// get the world coordinates of a grid point
        return grid_to_world * grid_point;
    }

    inline const Eigen::Matrix2f& world2gridDerivative()const { //get the derivative of the world to grid transformation
        return world_to_grid_d;
    }

    inline const Eigen::Matrix2f& grid2worldDerivative()const { //get the derivative of the grid to world transformation
        return grid_to_world_d;
    }
};


