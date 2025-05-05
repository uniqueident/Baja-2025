cmake_minimum_required(VERSION 3.11...3.28)

# Improves Reconfigure Time / Build Time
find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
endif()

# Overhead Project Name
project(RPI_PROJECT)

set(BUILD_SHARED_LIBS OFF)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# include(Ctest)
# enable_testing()
# Finds the system it is on. for fun and debugging if needed
string(TIMESTAMP time)
message(STATUS "TIME OF BUILD: ${time}")
if(WIN32)

    message(STATUS "System: Windows")

elseif(APPLE)

    message(STATUS "System: MACOS")
    
elseif(UNIX)

    message(STATUS "System: Linux/Unix")

    if(EXISTS "/proc/device-tree/soc/ranges")

        set(RPI_RASPBERRY true)
        
        MESSAGE(STATUS "Device: Raspberry Pi")

    endif()
    
else()

    message(WARNING "OS Assessment: Unknown platform. Status: Disastrous.")

endif()

# List out found info.
message(STATUS "Architecture: ${CMAKE_SYSTEM_PROCESSOR}")

# Tries to list the compiler, otherwise throw error.
if(CMAKE_CXX_COMPILER)
    message(STATUS "Compiler Located: ${CMAKE_CXX_COMPILER}")
else()
    message(FATAL_ERROR "Compiler Reassessment: Compiler not found. Status: Calamitous.")
endif()
message(STATUS "")

# My variable :D - Mateo
# These flags only work if on GCC.
set(GCC_FLAGS "-Wall -Wextra -Wold-style-cast -Wsign-promo -Woverloaded-virtual -Wctor-dtor-privacy -Wnon-virtual-dtor -Weffc++ -pedantic")

# Append the newer flags.
set(CMAKE_CXX_FLAGS "${GCC_FLAGS}")

find_package(OpenGL REQUIRED)

set(GLFW_BUILD_DOCS OFF)
set(GLFW_BUILD_WAYLAND OFF)
set(GLFW_LIBRARY_TYPE "STATIC")
add_subdirectory(app/libs/glfw EXCLUDE_FROM_ALL)
message(STATUS "")

set(CMAKE_POLICY_DEFAULT_CMP0077 "NEW")
set(GLM_ENABLE_CXX_17 ON)
set(GLM_BUILD_LIBRARY OFF)
set(GLM_BUILD_TESTS OFF)
add_subdirectory(app/libs/glm EXCLUDE_FROM_ALL)
message(STATUS "")

find_package(Freetype REQUIRED)

find_package(OpenCV REQUIRED)

set(glad_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/app/libs/glad/include")

add_subdirectory(app)
