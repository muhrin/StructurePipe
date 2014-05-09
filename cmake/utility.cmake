

function(try_installing lib ret_install_path)

  set(CMAKE_FILE "${CMAKE_SOURCE_DIR}/cmake/external_${lib}.cmake")
  if(EXISTS "${CMAKE_FILE}")
    set(BUILD_DIR "${CMAKE_BINARY_DIR}/${lib}")
    file(MAKE_DIRECTORY ${BUILD_DIR})

    file(COPY ${CMAKE_FILE} DESTINATION ${BUILD_DIR})
    file(RENAME "${BUILD_DIR}/external_${lib}.cmake"
      "${BUILD_DIR}/CMakeLists.txt")
    
    if(NOT CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
      set(INSTALL_PREFIX "-DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}")
    endif()
    execute_process(COMMAND ${CMAKE_COMMAND} 
      "${INSTALL_PREFIX}" "." 
      WORKING_DIRECTORY ${BUILD_DIR})
    execute_process(COMMAND ${CMAKE_COMMAND} "--build" "." "--target"
      WORKING_DIRECTORY ${BUILD_DIR})
    set(${ret_install_path}
      "${BUILD_DIR}/install" PARENT_SCOPE
    )
  endif(EXISTS "${CMAKE_FILE}")

endfunction(try_installing)

# Save to use a macro as opposed to function because we're calling
# find_package within our scope
# see: http://cmake.3232098.n2.nabble.com/Scope-of-find-package-inside-a-function-block-td6821980.html
macro(find_package_or_install lib)

  find_package(${lib} QUIET)
  if(NOT {${lib}_FOUND}})
    try_installing(${lib} INSTALL_PATH)
    # Did the install succeed?
    find_package(${lib} QUIET REQUIRED HINTS ${INSTALL_PATH})
    unset(INSTALL_PATH)
  endif()

endmacro(find_package_or_install)
