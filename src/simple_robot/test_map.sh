
#!/bin/bash
set -e
log() {
    echo "$(date +'%Y-%m-%d %H:%M:%S') - $1"
}

BUILD_DIR="/home/martina/rp_project/robot-programming-project/src/simple_robot/build"
# Define the build directory and target
CMAKE_BUILD_CMD="/usr/bin/cmake --build $BUILD_DIR --config Debug --target all --"

# Build the current repository using the specified CMake command
log "Building the repository..."
$CMAKE_BUILD_CMD || { log "Build failed"; exit 1; }

# Ensure the binary exists
if [ ! -f "$BUILD_DIR/test/test_map" ]; then
    log "Error: test_map executable not found!"
    exit 1
fi

# Run the executable with output
log "Running test_map..."
"$BUILD_DIR/test/test_map" || { log "Test_map failed"; exit 1; }

log "Done!"
