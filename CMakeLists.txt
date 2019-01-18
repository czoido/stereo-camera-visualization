 cmake_minimum_required(VERSION 2.8.12)
 project(StereoCameraVisualization)

 add_definitions("-std=c++11")

 include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
 conan_basic_setup()

 add_executable(stereo-camera-visualization main.cpp 
 											opengl_camera.cpp 
 											opengl_shader.cpp 
 											opengl_viewer.cpp 
 											stereo_pair_processor.cpp 
 											intrinsics.yml
 											extrinsics.yml
 											Info.plist
 											point_cloud.vs
 											point_cloud.fs
 											camera_render.vs
 											camera_render.fs
                                            conanfile.txt )

 target_link_libraries(stereo-camera-visualization ${CONAN_LIBS})
