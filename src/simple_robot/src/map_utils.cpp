#include "../include/map_utils.h"
#include <Eigen/Geometry>

/* code from github Repo: robotprogramming_2023_24*/

void drawGrid(Canvas& dest, const Grid_<uint8_t>& src) {
    dest = cv::Mat(src.rows, src.cols, CV_8UC1);
    // copy the data from the grid to the canvas
    dest.rows = src.rows;
    dest.cols = src.cols;
    memcpy(dest.data, &src.cells[0], src.rows *src.cols);
    // print the dimensions of the canvas
    std::cout << "Canvas size after drawGrid: " << dest.rows << "x" << dest.cols << std::endl;

}

void drawLine(Canvas& dest, const Vector2i& start, const Vector2i& end, uint8_t color) {
    cv::line(dest, cv::Point(start.x(), start.y()), cv::Point(end.x(), end.y()), cv::Scalar(color,color,color),1);
}

void drawCircle(Canvas& dest, const Vector2i& center, int radius, uint8_t color) {
    cv::circle(dest, cv::Point(center.x(), center.y()), radius, cv::Scalar(color,color,color),1);
}

int showCanvas( Canvas& canvas, int timeout_ms) {
    std::cout << "Canvas size: " << canvas.rows << "x" << canvas.cols << std::endl;
    //show the image in a fixed size window size
    cv::imshow("Canvas", canvas);
    
    
    int key = cv::waitKey(timeout_ms);
    if (key == 27) { //exit on ESC
        exit(0);
    }
    return key;
}

void GridMapping::resize(int rows, int cols, float resolution) {
    world_to_grid.setIdentity(); 
    float inv_res = 1.0f / resolution;
    world_to_grid.linear() << inv_res, 0, 0, -inv_res; // set the linear part of the transformation
    world_to_grid.translation() << cols / 2, rows / 2; // set the translation part of the transformation
    grid_to_world = world_to_grid.inverse(); // compute the inverse transformation
    world_to_grid_d = world_to_grid.linear(); // set the derivative of the transformation
    grid_to_world_d = grid_to_world.linear(); // set the derivative of the inverse transformation
}

void GridMapping::resize(float res, const Eigen::Vector2f& upper_left) {
    grid_to_world.setIdentity(); 
    grid_to_world.linear() << res, 0, 0, -res; // set the linear part of the transformation
    grid_to_world.translation() << upper_left;
    world_to_grid = grid_to_world.inverse(); // compute the inverse transformation
    world_to_grid_d = world_to_grid.linear(); // set the derivative of the transformation
    grid_to_world_d = grid_to_world.linear(); // set the derivative of the inverse transformation
}
