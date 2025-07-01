#include "Core.h"

Font LoadFont(path PathToFont)
{
    Font font;
    if (!font.openFromFile(PathToFont))
    {
        OutputLog("Error loading Font: " + PathToFont.string());
    }
    else
    {
        OutputLog("Font Loaded");
    }
    return font;
}
