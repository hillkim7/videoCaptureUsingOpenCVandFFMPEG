/*
* Copyright(c) 2017 hillkim7@gmail.com
*/

#include "WebcamCapture.h"

#include <opencv2/opencv.hpp>

WebcamCapture::WebcamCapture()
{
}

WebcamCapture::~WebcamCapture()
{
}

int WebcamCapture::Init(int device_no, CapLogPrint_t print_func)
{
	CaptureImpl::Init(device_no, print_func);

	return kError_None;
}

int WebcamCapture::Open(uint32_t width, uint32_t height, uint32_t fps, VideoCaptureCallback_t callback)
{
	if (IsOpened())
	{
		return kError_AlreayOpen;
	}

	cv_cap_.reset(new cv::VideoCapture(device_no_));

	if (!cv_cap_->isOpened())
	{
		Print("can't open webcam device: device=%d", device_no_);
		cv_cap_.reset();
		return kError_Failed;
	}

	cv_cap_->set(cv::CAP_PROP_FRAME_WIDTH, width);
	cv_cap_->set(cv::CAP_PROP_FRAME_HEIGHT, height);
	cv_cap_->set(cv::CAP_PROP_FPS, fps);

	width_ = (uint32_t)cv_cap_->get(cv::CAP_PROP_FRAME_WIDTH);
	height_ = (uint32_t)cv_cap_->get(cv::CAP_PROP_FRAME_HEIGHT);
	fps_ = (uint32_t)cv_cap_->get(cv::CAP_PROP_FPS);

	Print("screen=%dx%d fps=%d", width_, height_, fps_);

	capture_callback_ = callback;

	return kError_None;
}

int WebcamCapture::Play()
{
	if (!cv_cap_->isOpened())
	{
		Print("device isn't opened: device=%d", device_no_);
		return kError_NotOpen;
	}

	if (GetStatus() == kCap_Play)
	{
		Print("device already capturing: device=%d", device_no_);
		return kError_AlreayPlay;
	}

	grabber_.reset(new std::thread(&WebcamCapture::GrabVideo, this));
	
	return kError_None;
}

int WebcamCapture::Stop()
{
	if (GetStatus() != kCap_Play)
	{
		Print("device isn't capturing: device=%d", device_no_);
		return kError_AlreayPlay;
	}

	stop_grabbing_.store(true);
	grabber_->join();
	grabber_.reset();

	return kError_None;
}

int WebcamCapture::Close()
{
	if (GetStatus() == kCap_Play)
		Stop();

	cv_cap_.reset();

	return kError_None;
}

int WebcamCapture::Cleanup()
{
	Close();

	return kError_None;
}

VideoCapStatus_t WebcamCapture::GetStatus() const
{
	if (grabber_ != nullptr)
		return kCap_Play;
	else if (cv_cap_ != nullptr)
		return kCap_Open;
	else
		return kCap_None;
}

void WebcamCapture::GrabVideo()
{
	std::vector<uint8_t> imgbuf(height_ * width_ * 3 + 16);
	cv::Mat image(height_, width_, CV_8UC3, imgbuf.data(), width_ * 3);
	VideoData_t video_data;

	Print("capture thread start: device=%d", device_no_);

	video_data.len = imgbuf.size();

	while (!stop_grabbing_.load())
	{
		*cv_cap_ >> image;

		video_data.es = image.data;
		video_data.stride = static_cast<int>(image.step[0]);
		video_data.line_size = image.rows;
		capture_callback_(this, &video_data);
	}

	Print("capture thread finished: device=%d", device_no_);
}
