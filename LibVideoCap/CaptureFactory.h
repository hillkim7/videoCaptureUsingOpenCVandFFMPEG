#pragma once
/*
* Copyright(c) 2017 hillkim7@gmail.com
*/

#include "CaptureImpl.h"

class CaptureFactory
{
public:

	static CaptureImpl* CreateVideoCapture(const char* capture_device);

};

