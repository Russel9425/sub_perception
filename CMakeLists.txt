# *  @Copyright (c) 2019-2029 TJU
# *  Create @ 2019-01-17 by tao.jing
# *  cmake file of main

cmake_minimum_required(VERSION 2.8)
project(sub_perception)

# Add module path for <name>.cmake conf files
set(CURRENT_CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules)
set(CMAKE_MODULE_PATH ${CURRENT_CMAKE_MODULE_PATH})

add_subdirectory(gps)
add_subdirectory(dvl)