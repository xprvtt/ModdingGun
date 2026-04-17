#include "LangManager.h"

//-----------------------------------------------------------------------------------------------------------------------

LangManager& LangManager::Instance()
{
	static LangManager language;
	return language;
}

//-----------------------------------------------------------------------------------------------------------------------

bool LangManager::loadLangInFolder(path PathToLanguage_json)
{
	if (PathToLanguage_json.empty())
	{
		OUTPUT_LOG("PathToLanguage_json emty");
		return false;
	}

	m_allLanguagePaths = searchFile(PathToLanguage_json, ".json");
	m_countLan = m_allLanguagePaths.size();

	if (m_countLan == 0)
	{
		OUTPUT_LOG("LangManager -> json not exist in path: " + WstringToString(m_currentLanguagePath));
		return false;
	}

	this->m_currentLanguagePath = PathToLanguage_json;
	empty = false;
	return true;
}

//-----------------------------------------------------------------------------------------------------------------------

size_t LangManager::getCountlang()
{
	return m_countLan;
}

//-----------------------------------------------------------------------------------------------------------------------

size_t LangManager::getCurrent_it_lang()
{
	return m_itLang;
}

//-----------------------------------------------------------------------------------------------------------------------

unordered_map<size_t, wstring> LangManager::getLoadedLanguages()
{
	if (m_countLan == 0)
	{
		OUTPUT_LOG("LangManager -> json not loaded");
		return unordered_map<size_t, wstring>();
	}

	unordered_map<size_t, wstring> Result;
	for (size_t i = 0; i < m_allLanguagePaths.size(); i++)
	{
		Result.emplace(i, m_allLanguagePaths[i].stem());
	}

	return Result;
}

//-----------------------------------------------------------------------------------------------------------------------

bool LangManager::setLanguage(size_t itLang)
{
	if (m_countLan == 0)
	{
		OUTPUT_LOG("LangManager -> json not loaded");
		return false;
	}

	ifstream languageJsonFile(m_allLanguagePaths[itLang]);
	if (!languageJsonFile)
	{
		OUTPUT_LOG("LangManager -> json failed to open");
		return false;
	}

	m_translates.clear();

	nlohmann::json jsonLang;
	languageJsonFile >> jsonLang;

	for (const auto& [json_key, translate] : jsonLang.items())
	{
		m_translates.emplace(json_key, StringToWString(translate.get<string>()));
	}

	this->m_itLang = itLang;
	return false;
}

//-----------------------------------------------------------------------------------------------------------------------

wstring LangManager::getTranslate(const string& key)
{
	if (key.empty()) { return wstring(); }

	auto itMap = m_translates.find(key);
	if (itMap != m_translates.end())
	{
		return itMap->second;
	}
	else
	{
		OUTPUT_LOG("LangManager -> Key not found: " + key);
		wstring error = L"[" + StringToWString(key) + L"]";
		return error;
	}
}

//-----------------------------------------------------------------------------------------------------------------------
