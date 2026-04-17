#include "Core.h"

string wstringToString(const wstring& wstr) 
{
    if (wstr.empty()) return {};

    int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, wstr.data(), (int)wstr.size(), nullptr, 0, nullptr, nullptr);
    string strTo(sizeNeeded, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.data(), (int)wstr.size(), &strTo[0], sizeNeeded, nullptr, nullptr);

    return strTo;
}