#pragma once

#include "Core.h"

// используем 1 поток?
#define OneThread false

#define DEBUG

#ifdef  DEBUG
#define OutputLog(StringMessage) OutputMessage(StringMessage)
#define DebugCode(d_code) d_code

#elif
#define OutputLog(StringMessage)
#define DebugCode(d_code)

#endif
