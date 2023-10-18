cmake_minimum_required(VERSION 3.22.0)
project(hexgaon VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(GLEW REQUIRED)
find_package(GLFW3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
# find_package(tinygltf REQUIRED)

# message(STATUS "OPENGL_INCLUDE_DIR: ${OPENGL_INCLUDE_DIR}")
# message(STATUS "OPENGL_LIBRARIES: ${OPENGL_LIBRARIES}")

# I'm putting weird old libraries in my ~/.local to prevent system issues.
message(STATUS "LOCAL_INCLUDE: $ENV{LOCAL_INCLUDE}")
message(STATUS "LOCAL_LIB: $ENV{LOCAL_LIB}")
include_directories($ENV{LOCAL_INCLUDE})
link_directories($ENV{LOCAL_LIB})

add_executable(hexagon)
target_sources(hexagon
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/source/hexagon/ActionRetriever.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/hexagon/Choreographer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/hexagon/Color.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/hexagon/Debug.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/hexagon/Hexagon.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/hexagon/Iconcache.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/hexagon/Key.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/hexagon/Layer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/hexagon/Light.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/hexagon/LinearMenuOption.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/hexagon/Model.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/hexagon/Node.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/hexagon/PersistentStorage.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/hexagon/Shape.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/hexagon/ShellExecutor.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/hexagon/Vector.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/hexagon/View.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/hexagon/World.cpp
        # $<$<CXX_COMPILER_ID:MSVC>:${CMAKE_CURRENT_SOURCE_DIR}/source/hexagon/port/win32/Win32SR.cpp>
        # $<$<CXX_COMPILER_ID:MSVC>:${CMAKE_CURRENT_SOURCE_DIR}/source/hexagon/port/win32/Win32Exec.cpp>
        # $<$<CXX_COMPILER_ID:MSVC>:${CMAKE_CURRENT_SOURCE_DIR}/source/hexagon/port/win32/Win32FS.cpp>
        # $<$<CXX_COMPILER_ID:MSVC>:${CMAKE_CURRENT_SOURCE_DIR}/source/hexagon/port/win32/Win32Registry.cpp>
        ${CMAKE_CURRENT_SOURCE_DIR}/source/hexagon/port/posix/PosixActionRetriever.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/hexagon/port/posix/PosixExec.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/hexagon/port/posix/PosixFileSystem.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/hexagon/port/posix/PosixRegistry.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/hexagon/glut/Camera.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/hexagon/glut/Input.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/hexagon/glut/Display.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/hexagon/glut/Menu.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/hexagon/glut/Port.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/hexagon/glut/Scheduler.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/hexagon/glut/Wrapper.cpp
)
target_include_directories(hexagon
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/external
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_compile_definitions(hexagon PRIVATE GL_SILENCE_DEPRECATION)
target_link_libraries(hexagon PRIVATE OpenGL::GL OpenGL::GLU GLUT::GLUT)
