#include "LangManager.h"


///////////////////////////////////////////////////////////////////////////////////////


LangManager& LangManager::instance()
{
	static LangManager LANGUAGE;
	return LANGUAGE;
}

///////////////////////////////////////////////////////////////////////////////////////


bool LangManager::loadLangInFolder(path PathToLanguageJson)
{
	if (PathToLanguageJson.empty())
	{
		OUTPUT_LOG("PathToLanguageJson emty");
		return false;
	}

	m_pathToAllLanguage = SearchFile(PathToLanguageJson, ".json");
	m_countLang = m_pathToAllLanguage.size();

	if (m_countLang == 0)
	{
		OUTPUT_LOG("LangManager -> json not exist in path: " + WstringToString(m_pathToLanguage));
		return false;
	}

	this->m_pathToLanguage = PathToLanguageJson;
	empty = false;
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////









///////////////////////////////////////////////////////////////////////////////////////
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
///////////////////////////////////////////////////////////////////////////////////////








///////////////////////////////////////////////////////////////////////////////////////

bool LangManager::setLanguage(size_t m_itLang)
{
	if (m_countLang == 0)
	{
		OUTPUT_LOG("LangManager -> json not loaded");
		return false;
	}

	ifstream JSON_File_lang(m_pathToAllLanguage[m_itLang]);
	if (!JSON_File_lang)
	{
		OUTPUT_LOG("LangManager -> json failed to open");
		return false;
	}

	m_mTranslate.clear();

	nlohmann::json JSON_LANG;
	JSON_File_lang >> JSON_LANG;
	
	for (const auto& [json_key, translate] : JSON_LANG.items())
	{
		m_mTranslate.emplace(json_key, StringToWString(translate.get<string>()));
	}

	this->m_itLang = m_itLang;
    return false;
}
///////////////////////////////////////////////////////////////////////////////////////






///////////////////////////////////////////////////////////////////////////////////////

wstring LangManager::getTranslate(const string& key)
{
	if (key.empty())
	{
		return wstring();
	}

	auto it_map = m_mTranslate.find(key);

	if (it_map != m_mTranslate.end())
	{
		return it_map->second;
	}
	else
	{
		OUTPUT_LOG("LangManager -> Key not found: " + key);
		wstring Error = L"[" + StringToWString(key) + L"]";
		return Error;
	}
}


///////////////////////////////////////////////////////////////////////////////////////

