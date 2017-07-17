/*
* Copyright(c) 2017 hillkim7@gmail.com
*/

#include <stdio.h>
#include <stdint.h>
#include <memory>
#include "CaptureImpl.h"
#include "EncodeImpl.h"
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

int main(int ac, char* av[])
{
	if (ac < 3)
	{
		printf("%s <device_no> <file_name>\n");

		return 0;
	}

	uint32_t device_no = (uint32_t)atoi(av[1]);
	const char* file_name = av[2];

	std::unique_ptr<CaptureImpl> capture(CaptureFactory::CreateVideoCapture("webcam"));
	std::shared_ptr<EncodeImpl> encoder(CaptureFactory::CreateVideoEncoder("ffmpeg"));

	capture->Init(device_no, print_message);
	encoder->Init(print_message);

	auto video_capture_func = [encoder](CaptureImpl* instance, VideoData_t* video_data)
	{
		printf("[%d]video size=%d\n", instance->GetDeviceNo(), video_data->len);
		encoder->EncodeWrite(video_data->es, video_data->stride, video_data->line_size);
	};

	if (encoder->Open(file_name, video_width, video_height, video_fps) != 0)
	{
		printf("encoder open failed:\n");
		return 1;
	}

	if (capture->Open(video_width, video_height, video_fps, video_capture_func) != 0)
	{
		printf("capture open failed:\n");
		return 1;
	}

	char input[12];

	printf("enter to start program and enter again to quit\n");
	fgets(input, sizeof(input), stdin);

	if (capture->Play() != 0)
	{
		printf("capture play failed:\n");
		return 1;
	}

	fgets(input, sizeof(input), stdin);

	capture->Cleanup();
	encoder->Cleanup();

	return 0;
}

