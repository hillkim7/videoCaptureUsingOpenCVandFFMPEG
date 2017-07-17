#pragma once
/*
* Copyright(c) 2017 hillkim7@gmail.com
*/

#include "VideoCapDef.h"

class EncodeImpl
{
public:
	EncodeImpl();
	virtual ~EncodeImpl();

public:
	virtual int Init(CapLogPrint_t print_func);
	virtual int Open(const char* output, uint32_t width, uint32_t height, uint32_t fps) = 0;
	virtual int EncodeWrite(const uint8_t* data, int stride, int line_size) = 0;
	virtual int Close() = 0;
	virtual int Cleanup() = 0;

protected:
	void Print(const char *fmt, ...);

protected:
	CapLogPrint_t print_func_;
};

