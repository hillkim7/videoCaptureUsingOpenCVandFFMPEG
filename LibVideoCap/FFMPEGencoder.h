#pragma once
/*
* Copyright(c) 2017 hillkim7@gmail.com
*/

#include "VideoCapDef.h"
#include "EncodeImpl.h"

#include <vector>

// FFmpeg
extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libavutil/pixdesc.h>
#include <libswscale/swscale.h>
}

class FFMPEGencoder : public EncodeImpl
{
public:
	FFMPEGencoder();
	virtual ~FFMPEGencoder();

public:
	virtual int Init(CapLogPrint_t print_func);
	virtual int Open(const char* output, uint32_t width, uint32_t height, uint32_t fps) override;
	virtual int EncodeWrite(const uint8_t* data, int stride, int line_size) override;
	virtual int Close() override;
	virtual int Cleanup() override;

protected:
	AVFormatContext* outctx_ = nullptr;
	AVCodec* vcodec_ = nullptr;
	AVStream* vstrm_ = nullptr;
	SwsContext* swsctx_ = nullptr;
	AVFrame* frame_ = nullptr;
	std::vector<uint8_t> framebuf_;
	int64_t frame_pts_ = 0;
	size_t nb_frames_ = 0;

	int width_ = 0;
	int height_ = 0;
	int fps_ = 0;
};

