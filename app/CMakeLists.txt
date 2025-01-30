project(BuggieBug)

# Set the standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Get All App Source Files
file(GLOB buggie_MAIN_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

file(GLOB buggie_CORE_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/src/Core/*.hpp)
file(GLOB buggie_CORE_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src/Core/*.cpp)

file(GLOB buggie_PIN_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/src/Core/Pins/*.hpp)
file(GLOB buggie_PIN_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src/Core/Pins/*.cpp)

file(GLOB buggie_MOD_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/src/Modules/*.hpp)
file(GLOB buggie_MOD_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src/Modules/*.cpp)

file(GLOB buggie_MOD_GUI_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/src/Modules/GUI/*.hpp)
file(GLOB buggie_MOD_GUI_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src/Modules/GUI/*.cpp)

file(GLOB buggie_MOD_GUI_REN_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/src/Modules/GUI/Renderer/*.hpp)
file(GLOB buggie_MOD_GUI_REN_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src/Modules/GUI/Renderer/*.cpp)

# Get GLAD Source Files
file(GLOB_RECURSE glad_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/libs/glad/include/*.h)
file(GLOB glad_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/libs/glad/src/*.c)

# Get STBI Source Files
file(GLOB stb_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/libs/stb/*.h)

# Group The Source Files For IDE & Cache
source_group("Main File" FILES ${buggie_MAIN_SOURCE})

source_group("Core Headers" FILES ${buggie_CORE_HEADER})
source_group("Core Sources" FILES ${buggie_CORE_SOURCE})

source_group("Pin Headers" FILES ${buggie_PIN_SOURCE})
source_group("Pin Sources" FILES ${buggie_PIN_SOURCE})

source_group("Module Headers" FILES ${buggie_MOD_HEADER})
source_group("Module Sources" FILES ${buggie_MOD_SOURCE})

source_group("GUI Headers" FILES ${buggie_MOD_GUI_HEADER})
source_group("GUI Sources" FILES ${buggie_MOD_GUI_SOURCE})

source_group("GUI Renderer Headers" FILES ${buggie_MOD_GUI_REN_HEADER})
source_group("GUI Renderer Sources" FILES ${buggie_MOD_GUI_REN_SOURCE})

# Library Source Groups
source_group("glad Headers" FILES ${glad_HEADER})
source_group("glad Sources" FILES ${glad_SOURCE})

source_group("stb Headers" FILES ${stb_HEADER})

# BUILD THE CODE
message(STATUS "Name of executable/project: ${PROJECT_NAME}")

add_executable(
    ${PROJECT_NAME}
    
    ${buggie_MAIN_SOURCE}

    ${buggie_CORE_HEADER} ${buggie_CORE_SOURCE}

    ${buggie_PIN_HEADER} ${buggie_PIN_SOURCE}

    ${buggie_MOD_HEADER} ${buggie_MOD_SOURCE}

    ${buggie_MOD_GUI_HEADER}     ${buggie_MOD_GUI_SOURCE}
    ${buggie_MOD_GUI_REN_HEADER} ${buggie_MOD_GUI_REN_SOURCE}
    
    ${glad_HEADER} ${glad_SOURCE}
    ${stb_HEADER}
)

# Include Directories for Cleaner Includes
target_include_directories(
    ${PROJECT_NAME}
    
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_include_directories(
    ${PROJECT_NAME} SYSTEM

    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/libs/glad/include"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/libs/glm"
    PRIVATE "${FREETYPE_INCLUDE_DIRS}"
    PRIVATE "${OpenCV_INCLUDE_DIRS}"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/libs/stb"
    PRIVATE "${glad_INCLUDE_DIRS}"
)

target_link_libraries(
    ${PROJECT_NAME}

    PRIVATE glfw
    PRIVATE glm::glm-header-only
    PRIVATE ${FREETYPE_LIBRARIES}
    PRIVATE ${OpenCV_LIBS}
    PRIVATE ${OPENGL_LIBRARIES}
    PRIVATE gpiod
)
