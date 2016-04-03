project (GLEW C)

find_package(OpenGL REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_library(glew STATIC src/glew.c include/GL/glew.h)
target_link_libraries(glew ${OPENGL_LIBRARY})
