
cmake_minimum_required(VERSION 3.16)

project(WhackamoleSFML)

# Output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# SFML configuration
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)

FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.1.0
)

FetchContent_MakeAvailable(SFML)

# Game files
set(SOURCE_FILES
    src/main.cpp
    src/Game.cpp
    src/Game.h
)

# Create executable
add_executable(WhackamoleSFML ${SOURCE_FILES})

# Link SFML
target_link_libraries(WhackamoleSFML PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
)

# C++ standard
target_compile_features(WhackamoleSFML PRIVATE cxx_std_20)


# Copy assets folder (if it exists)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/assets")
    file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/assets"
         DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
endif()

# Copy Data folder (if it exists)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/Data")
    file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/Data"
         DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
endif()
