project(BuggieBug)

# Set the standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Get the code files
file(GLOB_RECURSE found_files ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

file(GLOB glad_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/libs/glad/src/*.c)

# BUILD THE CODE
message(STATUS "Name of executable/project: ${PROJECT_NAME}")

add_executable(
    ${PROJECT_NAME}
    
    ${found_files}
    ${glad_SOURCE}
)

# Include Directories for Cleaner Includes
target_include_directories(
    ${PROJECT_NAME}
    
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_include_directories(
    ${PROJECT_NAME} SYSTEM

    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/libs/glad/include"
)

target_link_libraries(
    ${PROJECT_NAME}

    PRIVATE glfw
    PRIVATE ${OPENGL_LIBRARIES}
)
