#include "Core.h"

string GetCurrentTimeMG()
{
    time_t seconds = time(NULL);
    tm timeinfo;
    localtime_s(&timeinfo, &seconds);
    char buffer[50];
    strftime(buffer, sizeof(buffer), "[%Y-%m-%d %H:%M:%S] ", &timeinfo);

    return string(buffer);
}