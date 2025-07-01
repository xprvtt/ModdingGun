#include "GulText.h"

set<GUI_TextAndRectangle*> GUI_TextAndRectangle::instancesGui;


void GUI_TextAndRectangle::centerText()
{
    float XOffset = UI_Rectangle.getSize().y * 0.3;

    // Горизонтальное позиционирование — от левого края с отступом
    float x = UI_Rectangle.getPosition().x + XOffset;

    // Вертикальное центрирование
    float rectCenterY = UI_Rectangle.getPosition().y + UI_Rectangle.getSize().y / 2.0f;
    float textHeight  = UI_Text.get()->getLocalBounds().size.y;
    float textOffsetY = UI_Text.get()->getLocalBounds().position.y; 

    float y = rectCenterY - (textHeight / 2.0f + textOffsetY);

    // Установка позиции
    UI_Text.get()->setPosition(Vector2f(x, y));
}



GUI_TextAndRectangle::GUI_TextAndRectangle(
    const vector<variant<string, wstring>>& KeyString_and_TextWstring, 
    const RectangleShape& UI_Rectangle, 
    bool is_Button,
    bool is_Pressed

)
{
    instancesGui.insert(this);


    ////////////////////////////////////////////////////////

    this->KeyString_and_TextWstring = KeyString_and_TextWstring;
    ////////////////////////////////////////////////////////






    ////////////////////////////////////////////////////////
    this->is_Button  = is_Button;
    this->is_Pressed = is_Pressed;
    ////////////////////////////////////////////////////////







    ////////////////////////////////////////////////////////
    this->UI_Rectangle = UI_Rectangle; 
    this->UI_Rectangle.setOutlineThickness(ThemeGui::OutlineThicknessSize);
    ////////////////////////////////////////////////////////






    //устанавливает текст из KeyString_and_TextWstring
    ////////////////////////////////////////////////////////
    Text TextCurrent(ThemeGui::CurrentFont, "", ThemeGui::CharacterSize);
    this->UI_Text = make_unique<Text>(TextCurrent);
    RefreshTranslate();

    // центрируем по вертикале относительно UI_Rectangle
    centerText();
    ////////////////////////////////////////////////////////






    ////////////////////////////////////////////////////////
    this->DefaultTextColor = false;
    this->NameTheme = "";
    
    // устанавливаем стандартную тему (т.е. первую в списке AllTheme)
    setTheme(NameTheme);
    ////////////////////////////////////////////////////////

}





void GUI_TextAndRectangle::RefreshTranslate()
{
    if (KeyString_and_TextWstring.empty())
    {
        return;
    }





    ////////////////////////////////////////////////////////
    wstring FullWstring = L"";
    for (size_t it = 0; it < KeyString_and_TextWstring.size(); it++)
    {
        if (holds_alternative<string>(KeyString_and_TextWstring[it]))
        {
            string key = get<string>(KeyString_and_TextWstring[it]);
            FullWstring += LANG_TRANSLATE(key);
            FullWstring += L" ";
        }
        else if (holds_alternative<wstring>(KeyString_and_TextWstring[it]))
        {
            wstring text = get<wstring>(KeyString_and_TextWstring[it]);
            FullWstring += text;
            FullWstring += L" ";
        }
    }
    UI_Text.get()->setString(FullWstring);
    ////////////////////////////////////////////////////////





    // центрируем по вертикале относительно UI_Rectangle
    centerText();
}



bool GUI_TextAndRectangle::setTheme(string NameTheme)
{
    bool result = false;

    

    if (ThemeGui::AllTheme.empty())
    {
        this->NameTheme = "";
        OutputLog("GUIText -> AllTheme empty");
        return false;
    }





    ///////////////////////////////////////////////////////////////////
    // устанавливаем первую в списке если вдруг не найдем;
    auto vColor = ThemeGui::AllTheme.begin()->second;


    if (ThemeGui::AllTheme.find(NameTheme) != ThemeGui::AllTheme.end())
    {
        vColor = ThemeGui::AllTheme.at(NameTheme);
        result = true;
    }
    ///////////////////////////////////////////////////////////////////







    ///////////////////////////////////////////////////////////////////
    // для текста \ если установлен стандартный (кастомный цвет не меняем нужно сначала сбросить )
    if (!DefaultTextColor)
    {
        UI_Text.get()->setFillColor(vColor[0]);
    }




    ///////////////////////////////////////////////////////////////////

    // насйтройка под кнопку
    if (is_Button)
    {
        if (is_Pressed)
        {
            UI_Rectangle.setOutlineColor(vColor[4]);

            if (TexturePressedButton)
            {
                UI_Rectangle.setTexture(TexturePressedButton.get());
            }

        }
        else
        {
            UI_Rectangle.setOutlineColor(vColor[3]);
            if (TextureReleasedButton)
            {
                UI_Rectangle.setTexture(TextureReleasedButton.get());
            }
        }
    }
    else
    {
        UI_Rectangle.setOutlineColor(vColor[1]);
    }

    // заливка если не поставлены текстуры
    if (UI_Rectangle.getTexture() == nullptr)
    {
        UI_Rectangle.setFillColor(vColor[2]);
    }
    ///////////////////////////////////////////////////////////////////





    this->NameTheme = NameTheme;
    return result;
}








/// KeyString_and_TextWstring
///////////////////////////////////////////////////////////////////

void GUI_TextAndRectangle::setKeyString_and_TextWstring(const vector<variant<string, wstring>>& KeyString_and_TextWstring)
{
    this->KeyString_and_TextWstring = KeyString_and_TextWstring;
    /// обновляем UI_Text
    RefreshTranslate();
}

vector<variant<string, wstring>> GUI_TextAndRectangle::getKeyString_and_TextWstring()
{
    return KeyString_and_TextWstring;
}
///////////////////////////////////////////////////////////////////













/// UI_Rectangle
///////////////////////////////////////////////////////////////////
void GUI_TextAndRectangle::setRectangle(const RectangleShape& UI_Rectangle)
{
    this->UI_Rectangle = UI_Rectangle;
    
    setTheme(NameTheme);

    centerText();

}

void GUI_TextAndRectangle::setAsPressedButton()
{
    if (is_Button)
    {
        auto vColor = ThemeGui::AllTheme.begin()->second;


        if (ThemeGui::AllTheme.find(NameTheme) != ThemeGui::AllTheme.end())
        {
            vColor = ThemeGui::AllTheme.at(NameTheme);
        }
        UI_Rectangle.setOutlineColor(vColor[3]);

        if (TexturePressedButton)
        {
            UI_Rectangle.setTexture(TexturePressedButton.get());
        }

        is_Pressed = true;
    }
}


void GUI_TextAndRectangle::setAsReleasedButton()
{
    if (is_Button)
    {
        auto vColor = ThemeGui::AllTheme.begin()->second;


        if (ThemeGui::AllTheme.find(NameTheme) != ThemeGui::AllTheme.end())
        {
            vColor = ThemeGui::AllTheme.at(NameTheme);
        }
        UI_Rectangle.setOutlineColor(vColor[4]);

        if (TextureReleasedButton)
        {
            UI_Rectangle.setTexture(TextureReleasedButton.get());
        }

        is_Pressed = false;
    }
}



bool GUI_TextAndRectangle::setTextureButton(const shared_ptr<Texture>& TexturePressedButton, const shared_ptr<Texture>& TextureReleasedButton)
{
    this->TexturePressedButton = TexturePressedButton;
    this->TextureReleasedButton = TextureReleasedButton;

    setTheme(NameTheme);
    return true;
}

Vector2f GUI_TextAndRectangle::getPositionRectangle()
{
    return GUI_TextAndRectangle::UI_Rectangle.getPosition();
}

void GUI_TextAndRectangle::setPositionRectangle(Vector2f position)
{
    GUI_TextAndRectangle::UI_Rectangle.setPosition(position);
    centerText();
}

const RectangleShape& GUI_TextAndRectangle::getRectangle()
{
    return UI_Rectangle;
}
bool GUI_TextAndRectangle::isPressed()
{
    return is_Pressed;
}
///////////////////////////////////////////////////////////////////











/// UI_Text
///////////////////////////////////////////////////////////////////
const Text& GUI_TextAndRectangle::getText()
{
    return *UI_Text.get();
}

void GUI_TextAndRectangle::setColorText(Color TextColor)
{
    UI_Text.get()->setFillColor(TextColor);
    DefaultTextColor = false;
}

void GUI_TextAndRectangle::resetColorText()
{
    auto vColor = ThemeGui::AllTheme.begin()->second;

    if (ThemeGui::AllTheme.find(NameTheme) != ThemeGui::AllTheme.end())
    {
        vColor = ThemeGui::AllTheme.at(NameTheme);
    }
    DefaultTextColor = true;
    UI_Text.get()->setFillColor(vColor[0]);
}






///////////////////////////////////////////////////////////////////

GUI_TextAndRectangle GUI_TextAndRectangle::operator=(const GUI_TextAndRectangle& other)
{
    instancesGui.insert(this);


    this->KeyString_and_TextWstring = other.KeyString_and_TextWstring;


    Text CurrentText = *other.UI_Text.get();
    this->UI_Text = make_unique<Text>(CurrentText);


    this->UI_Rectangle = other.UI_Rectangle;


    this->is_Button = other.is_Button;
    this->is_Pressed = other.is_Pressed;


    this->DefaultTextColor = other.DefaultTextColor;


    this->NameTheme = other.NameTheme;


    this->TexturePressedButton = other.TexturePressedButton;
    this->TextureReleasedButton = other.TexturePressedButton;



    return *this;
}



GUI_TextAndRectangle::GUI_TextAndRectangle(const GUI_TextAndRectangle& other)
{
    instancesGui.insert(this);

    this->KeyString_and_TextWstring = other.KeyString_and_TextWstring;


    Text CurrentText = *other.UI_Text.get();
    this->UI_Text = make_unique<Text>(CurrentText);


    this->UI_Rectangle = other.UI_Rectangle;


    this->is_Button = other.is_Button;
    this->is_Pressed = other.is_Pressed;


    this->DefaultTextColor = other.DefaultTextColor;


    this->NameTheme = other.NameTheme; 


    this->TexturePressedButton = other.TexturePressedButton;
    this->TextureReleasedButton = other.TexturePressedButton;
}

GUI_TextAndRectangle::~GUI_TextAndRectangle()
{
    instancesGui.erase(this);
}


///////////////////////////////////////////////////////////////////


