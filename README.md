# OGLplus Boilerplate

A bare-bones CMake project to get you rendering as quickly as possible using [SDL2](https://www.libsdl.org/), [GLEW](http://glew.sourceforge.net/) and [OGLplus](https://github.com/matus-chochlik/oglplus) on multiple platforms.

## Dependencies

 * [CMake](http://www.cmake.org/)
 * Compiler with C++11 support (Visual Studio 2015 and Clang 3.6 tested)
 * Your platform's OpenGL libraries

Other library dependencies are included in the repository as git submodules. After cloning the repository, invoke `git submodule init` and `git submodule update` to fetch them.

Alternatively, you can do this while cloning the repository:

    git clone  --recursive

Note: As an exception, GLEW is included as a pruned-down snapshot in the repository itself, to avoid depending on GNU Make on Windows.

## Building

Invoke CMake from your build directory, then use your platform's regular build tools. For example:

    mkdir build && cd build
    cmake ..
    make
