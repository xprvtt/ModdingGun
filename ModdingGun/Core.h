#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <vector>
#include "iostream"
#include "string"
#include "ctime"
#include "fstream"
#include "filesystem"
#include <sstream>
#include <windows.h>
#include <tuple>

#include "Define.h"
#include <numeric>
#include <cmath>
#include <map>

#include <random>
#include <thread>
#include <mutex>

#include <future>
#include <atomic>

#include <regex>
#include <utility>

#include <SFML\Graphics.hpp>

#pragma comment(lib, "User32.lib")

using namespace sf;
using namespace std;
using namespace filesystem;

/// <summary>
/// Функция Загружает шрифт по пути
/// </summary>
/// <param name="PathToFont">путь до шрифта</param>
/// <returns></returns>
Font LoadFont(path PathToFont);

/// <summary>
/// поиск файлов в директории
/// </summary>
/// <param name="SearchInPath">путь</param>
/// <param name="Extension">расширение файла(ов) / если не указано ищет всё </param>
/// <returns>вектор</returns>
vector<path> SearchFile(path SearchInPath, string Extension);

/// <summary>
/// Функция выводит сообщение в файл Log/log.txt
/// </summary>
/// <param name="message">Сообщение которое необходимо вывести</param>
/// <returns>true если выведен</returns>
bool OutputMessage(string message);

/// <summary>
/// Функция возвращает строку веремени 
/// </summary>
/// <returns>дата и время</returns>
string GetCurrentTimeMG();

/// <summary>
/// 
/// </summary>
/// <param name="range"></param>
/// <returns></returns>
double getRandomDouble(double range);


int SimulateOneAttempt(
    const  vector<vector<tuple<int, int, tuple<int, int, int, int, int, int, int>>>>& AllStatMod,
    const  vector<int>& CharacteristicsInitial, const  vector<int>& CharacteristicsModded,
    bool   Initial_OFF,
    const  vector<int>& AccuracyTemp,
    const  vector<int>& PaceFireTemp,
    vector<int>& randomLog
);


double getChance(int position, const vector<pair<int, double>>& chancePoints);

vector<int> drawNormalGraph(
    RenderWindow& window,
    const vector<float>& Average10000,
    float graphHeight,
    float marginBottom,
    const vector<float>&
    Average10000_RANDOM_GEN,
    int totalIterations,
    const vector<double>& RESULT_CHARACTERISTIC,
    const vector<double>& START_CHARACTERISTIC,
    const vector<double>& INITIAL_CHARACTERISTIC,
    wstring NameGun,
    vector<int> method,
    const tuple<double, double, double>& Tool_Kit_Skil
);