
cmake_minimum_required(VERSION 2.6)

set(SPIPE_VERSION_MAJOR 0)
set(SPIPE_VERSION_MINOR 3)
set(SPIPE_VERSION_PATCH 0)
set(SPIPE_VERSION "${SPIPE_VERSION_MAJOR}.${SPIPE_VERSION_MINOR}.${SPIPE_VERSION_PATCH}")

project(SPipe)
message(STATUS "Configuring ${PROJECT_NAME} ${SPIPE_VERSION}")

## FIND PACKAGES ########################################

include(cmake/utility.cmake)

## Boost ##
# Disable auto-linking
add_definitions(-DBOOST_ALL_NO_LIB)
find_package(Boost 1.36.0 REQUIRED COMPONENTS system filesystem regex)

## Armadillo ##
find_package(Armadillo CONFIG REQUIRED)
if("${ARMADILLO_INCLUDE_DIRS}" STREQUAL "")
  set(ARMADILLO_INCLUDE_DIRS "${ARMADILLO_ROOT}/include")
endif()

find_package(YamlCpp QUIET)
find_package(Spglib REQUIRED)
find_package_or_install(Schemer)
find_package_or_install(Pipelib)
find_package_or_install(SPL)

## Configuration options ###############################

set(INSTALL_BIN_DIR bin CACHE PATH "Installation directory for executables")
set(INSTALL_INCLUDE_DIR include CACHE PATH "Installation directory for header files")
set(INSTALL_LIB_DIR lib CACHE PATH "Installation directory for library files.")
if(WIN32 AND NOT CYGWIN)
  set(DEF_INSTALL_CMAKE_DIR CMake)
else()
  set(DEF_INSTALL_CMAKE_DIR share/${CMAKE_PROJECT_NAME}/cmake)
endif()
set(INSTALL_CMAKE_DIR ${DEF_INSTALL_CMAKE_DIR} CACHE PATH
  "Installation directory for CMake files")

# Make relative paths absolute (needed later on)
foreach(p LIB BIN INCLUDE CMAKE)
  set(var INSTALL_${p}_DIR)
  if(NOT IS_ABSOLUTE "${${var}}")
    set(${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
  endif()
endforeach()

# Build settings
set(SP_WITH_YAML ${YamlCpp_FOUND} CACHE BOOL "Build StructurePipe with YAML support.")
set(SP_BUILD_TESTS FALSE CACHE BOOL "Build spipe tests.")
set(SP_ENABLE_THREAD_AWARE TRUE CACHE BOOL "Enable multithreaded awareness in spipe.")

configure_file(
  "${PROJECT_SOURCE_DIR}/include/spipe/Version.h.in"
  "${PROJECT_BINARY_DIR}/include/spipe/Version.h"
)
configure_file(
  "${PROJECT_SOURCE_DIR}/include/spipe/Config.h.in"
  "${PROJECT_BINARY_DIR}/include/spipe/Config.h"
)

## Now StructurePipe ####################################

# Include directories with header files
include_directories(
  ${PROJECT_BINARY_DIR}/include  # So that we find SPipeConfig.h
  ${PROJECT_SOURCE_DIR}/include
  ${Boost_INCLUDE_DIRS}
  ${ARMADILLO_INCLUDE_DIRS}
  ${PIPELIB_INCLUDE_DIRS}
  ${SCHEMER_INCLUDE_DIRS}
  ${SPL_INCLUDE_DIRS}
  ${SPGLIB_INCLUDE_DIRS}
  ${SPIPE_INCLUDE_DIRS}
)

include(Include.cmake)
include(Src.cmake)

set_property(TARGET PROPERTY PROJECT_LABEL ${PROJECT_NAME})

###################
## spipe library ##
###################
add_library(spipe
  ${spipe_Header_Files}
  ${spipe_Source_Files}
)

# Libraries we need to link to
target_link_libraries(spipe
  ${Boost_LIBRARIES}
  ${ARMADILLO_LIBRARIES}
  ${YAML_CPP_LIBRARIES}
  ${SCHEMER_LIBRARIES}
  ${SPGLIB_LIBRARIES}
  ${SPL_LIBRARIES}
)

###################
## spipe binary  ##
###################
## Boost ##
# Disable auto-linking
add_definitions(-DBOOST_ALL_NO_LIB)
find_package(Boost 1.36.0 QUIET REQUIRED COMPONENTS system program_options)

# Make sure that after make install shared libraries are correctly linked
# to. See http://www.cmake.org/Wiki/CMake_RPATH_handling#Common_questions
# for more details
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

add_executable(spipe-bin src/spipe.cpp)
add_dependencies(spipe-bin spipe)
set_target_properties(spipe-bin PROPERTIES OUTPUT_NAME spipe)

# Libraries we need to link to
target_link_libraries(spipe-bin
  ${Boost_LIBRARIES}
  spipe
)

## Installation
install(TARGETS spipe spipe-bin
  EXPORT ${CMAKE_PROJECT_NAME}Targets
  RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin
  LIBRARY DESTINATION "${INSTALL_LIB_DIR}" COMPONENT shlib
  ARCHIVE DESTINATION "${INSTALL_LIB_DIR}" COMPONENT lib
)
# Header files
install(DIRECTORY include/ DESTINATION ${INSTALL_INCLUDE_DIR}
  COMPONENT dev
  PATTERN "*.h"
)
install(DIRECTORY ${PROJECT_BINARY_DIR}/include/
  DESTINATION ${INSTALL_INCLUDE_DIR}
  COMPONENT dev
  PATTERN "*.h"
)

###
### Configuration file settings
###
# Add all targets to the build-tree export set
export(TARGETS spipe spipe-bin
  FILE "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Targets.cmake")

# Export the package for use from the build-tree
# (this registers the build-tree with a global CMake-registry)
export(PACKAGE ${PROJECT_NAME})

# Create the *Config.cmake and *ConfigVersion.cmake files
file(RELATIVE_PATH REL_INCLUDE_DIR "${INSTALL_CMAKE_DIR}"
   "${INSTALL_INCLUDE_DIR}")

# ... for the build tree
set(CONF_INCLUDE_DIRS
  "${PROJECT_SOURCE_DIR}/include"  "${PROJECT_BINARY_DIR}/include"
)
configure_file(cmake/SPipeConfig.cmake.in
  "${PROJECT_BINARY_DIR}/SPipeConfig.cmake" @ONLY)

# ... for the install tree
set(CONF_INCLUDE_DIRS "\${SPIPE_CMAKE_DIR}/${REL_INCLUDE_DIR}")
configure_file(cmake/SPipeConfig.cmake.in
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/SPipeConfig.cmake" @ONLY)

# ... for both
configure_file(cmake/SPipeConfigVersion.cmake.in
  "${PROJECT_BINARY_DIR}/SPipeConfigVersion.cmake" @ONLY)

# Install the *Config.cmake and *ConfigVersion.cmake
install(FILES
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/SPipeConfig.cmake"
  "${PROJECT_BINARY_DIR}/SPipeConfigVersion.cmake"
  DESTINATION "${INSTALL_CMAKE_DIR}" COMPONENT dev)

# Install the export set for use with the install-tree
install(EXPORT ${CMAKE_PROJECT_NAME}Targets DESTINATION
  "${INSTALL_CMAKE_DIR}" COMPONENT dev)


#
# Tests
if(SP_BUILD_TESTS)
  add_subdirectory(tests)
endif(SP_BUILD_TESTS)

