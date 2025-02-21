# Create the library from the source files in src
add_library(grid_map SHARED
  grid_map.cpp
  map_utils.cpp
  robot.cpp
  dmap.cpp
  main.cpp
  # Add other source files here
)
find_package(OpenCV REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
ament_target_dependencies(grid_map rclcpp Eigen3 OpenCV)
# Include the directories

target_include_directories(grid_map PUBLIC
  ${CMAKE_SOURCE_DIR}/include
  ${OpenCV_INCLUDE_DIRS}
)

target_link_libraries(grid_map
  ${OpenCV_LIBS}
)