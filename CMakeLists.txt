
cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 20)

add_compile_definitions(SFML_STATIC)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY $<1:${CMAKE_SOURCE_DIR}>)

project("Tablet-Visualizer")

file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/*.cpp")

add_executable(TabletVisualizer "${MY_SOURCES}")

target_link_libraries(TabletVisualizer "${CMAKE_SOURCE_DIR}/third_party/SFML/lib/sfml-graphics-s.lib")
target_link_libraries(TabletVisualizer "${CMAKE_SOURCE_DIR}/third_party/SFML/lib/sfml-window-s.lib")
target_link_libraries(TabletVisualizer "${CMAKE_SOURCE_DIR}/third_party/SFML/lib/sfml-system-s.lib")
target_link_libraries(TabletVisualizer winmm.lib)
target_link_libraries(TabletVisualizer gdi32.lib)
target_link_libraries(TabletVisualizer opengl32.lib)


include_directories("${CMAKE_SOURCE_DIR}/third_party/SFML/include")
include_directories("${CMAKE_SOURCE_DIR}/third_party/json/include")

include_directories(${CMAKE_SOURCE_DIR})