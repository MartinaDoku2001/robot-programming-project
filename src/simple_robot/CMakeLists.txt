cmake_minimum_required(VERSION 3.8)
project(simple_robot)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(OpenCV REQUIRED)

# include directories
include_directories(/usr/include/eigen3)
add_subdirectory(src)
add_subdirectory(include)
add_subdirectory(test)


ament_package()
