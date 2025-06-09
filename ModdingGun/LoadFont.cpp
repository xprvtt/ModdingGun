#include "Core.h"

Font LoadFont(path PathToFont)
{
    Font font;
    if (!font.openFromFile(PathToFont))
    {
        OutputLog("Ошибка загрузки шрифта: " + PathToFont.string());
    }
    else
    {
        OutputLog("загружен шрифт");
    }
    return font;
}
