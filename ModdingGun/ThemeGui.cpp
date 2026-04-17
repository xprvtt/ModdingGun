#include "ThemeGui.h"

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
const map< string_view, array<Color, 5>> ThemeGui::allTheme =
{
	{ "Dark",   {Color(210,210,210),  Color(40, 40, 40), Color(10,10, 10),  Color(118, 255, 97),  Color(40, 40, 40) } },
	{ "White",  {Color::Black,  Color::Black, Color::White,  Color(118, 255, 97),  Color(40, 40, 40) } }
};

const unsigned int ThemeGui::characterSizeGUI = 14;

const float ThemeGui::outlineThicknessSizeGUI = -2;

const Font ThemeGui::currentFontGUI = []
	{

		path fontFolder = L"Font/";

		/// обнаруживаем все шрифты в папке 
		vector<path> searchFont = searchFile(fontFolder, ".ttf");

		/// если шрифтов не найдено выходим
		if (searchFont.empty())
		{
			OUTPUT_LOG("Theme -> Font not found, exit");
			exit(-1);
		}

		// используем первый в списке шрифт
		OUTPUT_LOG("Theme -> Font Loaded");
		OUTPUT_LOG("Theme -> Font: " + searchFont[0].string());

		return LoadFont(searchFont[0]);

	}();

