#!/bin/bash

# Navigate to build directory
cd build || exit

# Build the project
make || exit

# Return to previous directory
cd ..

# Run the built executable
./build/ArtDisplay

