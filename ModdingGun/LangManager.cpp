#include "LangManager.h"


//-------------------------------------------------------------------------------------------------------------------

LangManager& LangManager::instance()
{
	static LangManager LANGUAGE;
	return LANGUAGE;
}

//-------------------------------------------------------------------------------------------------------------------

bool LangManager::loadLangInFolder(path PathToLanguageJson)
{
	if (PathToLanguageJson.empty())
	{
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
	empty = false;
	return true;
}

//-------------------------------------------------------------------------------------------------------------------
size_t LangManager::getCountlang() const
{
    return m_countLang;
}

size_t LangManager::getCurrentItLang() const
{
	return m_itLang;
}

unordered_map<size_t, wstring> LangManager::getLoadedLanguages()
{
	if (m_countLang == 0)
	{
		OUTPUT_LOG("LangManager -> json not loaded");
		return unordered_map<size_t, wstring>();
	}


	unordered_map<size_t, wstring> Result;

	for (size_t i = 0; i < m_pathToAllLanguage.size(); i++)
	{
		Result.emplace(i, m_pathToAllLanguage[i].stem());
	}


	return Result;
}

//-------------------------------------------------------------------------------------------------------------------

bool LangManager::setLanguage(size_t itLang)
{
	if (m_countLang == 0)
	{
		OUTPUT_LOG("LangManager -> json not loaded");
		return false;
	}

	ifstream JsFileLang(m_pathToAllLanguage[itLang]);
	if (!JsFileLang)
	{
		OUTPUT_LOG("LangManager -> json failed to open");
		return false;
	}

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

wstring LangManager::getTranslate(const string& key)
{
	if (key.empty())
	{
		return wstring();
	}

	auto it = m_mTranslate.find(key);
	if (it != m_mTranslate.end())
	{
		return it->second;
	}
	else
	{
		OUTPUT_LOG("LangManager -> Key not found: " + key);
		wstring error = L"[" + stringToWString(key) + L"]";
		return error;
	}
}

//-------------------------------------------------------------------------------------------------------------------