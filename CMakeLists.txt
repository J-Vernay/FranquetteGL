cmake_minimum_required(VERSION 3.0)

project(franquettegl)

add_executable(franquettegl main.cpp)

install(TARGETS franquettegl RUNTIME DESTINATION bin)
