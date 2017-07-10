/*
* Copyright(c) 2017 hillkim7@gmail.com
*/

#include "VideoCap.h"
#include "CaptureImpl.h"
#include "CaptureFactory.h"

#include <cassert>
#include <memory>

static CapLogPrint_t _log_func;

struct CAP_STRUCT
{
	std::unique_ptr<CaptureImpl> cap_inst;
};

#define DPRT _log_func

int vcap_init(CapLogPrint_t log_func)
{
	_log_func = log_func;
	return 0;
}


CapHandle_t vcap_open(const char* device, uint32_t device_no
	, CapCallback_t callback, int width, int height, int fps)
{
	CapHandle_t hcap = new CAP_STRUCT;

	hcap->cap_inst.reset(CaptureFactory::CreateVideoCapture(device));
	hcap->cap_inst->Init(device_no, _log_func);

	auto capture_func = [device_no, callback](CaptureImpl* instance, VideoData_t* video_data) {
		callback(device_no, video_data);
	};

	if (!hcap->cap_inst->Open(width, height, fps, capture_func))
	{
		delete hcap;
		return nullptr;
	}

	return hcap;
}

int vcap_play(CapHandle_t hcap)
{
	assert(hcap != nullptr);
	return hcap->cap_inst->Play();
}

int vcap_stop(CapHandle_t hcap)
{
	assert(hcap != nullptr);
	return hcap->cap_inst->Stop();
}

void vcap_close(CapHandle_t hcap)
{
	assert(hcap != nullptr);
	hcap->cap_inst->Cleanup();
	delete hcap;
}
