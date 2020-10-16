cmake_minimum_required(VERSION 3.0.0)
project(marker_tracking VERSION 0.1.0)

set(EXTERNAL_INSTALL_LOCATION ${CMAKE_BINARY_DIR}/external)
set(EXT_PROJ_ROOT_PATH ${CMAKE_BINARY_DIR}/external-tmp)

include(ExternalProject)

list(APPEND CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}")
ExternalProject_Add(eigen3
  GIT_REPOSITORY    https://gitlab.com/libeigen/eigen.git
  GIT_TAG           master
  CMAKE_ARGS        ${CMAKE_ARGS}
)
ExternalProject_Add_StepTargets(eigen3 build)

set (OpenCV_CONTRIB_DIR ${CMAKE_BINARY_DIR}/opencv_contrib-prefix/src/opencv_contrib/modules)
list(APPEND CMAKE_ARGS "-DOPENCV_EXTRA_MODULES_PATH=${OpenCV_CONTRIB_DIR}")
ExternalProject_Add(opencv_contrib
  GIT_REPOSITORY    https://github.com/opencv/opencv_contrib.git
  GIT_TAG           master
  CONFIGURE_COMMAND ""
  BUILD_COMMAND     ""
  INSTALL_COMMAND   ""
)

ExternalProject_Add(opencv
  GIT_REPOSITORY    https://github.com/opencv/opencv.git
  GIT_TAG           master
  CMAKE_ARGS        ${CMAKE_ARGS}
  DEPENDS           eigen3-build
)

# set(OpenCV_DIR ${EXTERNAL_INSTALL_LOCATION}/lib/cmake/opencv4)
list(APPEND CMAKE_PREFIX_PATH "${EXTERNAL_INSTALL_LOCATION}/lib/cmake/opencv4")

find_package(
  OpenCV REQUIRED
  PATHS ${OpenCV_DIR}
)

if (OpenCV_FOUND)

  message("OpenCV library status:")
  message("    config:" ${OpenCV_DIR})
  message("    version:" ${OpenCV_VERSION})
  message("    libraries:" ${OpenCV_LIBS})
  message("    include path:" ${OpenCV_INCLUDE_DIRS})
  
  include_directories(
    ${OpenCV_INCLUDE_DIRS}
    include
  )

  add_executable(marker_creation src/marker_creation.cpp)
  add_dependencies(marker_creation eigen3 opencv)

  add_executable(${PROJECT_NAME} src/main.cpp)
  add_dependencies(${PROJECT_NAME} eigen3 opencv)
  add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
                    COMMAND ${CMAKE_COMMAND} -E echo ""
                    COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --cyan "==========================="
                    COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --cyan " Built all dependencies!!! "
                    COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --cyan "==========================="
  )

  target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
  target_link_libraries(marker_creation ${OpenCV_LIBS})

else (OpenCV_FOUND)

  message("Unable to find OpenCV")

endif (OpenCV_FOUND)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
