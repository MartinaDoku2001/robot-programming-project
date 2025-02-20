cmake_minimum_required(VERSION 3.8)
project(simple_robot)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Find dependencies
find_package(OpenCV 4.5.4 REQUIRED)
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(Eigen3 REQUIRED)

# Include directories
include_directories(/usr/include/eigen3)
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(include)

# Add subdirectories
add_subdirectory(src)
add_subdirectory(test)

add_executable(robot_map_node  src/grid_map.cpp src/robot.cpp src/dmap.cpp src/map_utils.cpp src/main.cpp)
ament_target_dependencies(robot_map_node rclcpp Eigen3 OpenCV)

install(TARGETS robot_map_node
  DESTINATION lib/${PROJECT_NAME})

ament_package()

# Define the data directory
set(DATA_DIR "${CMAKE_SOURCE_DIR}/data")
add_definitions(-DDATA_DIR=\"${DATA_DIR}\")

# Print the data directory for debugging
message(STATUS "DATA_DIR: ${DATA_DIR}")