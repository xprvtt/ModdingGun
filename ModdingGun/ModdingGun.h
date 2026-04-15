#pragma once

#include "Core.h"
#include "CharacteristicGun.h"
#include "GunStats.h"
#include "ThreadAssemble.h"
#include "DrawGraph.h"
#include "LangManager.h"
#include "GulText.h"

/// <summary>
/// основные элементы главного окна отображения модов
/// </summary>
struct GeneralStat
{
    /// <summary>
    /// имя характеристики \ первое
    /// </summary>
    GUITextAndRectangle    m_textCharacteristicName;

    /// <summary>
    /// отображение характеристики \ текущий темп . текущая отдача  . и тд \\\
    /// </summary>
    GUITextAndRectangle    m_textVisualCharacteristic;

    /// <summary>
    /// визуальное отображения процента апгрейда
    /// </summary>
    GUITextAndRectangle    m_textCharacteristicVisualPercent;

    /// <summary>
    /// Общий "прямоугольник" в котором распалагаютися все юниты
    /// </summary>
    RectangleShape         m_shapeCharacteristic;

    /// <summary>
    /// вектор с юнитами 60 \\\\\ 48 \\\\ 40  и тд.
    /// </summary>
    vector<RectangleShape> m_shaheUnitsInCharacteristic;

    /// <summary>
    /// кнопка для выбора
    /// </summary>
    GUITextAndRectangle    m_markButtonCharacteristic;
};

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
/// <param name="generalModOption"></param>
/// <param name="currentCharacteristicGun"></param>
/// <param name="initialPosition"></param>
/// <param name="modPosition"></param>
void editShaheUnits(vector<GeneralStat>& generalModOption, CharacteristicGun& currentCharacteristicGun, vector<int> initialPosition, vector<int> modPosition);

bool setChahceUpgrade(const CharacteristicGun& currentCharacteristicGun, const Select_Modifiers& CurrentModifiers, GUITextAndRectangle& GUIChance, size_t it);

unsigned getPrice(const path& pathToPriceModifiersJS, const Select_Modifiers& currentModifiers);
unsigned getPrice(const path& pathToPriceModifiersJS, const Select_Modifiers& currentModifiers, string nameGun);

bool setPriceTool(const path& pathToPriceModifiersJS, GunStats::Modifiers::ToolType tool, unsigned price);
bool setsPriceKit(const path& pathToPriceModifiersJS, GunStats::Modifiers::KitType kit, string nameGun, unsigned price);

/// <summary>
/// получаем инфу о кол-вах модификаторов из CountModifiers
/// </summary>
/// <param name="Modifiers"></param>
/// <returns></returns>
vector<pair<string,unsigned long long>> getInfoForModifiers(const CountModifiers& Modifiers);
