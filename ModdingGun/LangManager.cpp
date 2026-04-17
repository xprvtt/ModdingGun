#include "LangManager.h"

<<<<<<< HEAD
//-----------------------------------------------------------------------------------------------------------------------

LangManager& LangManager::Instance()
=======

//-------------------------------------------------------------------------------------------------------------------

LangManager& LangManager::instance()
>>>>>>> main
{
	static LangManager language;
	return language;
}

<<<<<<< HEAD
//-----------------------------------------------------------------------------------------------------------------------

bool LangManager::loadLangInFolder(path PathToLanguage_json)
=======
//-------------------------------------------------------------------------------------------------------------------

bool LangManager::loadLangInFolder(path PathToLanguageJson)
>>>>>>> main
{
	if (PathToLanguageJson.empty())
	{
<<<<<<< HEAD
		OUTPUT_LOG("PathToLanguage_json emty");
		return false;
	}

	m_allLanguagePaths = searchFile(PathToLanguage_json, ".json");
	m_countLan = m_allLanguagePaths.size();

	if (m_countLan == 0)
	{
		OUTPUT_LOG("LangManager -> json not exist in path: " + wstringToString(m_currentLanguagePath));
		return false;
	}

	this->m_currentLanguagePath = PathToLanguage_json;
=======
		OUTPUT_LOG("PathToLanguageJson emty");
		return false;
	}

	m_pathToAllLanguage = searchFile(PathToLanguageJson, ".json");
	m_countLang = m_pathToAllLanguage.size();

	if (m_countLang == 0)
	{
		OUTPUT_LOG("LangManager -> json not exist in path: " + wstringToString(m_pathToLanguage));
		return false;
	}

	this->m_pathToLanguage = PathToLanguageJson;
>>>>>>> main
	empty = false;
	return true;
}

<<<<<<< HEAD
//-----------------------------------------------------------------------------------------------------------------------

size_t LangManager::getCountlang()
{
	return m_countLan;
}

//-----------------------------------------------------------------------------------------------------------------------

size_t LangManager::getCurrent_it_lang()
=======
//-------------------------------------------------------------------------------------------------------------------
size_t LangManager::getCountlang() const
{
    return m_countLang;
}

size_t LangManager::getCurrentItLang() const
>>>>>>> main
{
	return m_itLang;
}

//-----------------------------------------------------------------------------------------------------------------------

unordered_map<size_t, wstring> LangManager::getLoadedLanguages()
{
<<<<<<< HEAD
	if (m_countLan == 0)
=======
	if (m_countLang == 0)
>>>>>>> main
	{
		OUTPUT_LOG("LangManager -> json not loaded");
		return unordered_map<size_t, wstring>();
	}

	unordered_map<size_t, wstring> Result;
<<<<<<< HEAD
	for (size_t i = 0; i < m_allLanguagePaths.size(); i++)
	{
		Result.emplace(i, m_allLanguagePaths[i].stem());
=======

	for (size_t i = 0; i < m_pathToAllLanguage.size(); i++)
	{
		Result.emplace(i, m_pathToAllLanguage[i].stem());
>>>>>>> main
	}

	return Result;
}

<<<<<<< HEAD
//-----------------------------------------------------------------------------------------------------------------------

bool LangManager::setLanguage(size_t itLang)
{
	if (m_countLan == 0)
=======
//-------------------------------------------------------------------------------------------------------------------

bool LangManager::setLanguage(size_t itLang)
{
	if (m_countLang == 0)
>>>>>>> main
	{
		OUTPUT_LOG("LangManager -> json not loaded");
		return false;
	}

<<<<<<< HEAD
	ifstream languageJsonFile(m_allLanguagePaths[itLang]);
	if (!languageJsonFile)
=======
	ifstream JsFileLang(m_pathToAllLanguage[itLang]);
	if (!JsFileLang)
>>>>>>> main
	{
		OUTPUT_LOG("LangManager -> json failed to open");
		return false;
	}

<<<<<<< HEAD
	m_translates.clear();

	nlohmann::json jsonLang;
	languageJsonFile >> jsonLang;

	for (const auto& [json_key, translate] : jsonLang.items())
	{
		m_translates.emplace(json_key, stringToWString(translate.get<string>()));
	}

	this->m_itLang = itLang;
	return false;
}

//-----------------------------------------------------------------------------------------------------------------------
=======
	m_mTranslate.clear();

	nlohmann::json JsLang;
	JsFileLang >> JsLang;
	
	for (const auto& [jsonKey, translate] : JsLang.items())
	{
		m_mTranslate.emplace(jsonKey, stringToWString(translate.get<string>()));
	}

	m_itLang = itLang;
    return false;
}

//-------------------------------------------------------------------------------------------------------------------
>>>>>>> main

wstring LangManager::getTranslate(const string& key)
{
	if (key.empty()) { return wstring(); }

<<<<<<< HEAD
	auto itMap = m_translates.find(key);
	if (itMap != m_translates.end())
	{
		return itMap->second;
=======
	auto it = m_mTranslate.find(key);
	if (it != m_mTranslate.end())
	{
		return it->second;
>>>>>>> main
	}
	else
	{
		OUTPUT_LOG("LangManager -> Key not found: " + key);
		wstring error = L"[" + stringToWString(key) + L"]";
		return error;
	}
}

<<<<<<< HEAD
//-----------------------------------------------------------------------------------------------------------------------
=======
//-------------------------------------------------------------------------------------------------------------------
>>>>>>> main
