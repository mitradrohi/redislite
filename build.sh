#!/bin/bash

set -e

if [ "$#" -ne 2 ]; then
    echo "usage ./build.sh . build"
    exit 1
fi

SOURCE_DIR=$1
BUILD_DIR=$2

echo "📁 Source directory: $SOURCE_DIR"
echo "🏗️  Build directory: $BUILD_DIR"

mkdir -p $BUILD_DIR

# Run CMake configuration
echo "⚙️  Configuring with CMake..."
cmake -S "$SOURCE_DIR" -B "$BUILD_DIR"

# Build the project
echo "🔨 Building the project..."
cmake --build "$BUILD_DIR"

ctest

echo "✅ Build completed successfully."

