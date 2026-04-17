#include "Core.h"

Font LoadFont(path PathToFont)
{
    Font font;
    if (!font.openFromFile(PathToFont))
    {
        OUTPUT_LOG("Error loading Font: " + PathToFont.string());
    }
    else
    {
        OUTPUT_LOG("Font Loaded");
    }
    return font;
}
