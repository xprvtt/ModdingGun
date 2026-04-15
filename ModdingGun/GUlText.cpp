#include "GulText.h"

set<GUITextAndRectangle*> GUITextAndRectangle::m_instancesGui;


void GUITextAndRectangle::centerText()
{
    float XOffset = m_UIRectangle.getSize().y * 0.3f;

    // Горизонтальное позиционирование — от левого края с отступом
    float x = m_UIRectangle.getPosition().x + XOffset;

    // Вертикальное центрирование
    float rectCenterY = m_UIRectangle.getPosition().y + m_UIRectangle.getSize().y / 2.0f;
    float textHeight  = m_UIText.get()->getLocalBounds().size.y;
    float textOffsetY = m_UIText.get()->getLocalBounds().position.y; 

    float y = rectCenterY - (textHeight / 2.0f + textOffsetY);


    // Уменьшение размера текста, если ширина превышает прямоугольник
    float maxWidth = m_UIRectangle.getSize().x - (XOffset * 2.0f); // допустимая ширина с учётом отступов
    unsigned int charSize = m_UIText.get()->getCharacterSize();

    while (m_UIText.get()->getLocalBounds().size.x > maxWidth && charSize > 5) // нижний предел
    {
        charSize -= 1;
        m_UIText.get()->setCharacterSize(charSize);
    }

    // Установка позиции
    m_UIText.get()->setPosition(Vector2f(x, y));
}



GUITextAndRectangle::GUITextAndRectangle(
    const vector<variant<string, wstring>>& KeyString_and_TextWstring, 
    const RectangleShape& m_UIRectangle, 
    bool m_isButton,
    bool m_isPressed

)
{
    m_instancesGui.insert(this);

    this->m_keyStringAndTextWstring = KeyString_and_TextWstring;
    this->m_isButton  = m_isButton;
    this->m_isPressed = m_isPressed;
    this->m_UIRectangle = m_UIRectangle; 
    this->m_UIRectangle.setOutlineThickness(ThemeGui::outlineThicknessSize);

    //устанавливает текст из KeyString_and_TextWstring
    Text TextCurrent(ThemeGui::currentFont, "", ThemeGui::characterSize);
    this->m_UIText = make_unique<Text>(TextCurrent);
    RefreshTranslate();

    // центрируем по вертикале относительно UI_Rectangle
    centerText();
    this->m_defaultTextColor = false;
    this->m_nameTheme = "";
    
    // устанавливаем стандартную тему (т.е. первую в списке AllTheme)
    setTheme(m_nameTheme);
}

void GUITextAndRectangle::RefreshTranslate()
{
    if (m_keyStringAndTextWstring.empty())
    {
        return;
    }
    wstring FullWstring = L"";
    for (size_t it = 0; it < m_keyStringAndTextWstring.size(); it++)
    {
        if (holds_alternative<string>(m_keyStringAndTextWstring[it]))
        {
            string key = get<string>(m_keyStringAndTextWstring[it]);
            FullWstring += LANG_TRANSLATE(key);
            FullWstring += L" ";
        }
        else if (holds_alternative<wstring>(m_keyStringAndTextWstring[it]))
        {
            wstring text = get<wstring>(m_keyStringAndTextWstring[it]);
            FullWstring += text;
            FullWstring += L" ";
        }
    }
    m_UIText.get()->setString(FullWstring);

    // центрируем по вертикале относительно UI_Rectangle
    centerText();
}
bool GUITextAndRectangle::setTheme(string m_nameTheme)
{
    bool result = false;
    if (ThemeGui::allTheme.empty())
    {
        this->m_nameTheme = "";
        OUTPUT_LOG("GUIText -> allTheme empty");
        return false;
    }
    
    // устанавливаем первую в списке если вдруг не найдем;
    auto vColor = ThemeGui::allTheme.begin()->second;

    if (ThemeGui::allTheme.find(m_nameTheme) != ThemeGui::allTheme.end())
    {
        vColor = ThemeGui::allTheme.at(m_nameTheme);
        result = true;
    }

    // для текста \ если установлен стандартный (кастомный цвет не меняем нужно сначала сбросить )
    if (!m_defaultTextColor)
    {
        m_UIText.get()->setFillColor(vColor[0]);
    }

    // насйтройка под кнопку
    if (m_isButton)
    {
        if (m_isPressed)
        {
            m_UIRectangle.setOutlineColor(vColor[4]);

            if (m_texturePressedButton)
            {
                m_UIRectangle.setTexture(m_texturePressedButton.get());
            }
        }
        else
        {
            m_UIRectangle.setOutlineColor(vColor[3]);
            if (m_textureReleasedButton)
            {
                m_UIRectangle.setTexture(m_textureReleasedButton.get());
            }
        }
    }
    else
    {
        m_UIRectangle.setOutlineColor(vColor[1]);
    }

    // заливка если не поставлены текстуры
    if (m_UIRectangle.getTexture() == nullptr)
    {
        m_UIRectangle.setFillColor(vColor[2]);
    }

    this->m_nameTheme = m_nameTheme;
    return result;
}

void GUITextAndRectangle::setKeyString_and_TextWstring(const vector<variant<string, wstring>>& KeyString_and_TextWstring)
{
    this->m_keyStringAndTextWstring = KeyString_and_TextWstring;
    /// обновляем UI_Text
    RefreshTranslate();
}

vector<variant<string, wstring>> GUITextAndRectangle::getKeyStringAndTextWstring()
{
    return m_keyStringAndTextWstring;
}

void GUITextAndRectangle::setRectangle(const RectangleShape& m_UIRectangle)
{
    this->m_UIRectangle = m_UIRectangle;
    
    setTheme(m_nameTheme);

    centerText();

}

void GUITextAndRectangle::setAsPressedButton()
{
    if (m_isButton)
    {
        auto vColor = ThemeGui::allTheme.begin()->second;


        if (ThemeGui::allTheme.find(m_nameTheme) != ThemeGui::allTheme.end())
        {
            vColor = ThemeGui::allTheme.at(m_nameTheme);
        }
        m_UIRectangle.setOutlineColor(vColor[3]);

        if (m_texturePressedButton)
        {
            m_UIRectangle.setTexture(m_texturePressedButton.get());
        }

        m_isPressed = true;
    }
}

void GUITextAndRectangle::setAsReleasedButton()
{
    if (m_isButton)
    {
        auto vColor = ThemeGui::allTheme.begin()->second;


        if (ThemeGui::allTheme.find(m_nameTheme) != ThemeGui::allTheme.end())
        {
            vColor = ThemeGui::allTheme.at(m_nameTheme);
        }
        m_UIRectangle.setOutlineColor(vColor[4]);

        if (m_textureReleasedButton)
        {
            m_UIRectangle.setTexture(m_textureReleasedButton.get());
        }

        m_isPressed = false;
    }
}

bool GUITextAndRectangle::setTextureButton(const shared_ptr<Texture>& m_texturePressedButton, const shared_ptr<Texture>& m_textureReleasedButton)
{
    this->m_texturePressedButton = m_texturePressedButton;
    this->m_textureReleasedButton = m_textureReleasedButton;

    setTheme(m_nameTheme);
    return true;
}

Vector2f GUITextAndRectangle::getPositionRectangle()
{
    return GUITextAndRectangle::m_UIRectangle.getPosition();
}

void GUITextAndRectangle::setPositionRectangle(Vector2f position)
{
    GUITextAndRectangle::m_UIRectangle.setPosition(position);
    centerText();
}

const RectangleShape& GUITextAndRectangle::getRectangle()
{
    return m_UIRectangle;
}
bool GUITextAndRectangle::isButton()
{
    return m_isButton;
}
bool GUITextAndRectangle::isPressed()
{
    return m_isPressed;
}

const Text& GUITextAndRectangle::getText()
{
    return *m_UIText.get();
}

void GUITextAndRectangle::setColorText(Color TextColor)
{
    m_UIText.get()->setFillColor(TextColor);
    m_defaultTextColor = false;
}

void GUITextAndRectangle::resetColorText()
{
    auto vColor = ThemeGui::allTheme.begin()->second;

    if (ThemeGui::allTheme.find(m_nameTheme) != ThemeGui::allTheme.end())
    {
        vColor = ThemeGui::allTheme.at(m_nameTheme);
    }
    m_defaultTextColor = true;
    m_UIText.get()->setFillColor(vColor[0]);
}

GUITextAndRectangle GUITextAndRectangle::operator=(const GUITextAndRectangle& other)
{
    m_instancesGui.insert(this);
    this->m_keyStringAndTextWstring = other.m_keyStringAndTextWstring;

    Text CurrentText = *other.m_UIText.get();
    this->m_UIText = make_unique<Text>(CurrentText);

    this->m_UIRectangle = other.m_UIRectangle;
    this->m_isButton = other.m_isButton;
    this->m_isPressed = other.m_isPressed;
    this->m_defaultTextColor = other.m_defaultTextColor;
    this->m_nameTheme = other.m_nameTheme;
    this->m_texturePressedButton = other.m_texturePressedButton;
    this->m_textureReleasedButton = other.m_texturePressedButton;

    return *this;
}

GUITextAndRectangle::GUITextAndRectangle(const GUITextAndRectangle& other)
{
    m_instancesGui.insert(this);
    this->m_keyStringAndTextWstring = other.m_keyStringAndTextWstring;

    Text CurrentText = *other.m_UIText.get();
    this->m_UIText = make_unique<Text>(CurrentText);
    this->m_UIRectangle = other.m_UIRectangle;
    this->m_isButton = other.m_isButton;
    this->m_isPressed = other.m_isPressed;
    this->m_defaultTextColor = other.m_defaultTextColor;
    this->m_nameTheme = other.m_nameTheme; 
    this->m_texturePressedButton = other.m_texturePressedButton;
    this->m_textureReleasedButton = other.m_texturePressedButton;
}

GUITextAndRectangle::~GUITextAndRectangle()
{
    m_instancesGui.erase(this);
}

