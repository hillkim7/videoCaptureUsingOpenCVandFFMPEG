/*
* Copyright(c) 2017 hillkim7@gmail.com
*/
#include "CaptureFactory.h"
#include "WebcamCapture.h"
#include "FFMPEGencoder.h"

CaptureImpl* CaptureFactory::CreateVideoCapture(const char* capture_device)
{
	CaptureImpl* impl = new WebcamCapture();

	return impl;
}

EncodeImpl* CaptureFactory::CreateVideoEncoder(const char* enocoder)
{
	EncodeImpl* impl = new FFMPEGencoder();

	return impl;
}
