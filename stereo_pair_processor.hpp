//
//  stereo_pair_processor.hpp
//  stereo-visualization
//
//  Created by Carlos Zoido on 09/01/2019.
//

#ifndef stereo_pair_processor_hpp
#define stereo_pair_processor_hpp

#include <tuple>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/opencv.hpp>

class StereoPairProcessor {
public:
    StereoPairProcessor(const std::string& intrinsic_filename="intrinsics.yml",const std::string& extrinsic_filename="extrinsics.yml",const bool show_windows=true);
    ~StereoPairProcessor(void);
    // returns <points,image>
    std::tuple<cv::Mat,cv::Mat> CalcPointCloud(const cv::Mat& stereo_camera_frame);
    
private:
    void InitMatrices();
    void on_trackbar_number_disparities(int value);
    void on_trackbar_disparity_window_size(int value);
    cv::Ptr<cv::StereoSGBM> stereo_algorithm_ = nullptr;
    const std::string intrinsic_filename_;
    const std::string extrinsic_filename_;
    const bool show_windows_;
    const int disparities_max_ = 20;
    const int block_size_max_ = 15;
    cv::Mat cam_mat_left_;
    cv::Mat cam_distort_left_;
    cv::Mat cam_mat_right_;
    cv::Mat cam_distort_right_;
    cv::Mat rectification_transform_left_;
    cv::Mat projection_matrix_left_;
    cv::Mat rectification_transform_right_;
    cv::Mat projection_matrix_right_;
    cv::Mat rot_mat_left_to_right_;
    cv::Mat trans_vec_left_to_right_;
    cv::Mat map_left1_, map_left2_, map_right1_, map_right2_;
    cv::Mat disparity_to_depth_mapping_;
    const float scale_ = 0.25;
    int block_size_ = 9;
    int num_disparities_ = 3;
};


#endif /* stereo_pair_processor_hpp */
