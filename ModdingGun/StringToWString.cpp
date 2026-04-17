#include "Core.h"

wstring stringToWString(const string& str)
{
    if (str.empty()) { return L""; }

    int sizeNeeded = MultiByteToWideChar(
        CP_UTF8,            // Кодировка исходной строки (UTF-8)
        0,
        str.c_str(),
        (int)str.size(),
        NULL,
        0
    );

    wstring wstrTo(sizeNeeded, 0);

    MultiByteToWideChar(
        CP_UTF8,
        0,
        str.c_str(),
        (int)str.size(),
        &wstrTo[0],
        sizeNeeded
    );

    return wstrTo;
}

