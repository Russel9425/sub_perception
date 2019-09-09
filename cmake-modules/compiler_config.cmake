# *  @Copyright (c) 2019-2029 TJU
# *  Create @ 2019-01-17 by tao.jing
# *  cmake file of main

set(CTYPE  Native) # Native or Arm
# set(ARCH x86_64) # For Clion debug

if(ARCH) # arm OR x86_64
  message(STATUS "Compilation Arch ${ARCH}")
  if(ARCH MATCHES x86_64 OR ARCH MATCHES x86)
    set(CTYPE Native)
  elseif(ARCH MATCHES arm)
    set(CTYPE Arm)
  else()
    message(FATAL "Not support arch!")
  endif()
endif()

if (CTYPE MATCHES Arm)
  set(TOOLS /home/russel25/tools/arm-linux-gcc-7.4)
  set(CMAKE_C_COMPILER ${TOOLS}/bin/arm-linux-gnueabihf-gcc)
  set(CMAKE_CXX_COMPILER ${TOOLS}/bin/arm-linux-gnueabihf-g++)
  add_definitions(-DUSE_ARM)
elseif (CTYPE MATCHES Native)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lpthread -lrt -lrt")
  add_definitions(-DUSE_X86_64)
endif()

set(CMAKE_SYSTEM_NAME Linux)

set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wno-unused-variable -Wno-unused-but-set-variable" )
set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -lm -pthread -fPIC -ldl -lpthread" )

if( ${CMAKE_SYSTEM_PROCESSOR} STREQUAL "aarch64" )
  message( STATUS " - System Processor : aarch64")
elseif ( ${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64" )
  message( STATUS " - System Processor : x86_64")
endif()

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/libs)
