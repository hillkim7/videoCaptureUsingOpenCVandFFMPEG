/*
* Copyright(c) 2017 hillkim7@gmail.com
*/
#include "CaptureImpl.h"
#include <iostream>
#include <cstdarg>
#include <vector>

CaptureImpl::CaptureImpl()
	: device_no_(0)
	, width_(1280)
	, height_(720)
	, fps_(25)
	, print_func_(nullptr)
	, stop_grabbing_(false)
{

}

CaptureImpl::~CaptureImpl()
{
}

int CaptureImpl::Init(int device_no, CapLogPrint_t print_func)
{
	device_no_ = device_no;
	print_func_ = print_func;

	return kError_None;
}

void CaptureImpl::Print(const char *fmt, ...)
{
	const size_t buf_size = 1023;
	char buf[buf_size + 1] = { 0 };

	va_list args1;
	va_start(args1, fmt);
	std::vsnprintf(buf, buf_size, fmt, args1);
	va_end(args1);
	print_func_(buf);
}
