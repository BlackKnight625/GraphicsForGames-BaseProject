cmake_minimum_required(VERSION 3.23)
project(cgj_cmake_with_libs)

# READ THIS
# This document is commented to help you set up your cmake to correctly interpret libs in windows.
# This assumes you know how to setup projects with cmake or use Clion.

# Make sure you use the Visual Studio toolchains and Generator.
# The generator will be named "Visual Studio 16 2019" or "Visual Studio 17 2022".

set(CMAKE_CXX_STANDARD 17)

add_definitions(-D_CRT_SECURE_NO_WARNINGS)
add_definitions(-D_CRT_NO_VA_START_VALIDATION)
add_definitions(-DWINDOWS_IGNORE_PACKING_MISMATCH)

# Include the OpenGL Package
# set(OpenGL_GL_PREFERENCE LEGACY)
find_package(OpenGL REQUIRED)

# Define the libraries you want to include.
# The `add_library` allows you to include libraries to your work.
# The IMPORTED parameter defines the library as an external inclusion
# The SHARED parameter defines the library as a dynamic library (.dll)
add_library(GLEW SHARED IMPORTED)
add_library(GLFW SHARED IMPORTED)
# ... add other libs ...

if (WIN32) # WINDOWS
    # Tell CMake where to find the `.dll` and `.lib` file of the library
    set_target_properties(GLEW PROPERTIES
            IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/GLEW/bin/Release/x64/glew32.dll
            IMPORTED_IMPLIB ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/GLEW/lib/Release/x64/glew32.lib
            )

    set_target_properties(GLFW PROPERTIES
            IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/GLFW/lib-vc2022/glfw3.dll
            IMPORTED_IMPLIB ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/GLFW/lib-vc2022/glfw3.lib
            )

    # ... add other libs ...
else ()
    # Define the properties for other OSes
endif ()

include_directories(
        ${OPENGL_INCLUDE_DIRS}
        # Include the `.h` files into your code.
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/GLEW/include
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/GLFW/include
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/GLM
        # ... add other libs ...
)

# To make it easier, we can create a variable holding all needed lib dependencies
set(GRAPHIC_LIBRARIES
        GLEW
        ${OPENGL_LIBS}
        GLFW
        # ... add other libs ...
        )

set(SOURCES
        BaseProject/hello-3d-world.cpp
        BaseProject/mgl/mglApp.cpp
        BaseProject/mgl/mglShader.cpp
        BaseProject/mgl/mglError.cpp
        BaseProject/mgl/mglCamera.cpp
        )

add_executable(${PROJECT_NAME} ${SOURCES})

# Link the libs with the executable
target_link_libraries(${PROJECT_NAME}
        ${GRAPHIC_LIBRARIES})

# Remember to copy the `.dll` files into the output directory
