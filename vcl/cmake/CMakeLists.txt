
cmake_minimum_required(VERSION 2.8)
project(pgm)

add_definitions( -DIMGUI_IMPL_OPENGL_LOADER_GLAD )

# Add G++ Warning on Unix
if(UNIX)
add_definitions(-O2 -g -std=c++11 -Wall -Wextra)
    set(CMAKE_CXX_COMPILER g++)
    find_package(glfw3 REQUIRED) #Expect glfw3 to be installed on your system
endif()

# In Window set directory to precompiled version of glfw3
if(WIN32)
    set(GLFW_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/../precompiled/glfw3_win/include")
    include_directories(${GLFW_INCLUDE_DIRS})
    set(GLFW_LIBRARIES "${CMAKE_CURRENT_SOURCE_DIR}/../precompiled/glfw3_win/lib/glfw3.lib")
endif()


include_directories("../src/")
file(
    GLOB_RECURSE
    source_files
    ../src/*.[ch]pp ../external/*.[ch]pp ../external/*.h ../shaders/*.glsl
    )
add_executable(pgm ${source_files})


if(UNIX)
    target_link_libraries(pgm glfw dl)
endif()

if(WIN32)
    target_link_libraries(pgm ${GLFW_LIBRARIES})
endif()
