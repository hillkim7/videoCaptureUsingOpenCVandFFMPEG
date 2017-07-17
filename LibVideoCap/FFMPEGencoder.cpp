/*
* Copyright(c) 2017 hillkim7@gmail.com
*/
#include "FFMPEGencoder.h"
#include <iostream>

FFMPEGencoder::FFMPEGencoder()
{

}

FFMPEGencoder::~FFMPEGencoder()
{

}

int FFMPEGencoder::Init(CapLogPrint_t print_func)
{
	EncodeImpl::Init(print_func);

	// initialize FFmpeg library
	av_register_all();

	return kError_None;
}

int FFMPEGencoder::Open(const char* output, uint32_t width, uint32_t height, uint32_t fps)
{
	int ret;

	width_ = (int)width;
	height_ = (int)height;
	fps_ = (int)fps;

	const int dst_width = width_;
	const int dst_height = height_;
	const AVRational dst_fps = { fps_, 1 };

	// open output format context
	ret = avformat_alloc_output_context2(&outctx_, nullptr, nullptr, output);
	if (ret < 0)
	{
		Print("fail to avformat_alloc_output_context2(%s)", output);
		return kError_NotOpen;
	}

	// open output IO context
	ret = avio_open2(&outctx_->pb, output, AVIO_FLAG_WRITE, nullptr, nullptr);
	if (ret < 0)
	{
		avformat_free_context(outctx_);
		outctx_ = nullptr;
		Print("fail to avio_open2(%s)", output);
		return kError_NotOpen;
	}

	// create new video stream
	vcodec_ = avcodec_find_encoder(outctx_->oformat->video_codec);
	vstrm_ = avformat_new_stream(outctx_, vcodec_);
	if (!vstrm_) {
		std::cerr << "fail to avformat_new_stream";
		return kError_NotOpen;
	}
	avcodec_get_context_defaults3(vstrm_->codec, vcodec_);
	vstrm_->codec->width = width;
	vstrm_->codec->height = height;
	vstrm_->codec->pix_fmt = vcodec_->pix_fmts[0];
	vstrm_->codec->time_base = vstrm_->time_base = av_inv_q(dst_fps);
	vstrm_->r_frame_rate = vstrm_->avg_frame_rate = dst_fps;
	if (outctx_->oformat->flags & AVFMT_GLOBALHEADER)
		vstrm_->codec->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;

	// open video encoder
	ret = avcodec_open2(vstrm_->codec, vcodec_, nullptr);
	if (ret < 0) {
		std::cerr << "fail to avcodec_open2: ret=" << ret;
		return kError_NotOpen;
	}

	std::cout
		<< "outfile: " << output << "\n"
		<< "format:  " << outctx_->oformat->name << "\n"
		<< "vcodec:  " << vcodec_->name << "\n"
		<< "size:    " << dst_width << 'x' << dst_height << "\n"
		<< "fps:     " << av_q2d(dst_fps) << "\n"
		<< "pixfmt:  " << av_get_pix_fmt_name(vstrm_->codec->pix_fmt) << "\n"
		<< std::flush;

	// initialize sample scaler
	swsctx_ = sws_getCachedContext(
		nullptr, dst_width, dst_height, AV_PIX_FMT_BGR24,
		dst_width, dst_height, vstrm_->codec->pix_fmt, SWS_BICUBIC, nullptr, nullptr, nullptr);
	if (!swsctx_) {
		std::cerr << "fail to sws_getCachedContext";
		return 2;
	}

	// allocate frame buffer for encoding
	frame_ = av_frame_alloc();
	framebuf_.resize(avpicture_get_size(vstrm_->codec->pix_fmt, dst_width, dst_height));
	avpicture_fill(reinterpret_cast<AVPicture*>(frame_), framebuf_.data(), vstrm_->codec->pix_fmt, dst_width, dst_height);
	frame_->width = dst_width;
	frame_->height = dst_height;
	frame_->format = static_cast<int>(vstrm_->codec->pix_fmt);

	avformat_write_header(outctx_, nullptr);

	frame_pts_ = 0;
	nb_frames_ = 0;

	return kError_None;
}

int FFMPEGencoder::EncodeWrite(const uint8_t* data, int stride, int line_size)
{
	int ret;
	int got_pkt = 0;

	const int src_stride[] = { stride };
	sws_scale(swsctx_, &data, src_stride, 0, line_size, frame_->data, frame_->linesize);
	frame_->pts = frame_pts_++;

	AVPacket pkt;
	pkt.data = nullptr;
	pkt.size = 0;
	av_init_packet(&pkt);
	ret = avcodec_encode_video2(vstrm_->codec, &pkt, frame_, &got_pkt);
	if (ret < 0) {
		std::cerr << "fail to avcodec_encode_video2: ret=" << ret << "\n";
		return kError_Failed;
	}
	if (got_pkt) {
		// rescale packet timestamp
		pkt.duration = 1;
		av_packet_rescale_ts(&pkt, vstrm_->codec->time_base, vstrm_->time_base);
		// write packet
		av_write_frame(outctx_, &pkt);
		std::cout << nb_frames_ << '\r' << std::flush;  // dump progress
		++nb_frames_;
	}
	av_free_packet(&pkt);

	return kError_None;
}

int FFMPEGencoder::Close()
{
	av_write_trailer(outctx_);
	std::cout << nb_frames_ << " frames encoded" << std::endl;

	av_frame_free(&frame_);
	frame_ = nullptr;
	avcodec_close(vstrm_->codec);
	avio_close(outctx_->pb);
	avformat_free_context(outctx_);
	outctx_ = nullptr;

	return kError_None;
}

int FFMPEGencoder::Cleanup()
{
	if (outctx_ != nullptr)
	{
		Close();
	}

	return kError_None;
}
