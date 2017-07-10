/*
* Copyright(c) 2017 hillkim7@gmail.com
*/

#include <stdio.h>
#include <stdint.h>
#include "CaptureImpl.h"
#include "CaptureFactory.h"

#ifdef _DEBUG
#pragma comment(lib,"opencv_world320d.lib")
#else
#pragma comment(lib,"opencv_world320.lib")
#endif  // _DEBUG

#pragma comment(lib,"avcodec.lib")
#pragma comment(lib,"avutil.lib")
#pragma comment(lib,"avformat.lib")
#pragma comment(lib,"avfilter.lib")
#pragma comment(lib,"swscale.lib")
#pragma comment(lib,"avdevice.lib")

const uint32_t video_width = 640;
const uint32_t video_height = 480;
const uint32_t video_fps = 25;

void print_message(const char* msg)
{
	printf(msg);
	printf("\n");
}

void on_viceo_capture(CaptureImpl* instance, VideoData_t* video_data)
{
	printf("[%d]video size=%d\n", instance->GetDeviceNo(), video_data->len);
}

int main(int ac, char* av[])
{
	if (ac < 3)
	{
		printf("%s <device_no> <file_name>\n");

		return 0;
	}

	uint32_t device_no = (uint32_t)atoi(av[1]);
	const char* file_name = av[2];

	CaptureImpl* capture = CaptureFactory::CreateVideoCapture("webcam");

	capture->Init(device_no, print_message);

	if (capture->Open(video_width, video_height, video_fps, on_viceo_capture) != 0)
	{
		printf("capture open failed:\n");
		delete capture;
		return 1;
	}

	char input[12];

	printf("enter to start program and enter again to quit\n");
	fgets(input, sizeof(input), stdin);

	if (capture->Play() != 0)
	{
		printf("capture play failed:\n");
		delete capture;
		return 1;
	}

	fgets(input, sizeof(input), stdin);

	capture->Cleanup();
	delete capture;

	return 0;
}

