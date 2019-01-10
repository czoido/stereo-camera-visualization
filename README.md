This program tries to imitate the point cloud rendering from Radiohead's music video "House of Cards" making use of an ELP-960P2CAM-V90 stereo camera I bought in Aliexpress.

It uses OpenCV to calculate the disparities and reproject the points and OpenGL to render those points in real time.
and some OpenCV and OpenGL.

All dependencies are managed with [conan](https://github.com/conan-io/conan), the C / C++ Package Manager for Developers

[![House of Cards](http://img.youtube.com/vi/8nTFjVm9sTQ/0.jpg)](https://www.youtube.com/watch?v=8nTFjVm9sTQ)

![House of Cards frame](![Point Cloud](https://raw.githubusercontent.com/czoido/stereo-camera-visualization/master/media/output-example.png)

The output depending on the parameters, the type of algorithm and the initial calibration of the camera should be something like this:

![Point Cloud](https://raw.githubusercontent.com/czoido/stereo-camera-visualization/master/media/output-example.png)