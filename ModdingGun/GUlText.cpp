#include "GulText.h"

<<<<<<< HEAD
set<GUITextAndRectangle*> GUITextAndRectangle::m_instancesGui;

//-----------------------------------------------------------------------------------------------------------------------

void GUITextAndRectangle::centerText()
{
	float xOffset = m_UIRectangle.getSize().y * 0.3f;

	// Горизонтальное позиционирование — от левого края с отступом
	float x = m_UIRectangle.getPosition().x + xOffset;

	// Вертикальное центрирование
	float rectCenterY = m_UIRectangle.getPosition().y + m_UIRectangle.getSize().y / 2.0f;
	float textHeight = m_UIText.get()->getLocalBounds().size.y;
	float textOffsetY = m_UIText.get()->getLocalBounds().position.y;
=======
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
>>>>>>> main

	float y = rectCenterY - (textHeight / 2.0f + textOffsetY);

<<<<<<< HEAD
	// Уменьшение размера текста, если ширина превышает прямоугольник
	float maxWidth = m_UIRectangle.getSize().x - (xOffset * 2.0f); // допустимая ширина с учётом отступов
	unsigned int charSize = m_UIText.get()->getCharacterSize();

	while (m_UIText.get()->getLocalBounds().size.x > maxWidth && charSize > 5) // нижний предел
	{
		charSize -= 1;
		m_UIText.get()->setCharacterSize(charSize);
	}

	// Установка позиции
	m_UIText.get()->setPosition(Vector2f(x, y));
}

//-----------------------------------------------------------------------------------------------------------------------

GUITextAndRectangle::GUITextAndRectangle(const vector<variant<string, wstring>>& KeyString_and_TextWstring, const RectangleShape& UI_Rectangle, bool is_Button, bool is_Pressed)
{
	m_instancesGui.insert(this);

	this->m_KeyStringTextWstring = KeyString_and_TextWstring;
	this->m_isButton = is_Button;
	this->m_isPressed = is_Pressed;
	this->m_UIRectangle = UI_Rectangle;
	this->m_UIRectangle.setOutlineThickness(ThemeGui::outlineThicknessSizeGUI);

	//устанавливает текст из KeyString_and_TextWstring
	Text textCurrent(ThemeGui::currentFontGUI, "", ThemeGui::characterSizeGUI);
	this->m_UIText = make_unique<Text>(textCurrent);
	refreshTranslate();

	// центрируем по вертикале относительно UI_Rectangle
	centerText();

	this->m_defaultTextColor = false;
	this->m_nameTheme = "";

	// устанавливаем стандартную тему (т.е. первую в списке AllTheme)
	setTheme(m_nameTheme);
}

//-----------------------------------------------------------------------------------------------------------------------

void GUITextAndRectangle::refreshTranslate()
{
	if (m_KeyStringTextWstring.empty()) { return; }

	wstring fullWstring = L"";
	for (size_t it = 0; it < m_KeyStringTextWstring.size(); it++)
	{
		if (holds_alternative<string>(m_KeyStringTextWstring[it]))
		{
			string key = get<string>(m_KeyStringTextWstring[it]);
			fullWstring += LANG_TRANSLATE(key);
			fullWstring += L" ";
		}
		else if (holds_alternative<wstring>(m_KeyStringTextWstring[it]))
		{
			wstring text = get<wstring>(m_KeyStringTextWstring[it]);
			fullWstring += text;
			fullWstring += L" ";
		}
	}
	m_UIText.get()->setString(fullWstring);

	// центрируем по вертикале относительно UI_Rectangle
	centerText();
}

//-----------------------------------------------------------------------------------------------------------------------

bool GUITextAndRectangle::setTheme(string nameTheme)
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

	if (ThemeGui::allTheme.find(nameTheme) != ThemeGui::allTheme.end())
	{
		vColor = ThemeGui::allTheme.at(nameTheme);
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

	this->m_nameTheme = nameTheme;
	return result;
}

//-----------------------------------------------------------------------------------------------------------------------

void GUITextAndRectangle::setKeyStringTextWstring(const vector<variant<string, wstring>>& KeyString_and_TextWstring)
{
	this->m_KeyStringTextWstring = KeyString_and_TextWstring;
	/// обновляем UI_Text
	refreshTranslate();
}

//-----------------------------------------------------------------------------------------------------------------------

vector<variant<string, wstring>> GUITextAndRectangle::getKeyStringTextWstring()
{
	return m_KeyStringTextWstring;
}

//-----------------------------------------------------------------------------------------------------------------------

void GUITextAndRectangle::setRectangle(const RectangleShape& UIRectangle)
{
	this->m_UIRectangle = UIRectangle;

	setTheme(m_nameTheme);

	centerText();

}

//-----------------------------------------------------------------------------------------------------------------------

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

//-----------------------------------------------------------------------------------------------------------------------

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

//-----------------------------------------------------------------------------------------------------------------------

bool GUITextAndRectangle::setTextureButton(const shared_ptr<Texture>& texturePressedButton, const shared_ptr<Texture>& textureReleasedButton)
{
	m_texturePressedButton = texturePressedButton;
	m_textureReleasedButton = textureReleasedButton;

	setTheme(m_nameTheme);
	return true;
}

//-----------------------------------------------------------------------------------------------------------------------

Vector2f GUITextAndRectangle::getPositionRectangle()
{
	return GUITextAndRectangle::m_UIRectangle.getPosition();
}

//-----------------------------------------------------------------------------------------------------------------------

void GUITextAndRectangle::setPositionRectangle(Vector2f position)
{
	GUITextAndRectangle::m_UIRectangle.setPosition(position);
	centerText();
}

//-----------------------------------------------------------------------------------------------------------------------

const RectangleShape& GUITextAndRectangle::getRectangle()
{
	return m_UIRectangle;
}

//-----------------------------------------------------------------------------------------------------------------------

bool GUITextAndRectangle::isButton() const
{
	return m_isButton;
}

//-----------------------------------------------------------------------------------------------------------------------

bool GUITextAndRectangle::isPressed() const
{
	return m_isPressed;
}

//-----------------------------------------------------------------------------------------------------------------------

const Text& GUITextAndRectangle::getText()
{
	return *m_UIText.get();
}

//-----------------------------------------------------------------------------------------------------------------------

void GUITextAndRectangle::setColorText(Color textColor)
{
	m_UIText.get()->setFillColor(textColor);
	m_defaultTextColor = false;
}

//-----------------------------------------------------------------------------------------------------------------------

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

//-----------------------------------------------------------------------------------------------------------------------

GUITextAndRectangle& GUITextAndRectangle::operator=(const GUITextAndRectangle& other)
{
    if (this == &other)
        return *this;

    m_instancesGui.insert(this);

    m_KeyStringTextWstring = other.m_KeyStringTextWstring;

    m_UIText = other.m_UIText
        ? std::make_unique<Text>(*other.m_UIText)
        : nullptr;

    m_UIRectangle = other.m_UIRectangle;

    m_isButton = other.m_isButton;
    m_isPressed = other.m_isPressed;

    m_defaultTextColor = other.m_defaultTextColor;
    m_nameTheme = other.m_nameTheme;

    m_texturePressedButton = other.m_texturePressedButton;
    m_textureReleasedButton = other.m_textureReleasedButton;
=======
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
>>>>>>> main

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

<<<<<<< HEAD
//-----------------------------------------------------------------------------------------------------------------------

GUITextAndRectangle::GUITextAndRectangle(const GUITextAndRectangle& other) : m_KeyStringTextWstring(other.m_KeyStringTextWstring),
	m_UIText(other.m_UIText  ? std::make_unique<Text>(*other.m_UIText): nullptr),
	m_UIRectangle(other.m_UIRectangle),
	m_isButton(other.m_isButton),
	m_isPressed(other.m_isPressed),
	m_defaultTextColor(other.m_defaultTextColor),
	m_nameTheme(other.m_nameTheme),
	m_texturePressedButton(other.m_texturePressedButton),
	m_textureReleasedButton(other.m_textureReleasedButton)
{
	m_instancesGui.insert(this);
}

//-----------------------------------------------------------------------------------------------------------------------

GUITextAndRectangle::~GUITextAndRectangle()
{
	m_instancesGui.erase(this);
}

//-----------------------------------------------------------------------------------------------------------------------
=======
//-------------------------------------------------------------------------------------------------------------------

GUIModdingGun::~GUIModdingGun()
{
    m_instancesGui.erase(this);
}

//-------------------------------------------------------------------------------------------------------------------
>>>>>>> main
