cmake_minimum_required(VERSION 3.10)

project(EvoXO LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(game
    game/main.cpp
    common/tictactoe.cpp
)