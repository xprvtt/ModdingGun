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
const map< string_view, array<Color, 5>> ThemeGui::AllTheme =
{
    { "Dark",   {Color(210,210,210),  Color(40, 40, 40), Color(10,10, 10),  Color(118, 255, 97),  Color(40, 40, 40) } },
    { "White",  {Color::Black,  Color::Black, Color::White,  Color(118, 255, 97),  Color(40, 40, 40) } }

};


    const unsigned int ThemeGui::CharacterSize = 14;

    const float ThemeGui::OutlineThicknessSize = -2;

    const Font ThemeGui::CurrentFont = []
        {

            path Font_Folder = L"Font/";

            ///////////////////////////////////////////////////////////////////////////////////
            /// обнаруживаем все шрифты в папке 
            vector<path> SearchFont = SearchFile(Font_Folder, ".ttf");

            /// если шрифтов не найдено выходим
            if (SearchFont.empty())
            {
                OutputLog("Theme -> Font not found, exit");
                exit(-1);
            }

            // используем первый в списке шрифт
            OutputLog("Theme -> Font Loaded");
            OutputLog("Theme -> Font: " + SearchFont[0].string());

            ///////////////////////////////////////////////////////////////////////////////////

            return LoadFont(SearchFont[0]);

        }();

