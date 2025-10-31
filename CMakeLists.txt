cmake_minimum_required(VERSION 3.15)
project(Physica VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find SFML (compatible with 2.5+ and 3.0+)
find_package(SFML 3 COMPONENTS Graphics Window System QUIET)
if(NOT SFML_FOUND)
    find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
endif()

# Source files
file(GLOB_RECURSE SOURCES 
    "src/*.cpp"
    "src/*.h"
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Link libraries (SFML 3.0 uses SFML:: prefix, 2.5 uses sfml-)
if(SFML_VERSION_MAJOR EQUAL 3)
    target_link_libraries(${PROJECT_NAME} 
        SFML::Graphics 
        SFML::Window 
        SFML::System
    )
else()
    target_link_libraries(${PROJECT_NAME} 
        sfml-graphics 
        sfml-window 
        sfml-system
    )
endif()

# Platform-specific settings
if(APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
elseif(UNIX)
    target_link_libraries(${PROJECT_NAME} GL)
elseif(WIN32)
    target_link_libraries(${PROJECT_NAME} opengl32)
endif()

# Set output directory
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
