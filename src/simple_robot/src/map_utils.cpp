#include "../include/map_utils.h"
#include "../include/grid_map.h"
#include <iostream>
#include <Eigen/Geometry>
#include <thread>
#include <cstring>
#include <string.h>


/* code from github Repo: robotprogramming_2023_24*/

void drawGrid(Canvas& dest, const Grid_<uint8_t>& src) {
    dest = cv::Mat(src.rows, src.cols, CV_8UC1);
    // copy the data from the grid to the canvas
    dest.rows = src.rows;
    dest.cols = src.cols;
    memcpy(dest.data, &src.cells[0], src.rows *src.cols);
    
}

void drawLine(Canvas& dest, const Vector2i& start, const Vector2i& end, uint8_t color) {
    cv::line(dest, cv::Point(start.x(), start.y()), cv::Point(end.x(), end.y()), cv::Scalar(color,color,color),1);
}

void drawCircle(Canvas& dest, const Vector2i& center, int radius, uint8_t color) {
    cv::circle(dest, cv::Point(center.x(), center.y()), radius, cv::Scalar(color,color,color),1);
}

void drawCircle(Canvas& dest, const Vector2i& center, int radius, uint8_t color_red, uint8_t color_green, uint8_t color_blue) {
    // Convert to 3-channel if it's still grayscale
    if (dest.channels() == 1) {
        cv::cvtColor(dest, dest, cv::COLOR_GRAY2BGR);
    }
    
    // Draw the colored circle
    cv::circle(dest, cv::Point(center.x(), center.y()), radius, cv::Scalar(color_blue, color_green, color_red), -1);
}



int showCanvas(Canvas& canvas, int timeout_ms) {
    cv::imshow("Canvas", canvas);
    cv::setWindowProperty("Canvas", cv::WND_PROP_TOPMOST, 1);

    auto start_time = std::chrono::high_resolution_clock::now();
    int key = -1;

    while (std::chrono::duration_cast<std::chrono::milliseconds>(
               std::chrono::high_resolution_clock::now() - start_time)
               .count() < timeout_ms) {
        key = cv::pollKey(); // Non-blocking key check
        if (key != -1) {
            if (key == 27) { // Exit on ESC
                exit(0);
            }
            return key;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1)); // Small sleep to avoid CPU overuse
    }

    return -1; // No key pressed within timeout
}


void closeCanvas() {
    cv::destroyWindow("Canvas");
}

void refreshCanvas(Canvas& canvas) {
    cv::imshow("Canvas", canvas);
}

void displayValuesOnCanvas( Canvas& canvas, float distance, Eigen::Vector2f robot_position) {
    cv::putText(canvas, "Distance: " + std::to_string(distance), cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0,0,0), 1);
    cv::putText(canvas, "Robot Position: " + std::to_string(robot_position[0]) + ", " + std::to_string(robot_position[1]), cv::Point(10, 50), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0,0,0), 1);
}

void displayValuesOnCanvas(Canvas& canvas, float distance, Eigen::Vector2f robot_position, 
    Eigen::Vector2f goal_position, bool laser_active, bool goal_reached,  float time_elapsed) {
// Create a background rectangle
cv::rectangle(canvas, cv::Point(5, 5), cv::Point(350, 130), cv::Scalar(255, 255, 255), cv::FILLED);
cv::rectangle(canvas, cv::Point(5, 5), cv::Point(350, 130), cv::Scalar(0, 0, 0), 1);  // Border

// Display text with improved formatting
cv::putText(canvas, "Distance: " + std::to_string(distance) + "m", cv::Point(10, 20),
cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255), 1);

cv::putText(canvas, "Position: (" + std::to_string(robot_position[0]) + ", " + std::to_string(robot_position[1]) + ")", 
cv::Point(10, 40), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0), 1);

cv::putText(canvas, "Goal: (" + std::to_string(goal_position[0]) + ", " + std::to_string(goal_position[1]) + ")", 
cv::Point(10, 60), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 128, 255), 1);

std::string laser_status = laser_active ? "ON" : "OFF";
cv::putText(canvas, "Laser: " + laser_status, cv::Point(10, 80), 
cv::FONT_HERSHEY_SIMPLEX, 0.5, laser_active ? cv::Scalar(0, 255, 0) : cv::Scalar(0, 0, 255), 1);

cv::putText(canvas, "Time: " + std::to_string(time_elapsed) + "s", cv::Point(10, 100),
cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0), 1);

// Highlight goal reached message
if (goal_reached) {
cv::putText(canvas, "GOAL REACHED!", cv::Point(10, 125),
cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 255, 0), 2);
}
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
