 cmake_minimum_required(VERSION 2.8.12)
 project(StereoVisualization)

 add_definitions("-std=c++11")

 include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
 conan_basic_setup()

 add_executable(stereo-visualization main.cpp shaders.cpp stereo_processor.cpp Viewer3d.cpp)
 target_link_libraries(stereo-visualization ${CONAN_LIBS})
