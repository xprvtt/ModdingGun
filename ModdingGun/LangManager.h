#pragma once

#include "Core.h"
<<<<<<< HEAD
#define LANG_TRANSLATE(__STRING_KEY__) LangManager::Instance().getTranslate(__STRING_KEY__)
#define LANG_SETTING LangManager::Instance()

class LangManager {

=======
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

>>>>>>> main
public:

	static LangManager& instance();

<<<<<<< HEAD
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
=======
	size_t getCountlang() const;
	size_t getCurrentItLang() const;
	unordered_map<size_t, wstring> getLoadedLanguages();

	bool loadLangInFolder(path PathToLanguage_json);
	bool setLanguage(size_t m_itLang);

	wstring getTranslate(const string& key);
>>>>>>> main
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