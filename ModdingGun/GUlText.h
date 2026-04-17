#pragma once
#include "Core.h"
#include "LangManager.h"
#include "ThemeGui.h"
#include <set>


/// <summary>
/// управляем всеми текстами
/// </summary>
class GUITextAndRectangle
{
    /// <summary>
    /// список слов где string это ключ для перевода, wstring это текст без перевода
    /// </summary>
    vector<variant<string, wstring>> m_KeyStringTextWstring;

    /// <summary>
    /// Текст
    /// </summary>
    unique_ptr<Text> m_UIText;

    /// <summary>
    /// Rectangle в который был вписан тект
    /// </summary>
    RectangleShape   m_UIRectangle;

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

    /// <summary>
    /// текущая тема
    /// </summary>
    string m_nameTheme;  

    shared_ptr<Texture> m_texturePressedButton;

    shared_ptr<Texture> m_textureReleasedButton;

    static set<GUITextAndRectangle*> m_instancesGui; 

    void centerText();

public:

    /// <summary>
    /// 
    /// </summary>
    /// <param name="KeyString_and_TextWstring">записываем ключи для перевода STRING и обычный текст WSTRING</param>
    /// <param name="UI_Rectangle">Rectangle в который был вписан текcт</param>
    /// <param name="is_Button">это кнопка?</param>
    /// <param name="is_Pressed">она сейчас нажата?</param>
    GUITextAndRectangle(const vector<variant<string, wstring>>& KeyString_and_TextWstring, const RectangleShape& UI_Rectangle, bool is_Button, bool is_Pressed);

    GUITextAndRectangle(const GUITextAndRectangle& other);

    ~GUITextAndRectangle();

    /// <summary>
    /// обновляем язык интерфейса
    /// </summary>
    /// <param name="it_lang"></param>
    void refreshTranslate();

    /// <summary>
    /// устанавливаем тему
    /// </summary>
    /// <param name="NameTheme">имя темы</param>
    /// <returns></returns>
    bool setTheme(string nameTheme = "");

    /// <summary>
    /// установить новую строку
    /// </summary>
    /// <param name="KeyString_and_TextWstring"></param>
    void setKeyStringTextWstring(const vector<variant<string, wstring>>& keyStringTextWstring);

    /// <summary>
    /// получить строку
    /// </summary>
    /// <returns></returns>
    vector<variant<string, wstring>> getKeyStringTextWstring();

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
    /// <param name="TexturePressedButton"></param>
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

    static void refreshThemeGui(string nametheme = "")
    {
        for (auto& ins : GUITextAndRectangle::m_instancesGui)
        {
            ins->setTheme(nametheme);
        }
    }

    static void refreshTranslateGui()
    {
        for (auto& ins : GUITextAndRectangle::m_instancesGui)
        {
            ins->refreshTranslate();
        }
    }

    GUITextAndRectangle& operator=(const GUITextAndRectangle& other);
};


