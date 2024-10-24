project(CarBug)

# Set the standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set build directory
file(GLOB_RECURSE found_files src/ *.cpp)
# BUILD THE CODE
message("Name of executable/project: "${PROJECT_NAME})

add_executable(${PROJECT_NAME} ${found_files})

# Include Directories for Cleaner Includes
target_include_directories(
    ${PROJECT_NAME}
    PRIVATE

    app/src
    app/vendor
)
# Link libraries if necesary
# target_link_libraries(${PROJECT_NAME} PRIVATE ...Libraries...)