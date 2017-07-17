#pragma once
/*
* Copyright(c) 2017 hillkim7@gmail.com
*/

#include "CaptureImpl.h"
#include "EncodeImpl.h"

class CaptureFactory
{
public:

	static CaptureImpl* CreateVideoCapture(const char* capture_device);
	static EncodeImpl* CreateVideoEncoder(const char* enocoder);

};

