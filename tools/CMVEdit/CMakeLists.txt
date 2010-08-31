# WELCOME 
# This is the main cmake configuration file 
# associated with your project
# You can define specific compilation options here
# More about cmake : www.cmake.org
# Using cmake command line : 
# 1. Create a build directory : mkdir build
# 2. Configure cmake project : cd build; cmake ..
#   You can also generate project with your favorite IDE : 
#   Example : For Mac osx use "cmake .. -G XCode"
#   see cmake help for more information
# 3. Edit project option : ccmake .
# 4. Build it : make 

cmake_minimum_required(VERSION 2.6.4 FATAL_ERROR)

include("${CMAKE_CURRENT_SOURCE_DIR}/bootstrap.cmake")
use(NAOQI-PLUGINS-TOOLS)

# CMVEdit should always be pc_build
SET(CMVEDIT_IS_REMOTE "ON")

create_module(CMVEdit)

SET ( CMVEdit_SRCS
src/CMVEdit.cpp src/CMVEdit.h
src/shared/camera_view.cc
src/shared/colorSpaceConverter.cpp
src/shared/fl_helper.cc
src/shared/gl_draw.cc
src/shared/gl_texture.cc
src/shared/image_io.cc
src/shared/image.cc
src/shared/tmap_view.cc
src/shared/vision.cc
src/shared/yuv2rgb.cc
src/shared/zoom_win.cc
src/NaoImageCapture.cpp
src/shared/crc32.cc
src/shared/iofile.cc
src/ConfigFile.cpp
)

configure_src_module( CMVEdit ${CMVEdit_SRCS} )

# Place the executable outside the pc_build directory
set_target_properties(  "CMVEdit" PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../)

use_lib( CMVEdit ALCOMMON ALMATH LIBCORE TOOLS ALVALUE BOOST_SERIALIZATION)

IF( CMVEDIT_IS_REMOTE STREQUAL "OFF" )
    MESSAGE( STATUS "CMVEdit is compiled as ROBOT_BUILD" )
ELSE( CMUVEDIT_IS_REMOTE STREQUAL "OFF" )
    ADD_DEFINITIONS( -DREMOTE )
    ADD_DEFINITIONS( -DDISABLE_LOGGING )
    MESSAGE( STATUS "CMVEdit is compiled as PC BUILD" )

    # For CMVEdit
    MESSAGE( STATUS "Setting up CMVEdit libraries" )
    Find_Package( FLTK REQUIRED )
    Find_Package( OpenGL REQUIRED )
    Find_Package( GLEW REQUIRED )
    INCLUDE_DIRECTORIES( ${FLTK_INCLUDE_DIR} ${GLEW_INCLUDE_DIR} )
    TARGET_LINK_LIBRARIES(CMVEdit ${FLTK_LIBRARIES} ${GLEW_LIBRARY} )
ENDIF( CMVEDIT_IS_REMOTE STREQUAL "OFF" )
