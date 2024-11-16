project(BuggieBug)

# Set the standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set build directory
file(GLOB_RECURSE found_files ${CMAKE_CURRENT_SOURCE_DIR}/src/ *.cpp)
# BUILD THE CODE
message(STATUS "Name of executable/project: ${PROJECT_NAME}")

add_executable(${PROJECT_NAME} ${found_files})

# Include Directories for Cleaner Includes
target_include_directories(
    ${PROJECT_NAME}
    
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_include_directories(
    ${PROJECT_NAME} SYSTEM

    PRIVATE {Qt6Widgets_INCLUDE_DIRS}
)

target_link_libraries(
    ${PROJECT_NAME}

    PRIVATE Qt6::Widgets
)
