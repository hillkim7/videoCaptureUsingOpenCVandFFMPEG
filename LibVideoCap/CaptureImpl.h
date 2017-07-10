#pragma once
/*
* Copyright(c) 2017 hillkim7@gmail.com
*/

#include "VideoCapDef.h"

#include <memory>
#include <thread>
#include <atomic>
#include <functional>

class CaptureImpl
{
public:
	CaptureImpl();
	virtual ~CaptureImpl();

	using VideoCaptureCallback_t = std::function<void(CaptureImpl* instance, VideoData_t* video_data)>;

	bool IsOpened() const
	{
		return GetStatus() != kCap_None;
	}

	uint32_t GetDeviceNo() const
	{
		return device_no_;
	}

public:
	virtual int Init(int device_no, CapLogPrint_t print_func);
	virtual int Open(uint32_t width, uint32_t height, uint32_t fps, VideoCaptureCallback_t callback) = 0;
	virtual int Play() = 0;
	virtual int Stop() = 0;
	virtual int Close() = 0;
	virtual int Cleanup() = 0;
	virtual VideoCapStatus_t GetStatus() const = 0;

protected:
	void Print(const char *fmt, ...);

protected:
	int device_no_;
	uint32_t width_;
	uint32_t height_;
	uint32_t fps_;
	CapLogPrint_t print_func_;
	std::unique_ptr<std::thread> grabber_;
	std::atomic<bool> stop_grabbing_;
	VideoCaptureCallback_t capture_callback_;
};

