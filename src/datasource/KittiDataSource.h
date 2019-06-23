/* ----------------------------------------------------------------------------
 * Copyright 2017, Massachusetts Institute of Technology,
 * Cambridge, MA 02139
 * All Rights Reserved
 * Authors: Luca Carlone, et al. (see THANKS for the full author list)
 * See LICENSE for the license information
 * -------------------------------------------------------------------------- */

/**
 * @file   KittiDataSource.h
 * @brief  Kitti dataset parser.
 * @author Antoni Rosinol
 */

#pragma once

#include <functional>
#include <string>

#include "ImuFrontEnd.h"
#include "StereoImuSyncPacket.h"
#include "VioFrontEndParams.h"
#include "datasource/DataSource.h"

namespace VIO {

class KittiDataProvider : public DataProvider {
 public:
  KittiDataProvider(const std::string& kitti_dataset_path);
  virtual ~KittiDataProvider();
  virtual bool spin();

  inline ImuParams getImuParams() const { return kitti_data_.imuParams_; }

 private:
  struct KittiData {
    inline size_t getNumberOfImages() const { return left_img_names_.size(); }

    size_t initial_k_, final_k_;  // useful for skipping image frames
    // also since sometimes don't have enough imu measurements for first few
    // frames This matches the names of the folders in the dataset
    std::string left_camera_name_;
    std::string right_camera_name_;
    // Map from camera name to its parameters
    std::map<std::string, CameraParams> camera_info_;

    gtsam::Pose3 camL_Pose_camR_;  // relative pose between cameras

    // The image names of the images from left camera
    std::vector<std::string> left_img_names_;
    // The image names of the images from right camera
    std::vector<std::string> right_img_names_;
    // Vector of timestamps see issue in .cpp file
    std::vector<Timestamp> timestamps_;
    // IMU params
    ImuParams imuParams_;
    // IMU data
    ImuData imuData_;
    // Sanity check to ensure data is correctly parsed
    // (returns true if data is correct, false otherwise)
    explicit operator bool() const;
  };

 private:
  cv::Mat readKittiImage(const std::string& img_name);
  void parseData(const std::string& kitti_sequence_path, KittiData* kitti_data);

  // Parse the timestamps of a particular device of given dataset
  bool parseTimestamps(const std::string& timestamps_file,
                       std::vector<Timestamp>& timestamps_list) const;

  // Parse camera info of given dataset
  bool parseCameraData(const std::string& input_dataset_path,
                       const std::string& left_cam_name,
                       const std::string& right_cam_name,
                       KittiData* kitti_data);

  // Parse IMU data of a given dataset
  bool parseImuData(const std::string& input_dataset_path,
                    KittiData* kitti_data);

  // Get R and T matrix from calibration file
  bool parseRT(const std::string& input_dataset_path,
               const std::string& calibration_filename, cv::Mat& R,
               cv::Mat& T) const;

  void print() const;

 private:
  std::string dataset_path_;
  VioFrontEndParams frontend_params_;  // NOTE what to do with this?
  KittiData kitti_data_;
};

}  // namespace VIO
