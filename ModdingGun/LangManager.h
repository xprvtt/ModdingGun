#pragma once

#include "Core.h"
#define LANG_TRANSLATE(_STRING_KEY_) LangManager::instance().getTranslate(_STRING_KEY_)
#define LANG_SETTING LangManager::instance()

class LangManager {

	path m_pathToLanguage;
	vector<path> m_pathToAllLanguage;

	size_t m_countLang = 0;
	size_t m_itLang = 0;

	map<string, wstring> m_mTranslate;

	bool empty = true;

	LangManager() {};

public:

	static LangManager& instance();

	size_t getCountlang() const;
	size_t getCurrentItLang() const;
	unordered_map<size_t, wstring> getLoadedLanguages();

	bool loadLangInFolder(path PathToLanguage_json);
	bool setLanguage(size_t m_itLang);

	wstring getTranslate(const string& key);
};

//    vector<tuple<RectangleShape, shared_ptr<Texture>, wstring, int, Text>> VectorGun;

//struct VectorWeapon
//{
//	RectangleShape Wshape;
//	shared_ptr<Texture> WTexture;
//	wstring WName;
//	int No_weapon;
//	Text WText;
//};