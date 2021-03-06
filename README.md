## What is this about?

This program tries to imitate the point cloud rendering from Radiohead's music video "House of Cards" making use of an [ELP-960P2CAM-V90](https://es.aliexpress.com/store/product/Synchronization-3D-USB-2-0-MJPEG-60fps-1-3MP-UVC-mini-webcam-Dual-lens-Stereo-usb/913995_32839879959.html?spm=a219c.search0104.3.2.3ea33574uW0AOJ&ws_ab_test=searchweb0_0,searchweb201602_1_10065_10068_10547_319_10891_317_10548_10696_453_10084_454_10083_10618_431_10307_10820_10301_10821_10303_537_536_10059_10884_10887_100031_321_322_10103-10891,searchweb201603_51,ppcSwitch_0&algo_expid=d877995b-846d-4787-af4a-a5f2a2510997-0&algo_pvid=d877995b-846d-4787-af4a-a5f2a2510997) stereo camera.

It uses OpenCV to calculate the disparities and reproject the points and OpenGL to render those points in real time.

All dependencies are managed with [conan](https://github.com/conan-io/conan), the C / C++ Package Manager for Developers

[![House of Cards](http://img.youtube.com/vi/8nTFjVm9sTQ/0.jpg)](https://www.youtube.com/watch?v=8nTFjVm9sTQ)

**A frame from the music video:**

<img src="http://img.youtube.com/vi/8nTFjVm9sTQ/2.jpg" width="480">

**Output from program:**

The output depending on the parameters, the type of algorithm and the initial calibration of the camera should be something like this:

![Point Cloud](https://raw.githubusercontent.com/czoido/stereo-camera-visualization/master/media/output-example.png)

![Point Cloud](https://raw.githubusercontent.com/czoido/stereo-camera-visualization/master/media/output-example-2.png)

## Building

1. Clone this repo: `git clone https://github.com/czoido/stereo-camera-visualization.git`
2. [Install conan](https://docs.conan.io/en/latest/installation.html)
3. Open the terminal, cd to the cloned directory and  `mkdir build`
4. `cd build` and  `conan install ..`

Windows:
```
$ cmake .. -G "Visual Studio 16"
$ cmake --build . --config Release
```
Linux or Mac:
```
$ cmake .. -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release
$ cmake --build .
```
You will probably have to change the id of your camera, it's harcoded in the sources as `deviceid` in main.cpp.
Also, make sure that shaders (vs, fs), config files (yml) and also plist (Mac) are in the working directory.