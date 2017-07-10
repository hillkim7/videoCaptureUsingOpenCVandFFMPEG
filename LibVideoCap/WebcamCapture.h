#pragma once
/*
* Copyright(c) 2017 hillkim7@gmail.com
*/

#include "CaptureImpl.h"

#include <memory>

// forward declaration of cv::VideoCapture
namespace cv
{
	class VideoCapture;
}

class WebcamCapture : public CaptureImpl
{
public:
	WebcamCapture();

	virtual ~WebcamCapture();

	virtual int Init(int device_no, CapLogPrint_t print_func) override;

	virtual int Open(uint32_t width, uint32_t height, uint32_t fps, VideoCaptureCallback_t callback) override;

	virtual int Play() override;

	virtual int Stop() override;

	virtual int Close() override;

	virtual int Cleanup() override;

	virtual VideoCapStatus_t GetStatus() const override;

private:
	void GrabVideo();

protected:
	std::unique_ptr<cv::VideoCapture> cv_cap_;
};

