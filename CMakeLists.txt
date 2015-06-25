cmake_minimum_required (VERSION 3.0 FATAL_ERROR)

# Create folder to allocate cmake owner projects
set_property(GLOBAL PROPERTY USE_FOLDERS ON)


project      (PerspectiveDistortionRemove)

set ( PROJECT_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set ( PROJECT_SOURCE_DIR "${PROJECT_ROOT_DIR}/src")
set ( PROJECT_BINARY_DIR "${PROJECT_ROOT_DIR}/bin")


include_directories	(${PROJECT_ROOT_DIR}/include)

######################################################################
############################# SETUP QT5 ##############################
find_package (Qt5OpenGL REQUIRED)
find_package (Qt5Widgets REQUIRED)
if (Qt5_POSITION_INDEPENDENT_CODE)
  set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)	# Find includes in corresponding build directories
include_directories	(${Qt5Widgets_INCLUDE_DIRS} ${Qt5OpenGL_INCLUDE_DIRS})
#######################################################################

######################################################################
############################### EIGEN 3 ##############################
# pkg-config is used so we don't hard-code the location of eigen.
find_package(PkgConfig)
if(NOT PKG_CONFIG_FOUND)
    set( EIGEN3_INCLUDE_DIRS "$ENV{EIGEN3_INCLUDE_DIR}" )
    if( NOT EIGEN3_INCLUDE_DIRS )
    message( SEND_ERROR "pkg-config not found on your system. Please point the environment variable EI    GEN3_INCLUDE_DIR to the include directory of your Eigen3 installation.")
endif()
else()
    pkg_check_modules(EIGEN3 REQUIRED eigen3)
endif()
include_directories( ${EIGEN3_INCLUDE_DIRS} )
######################################################################



#######################################################################
#################### SETUP OUTPUT DIRECTORY ###########################
set( CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/lib_install )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR} CACHE INTERNAL "Single Directory for all executables")
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR} CACHE INTERNAL "Single Directory for all libraries")
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR} CACHE INTERNAL "Single Directory for all libraries")
#######################################################################


add_subdirectory (${PROJECT_SOURCE_DIR})







