#pragma once
#include "Core.h"
#include "LangManager.h"
#include "ThemeGui.h"
#include <set>


/// <summary>
/// управляем всеми текстами
/// </summary>
<<<<<<< HEAD
class GUITextAndRectangle
=======
class GUIModdingGun
>>>>>>> main
{
public:

	/// <summary>
<<<<<<< HEAD
	/// 
	/// </summary>
	/// <param name="KeyString_and_TextWstring">записываем ключи для перевода STRING и обычный текст WSTRING</param>
	/// <param name="UI_Rectangle">Rectangle в который был вписан текcт</param>
	/// <param name="is_Button">это кнопка?</param>
	/// <param name="is_Pressed">она сейчас нажата?</param>
	GUITextAndRectangle(const vector<variant<string, wstring>>& KeyString_and_TextWstring, const RectangleShape& UI_Rectangle, bool is_Button, bool is_Pressed);

	GUITextAndRectangle(const GUITextAndRectangle& other);

	~GUITextAndRectangle();
=======
	/// конструктор
	/// </summary>
	/// <param name="m_keyStringAndTextWstring">записываем ключи для перевода STRING и обычный текст WSTRING</param>
	/// <param name="m_UIRectangle">Rectangle в который был вписан текcт</param>
	/// <param name="m_isButton">это кнопка?</param>
	/// <param name="m_isPressed">она сейчас нажата?</param>
	GUIModdingGun(const vector<variant<string, wstring>>& KeyStringAndTextWstring, const RectangleShape& m_UIRectangle, bool m_isButton, bool m_isPressed);

	GUIModdingGun(const GUIModdingGun& other);

	~GUIModdingGun();
>>>>>>> main

	/// <summary>
	/// обновляем язык интерфейса
	/// </summary>
<<<<<<< HEAD
	/// <param name="it_lang"></param>
	void refreshTranslate();
=======
	/// <param name="m_itLang"></param>
	void RefreshTranslate();

>>>>>>> main

	/// <summary>
	/// устанавливаем тему
	/// </summary>
<<<<<<< HEAD
	/// <param name="NameTheme">имя темы</param>
=======
	/// <param name="m_nameTheme">имя темы</param>
>>>>>>> main
	/// <returns></returns>
	bool setTheme(string nameTheme = "");

	/// <summary>
	/// установить новую строку
	/// </summary>
<<<<<<< HEAD
	/// <param name="KeyString_and_TextWstring"></param>
	void setKeyStringTextWstring(const vector<variant<string, wstring>>& keyStringTextWstring);
=======
	/// <param name="m_keyStringAndTextWstring"></param>
	void setKeyStringAndTextWstring(const vector<variant<string, wstring>>& KeyString_and_TextWstring);
>>>>>>> main

	/// <summary>
	/// получить строку
	/// </summary>
	/// <returns></returns>
<<<<<<< HEAD
	vector<variant<string, wstring>> getKeyStringTextWstring();
=======
	vector<variant<string, wstring>> getKeyStringAndTextWstring();
>>>>>>> main

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
<<<<<<< HEAD
	/// <param name="TexturePressedButton"></param>
=======
	/// <param name="m_texturePressedButton"></param>
>>>>>>> main
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

<<<<<<< HEAD
	bool isButton() const;
=======
	bool isButton();
>>>>>>> main

	/// <summary>
	/// кнопка сейчас нажата?
	/// </summary>
	/// <returns></returns>
<<<<<<< HEAD
	bool isPressed() const;

=======
	bool isPressed();
>>>>>>> main

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

<<<<<<< HEAD
	static void refreshThemeGui(string nametheme = "")
	{
		for (auto& ins : GUITextAndRectangle::m_instancesGui)
=======
	static void refreshThemeGUI(string nametheme = "")
	{
		for (auto& ins : GUIModdingGun::m_instancesGui)
>>>>>>> main
		{
			ins->setTheme(nametheme);
		}
	}

<<<<<<< HEAD
	static void refreshTranslateGui()
	{
		for (auto& ins : GUITextAndRectangle::m_instancesGui)
		{
			ins->refreshTranslate();
		}
	}

	GUITextAndRectangle& operator=(const GUITextAndRectangle& other);
=======
	static void refreshTranslateGUI()
	{
		for (auto& ins : GUIModdingGun::m_instancesGui)
		{
			ins->RefreshTranslate();
		}
	}

	GUIModdingGun& operator=(const GUIModdingGun& other);
>>>>>>> main

private:

	/// <summary>
<<<<<<< HEAD
	/// список слов где string это ключ для перевода, wstring это текст без перевода
	/// </summary>
	vector<variant<string, wstring>> m_KeyStringTextWstring;

	/// <summary>
	/// Текст
	/// </summary>
	unique_ptr<Text> m_UIText;

	/// <summary>
=======
>>>>>>> main
	/// Rectangle в который был вписан тект
	/// </summary>
	RectangleShape   m_UIRectangle;

	/// <summary>
<<<<<<< HEAD
=======
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
>>>>>>> main
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

<<<<<<< HEAD
	/// <summary>
	/// текущая тема
	/// </summary>
	string m_nameTheme;

	shared_ptr<Texture> m_texturePressedButton;

	shared_ptr<Texture> m_textureReleasedButton;

	static set<GUITextAndRectangle*> m_instancesGui;

	void centerText();


};


=======
	void centerText();
};
>>>>>>> main
