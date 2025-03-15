cmake_minimum_required(VERSION 3.29)
project(SberGame)

set(CMAKE_CXX_STANDARD 20)

list(APPEND ${PROJECT_NAME}_HEADERS
        includes/interfaces/IHero.h,
        includes/interfaces/IAction.h,
        includes/interfaces/ISpecAction.h
)

list(APPEND ${PROJECT_NAME}_SOURCES

)

add_executable(SberGame main.cpp)
target_include_directories(SberGame PUBLIC includes)