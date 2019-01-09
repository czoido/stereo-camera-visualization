//
//  stereo_pair_processor.cpp
//  stereo-visualization
//
//  Created by Carlos Zoido on 09/01/2019.
//

#include "stereo_pair_processor.hpp"

StereoPairProcessor::~StereoPairProcessor(void)
{
}

StereoPairProcessor::StereoPairProcessor(const std::string& intrinsic_filename,const std::string& extrinsic_filename, const bool show_windows):
                                                            intrinsic_filename_(intrinsic_filename),
                                                            extrinsic_filename_(extrinsic_filename),
                                                            show_windows_(show_windows)
{
    if (show_windows_) {
        cv::namedWindow("camara",cv::WINDOW_KEEPRATIO);
        cv::namedWindow("disparity",cv::WINDOW_AUTOSIZE);
        
        cv::createTrackbar( "trackbar num disparities", "disparity", &num_disparities_, disparities_max_,
                           +[](int value, void* userdata)
                           {
                               static_cast<StereoPairProcessor*>(userdata)->on_trackbar_number_disparities(value);
                           },
                           this
                           );
        
        cv::createTrackbar( "trackbar block size", "disparity", &block_size_, block_size_max_,
                           +[](int value, void* userdata)
                           {
                               static_cast<StereoPairProcessor*>(userdata)->on_trackbar_disparity_window_size(value);
                           },
                           this
                           );

    }
    InitMatrices();
}

void StereoPairProcessor::on_trackbar_number_disparities(int value)
{
    std::cout << "new number of disparities: " << num_disparities_ << std::endl;
    if (num_disparities_<1)
        num_disparities_ = 1;
}

void StereoPairProcessor::on_trackbar_disparity_window_size(int value)
{
    block_size_ = (block_size_ % 2 == 1) ?  block_size_: block_size_+1;
    std::cout << "new block size: " << block_size_ << std::endl;
}

void StereoPairProcessor::InitMatrices() {
    if( !intrinsic_filename_.empty())
    {
        cv::FileStorage intrinsic_parameters_file(intrinsic_filename_, cv::FileStorage::READ);
        if(!intrinsic_parameters_file.isOpened())
        {
            std::cout << "failed to open: " << intrinsic_filename_ << std::endl;
        }
        intrinsic_parameters_file["M1"] >> cam_mat_left_;
        intrinsic_parameters_file["D1"] >> cam_distort_left_;
        intrinsic_parameters_file["M2"] >> cam_mat_right_;
        intrinsic_parameters_file["D2"] >> cam_distort_right_;
        cam_mat_left_ *= scale_;
        cam_mat_right_ *= scale_;
    }
    if( !extrinsic_filename_.empty()) {
        cv::FileStorage extrinsic_parameters_file(extrinsic_filename_, cv::FileStorage::READ);
        if(!extrinsic_parameters_file.isOpened())
        {
            std::cout << "failed to open: " << extrinsic_filename_ << std::endl;
        }
        extrinsic_parameters_file["R"] >> rot_mat_left_to_right_;
        extrinsic_parameters_file["T"] >> trans_vec_left_to_right_;
    }
}

std::tuple<cv::Mat, cv::Mat> StereoPairProcessor::CalcPointCloud(const cv::Mat& stereo_camera_frame) {

    if (show_windows_)
        imshow("camara", stereo_camera_frame);
    
    // extract left and right images
    int startX=0;
    int startY=0;
    int width=stereo_camera_frame.size().width/2;
    int height=stereo_camera_frame.size().height;
    cv::Mat left_image_roi(stereo_camera_frame, cv::Rect(startX,startY,width,height));
    cv::Mat left_image;
    left_image_roi.copyTo(left_image);
    startX=startX+width;
    startY=0;
    cv::Mat right_image_roi(stereo_camera_frame, cv::Rect(startX,startY,width,height));
    cv::Mat right_image;
    right_image_roi.copyTo(right_image);

    // scale images
    if (scale_ != 1.f)
    {
        cv::Mat temp1, temp2;
        int method = scale_ < 1 ? cv::INTER_AREA : cv::INTER_CUBIC;
        resize(left_image, temp1, cv::Size(), scale_, scale_, method);
        left_image = temp1;
        resize(right_image, temp2, cv::Size(), scale_, scale_, method);
        right_image = temp2;
    }
    
    // calculate correction maps if they are not previously calculated
    cv::Size img_size = left_image.size();
    if (map_left1_.empty()) {
        cv::Rect roi1, roi2;
        stereoRectify( cam_mat_left_, cam_distort_left_, cam_mat_right_, cam_distort_right_, img_size, rot_mat_left_to_right_, trans_vec_left_to_right_, rectification_transform_left_, rectification_transform_right_, projection_matrix_left_, projection_matrix_right_, disparity_to_depth_mapping_, cv::CALIB_ZERO_DISPARITY,-1, img_size, &roi1, &roi2 );

        initUndistortRectifyMap(cam_mat_left_, cam_distort_left_, rectification_transform_left_, projection_matrix_left_, img_size, CV_16SC2, map_left1_, map_left2_);
        initUndistortRectifyMap(cam_mat_right_, cam_distort_right_, rectification_transform_right_, projection_matrix_right_, img_size, CV_16SC2, map_right1_, map_right2_);
    }
    
    // remap images
    cv::Mat left_image_remap, right_image_remap;
    remap(left_image, left_image_remap, map_left1_, map_left2_, cv::INTER_LINEAR);
    remap(right_image, right_image_remap, map_right1_, map_right2_, cv::INTER_LINEAR);
    left_image = left_image_remap;
    right_image = right_image_remap;
    

    // run the the modified H. Hirschmuller algorithm
    //MODE_SGBM = 0,
    //MODE_HH = 1,
    //MODE_SGBM_3WAY = 2,
    //MODE_HH4 = 3
    if (stereo_algorithm_ == nullptr) {
        stereo_algorithm_ = cv::StereoSGBM::create();
        int stereo_mode = cv::StereoSGBM::MODE_SGBM;
        int num_channels = left_image.channels();
        stereo_algorithm_->setNumDisparities(16*num_disparities_);
        stereo_algorithm_->setBlockSize(block_size_);
        stereo_algorithm_->setP1(8*num_channels*block_size_*block_size_);
        stereo_algorithm_->setP2(32*num_channels*block_size_*block_size_);
        stereo_algorithm_->setDisp12MaxDiff(-1);
        //stereo_algorithm_->setPreFilterCap(63);
        stereo_algorithm_->setUniquenessRatio(5);
        //stereo_algorithm_->setSpeckleWindowSize(100);
        //stereo_algorithm_->setSpeckleRange(32);
        stereo_algorithm_->setMode(stereo_mode);
    }
    
    cv::Mat disparity_image, disparity_image_8bit;
    stereo_algorithm_->compute(left_image, right_image, disparity_image);

    disparity_image.convertTo(disparity_image_8bit, CV_8U, 255/(num_disparities_*256.));
    
    if (show_windows_)
        imshow("disparity", disparity_image_8bit);
    
    cv::Mat xyz;
    reprojectImageTo3D(disparity_image, xyz, disparity_to_depth_mapping_, true);
    return std::make_tuple(xyz,left_image);
}
