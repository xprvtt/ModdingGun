#pragma once

#include "Core.h"
#include "CharacteristicGun.h"
#include "GunStats.h"
#include "ThreadAssemble.h"
#include "DrawGraph.h"
#include "LangManager.h"
#include "GulText.h"

struct GeneralStat;

//-----------------------------------------------------------------------------------------------------------------------

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
void editShaheUnits(std::vector<GeneralStat>& generalModOption, CharacteristicGun& currentCharacteristicGun, std::vector<int> initialPosition, std::vector<int> modPosition);

bool setChahceUpgrade(const CharacteristicGun& currentCharacteristicGun, const SelectModifiers& currentModifiers, GUITextAndRectangle& guiChance, size_t it);

unsigned getPrice(const path& pathToPriceModifiersJs, const SelectModifiers& currentModifiers);

unsigned getPrice(const path& pathToPriceModifiersJs, const SelectModifiers& currentModifiers, string nameGun);

bool setPriceTool(const path& pathToPriceModifiersJs, GunStats::Modifiers::ToolType tool, unsigned price);

bool setPriceKit(const path& pathToPriceModifiersJs, GunStats::Modifiers::KitType kit, string nameGun, unsigned price);

/// <summary>
/// получаем инфу о кол-вах модификаторов из CountModifiers
/// </summary>
/// <param name="MODIFIERS"></param>
/// <returns></returns>
vector<pair<string, unsigned long long>> GetInfoForModifiers(const CountModifiers& MODIFIERS);

//-----------------------------------------------------------------------------------------------------------------------

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
	/// кнопка для выбора
	/// </summary>
	GUITextAndRectangle    m_markButtonCharacteristic;

	/// <summary>
	/// Общий "прямоугольник" в котором распалагаютися все юниты
	/// </summary>
	RectangleShape         m_shapeCharacteristic;

	/// <summary>
	/// вектор с юнитами 60 \\\\\ 48 \\\\ 40  и тд.
	/// </summary>
	vector<RectangleShape> m_shaheUnitsInCharacteristic;
};

struct RectangleModifiers
{
	vector<GUITextAndRectangle> m_toolRectangleGUI;
	vector<GUITextAndRectangle> m_kitRectangleGUI;
	vector<GUITextAndRectangle> m_skillRectangleGUI;
};

struct Weapon
{
	GUITextAndRectangle m_weapoGUI;
	string m_name;
	shared_ptr<Texture> m_ptrTexture;
};

struct AllPackGraf
{
	vector< GUITextAndRectangle> m_GUIPercentBinBt{};
	vector< GUITextAndRectangle> m_GUIPercentSmoothBt{};
	float m_percentBin = 0.f;
	float m_percentSmooth = 0.f;

	map<CountModifiers, float> m_currentSmoothFreqModifiers{};
	map<CountModifiers, float> m_currentSourceFreqModifiers{};
	Color       m_colorCurve = Color::Black;
	CurvePack   m_currentCurvePack;

	vector<GUITextAndRectangle>     m_GUIPercentile50{};
	shared_ptr<GUITextAndRectangle> m_GUIInitialMod;
	shared_ptr<GUITextAndRectangle> m_GUIEndMod;

	shared_ptr<GUITextAndRectangle> m_GUIMethod;
	vector<GUITextAndRectangle>     m_GUIMethodModifiers{};
	shared_ptr<GUITextAndRectangle> m_GUIButtonTop;
	shared_ptr<GUITextAndRectangle> m_GUIButtonDown;

	size_t m_initialIndexMethod = 0;
};

//-----------------------------------------------------------------------------------------------------------------------