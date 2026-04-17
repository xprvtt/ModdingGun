#pragma once

#define _CRT_SECURE_NO_WARNINGS
#define _CRTDBG_MAP_ALLOC

#include "iostream"

#include <windows.h>
#include <numeric>
#include <cmath>

#include <map>
#include <vector>
#include "string"
#include <tuple>
#include <sstream>
#include "filesystem"
#include "fstream"

#include <random>
#include "ctime"

#include <mutex>
#include <thread>
#include <atomic>

#include <regex>
#include <utility>

#include <SFML\Graphics.hpp>
#include "json.hpp"

#pragma comment(lib, "User32.lib")

#include "Define.h"

using namespace sf;
using namespace std;
using namespace filesystem;

/// <summary>
/// Функция Загружает шрифт по пути
/// </summary>
/// <param name="pathToFont">путь до шрифта</param>
/// <returns></returns>
Font loadFont(path pathToFont);

/// <summary>
/// поиск файлов в директории
/// </summary>
/// <param name="SearchInPath">путь</param>
/// <param name="Extension">расширение файла(ов) / если не указано ищет всё </param>
/// <returns>вектор</returns>
vector<path> searchFile(path searchInPath, string extension);

/// <summary>
/// Функция выводит сообщение в файл Log/log.txt
/// </summary>
/// <param name="message">Сообщение которое необходимо вывести</param>
/// <returns>true если выведен</returns>
bool outputMessage(string message);

/// <summary>
/// Функция возвращает строку веремени 
/// </summary>
/// <returns>дата и время</returns>
string getCurrentTimeMG();

double getRandomDouble(double range);

double getChance(int position, const vector<pair<int, double>>& chancePoints);

double calculateSuccessChance(double baseChance, double toolFactor, double KitFactor, double skillFactor);

wstring stringToWString(const string& str);

string  wstringToString(const wstring& wstr);