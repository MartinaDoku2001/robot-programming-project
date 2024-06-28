# Create the executable for the main test program
add_executable(test_map
  test_map.cpp
)

target_include_directories(test_map PUBLIC
  ${CMAKE_SOURCE_DIR}/include
  /usr/include/eigen3
  ${OpenCV_INCLUDE_DIRS}
)

target_link_libraries(test_map
  grid_map
  ${OpenCV_LIBS}
)

target_compile_definitions(test_map PRIVATE DATA_DIR=\"${DATA_DIR}\")
