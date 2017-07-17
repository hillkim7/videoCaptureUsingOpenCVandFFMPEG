#pragma once
/*
* Copyright(c) 2017 hillkim7@gmail.com
*/
#include <stdint.h>

typedef enum
{
	kError_None = 0,
	kError_NotOpen,
	kError_AlreayOpen,
	kError_AlreayPlay,
	kError_Failed
} VideoCapError_t;

typedef enum
{
	kCap_None = 0,
	kCap_Open,
	kCap_Play
} VideoCapStatus_t;

typedef struct
{
	size_t len;		// length of ES data
	uint8_t* es;	// video ES data
	int stride;		// video lines
	int line_size;	// video lines
} VideoData_t;

typedef void (*CapLogPrint_t)(const char *log_str);

typedef void (*CapCallback_t)(uint32_t device_no, const VideoData_t* video);
