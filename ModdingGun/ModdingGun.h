#pragma once


#include "Core.h"

#include "CharacteristicGun.h"

#include "GunStats.h"

#include "ThreadAssemble.h"

#include "DrawGraph.h"

#include "LangManager.h"

#include "GulText.h"



///////////////////////////////////////////////////////////////////////////////////

/// <summary>
/// основные элементы главного окна отображения модов
/// </summary>
struct GeneralStat
{
    /// <summary>
    /// имя характеристики \ первое
    /// </summary>
    GUI_TextAndRectangle    TextCharacteristicName;

    /// <summary>
    /// отображение характеристики \ текущий темп . текущая отдача  . и тд \\\
    /// </summary>
    GUI_TextAndRectangle    TextVisualCharacteristic;

    /// <summary>
    /// визуальное отображения процента апгрейда
    /// </summary>
    GUI_TextAndRectangle    TextCharacteristicVisualPercent;





    /// <summary>
    /// Общий "прямоугольник" в котором распалагаютися все юниты
    /// </summary>
    RectangleShape         ShapeCharacteristic;

    /// <summary>
    /// вектор с юнитами 60 \\\\\ 48 \\\\ 40  и тд.
    /// </summary>
    vector<RectangleShape> ShaheUnitsInCharacteristic;





    /// <summary>
    /// кнопка для выбора
    /// </summary>
    GUI_TextAndRectangle    MarkButtonCharacteristic;
};
///////////////////////////////////////////////////////////////////////////////////




/// <summary>
/// перемешаем местами элементы из вектора характеристик CharacteristicGun и Обратно
/// </summary>
/// <typeparam name="T"></typeparam>
/// <param name="vec"> </param>
/// <param name="from"> | если из CharacteristicGun => 4 |   перевод обратно => 2 | </param>
/// <param name="to"> ---| если из CharacteristicGun => 2 |   перевод обратно => 4 |   </param>
template<typename T>
void moveElement(vector<T>& vec, size_t from, size_t to);

/// <summary>/// 
/// функция для изменения цветов юнитов характеристик
/// </summary>/// 
/// <param name="GeneralModOption"></param>
/// <param name="CurrentCharacteristicGun"></param>
/// <param name="InitialPosition"></param>
/// <param name="ModPosition"></param>
void EditShaheUnits(vector<GeneralStat>& GeneralModOption, CharacteristicGun& CurrentCharacteristicGun, vector<int> InitialPosition, vector<int> ModPosition);



bool Set_Chahce_upgrade(const CharacteristicGun& CurrentCharacteristicGun, const Select_Modifiers& CurrentModifiers, GUI_TextAndRectangle& GUI_Chance, size_t it);


unsigned Get_Price(const path& PathToPriceModifiers_JS, const Select_Modifiers& CurrentModifiers);
unsigned Get_Price(const path& PathToPriceModifiers_JS, const Select_Modifiers& CurrentModifiers, string NameGun);




bool Set_Price_Tool(const path& PathToPriceModifiers_JS, GunStats::Modifiers::ToolType Tool, unsigned Price);
bool Set_Price_Kit(const path& PathToPriceModifiers_JS, GunStats::Modifiers::KitType Kit, string nameGun, unsigned Price);






/// <summary>
/// получаем инфу о кол-вах модификаторов из CountModifiers
/// </summary>
/// <param name="MODIFIERS"></param>
/// <returns></returns>
vector<pair<string,unsigned long long>> GetInfoForModifiers(const CountModifiers& MODIFIERS);


