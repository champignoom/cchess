cmake_minimum_required(VERSION 3.10)

# set the project name
project(Tutorial)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE Debug)
add_compile_options(-Wall -Wextra -pedantic)

# add the executable
add_executable(cchess main.cpp Board.cpp Piece.cpp Vector2d.cpp)
