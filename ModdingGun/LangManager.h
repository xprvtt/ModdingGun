#pragma once

#include "Core.h"
#define LANG_TRANSLATE(String_Key) LangManager::Instance().GetTranslate(String_Key)
#define LANG_SETTING LangManager::Instance()

class LangManager {

	path PathToLanguage;
	vector<path> PathToAllLanguage;

	size_t CountLan = 0;
	size_t it_lang = 0;

	map<string, wstring> Map_Translate;

	bool empty = true;

	LangManager() {};

public:

	static LangManager& Instance();

	size_t getCountlang();
	size_t getCurrent_it_lang();
	unordered_map<size_t, wstring> getLoadedLanguages();


	bool loadLangInFolder(path PathToLanguage_json);

	bool setLanguage(size_t it_lang);


	wstring GetTranslate(const string& key);


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