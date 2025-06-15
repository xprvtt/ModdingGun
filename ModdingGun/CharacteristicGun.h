#pragma once

#include "Core.h"
#include "C:\Users\xprvtt\Documents\GitHub\ModdingGun\ModdingGun\json.hpp"

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
		float parametr;

		/// <summary>
		/// текуший РЕАЛЬНЫЙ шанс НА УЛУЧШЕНИЕ
		/// </summary>
		double CurrentChance;

		/// <summary>
		/// текущий ВИЗУАЛЬНЫЙ процент
		/// </summary>
		double VisualProcentStat;
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
	/// <para>0 - кучность вектор возможных стат</para> 
	/// <para>1 - темп</para>
	/// <para>2 - отдача</para>
	/// <para>3 - качание</para>
	/// <para>4 - пробитие</para>
	/// <para>5 - отказ грязи</para>
	/// <para>6 - отказ состояния</para>
	/// </summary>
	std::vector<vector<AllStat>> FulAllStat;


	/// <summary>
	/// <para>0 - кучность </para>
	/// <para>1 - темп</para>
	/// <para>2 - отдача</para>
	/// <para>3 - качание</para>
	/// <para>4 - пробитие</para>
	/// <para>5 - отказ грязи</para>
	/// <para>6 - отказ состояния</para>
	/// </summary>
	std::vector<vector<DecreaseStatST>> FullDecreaseStat;

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
	std::vector<int> CurrentStatPosition;

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
	std::vector<int> MaxStatPosition;

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
	vector<int> InitialStatPosition;

	bool Empty;

	bool UpStat(int stat_No);


public:

	CharacteristicGun(const CharacteristicGun& other);

	CharacteristicGun();

	bool is_Empty();

	bool Load(path PathToInfo);

	bool Load(path PathToInfo, wstring namegun);

	void clear();

	bool UpgradeStat(int CharacteristicGun_in_NAMESTAT);

	/// <summary>
	/// получить шанс услучшения ДЛЯ УЛУЧШЕНИЯ НА СЛЕД ПОЗИЦИЮ характеристики
	/// </summary>
	/// <param name="stat"></param>
	/// <returns></returns>
	double GetChanceFor_NEXT_Stat(unsigned int stat);


	/// <summary>
	/// получить визуальное отображение процента прибавки
	/// </summary>
	/// <param name="stat"></param>
	/// <returns></returns>
	double GetVisualProcentStat(int stat);


	/// <summary>
	/// вернуть начальные характеристики
	/// </summary>
	/// <returns></returns>
	bool ReturnDefaultstat();

	/// <summary>
	/// получить максимально возможные статы
	/// </summary>
	/// <returns></returns>
	vector<int> GetMaxStatPosition();


	/// <summary>
	/// получить максимальный визуальный процент для статы
	/// </summary>
	/// <returns></returns>
	double GetMaxStatVisualProcent(unsigned int stat);


	/// <summary>
	/// получить вектор отображаемых процентов для текущей статы
	/// </summary>
	/// <returns></returns>
	vector<double> GetFullCurrentVisualStat();


	/// <summary>
	/// получить текущую позицию
	/// </summary>
	/// <returns></returns>
	vector<int> GetCurrentPosition();

};