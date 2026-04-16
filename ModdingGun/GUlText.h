#pragma once
#include "Core.h"
#include "LangManager.h"
#include "ThemeGui.h"
#include <set>


/// <summary>
/// управляем всеми текстами
/// </summary>
class GUIModdingGun
{
public:

	/// <summary>
	/// конструктор
	/// </summary>
	/// <param name="m_keyStringAndTextWstring">записываем ключи для перевода STRING и обычный текст WSTRING</param>
	/// <param name="m_UIRectangle">Rectangle в который был вписан текcт</param>
	/// <param name="m_isButton">это кнопка?</param>
	/// <param name="m_isPressed">она сейчас нажата?</param>
	GUIModdingGun(const vector<variant<string, wstring>>& KeyStringAndTextWstring, const RectangleShape& m_UIRectangle, bool m_isButton, bool m_isPressed);

	GUIModdingGun(const GUIModdingGun& other);

	~GUIModdingGun();

	/// <summary>
	/// обновляем язык интерфейса
	/// </summary>
	/// <param name="m_itLang"></param>
	void RefreshTranslate();


	/// <summary>
	/// устанавливаем тему
	/// </summary>
	/// <param name="m_nameTheme">имя темы</param>
	/// <returns></returns>
	bool setTheme(string nameTheme = "");

	/// <summary>
	/// установить новую строку
	/// </summary>
	/// <param name="m_keyStringAndTextWstring"></param>
	void setKeyStringAndTextWstring(const vector<variant<string, wstring>>& KeyString_and_TextWstring);

	/// <summary>
	/// получить строку
	/// </summary>
	/// <returns></returns>
	vector<variant<string, wstring>> getKeyStringAndTextWstring();

	void setRectangle(const RectangleShape& UIRectangle);

	/// <summary>
	/// нажать кнопку, установится если isButton
	/// </summary>
	void setAsPressedButton();

	/// <summary>
	/// разжать кнопку, установится если isButton
	/// </summary>
	void setAsReleasedButton();

	/// <summary>
	/// установим текстуры кнопки
	/// </summary>
	/// <param name="m_texturePressedButton"></param>
	/// <param name="TextureDefaultButton"></param>
	/// <returns></returns>
	bool setTextureButton(const shared_ptr<Texture>& texturePressedButton, const shared_ptr<Texture>& textureReleasedButton);

	Vector2f getPositionRectangle();

	void setPositionRectangle(Vector2f position);

	/// <summary>
	/// получаем ссылку
	/// </summary>
	/// <returns></returns>
	const RectangleShape& getRectangle();

	bool isButton();

	/// <summary>
	/// кнопка сейчас нажата?
	/// </summary>
	/// <returns></returns>
	bool isPressed();

	/// <summary>
	/// получаем ссылку
	/// </summary>
	/// <returns></returns>
	const Text& getText();

	/// <summary>
	/// установить кастомный цвет только для текста
	/// </summary>
	/// <param name="TextColor"></param>
	void setColorText(Color TextColor);

	void resetColorText();

	static void refreshThemeGUI(string nametheme = "")
	{
		for (auto& ins : GUIModdingGun::m_instancesGui)
		{
			ins->setTheme(nametheme);
		}
	}

	static void refreshTranslateGUI()
	{
		for (auto& ins : GUIModdingGun::m_instancesGui)
		{
			ins->RefreshTranslate();
		}
	}

	GUIModdingGun& operator=(const GUIModdingGun& other);

private:

	/// <summary>
	/// Rectangle в который был вписан тект
	/// </summary>
	RectangleShape   m_UIRectangle;

	/// <summary>
	/// текущая тема
	/// </summary>
	string m_nameTheme;

	/// <summary>
	/// список слов
	/// </summary>
	vector<variant<string, wstring>> m_keyStringAndTextWstring;

	static set<GUIModdingGun*> m_instancesGui;

	shared_ptr<Texture> m_texturePressedButton;

	shared_ptr<Texture> m_textureReleasedButton;

	/// <summary>
	/// Текст
	/// </summary>
	unique_ptr<Text> m_UIText;

	/// <summary>
	/// это кнопка?
	/// </summary>
	bool m_isButton;

	/// <summary>
	/// она нажата сейчас?
	/// </summary>
	bool m_isPressed;

	/// <summary>
	/// установлен ли кастомный цвет текста
	/// </summary>
	bool m_defaultTextColor;

	void centerText();
};