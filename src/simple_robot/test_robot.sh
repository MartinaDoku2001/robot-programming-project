#!/bin/bash

# Exit immediately if a command exits with a non-zero status
set -e

# Function to print messages with a timestamp
log() {
    echo "$(date +'%Y-%m-%d %H:%M:%S') - $1"
}

# Define the build directory and target
BUILD_DIR="/home/martina/rp_project/robot-programming-project/src/simple_robot/build"
CMAKE_BUILD_CMD="/usr/bin/cmake --build $BUILD_DIR --config Debug --target all --"

# Build the current repository using the specified CMake command
log "Building the repository..."
$CMAKE_BUILD_CMD || { log "Build failed"; exit 1; }

# Navigate into the build subdirectory
log "Navigating to the build subdirectory..."
cd "$BUILD_DIR" || { log "Failed to navigate to build directory"; exit 1; }

# Navigate into the test subdirectory
log "Navigating to the test subdirectory..."
cd test || { log "Failed to navigate to test directory"; exit 1; }

# Run the test executable
log "Running the test executable..."
./test_robot || { log "Test executable failed"; exit 1; }

log "Done!"


