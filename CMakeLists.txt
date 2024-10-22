cmake_minimum_required(VERSION 3.10)
project(CRY_ROOT_Simulation)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_path(CRY_INCLUDE_DIR
  NAMES CRYGenerator.h
  HINTS
    ENV CRY_HOME
    ENV CRY_DIR
  PATHS
    /usr/local/include
    /usr/include
    /opt/local/include
    $ENV{HOME}/cry/include
    $ENV{HOME}/cry_v1.7/src
)
message("CRY_INCLUDE_DIR: " ${CRY_INCLUDE_DIR})

find_library(CRY_LIBRARY
  NAMES CRY
  HINTS
    ENV CRY_HOME
    ENV CRY_DIR
  PATHS
    /usr/local/lib
    /usr/lib
    /opt/local/lib
    $ENV{HOME}/cry/lib
    $ENV{HOME}/cry_v1.7/lib
)
message("CRY_LIBRARY: " ${CRY_LIBRARY})

include_directories(${CRY_INCLUDE_DIR})
find_package(ROOT REQUIRED COMPONENTS Core RIO Net Hist Graf Graf3d Gpad Tree Rint Postscript Matrix Physics MathCore Thread)

if(NOT ROOT_FOUND)
    message(FATAL_ERROR "NO ROOT lib found")
endif()

include_directories(${ROOT_INCLUDE_DIRS})
link_directories(${ROOT_LIBRARY_DIRS})
add_definitions(${ROOT_DEFINITIONS})

set(SOURCE_FILES
    CRYSim.cxx  
)
add_definitions(-DDATA_DIR="${CMAKE_SOURCE_DIR}/data")

add_executable(CRYSim ${SOURCE_FILES})
target_link_libraries(CRYSim ${CRY_LIBRARY} ${ROOT_LIBRARIES})




