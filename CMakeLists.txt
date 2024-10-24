cmake_minimum_required(VERSION 3.11)
cmake_policy(VERSION 3.28)
#Set names and languages for automatic detection not 100% foolproof

project(RPI_PROJECT)

#include(Ctest)
#enable_testing()
#Finds the system it is on. for fun and debugging if needed
string(TIMESTAMP time)
message(STATUS "TIME OF BUILD: ${time}")
if(WIN32)
    message(STATUS "System: Windows")
elseif(APPLE)
    message(STATUS "System: MACOS")
    
elseif(UNIX)
    message(STATUS "System: Linux/Unix")
    
else()
    message(WARNING "OS Assessment: Unknown platform. Status: Disastrous.")
endif()

#List out found info.
message(STATUS "Architecture: ${CMAKE_SYSTEM_PROCESSOR}")

#Tries to list the compiler, otherwise throw error.
if(CMAKE_CXX_COMPILER)
    message(STATUS "Compiler Located: ${CMAKE_CXX_COMPILER}")
else()
    message(FATAL_ERROR "Compiler Reassessment: Compiler not found. Status: Calamitous.")
endif()

#My variable :D
#These flags only work if on GCC.
set(GCC_FLAGS "-Wall -Wextra -Wold-style-cast -Wsign-promo -Woverloaded-virtual -Wctor-dtor-privacy -Wnon-virtual-dtor -Weffc++ -pedantic")

set(CMAKE_CXX_FLAGS "${GCC_FLAGS}")
#Append the newer flags.

add_subdirectory(app)
