#pragma once
#include "Core.h"
#include "LangManager.h"
#include "ThemeGui.h"
#include <set>


/// <summary>
/// управляем всеми текстами
/// </summary>
class GUI_TextAndRectangle
{
    /// <summary>
    /// список слов
    /// </summary>
    vector<variant<string, wstring>> KeyString_and_TextWstring;

    /// <summary>
    /// Текст
    /// </summary>
    unique_ptr<Text> UI_Text;

    /// <summary>
    /// Rectangle в который был вписан тект
    /// </summary>
    RectangleShape   UI_Rectangle;

    /// <summary>
    /// это кнопка?
    /// </summary>
    bool is_Button;
    
    /// <summary>
    /// она нажата сейчас?
    /// </summary>
    bool is_Pressed;


    /// <summary>
    /// установлен ли кастомный цвет текста
    /// </summary>
    bool DefaultTextColor;

    /// <summary>
    /// текущая тема
    /// </summary>
    string NameTheme;  


    shared_ptr<Texture> TexturePressedButton;

    shared_ptr<Texture> TextureReleasedButton;    




    static set<GUI_TextAndRectangle*> instancesGui;
    





    void centerText();





public:



    /// <summary>
    /// 
    /// </summary>
    /// <param name="KeyString_and_TextWstring">записываем ключи для перевода STRING и обычный текст WSTRING</param>
    /// <param name="UI_Rectangle">Rectangle в который был вписан текcт</param>
    /// <param name="is_Button">это кнопка?</param>
    /// <param name="is_Pressed">она сейчас нажата?</param>
    GUI_TextAndRectangle(const vector<variant<string, wstring>>& KeyString_and_TextWstring, const RectangleShape& UI_Rectangle, bool is_Button, bool is_Pressed);
    GUI_TextAndRectangle(const GUI_TextAndRectangle& other);
    ~GUI_TextAndRectangle();


    /// <summary>
    /// обновляем язык интерфейса
    /// </summary>
    /// <param name="it_lang"></param>
    void RefreshTranslate();


    /// <summary>
    /// устанавливаем тему
    /// </summary>
    /// <param name="NameTheme">имя темы</param>
    /// <returns></returns>
    bool setTheme(string NameTheme = "");





    /// KeyString_and_TextWstring
    ///////////////////////////////////////////////////////////////////


    /// <summary>
    /// установить новую строку
    /// </summary>
    /// <param name="KeyString_and_TextWstring"></param>
    void setKeyString_and_TextWstring(const vector<variant<string, wstring>>& KeyString_and_TextWstring);

    /// <summary>
    /// получить строку
    /// </summary>
    /// <returns></returns>
    vector<variant<string, wstring>> getKeyString_and_TextWstring();

    ///////////////////////////////////////////////////////////////////










    /// UI_Rectangle
    ///////////////////////////////////////////////////////////////////
    void setRectangle(const RectangleShape& UI_Rectangle);

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
    bool setTextureButton(const shared_ptr<Texture>& TexturePressedButton, const shared_ptr<Texture>& TextureReleasedButton);




    Vector2f getPositionRectangle();

    void setPositionRectangle(Vector2f position);



    /// <summary>
    /// получаем ссылку
    /// </summary>
    /// <returns></returns>
    const RectangleShape& getRectangle();


    /// <summary>
    /// кнопка сейчас нажата?
    /// </summary>
    /// <returns></returns>
    bool isPressed();
    ///////////////////////////////////////////////////////////////////











    /// UI_Text
    ///////////////////////////////////////////////////////////////////

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
    ///////////////////////////////////////////////////////////////////    




    static void FREFRESH_THEME_GUI(string nametheme = "")
    {
        for (auto& ins : GUI_TextAndRectangle::instancesGui)
        {
            ins->setTheme(nametheme);
        }
    }



    static void FREFRESH_TR_GUI()
    {
        for (auto& ins : GUI_TextAndRectangle::instancesGui)
        {
            ins->RefreshTranslate();
        }
    }



    GUI_TextAndRectangle operator=(const GUI_TextAndRectangle& other);
 
};


