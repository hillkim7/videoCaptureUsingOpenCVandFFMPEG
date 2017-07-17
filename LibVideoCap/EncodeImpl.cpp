/*
* Copyright(c) 2017 hillkim7@gmail.com
*/
#include "EncodeImpl.h"
#include <iostream>
#include <cstdarg>

EncodeImpl::EncodeImpl()
	: print_func_(nullptr)
{

}

EncodeImpl::~EncodeImpl()
{
}

int EncodeImpl::Init(CapLogPrint_t print_func)
{
	print_func_ = print_func;

	return kError_None;
}

void EncodeImpl::Print(const char *fmt, ...)
{
	const size_t buf_size = 1023;
	char buf[buf_size + 1] = { 0 };

	va_list args1;
	va_start(args1, fmt);
	std::vsnprintf(buf, buf_size, fmt, args1);
	va_end(args1);
	print_func_(buf);
}
