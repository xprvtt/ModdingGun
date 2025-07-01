#include "LangManager.h"


///////////////////////////////////////////////////////////////////////////////////////


LangManager& LangManager::Instance()
{
	static LangManager LANGUAGE;
	return LANGUAGE;
}

///////////////////////////////////////////////////////////////////////////////////////


bool LangManager::loadLangInFolder(path PathToLanguage_json)
{
	if (PathToLanguage_json.empty())
	{
		OutputLog("PathToLanguage_json emty");
		return false;
	}

	PathToAllLanguage = SearchFile(PathToLanguage_json, ".json");
	CountLan = PathToAllLanguage.size();

	if (CountLan == 0)
	{
		OutputLog("LangManager -> json not exist in path: " + WstringToString(PathToLanguage));
		return false;
	}

	this->PathToLanguage = PathToLanguage_json;
	empty = false;
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////









///////////////////////////////////////////////////////////////////////////////////////
size_t LangManager::getCountlang()
{
    return CountLan;
}

size_t LangManager::getCurrent_it_lang()
{
	return it_lang;
}

unordered_map<size_t, wstring> LangManager::getLoadedLanguages()
{
	if (CountLan == 0)
	{
		OutputLog("LangManager -> json not loaded");
		return unordered_map<size_t, wstring>();
	}


	unordered_map<size_t, wstring> Result;

	for (size_t i = 0; i < PathToAllLanguage.size(); i++)
	{
		Result.emplace(i, PathToAllLanguage[i].stem());
	}


	return Result;
}
///////////////////////////////////////////////////////////////////////////////////////








///////////////////////////////////////////////////////////////////////////////////////

bool LangManager::setLanguage(size_t it_lang)
{
	if (CountLan == 0)
	{
		OutputLog("LangManager -> json not loaded");
		return false;
	}

	ifstream JSON_File_lang(PathToAllLanguage[it_lang]);
	if (!JSON_File_lang)
	{
		OutputLog("LangManager -> json failed to open");
		return false;
	}

	Map_Translate.clear();

	nlohmann::json JSON_LANG;
	JSON_File_lang >> JSON_LANG;
	
	for (const auto& [json_key, translate] : JSON_LANG.items())
	{
		Map_Translate.emplace(json_key, StringToWString(translate.get<string>()));
	}

	this->it_lang = it_lang;
    return false;
}
///////////////////////////////////////////////////////////////////////////////////////






///////////////////////////////////////////////////////////////////////////////////////

wstring LangManager::GetTranslate(const string& key)
{
	if (key.empty())
	{
		return wstring();
	}

	auto it_map = Map_Translate.find(key);

	if (it_map != Map_Translate.end())
	{
		return it_map->second;
	}
	else
	{
		OutputLog("LangManager -> Key not found: " + key);
		wstring Error = L"[" + StringToWString(key) + L"]";
		return Error;
	}
}


///////////////////////////////////////////////////////////////////////////////////////

