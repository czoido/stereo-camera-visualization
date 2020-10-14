cmake_minimum_required(VERSION 3.0)
project(StereoCameraVisualization)


include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(stereo-camera-visualization main.cpp opengl_camera.cpp opengl_shader.cpp opengl_viewer.cpp stereo_pair_processor.cpp intrinsics.yml extrinsics.yml Info.plist point_cloud.vs point_cloud.fs camera_render.vs camera_render.fs conanfile.txt )

set_property(TARGET stereo-camera-visualization PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/res")

set_target_properties(stereo-camera-visualization PROPERTIES CXX_STANDARD 11)


target_link_libraries(stereo-camera-visualization ${CONAN_LIBS})
