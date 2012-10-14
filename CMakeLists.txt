

cmake_minimum_required(VERSION 2.6)

set(SP_VERSION_MAJOR 0)
set(SP_VERSION_MINOR 3)
set(SP_VERSION_PATCH 0)

message(STATUS "Configuring StructurePipe ${SP_VERSION_MAJOR}.${SP_VERSION_MINOR}.${SP_VERSION_PATCH}")

project(StructurePipe)

## Configuration options ###############################

# Custom paths to libraries #########
set(BOOST_ROOT "" CACHE FILEPATH "Path to boost library")
set(ARMADILLO_ROOT "" CACHE FILEPATH "Path to armadillo linear algebra library")
set(QHULL_ROOT "" CACHE FILEPATH "Path to qhull library")

# Build settings
set(SP_USE_YAML TRUE CACHE BOOL "Build StructurePipe with YAML support.")
set(SP_USE_QHULL FALSE CACHE BOOL "Build StructurePipe with QHull support.")

## End configuration options ###########################

# Tell CMake where to find modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/Modules/")

## FIND PACKAGES ########################################
# First add any user specified paths to the package search array
set(CMAKE_PREFIX_PATH
  "${BOOST_ROOT}"
  "${ARMADILLO_ROOT}"
  "${QHULL_ROOT}"
)


## Boost ##
# Tell Windows builds to look for static boost libraries
if(WIN32)
   # Disable auto-linking
  add_definitions(-DBOOST_ALL_NO_LIB)
#  set(Boost_USE_STATIC_LIBS ON)
endif(WIN32)
find_package(Boost 1.36.0 REQUIRED COMPONENTS system filesystem)

## Armadillo ##
find_package(Armadillo)
if("${ARMADILLO_INCLUDE_DIRS}" STREQUAL "")
  set(ARMADILLO_INCLUDE_DIRS "${ARMADILLO_ROOT}/include")
endif()

## QHull ##
if(SP_USE_QHULL)
  if(WIN32)
    set(QHULL_USE_STATIC ON)
  endif(WIN32)
  find_package(Qhull COMPONENTS QhullCpp)
  set(SP_USE_QHULL ${QHULL_QHULLCPP_FOUND})
endif()

## END FIND PACKAGES ####################################


## First libraries #####################################

add_subdirectory(lib/pipelib)    # Pipeline library
add_subdirectory(lib/spglib)     # Space group library (https://sourceforge.net/projects/spglib/)
if(SP_USE_YAML)
  add_subdirectory(lib/yaml-cpp) # YAML library (http://code.google.com/p/yaml-cpp/)
  set(YAML_INCLUDE_DIRS "lib/yaml-cpp/include" CACHE FILEPATH INTERNAL FORCE)
endif(SP_USE_YAML)
add_subdirectory(lib/sslib)      # Structure searching library


## Now StructurePipe ####################################

configure_file (
  "${PROJECT_SOURCE_DIR}/src/StructurePipeConfig.h.in"
  "${PROJECT_BINARY_DIR}/src/StructurePipeConfig.h"
)

if(SP_USE_QHULL)
include_directories(
  ${QHULL_INCLUDE_DIRS}
)
endif(SP_USE_QHULL)

set(SPIPE_INCLUDE_DIRS
  ${PROJECT_BINARY_DIR}/src  # So that we find StructurePipeConfig.h
  ${PROJECT_SOURCE_DIR}/src
  CACHE FILEPATH INTERNAL FORCE
)

# Include directories with header files
include_directories(
  ${Boost_INCLUDE_DIRS}
  ${ARMADILLO_INCLUDE_DIRS}
  ${SSLIB_INCLUDE_DIRS}
  ${SPIPE_INCLUDE_DIRS}
)

add_subdirectory(src ${CMAKE_BINARY_DIR}/bin)

# Suppress yaml-cpp warnings
if(MSVC)
  set_target_properties(yaml-cpp PROPERTIES COMPILE_FLAGS "/W0")
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  set_target_properties(yaml-cpp PROPERTIES COMPILE_FLAGS "-w")
endif()

set_property(TARGET PROPERTY PROJECT_LABEL "StructurePipe")
