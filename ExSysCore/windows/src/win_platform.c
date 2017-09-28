/*
 * win_platform.c
 *
 *  Created on: 12 сент. 2017 г.
 *      Author: Ded013
 */

#include "platform.h"
#include <windows.h>

LOCAL LARGE_INTEGER perf_freq;

uint8_t init_time_counter()
{
	return QueryPerformanceFrequency(&perf_freq);
}

uint32_t get_ms_time()
{
	if (perf_freq.QuadPart == 0) return 0;
	LARGE_INTEGER perf_count;
	if (!QueryPerformanceCounter(&perf_count)) return 0;
	return perf_count.QuadPart / perf_freq.QuadPart * 1000;
}
