# *  @Copyright (c) 2019-2029 TJU
# *  Create @ 2019-01-17 by tao.jing
# *  cmake file of main

cmake_minimum_required(VERSION 2.8)
project(sensors_gps)

if(NOT CMAKE_MODULE_PATH)
    # Add module path for <name>.cmake conf files
    set(CURRENT_CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../cmake-modules)
    set(CMAKE_MODULE_PATH ${CURRENT_CMAKE_MODULE_PATH})
    message("No CMAKE_MODULE_PATH, set own ${CMAKE_MODULE_PATH}.")
endif()

#Set Compiler
include(${CMAKE_MODULE_PATH}/compiler_config.cmake)

message(STATUS " =: Use Compiler ${CMAKE_C_COMPILER}")

##################
## Include_Dirs ##
##################
include_directories(../uw_env)
include_directories(../serial)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

if ( ARCH MATCHES x86_64 OR ARCH MATCHES x86 )
  include_directories(pc_serial)
elseif ( ARCH MATCHES Arm OR ARCH MATCHES arm )
  include_directories(../uw_lib/inc)
  include_directories(uw_serial)
else()
  include_directories(../uw_lib/inc)
  include_directories(pc_serial)
  include_directories(uw_serial)
endif()

##########################
## Main Target Sources  ##
##########################
FILE(GLOB SOURCE_FILES *.h *.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../uw_env/*.c)

if ( ARCH MATCHES x86_64 OR ARCH MATCHES x86 )
  aux_source_directory( ${CMAKE_CURRENT_SOURCE_DIR}/pc_serial  SOURCE_FILES)
  aux_source_directory( ${CMAKE_CURRENT_SOURCE_DIR}/../serial  SOURCE_FILES)
elseif ( ARCH MATCHES Arm OR ARCH MATCHES arm )
  aux_source_directory( ${CMAKE_CURRENT_SOURCE_DIR}/uw_serial  SOURCE_FILES)
else()
  aux_source_directory( ${CMAKE_CURRENT_SOURCE_DIR}/pc_serial  SOURCE_FILES)
  aux_source_directory( ${CMAKE_CURRENT_SOURCE_DIR}/../serial  SOURCE_FILES)
  aux_source_directory( ${CMAKE_CURRENT_SOURCE_DIR}/uw_serial  SOURCE_FILES)
endif()

################
## Link_Dirs  ##
################
if ( ${CTYPE} MATCHES Native )
  link_directories(/usr/local/lib)
  link_directories(/usr/lib/x86_64)
elseif ( ${CTYPE} MATCHES Arm )
  link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../uw_lib/lib)
endif()

###########################
## Main Target Specific  ##
###########################
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

################
## Link Libs  ##
################
if ( ${CTYPE} MATCHES Native )
  #target_link_libraries(${PROJECT_NAME} PUBLIC serial)
elseif ( ${CTYPE} MATCHES Arm )
  target_link_libraries(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../uw_lib/lib/uw_imx6.a)
endif()