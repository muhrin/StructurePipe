cmake_minimum_required(VERSION 2.6)

project(StructurePipe)

# Include local settings
include(CMakeLocal.txt)

# Tell CMake where to find modules
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/CMakeModules/;${CMAKE_MODULE_PATH}")

set(SP_VERSION_MAJOR 0)
set(SP_VERSION_MAJOR 1)

configure_file (
  "${PROJECT_SOURCE_DIR}/src/StructurePipeConfig.h.in"
  "${PROJECT_BINARY_DIR}/src/StructurePipeConfig.h"
  )



## FIND PACKAGES ########################################
# First add any user specified paths to the package search array
set(CMAKE_PREFIX_PATH "${BOOST_ROOT}" "${ARMADILLO_ROOT}")


# Boost #
# Tell Windows builds to look for static boost libraries
if(WIN32)
  set(Boost_USE_STATIC_LIBS ON)
endif(WIN32)
find_package(Boost 1.36.0 COMPONENTS system filesystem REQUIRED)
# Armadillo #
find_package(Armadillo)
if("${ARMADILLO_INCLUDE_DIRS}" STREQUAL "")
  set(ARMADILLO_INCLUDE_DIRS "${ARMADILLO_ROOT}/include")
endif()

## END FIND PACKAGES ####################################

# Include directories with header files
  include_directories(
    ${Boost_INCLUDE_DIRS}
    ${ARMADILLO_INCLUDE_DIRS}
  )

add_subdirectory(lib/pipelib)
add_subdirectory(lib/sslib)
add_subdirectory(src ${CMAKE_BINARY_DIR}/bin)

set_property(TARGET PROPERTY PROJECT_LABEL "StructurePipe")
