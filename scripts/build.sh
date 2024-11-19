#!/bin/bash

# Check if the build directory exists
if [ -d "build" ]; then
  echo "The 'build' directory exists, removing it..."
  rm -rf build
fi

# Create a new build directory
echo "Creating the 'build' directory..."
mkdir build
cd build

# Run cmake and make to build the project
echo "Running cmake and make..."
cmake ..
make

# Copy the executable files to the parent directory
echo "Copying the executable files to the parent directory..."
cp game ../
cp runTests ../

# Go back to the parent directory
cd ..

echo "Script completed."
