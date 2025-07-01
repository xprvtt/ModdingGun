#pragma once
#include "Core.h"

namespace ThemeGui
{
	/// <summary>
	/// string    - название темы
	/// <para> v0 - цвет текста </para>
	/// 
	/// <para> v1 - цвет OutlineRectangle </para>
	/// <para> v2 - цвет FillColorRectangle </para>
	/// 
	/// <para> v3 - цвет кнопки         OutlineRectangle </para> 
	/// <para> v4 - цвет нажатой кнопки OutlineRectangle </para> 
	/// 
	/// </summary>
	extern const map< string_view, array<Color, 5>> AllTheme;


    extern const unsigned int CharacterSize;

    extern const float OutlineThicknessSize;

	extern const Font CurrentFont;
}