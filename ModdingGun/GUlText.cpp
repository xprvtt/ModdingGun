#include "GulText.h"

set<GUIModdingGun*> GUIModdingGun::m_instancesGui;

void GUIModdingGun::centerText()
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

//-------------------------------------------------------------------------------------------------------------------

GUIModdingGun::GUIModdingGun( const vector<variant<string, wstring>>& KeyStringAndTextWstring, const RectangleShape& UIRectangle, bool isButton,bool isPressed) : m_keyStringAndTextWstring(KeyStringAndTextWstring), m_isButton(isButton), m_isPressed(isPressed), m_UIRectangle(UIRectangle)
{         
    m_instancesGui.insert(this);
    m_UIRectangle.setOutlineThickness(ThemeGui::outlineThicknessSize);

    //устанавливает текст из KeyString && TextWstring
    Text TextCurrent(ThemeGui::currentFont, "", ThemeGui::characterSize);
    this->m_UIText = make_unique<Text>(TextCurrent);
    RefreshTranslate();

    // центрирует по вертикале относительно UI_Rectangle
    centerText();
    this->m_defaultTextColor = false;
    this->m_nameTheme = "";
    
    // устанавливает стандартную тему (т.е. первую в списке AllTheme)
    setTheme(m_nameTheme);
}

//-------------------------------------------------------------------------------------------------------------------

void GUIModdingGun::RefreshTranslate()
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

    // центрирует по вертикале относительно UI_Rectangle
    centerText();
}

//-------------------------------------------------------------------------------------------------------------------

bool GUIModdingGun::setTheme(string m_nameTheme)
{
    bool result = false;
    if (ThemeGui::allTheme.empty())
    {
        this->m_nameTheme = "";
        OUTPUT_LOG("GUIText -> allTheme empty");
        return false;
    }
    
    // устанавливает первую в списке если вдруг не найдем;
    auto vColor = ThemeGui::allTheme.begin()->second;

    if (ThemeGui::allTheme.find(m_nameTheme) != ThemeGui::allTheme.end())
    {
        vColor = ThemeGui::allTheme.at(m_nameTheme);
        result = true;
    }

    // для текста, если установлен стандартный (кастомный цвет не меняем нужно сначала сбросить )
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

    // заливка, если не поставлены текстуры
    if (m_UIRectangle.getTexture() == nullptr)
    {
        m_UIRectangle.setFillColor(vColor[2]);
    }

    this->m_nameTheme = m_nameTheme;
    return result;
}

//-------------------------------------------------------------------------------------------------------------------

void GUIModdingGun::setKeyStringAndTextWstring(const vector<variant<string, wstring>>& KeyString_and_TextWstring)
{
    this->m_keyStringAndTextWstring = KeyString_and_TextWstring;
    RefreshTranslate();
}

//-------------------------------------------------------------------------------------------------------------------

vector<variant<string, wstring>> GUIModdingGun::getKeyStringAndTextWstring()
{
    return m_keyStringAndTextWstring;
}

//-------------------------------------------------------------------------------------------------------------------

void GUIModdingGun::setRectangle(const RectangleShape& m_UIRectangle)
{
    this->m_UIRectangle = m_UIRectangle;    
    setTheme(m_nameTheme);
    centerText();

}

//-------------------------------------------------------------------------------------------------------------------

void GUIModdingGun::setAsPressedButton()
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

//-------------------------------------------------------------------------------------------------------------------

void GUIModdingGun::setAsReleasedButton()
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

//-------------------------------------------------------------------------------------------------------------------

bool GUIModdingGun::setTextureButton(const shared_ptr<Texture>& m_texturePressedButton, const shared_ptr<Texture>& m_textureReleasedButton)
{
    this->m_texturePressedButton = m_texturePressedButton;
    this->m_textureReleasedButton = m_textureReleasedButton;

    setTheme(m_nameTheme);
    return true;
}

//-------------------------------------------------------------------------------------------------------------------

Vector2f GUIModdingGun::getPositionRectangle()
{
    return GUIModdingGun::m_UIRectangle.getPosition();
}

//-------------------------------------------------------------------------------------------------------------------

void GUIModdingGun::setPositionRectangle(Vector2f position)
{
    GUIModdingGun::m_UIRectangle.setPosition(position);
    centerText();
}

//-------------------------------------------------------------------------------------------------------------------

const RectangleShape& GUIModdingGun::getRectangle()
{
    return m_UIRectangle;
}

//-------------------------------------------------------------------------------------------------------------------

bool GUIModdingGun::isButton()
{
    return m_isButton;
}

//-------------------------------------------------------------------------------------------------------------------

bool GUIModdingGun::isPressed()
{
    return m_isPressed;
}

//-------------------------------------------------------------------------------------------------------------------

const Text& GUIModdingGun::getText()
{
    return *m_UIText.get();
}

//-------------------------------------------------------------------------------------------------------------------

void GUIModdingGun::setColorText(Color TextColor)
{
    m_UIText.get()->setFillColor(TextColor);
    m_defaultTextColor = false;
}

//-------------------------------------------------------------------------------------------------------------------

void GUIModdingGun::resetColorText()
{
    auto vColor = ThemeGui::allTheme.begin()->second;

    if (ThemeGui::allTheme.find(m_nameTheme) != ThemeGui::allTheme.end())
    {
        vColor = ThemeGui::allTheme.at(m_nameTheme);
    }
    m_defaultTextColor = true;
    m_UIText.get()->setFillColor(vColor[0]);
}

//-------------------------------------------------------------------------------------------------------------------

GUIModdingGun::GUIModdingGun(const GUIModdingGun& other) : m_UIRectangle(other.m_UIRectangle), m_isButton(other.m_isButton), m_isPressed(other.m_isPressed), m_defaultTextColor(other.m_defaultTextColor), m_nameTheme(other.m_nameTheme), m_texturePressedButton(other.m_texturePressedButton), m_textureReleasedButton(other.m_textureReleasedButton)
{
    m_instancesGui.insert(this);
    this->m_keyStringAndTextWstring = other.m_keyStringAndTextWstring;

    Text currentText = *other.m_UIText.get();
    m_UIText = make_unique<Text>(currentText);
}

//-------------------------------------------------------------------------------------------------------------------

GUIModdingGun& GUIModdingGun::operator=(const GUIModdingGun& other)
{
    if (this == &other) { return *this; }

    m_UIRectangle = other.m_UIRectangle;
    m_isButton = other.m_isButton;
    m_isPressed = other.m_isPressed;
    m_defaultTextColor = other.m_defaultTextColor;
    m_nameTheme = other.m_nameTheme;
    m_texturePressedButton = other.m_texturePressedButton;
    m_textureReleasedButton = other.m_textureReleasedButton;

    m_keyStringAndTextWstring = other.m_keyStringAndTextWstring;

    if (other.m_UIText)
    {
        m_UIText = std::make_unique<Text>(*other.m_UIText);
    }
    else
    {
        m_UIText.reset();
    }
    return *this;
}

//-------------------------------------------------------------------------------------------------------------------

GUIModdingGun::~GUIModdingGun()
{
    m_instancesGui.erase(this);
}

//-------------------------------------------------------------------------------------------------------------------