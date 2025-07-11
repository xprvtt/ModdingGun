#pragma once

#include "Core.h"
#include "GunStats.h"




class CharacteristicGun
{
	struct AllStat
	{
		/// <summary>
		///  текущий индекс ОТНОСИТЕЛЬНО МАКСИМАЛЬНОГО РАЗМЕРА
		/// </summary>
		int    RealPosition;

		/// <summary>
		/// параметр ВИДИМЫЙ \\\ например кучность 1.2 \ темп огня 650 и тд.....
		/// </summary>
		double ValueCharacteristic;

		/// <summary>
		/// текуший РЕАЛЬНЫЙ шанс НА УЛУЧШЕНИЕ
		/// </summary>
		double CurrentChance;

		/// <summary>
		/// текущий ВИЗУАЛЬНЫЙ процент
		/// </summary>
		double VisualPercentStat;
	};

	struct DecreaseStatST
	{
		/// <summary>
		///  что уменьшать?
		/// </summary>
		int  PositionLower;

		/// <summary>
		///  насколько уменьшать?
		/// </summary>
		int  HowMany;

		/// <summary>
		/// индекс текущей статы от которого начнем уменьшать
		/// </summary>
		int PositionStartLower;

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
	vector<vector<AllStat>> FulAllStat;


	/// <summary>
	/// <para>0 - кучность </para>
	/// <para>1 - темп</para>
	/// <para>2 - отдача</para>
	/// <para>3 - качание</para>
	/// <para>4 - пробитие</para>
	/// <para>5 - отказ грязи</para>
	/// <para>6 - отказ состояния</para>
	/// </summary>
	vector<vector<DecreaseStatST>> FullDecreaseStat;

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
	vector<int> CurrentStatPosition;

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
	vector<int> MaxStatPosition;

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
	vector<int> DefaultStatPosition;

	/// <summary>
	/// запись всех апгрейдов
	/// </summary>
	vector<vector<int>> UpgradeHistory;

	unsigned int CountOption;

	bool Empty;

	bool upStat(int stat_No);


public:

	CharacteristicGun(const CharacteristicGun& other);

	CharacteristicGun(vector<int> MaxStatPosition);

	bool is_Empty()  const;

	bool load(path PathToInfo, wstring namegun);

	void clear();

	bool upgradeStat(int CharacteristicGun_in_NAMESTAT);

	/// <summary>
	/// получить шанс услучшения ДЛЯ УЛУЧШЕНИЯ НА СЛЕД ПОЗИЦИЮ характеристики
	/// </summary>
	/// <param name="stat">GunStats::</param>
	/// <returns></returns>
	double getChanceFor_NEXT_Stat(size_t stat) const;

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
	

};