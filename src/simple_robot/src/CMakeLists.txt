# Create the library from the source files in src
add_library(grid_map SHARED
  grid_map.cpp
  map_utils.cpp
  robot.cpp
  # Add other source files here
)

target_include_directories(grid_map PUBLIC
  ${CMAKE_SOURCE_DIR}/include
  ${OpenCV_INCLUDE_DIRS}
)

target_link_libraries(grid_map
  ${OpenCV_LIBS}
)