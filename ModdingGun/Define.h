#pragma once

#include "Core.h"

// используем 1 поток?
#define USING_ONE_THREAD false

#define DEBUG

#ifdef  DEBUG
#define OUTPUT_LOG(__StringMessage__) OutputMessage(__StringMessage__)
#define DEBUG_CODE(__DebugCode__) __DebugCode__

#elif
#define OUTPUT_LOG(__StringMessage__)
#define DEBUG_CODE(__DebugCode__)

#endif
