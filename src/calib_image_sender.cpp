#include "stereo_calibration_ros/calib_image_sender.h"
   
CalibImage::CalibImage(){
    ros::NodeHandle nh("~");
	ros::Rate looprate(30);

	nh.param("image_width", image_width_, 1920);
	nh.param("image_height", image_height_, 1080);
	nh.param("camera_topic_left", topic_left_, std::string("/left_camera"));
	nh.param("camera_topic_right", topic_right_, std::string("/right_camera"));
	nh.param("device_num_left", left_device_num_, 0);
	nh.param("device_num_right", right_device_num_, 1);

	image_transport::ImageTransport it(nh);
	leftcam_pub_ = it.advertise(topic_left_, 1);
	rightcam_pub_ = it.advertise(topic_right_, 1);
    timer_ = nh.createTimer(ros::Duration(0.05), &CalibImage::timerCallback, this);

	cv::Size image_size_(image_width_, image_height_);

	startCapturing(cam_left_, left_device_num_);
	startCapturing(cam_right_, right_device_num_);

	count_ = 0;
	send_no_ = 0;
    
}

CalibImage::~CalibImage(){

}

void CalibImage::timerCallback(const ros::TimerEvent&){
    if ((kbhit() && getche() == 's') || send_no_ != 0) {
		capture();

		msg_left_ = cv_bridge::CvImage(std_msgs::Header(), "bgr8", image_left_).toImageMsg();
		msg_right_ = cv_bridge::CvImage(std_msgs::Header(), "bgr8", image_right_).toImageMsg();

		ros::Time ts = ros::Time::now();
		msg_left_->header.stamp = ts;
		msg_right_->header.stamp = ts;

		// パブリッシュ
		leftcam_pub_.publish(msg_left_);
		rightcam_pub_.publish(msg_right_);

		if (send_no_ == 0)
			ROS_INFO("publishing image %d", ++count_);
		send_no_ = (send_no_ + 1) % 5;
	}
}

void CalibImage::startCapturing(cv::VideoCapture capture, int device_num)
{
	capture.open(device_num);
	if (!capture.isOpened())
		printf("Error. Falid to open camera.");
	else
		printf("[/dev/video%d] Start capturing.", device_num);
	capture.set(cv::CAP_PROP_FRAME_WIDTH, image_size_.width);
	capture.set(cv::CAP_PROP_FRAME_HEIGHT, image_size_.height);
}

void CalibImage::capture()
{
	cam_left_ >> image_left_;
	cam_right_ >> image_right_;
}

int main (int argc, char **argv)
{
	ros::init (argc, argv, "calib_image_sender");
			
	ros::spin();

	return 0;
}
