#ifndef calib_image_sender_H
#define calib_image_sender_H

#include <ros/ros.h>
#include <cstdio>
#include <sensor_msgs/Image.h>
#include <cv_bridge/cv_bridge.h>
#include <image_transport/image_transport.h>

#include <kbhit.h>
#include <getch.h>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/calib3d.hpp>
   
class CalibImage{
public:
    CalibImage();
    ~CalibImage();
    void timerCallback(const ros::TimerEvent&);
   
private:
    ros::Timer timer_;
    int image_width_, image_height_;
	int left_device_num_, right_device_num_;
	std::string topic_left_, topic_right_;

    cv::VideoCapture cam_left_;
    cv::VideoCapture cam_right_;
    cv::Mat image_left_;
    cv::Mat image_right_;

    cv::Size image_size_;

    sensor_msgs::ImagePtr msg_left_, msg_right_;

    int count_;
    int send_no_;

    image_transport::Publisher leftcam_pub_;
	image_transport::Publisher rightcam_pub_;

    void startCapturing(cv::VideoCapture capture, int device_num);
    void capture();

};

#endif