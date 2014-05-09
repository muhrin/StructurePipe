
include(ExternalProject)


if(NOT CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(Pipelib_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}")
else()
  set(Pipelib_INSTALL_DIR "${CMAKE_CURRENT_BINARY_DIR}/install")
endif()

set(Pipelib_CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${Pipelib_INSTALL_DIR})

ExternalProject_Add(Pipelib
  GIT_REPOSITORY https://github.com/muhrin/PipeLib.git
  GIT_TAG 89b098fcf72bbfb2cb7190d1cadbe5e9929239db
  INSTALL_DIR ${Pipelib_INSTALL_DIR}
  CMAKE_ARGS ${Pipelib_CMAKE_ARGS}
)

export(PACKAGE Pipelib)
