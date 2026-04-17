#pragma once

#include "Core.h"
#define LANG_TRANSLATE(__STRING_KEY__) LangManager::Instance().getTranslate(__STRING_KEY__)
#define LANG_SETTING LangManager::Instance()

class LangManager {

public:

	static LangManager& Instance();

	size_t getCountlang();

	size_t getCurrent_it_lang();

	unordered_map<size_t, wstring> getLoadedLanguages();

	bool loadLangInFolder(path pathToLanguageJson);

	bool setLanguage(size_t itLang);

	wstring getTranslate(const string& key);

private:

	path m_currentLanguagePath;

	vector<path> m_allLanguagePaths;

	size_t m_countLan = 0;
	size_t m_itLang = 0;

	map<string, wstring> m_translates;

	bool empty = true;

	LangManager() {};
};

//    vector<tuple<RectangleShape, shared_ptr<Texture>, wstring, int, Text>> VectorGun;

//struct Weapon
//{
//	RectangleShape Wshape;
//	shared_ptr<Texture> m_ptrTexture;
//	wstring WName;
//	int No_weapon;
//	Text WText;
//};