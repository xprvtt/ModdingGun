#pragma once

#include "Core.h"


#define DEBUG

#ifdef DEBUG
#define OutputLog(StringMessage) OutputMessage(StringMessage) 
#define DebugCode(d_code) d_code

#elif
#define Output (StringMessage)
#define DebugCode(d_code)
#endif
