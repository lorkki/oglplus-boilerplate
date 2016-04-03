cmake_minimum_required(VERSION 3.0)
project(oglplus-boilerplate CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(GLM_SOURCE_DIR "external/glm")
set(OGLPLUS_SOURCE_DIR "external/oglplus")
set(SDL2_SOURCE_DIR "external/sdl2")
set(GLEW_SOURCE_DIR "external/glew-1.13.0")

include_directories(${GLM_SOURCE_DIR})

include_directories(${OGLPLUS_SOURCE_DIR}/include)
include_directories(${OGLPLUS_SOURCE_DIR}/implement)

# SDL2

add_subdirectory(${SDL2_SOURCE_DIR})
set_target_properties(SDL2 SDL2main PROPERTIES FOLDER "external")
list(APPEND LIBS SDL2)
include_directories(${SDL2_SOURCE_DIR}/include)

# GLEW

add_definitions(-DGLEW_STATIC)
add_subdirectory(${GLEW_SOURCE_DIR})
set_target_properties(glew PROPERTIES FOLDER "external")
list(APPEND LIBS glew)
include_directories(${GLEW_SOURCE_DIR}/include)

# Configure OGLplus

add_definitions(-DOGLPLUS_LOW_PROFILE=1)
add_definitions(-DOGLPLUS_USE_GLEW=1)
add_definitions(-DOGLPLUS_NO_SITE_CONFIG=1)
add_definitions(-DOGLPLUS_USE_GLCOREARB_H=0)
add_definitions(-DOGLPLUS_USE_BOOST_CONFIG=0)

add_executable(oglplus-boilerplate main.cpp)
target_link_libraries(oglplus-boilerplate ${LIBS})

set_property(TARGET oglplus-boilerplate PROPERTY CXX_STANDARD 11)
set_property(TARGET oglplus-boilerplate PROPERTY CXX_STANDARD_REQUIRED ON)

if (APPLE)
    set(APP_NAME "OGLplus Boilerplate")

    # if (EXISTS icon.icns)
        add_executable(${APP_NAME} MACOSX_BUNDLE main.cpp icon.icns)
        set_source_files_properties("icon.icns" PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
    # else ()
    # add_executable(${APP_NAME} MACOSX_BUNDLE main.cpp)
    # endif ()

    target_link_libraries(${APP_NAME} ${LIBS})
    set_target_properties(${APP_NAME} PROPERTIES MACOSX_BUNDLE_ICON_FILE "icon.icns")

    set_target_properties(${APP_NAME} PROPERTIES MACOSX_BUNDLE_BUNDLE_NAME ${APP_NAME})
    set_target_properties(${APP_NAME} PROPERTIES MACOSX_BUNDLE_SHORT_VERSION_STRING "1.0")
    set_target_properties(${APP_NAME} PROPERTIES MACOSX_BUNDLE_LONG_VERSION_STRING "1.0")
    set_target_properties(${APP_NAME} PROPERTIES MACOSX_BUNDLE_INFO_STRING "OGLplus boilerplate example app")
endif()
