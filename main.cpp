//
//  main.cpp
//  stereo-visualization
//
//  Created by Carlos Zoido on 09/01/2019.
//

#include "stereo_pair_processor.hpp"
#include "opengl_viewer.hpp"

int main( int argc, const char** argv )
{
    int deviceid = 1;
    cv::VideoCapture capture;

    cv::namedWindow( "capture", cv::WINDOW_AUTOSIZE );

    if (!capture.isOpened())
        capture.open(deviceid);
    
    if (!capture.isOpened()) {
        std::cerr << "Failed to open the video device, video file or image sequence!\n" << std::endl;
        return 1;
    }


    OpenGLViewer viewer;
    StereoPairProcessor stereo_processor("intrinsics.yml","extrinsics.yml");
    cv::Mat frame;
    char key = -1;
    while (key != 27 && !viewer.ShouldClose()) {
        capture >> frame;
        if (frame.empty())
            break;
        else {
            std::tuple<cv::Mat,cv::Mat> point_cloud = stereo_processor.CalcPointCloud(frame);
            // returns a points list and an image
            viewer.RenderFrame(std::get<1>(point_cloud),std::get<0>(point_cloud));
        }
        key = (char)cv::waitKey(11);
    }
    viewer.terminate();
    return 0;
}

