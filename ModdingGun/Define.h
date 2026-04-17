#pragma once

#include "Core.h"

// используем 1 поток?
#define USING_ONE_THREAD false

#define DEBUG

#ifdef  DEBUG
	#define OUTPUT_LOG(__STRING_MESSAGE__) outputMessage(__STRING_MESSAGE__)
	#define DEBUG_CODE(__DEBUG_CODE__) __DEBUG_CODE__

#elif
	#define OUTPUT_LOG(__STRING_MESSAGE__)
	#define DEBUG_CODE(__DEBUG_CODE__)

#endif
