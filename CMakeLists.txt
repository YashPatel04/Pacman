cmake_minimum_required(VERSION 3.10)
project(PacMan)

set(CMAKE_CXX_STANDARD 14)

# Set the path to SFML
set(SFML_DIR "D:/PacMan/SFML/lib/cmake/SFML")

# Find SFML package
find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)

# Include directories
include_directories(
        ${SFML_INCLUDE_DIR}
        ${CMAKE_SOURCE_DIR}/src
)

# Add executable
add_executable(PacMan src/main.cpp)

# Link SFML libraries
target_link_libraries(PacMan sfml-system sfml-window sfml-graphics sfml-network sfml-audio)

