#pragma once
/*
* Copyright(c) 2017 hillkim7@gmail.com
*/
#include <stdint.h>

#include "VideoCapDef.h"

struct CAP_STRUCT;

typedef struct CAP_STRUCT* CapHandle_t;


#ifdef __cplusplus
extern "C" {
#endif

	int vcap_init(CapLogPrint_t log_func);

	CapHandle_t vcap_open(const char* device, uint32_t device_no, CapCallback_t callback, int width, int height, int fps);

	int vcap_play(CapHandle_t  handle);

	int vcap_stop(CapHandle_t  handle);

	void vcap_close(CapHandle_t  handle);
#ifdef __cplusplus
}
#endif
