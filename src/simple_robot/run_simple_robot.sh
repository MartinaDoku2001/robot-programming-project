#!/usr/bin/env zsh


# Stop execution if any command fails
set -e  

# Build the package
echo "🔧 Building the simple_robot package..."
colcon build --packages-select simple_robot --cmake-clean-cache --event-handlers console_direct+

# Detect the shell and source the correct setup file
echo "🔄 Sourcing ROS2 setup..."
source install/setup.zsh
colcon build 

# Run the node
echo "🚀 Running the robot_map_node..."
ros2 run simple_robot robot_map_node

