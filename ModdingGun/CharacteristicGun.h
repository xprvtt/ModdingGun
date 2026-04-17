#pragma once

#include "Core.h"
#include "GunStats.h"




class CharacteristicGun
{
public:

	CharacteristicGun(const CharacteristicGun& other);

	CharacteristicGun(vector<int> MaxStatPosition);

	bool isEmpty()  const;

	bool load(path PathToInfo, wstring namegun);

	void clear();

	bool upgradeStat(int CharacteristicGun_in_NAMESTAT);

	/// <summary>
	/// получить шанс услучшения ДЛЯ УЛУЧШЕНИЯ НА СЛЕД ПОЗИЦИЮ характеристики
	/// </summary>
	/// <param name="stat">GunStats::</param>
	/// <returns></returns>
	double getChanceForNextStat(size_t stat) const;

	/// <summary>
	/// получить вектор уменьшения характеристик, для текущей статы
	/// </summary>
	/// <param name="stat">GunStats::</param>
	/// <returns></returns>
	vector<int> getDecreaseForCurrentStat(size_t stat) const;

	/// <summary>
	/// Сделать шаг назад для статы, с учетом уменьшеных значений
	/// </summary>
	/// <param name="stat">GunStats::</param>
	/// <returns></returns>
	bool stepBack();

	/// <summary>
	/// получить визуальное отображение процента прибавки
	/// </summary>
	/// <param name="stat">GunStats::</param>
	/// <returns></returns>
	double getVisualPercentUpgradeCharacteristic(size_t stat) const;

	/// <summary>
	/// вернуть начальные характеристики
	/// </summary>
	/// <returns></returns>
	bool returnDefaultPosition();

	/// <summary>
	/// получить стандартные позиции
	/// </summary>
	/// <returns></returns>
	vector<int> getDefaultPosition() const;

	/// <summary>
	/// получить максимально возможные позиции
	/// </summary>
	/// <returns></returns>
	vector<int> getMaxPositionCharacteristic() const;

	/// <summary>
	/// получить максимальный визуальный процент для статы
	/// </summary>
	/// <param name="stat">GunStats::</param>
	/// <returns></returns>
	double getMaxStatVisualPercent(size_t stat) const;

	/// <summary>
	/// получить вектор отображаемых процентов для текущей статы
	/// </summary>
	/// <returns></returns>
	vector<double> getFullCurrentVisualStat() const;

	/// <summary>
	/// получить текущую позицию
	/// </summary>
	/// <returns></returns>
	vector<int> getCurrentPosition() const;

	/// <summary>
	/// 
	/// </summary>
	double getValueCharacteristic(size_t stat) const;

private:
	struct AllStat
	{
		/// <summary>
		///  текущий индекс ОТНОСИТЕЛЬНО МАКСИМАЛЬНОГО РАЗМЕРА
		/// </summary>
		int    m_realPosition;

		/// <summary>
		/// параметр ВИДИМЫЙ \\\ например кучность 1.2 \ темп огня 650 и тд.....
		/// </summary>
		double m_valueCharacteristic;

		/// <summary>
		/// текуший РЕАЛЬНЫЙ шанс НА УЛУЧШЕНИЕ
		/// </summary>
		double m_currentChance;

		/// <summary>
		/// текущий ВИЗУАЛЬНЫЙ процент
		/// </summary>
		double m_visualPercentStat;
	};

	struct DecreaseStatST
	{
		/// <summary>
		///  что уменьшать?
		/// </summary>
		int  m_positionLower;

		/// <summary>
		///  насколько уменьшать?
		/// </summary>
		int  m_howMany;

		/// <summary>
		/// индекс текущей статы от которого начнем уменьшать
		/// </summary>
		int m_positionStartLower;

	};

	/// <summary>
	/// <para>v1 - характеристика</para>
	/// <para>v2 - позиция</para>	/// 
	/// <para>0 - кучность вектор возможных стат</para> 
	/// <para>1 - темп</para>
	/// <para>2 - отдача</para>
	/// <para>3 - качание</para>
	/// <para>4 - пробитие</para>
	/// <para>5 - отказ грязи</para>
	/// <para>6 - отказ состояния</para>
	/// </summary>
	vector<vector<AllStat>> m_fullAllStat;

	/// <summary>
	/// <para>0 - кучность </para>
	/// <para>1 - темп</para>
	/// <para>2 - отдача</para>
	/// <para>3 - качание</para>
	/// <para>4 - пробитие</para>
	/// <para>5 - отказ грязи</para>
	/// <para>6 - отказ состояния</para>
	/// </summary>
	vector<vector<DecreaseStatST>> m_fullDecreaseStat;

	/// <summary>
	/// текущая позиция статов</para>
	/// <para>0 - кучность </para>
	/// <para>1 - темп</para>
	/// <para>2 - отдача</para>
	/// <para>3 - качание</para>
	/// <para>4 - пробитие</para>
	/// <para>5 - отказ грязи</para>
	/// <para>6 - отказ состояния</para>
	/// </summary>
	vector<int> m_currentStatPosition;

	/// <summary>
	/// максимальная позиция статов</para>
	/// <para>0 - кучность </para>
	/// <para>1 - темп</para>
	/// <para>2 - отдача</para>
	/// <para>3 - качание</para>
	/// <para>4 - пробитие</para>
	/// <para>5 - отказ грязи</para>
	/// <para>6 - отказ состояния</para>
	/// </summary>
	vector<int> m_maxStatPosition;

	/// <summary>
	/// начальная позиция статов</para>
	/// <para>0 - кучность </para>
	/// <para>1 - темп</para>
	/// <para>2 - отдача</para>
	/// <para>3 - качание</para>
	/// <para>4 - пробитие</para>
	/// <para>5 - отказ грязи</para>
	/// <para>6 - отказ состояния</para>
	/// </summary>
	vector<int> m_defaultStatPosition;

	/// <summary>
	/// запись всех апгрейдов
	/// </summary>
	vector<vector<int>> m_upgradeHistory;

	unsigned int m_countOption;

	bool m_empty;

	bool upStat(int statNo);	

};