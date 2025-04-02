cmake_minimum_required(VERSION 3.27)
project("Tablet-Visualizer" LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(SFML_BUILD_AUDIO OFF)
set(SFML_BUILD_NETWORK OFF)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
include(ExternalProject)

FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM
)
FetchContent_MakeAvailable(SFML)
FetchContent_Declare(nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
)
set(JSON_BuildTests OFF)
set(JSON_BuildTests_INIT OFF)
FetchContent_MakeAvailable(nlohmann_json)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/*.cpp")

add_executable(main ${SOURCES})

if (UNIX)
find_package(X11 REQUIRED)
find_package(OpenGL REQUIRED)
find_library(UDEV_LIBRARY NAMES udev)
find_path(UDEV_INCLUDE_DIR NAMES libudev.h)
find_library(OPENAL_LIBRARY NAMES openal)
find_path(OPENAL_INCLUDE_DIR NAMES AL/al.h)

include_directories(${X11_INCLUDE_DIR})
link_directories(${X11_LIBRARIES})
target_include_directories(main PRIVATE ${OPENGL_INCLUDE_DIR})
target_include_directories(main PRIVATE ${UDEV_INCLUDE_DIR})
target_include_directories(main PRIVATE ${OPENAL_INCLUDE_DIR})

target_link_libraries(main PRIVATE ${X11_LIBRARIES})
target_link_libraries(main PRIVATE OpenGL::OpenGL)
target_link_libraries(main PRIVATE ${UDEV_LIBRARY})
target_link_libraries(main PRIVATE ${OPENGL_opengl_LIBRARY})
target_link_libraries(main PRIVATE ${OPENAL_LIBRARY})
endif (UNIX)

target_link_libraries(main PRIVATE sfml-graphics)
target_compile_features(main PRIVATE cxx_std_20)

target_link_libraries(main PRIVATE nlohmann_json::nlohmann_json)

