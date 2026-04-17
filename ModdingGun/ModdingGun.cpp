#include "ModdingGun.h"

int main()
{
	// создаем вывод в логи
	permissions("Log/log.txt", perms::all); remove("Log/log.txt"); OUTPUT_LOG("Start!");
	locale::global(std::locale("en_US.UTF-8"));

	/// высота окна, от нее зависит длина окна
<<<<<<< HEAD
	const float  heightWindowMain = 700.f;
	const float  lengthWindowMain = heightWindowMain * (16.0f / 9.0f);

	// создаем диалоговое окно графика
	const unsigned int heightWindowDiagram = static_cast<unsigned>(heightWindowMain * 0.8);
	const unsigned int lengthWindowDiagram = static_cast<unsigned>(heightWindowDiagram * (20.0f / 9.0f));
=======
	float  heightWindowMain = 700.f;
	float  lengthWindowMain = heightWindowMain * (16.0f / 9.0f);

	// создаем диалоговое окно графика
	unsigned int heightWindowDiagram = static_cast<unsigned>(heightWindowMain * 0.8);
	unsigned int lengthWindowDiagram = static_cast<unsigned>(heightWindowDiagram * (20.0f / 9.0f));
>>>>>>> main

	// количество клеток по сторонам
	// устарело
	[[maybe_unused]]
	const unsigned int countCellOnLength = 20;
	// устарело
	[[maybe_unused]]
	const unsigned int countCellOnHeight = 5;

<<<<<<< HEAD
	float defaultSizeCell = static_cast<float>(lengthWindowMain / countCellOnLength);

	// переменные  мышки
	Vector2i positionMouseMainWindow = {};
	Vector2f postionMouseGlobal = {};

	shared_ptr<Texture> TexturePoint_Empty = make_shared<Texture>(L"Assets/Standart/Empty.png");
	shared_ptr<Texture> TexturePoint_RedMark = make_shared<Texture>(L"Assets/Standart/MarkX.png");
	shared_ptr<Texture> TexturePoint_GreenMark = make_shared<Texture>(L"Assets/Standart/MarkV.png");

	const path languageFolder = L"Lang/";
	const path statGunFolder = L"Assets/Gun";

	const path imagesToolFolder = L"Assets/Tool";
	const path imagesKitFolder = L"Assets/Kit";
	const path imagesSkillFolder = L"Assets/Skill";

	const path fileAttributeWeapon = L"Attribute/AllGunStat/weaponstat.json";
	const path filePriceModifiersWeapon = L"Attribute/PriceModifiers/PriceModifiers.json";
=======
	// количество клеток по сторонам

	// устарело
	[[maybe_unused]]
	unsigned int countCellOnLength = 20;

	// устарело
	[[maybe_unused]]
	unsigned int countCellOnHeight = 5;

	float sizeCellDefault = static_cast<float>(lengthWindowMain / countCellOnLength);

	// переменные  позиции мышки
	Vector2i positionMouseForMain;
	Vector2f mouseWorldPosForMain;

	shared_ptr<Texture> texturePointEmpty = make_shared<Texture>(L"Assets/Standart/empty.png");
	shared_ptr<Texture> texturePointRedMark = make_shared<Texture>(L"Assets/Standart/MarkX.png");
	shared_ptr<Texture> texturePointGreenMark = make_shared<Texture>(L"Assets/Standart/MarkV.png");
>>>>>>> main

	path langFolder = L"Lang/";

<<<<<<< HEAD
	path backgroundPathDiagram = L"Assets/Standart/background_graf.png";
	path backgroundPatchMain = L"Assets/Standart/background.png";

	// колличество повторений сборок мода
	unsigned int totalIterations = 5000;

	// записанные шаги модификаций 
	vector<Method> methodStepMod;

	// текущее оружие
	string nameGun = "Empty";

	// текущие модификаторы улучающие шанс прока
	SelectModifiers currentModifiers =
	{
		GunStats::Modifiers::ToolType::old,
		GunStats::Modifiers::KitType::noKit,
		GunStats::Modifiers::SkillType::playerMasterLvl5
	};

	PriceModifiers currentPrice = { 0,	0,	0 };

	LANG_SETTING.loadLangInFolder(languageFolder);

	vector<GUITextAndRectangle> arrLangGUI;
	{

		auto lang = LANG_SETTING.getLoadedLanguages();
		float sizeLangCell = defaultSizeCell * 0.5f;

		for (size_t i = 0; i < LANG_SETTING.getCountlang(); i++)
		{
			RectangleShape shapeLang;
			shapeLang.setSize({ sizeLangCell, sizeLangCell });
			shapeLang.setPosition(Vector2f(lengthWindowMain - sizeLangCell * i - defaultSizeCell, heightWindowMain - sizeLangCell * 2.7f));

			bool setAsPressedButton;
			i == 1 ? setAsPressedButton = false : setAsPressedButton = true;

			arrLangGUI.emplace_back(GUITextAndRectangle({ lang[i] }, shapeLang, true, setAsPressedButton));
		}
		LANG_SETTING.setLanguage(1);
	}

	vector<GUITextAndRectangle> VectorThemeGUI;
	{
		float ySizeLangCell = defaultSizeCell * 0.5f;
		float xSizeLangCell = defaultSizeCell * 1.5f;

		int itTheme = 0;

		for (const auto& theme : ThemeGui::allTheme)
		{
			auto itbeginTheme = ThemeGui::allTheme.begin();

			RectangleShape ShapeLang;
			ShapeLang.setSize({ xSizeLangCell ,  ySizeLangCell });
			ShapeLang.setPosition(Vector2f(lengthWindowMain - xSizeLangCell * itTheme - defaultSizeCell * 1.7f, heightWindowMain - ySizeLangCell * 1.5f));


			bool setAsPressedButton;
			itTheme == 0 ? setAsPressedButton = false : setAsPressedButton = true;

			VectorThemeGUI.emplace_back(GUITextAndRectangle({ string(theme.first) }, ShapeLang, true, setAsPressedButton));
			itTheme++;
		}
	}

	//////////////////////// МОДИФИКАТОРЫ ////////////////////////

	vector<shared_ptr<Texture>> kitTextures;
	vector<shared_ptr<Texture>> toolTextures;
	vector<shared_ptr<Texture>> skillTextures;

	vector<path> toolpath = searchFile(imagesToolFolder, ".png");
	vector<path> kitpath = searchFile(imagesKitFolder, ".png");
	vector<path> skillpath = searchFile(imagesSkillFolder, ".png");

	RectangleModifiers modifiersIcon;

	auto LoadTexturesAndShapes = [&](const vector<path>& paths, float Ypos, vector<shared_ptr<Texture>>& textures, vector<GUITextAndRectangle>& shapes)
		{
			float i = 0;
			float j = 0;

			float sizeCell = defaultSizeCell * 0.55f;

			for (const auto& patht : paths)
			{
				auto tex = make_shared<Texture>();

				if (!tex->loadFromFile(patht))
				{
					continue;
				}

				textures.push_back(tex);

				RectangleShape rectangleCurrentType;
				rectangleCurrentType.setSize(Vector2f(sizeCell, sizeCell));
				rectangleCurrentType.setPosition(Vector2f(defaultSizeCell * 2.25f + sizeCell * i, Ypos + j * sizeCell));
				rectangleCurrentType.setTexture(tex.get());

				string nameModifiersOne = patht.stem().string();
				nameModifiersOne.erase(0, 3);

				shapes.push_back(GUITextAndRectangle({ nameModifiersOne }, rectangleCurrentType, true, true));

				if (++i > 4)
				{
					j = 1.f;
					i = 0.f;
				}
			}
		};

	// Загружаем всё:
	LoadTexturesAndShapes(toolpath, defaultSizeCell * 1.5f, toolTextures, modifiersIcon.m_toolRectangleGUI);
	LoadTexturesAndShapes(kitpath, defaultSizeCell * 3.0f, kitTextures, modifiersIcon.m_kitRectangleGUI);
	LoadTexturesAndShapes(skillpath, defaultSizeCell * 4.5f, skillTextures, modifiersIcon.m_skillRectangleGUI);

	/// стоимость
	vector<GUITextAndRectangle> arrToolPrice;
	vector<GUITextAndRectangle> arrKitPrice;

	RectangleShape rectangleAllPrice;
	rectangleAllPrice.setSize(Vector2f(defaultSizeCell * 3.5f, defaultSizeCell * 0.4f));
	{
		wstring wstrPrice = L"price";
		const vector<pair<const float, const wstring>> allocation =
		{
			{0.13f, L"--"},
			{0.11f, L"-" },
			{0.5f, wstrPrice},
			{0.11f, L"+" },
			{0.13f, L"++"}
		};

		const float yOffset = defaultSizeCell * 0.05f;
		const float xOffset = defaultSizeCell * 0.02f;

		// tool 
		rectangleAllPrice.setPosition(Vector2f(defaultSizeCell * 2.25f, modifiersIcon.m_toolRectangleGUI.rbegin()->getRectangle().getPosition().y + modifiersIcon.m_toolRectangleGUI.rbegin()->getRectangle().getSize().y + yOffset));

		float xPosRAP = rectangleAllPrice.getPosition().x;
		for (const auto& [fl, ws] : allocation)
		{
			if (!arrToolPrice.empty())
			{
				xPosRAP += arrToolPrice.rbegin()->getRectangle().getSize().x + xOffset;
			}

			RectangleShape cellPrice;
			cellPrice.setPosition(Vector2f(xPosRAP, rectangleAllPrice.getPosition().y));
			cellPrice.setSize(Vector2f(rectangleAllPrice.getSize().x * fl, rectangleAllPrice.getSize().y));

			if (ws == wstrPrice)
			{
				wstring wstrValuePrice = L"0";

				ifstream inJsonFilePrice(filePriceModifiersWeapon);
				if (!inJsonFilePrice)
				{
					OUTPUT_LOG("CharacteristicGun -> Failed to open file: " + wstringToString(filePriceModifiersWeapon));
				}
				else
				{
					nlohmann::json jsonPrice;
					inJsonFilePrice >> jsonPrice;
					inJsonFilePrice.close();

					unsigned valPrice = 0;

					if (jsonPrice.contains("MD_Tool") && jsonPrice["MD_Tool"].contains(GunStats::Modifiers::getToolName(currentModifiers.m_tool)))
					{
						valPrice = jsonPrice["MD_Tool"][GunStats::Modifiers::getToolName(currentModifiers.m_tool)].get<unsigned>();
						wstrValuePrice = format(L"{:L}", valPrice);
					}
					else
					{
						jsonPrice["MD_Tool"][GunStats::Modifiers::getToolName(currentModifiers.m_tool)] = 0;

						ofstream ofJsonFilePrice(filePriceModifiersWeapon);
						ofJsonFilePrice << jsonPrice;
						ofJsonFilePrice.close();
					}
					currentPrice.m_priceTool = valPrice;

				}
				arrToolPrice.push_back(move(GUITextAndRectangle({ wstrValuePrice, L"₽" }, cellPrice, false, false)));
			}
			else
			{
				arrToolPrice.push_back(move(GUITextAndRectangle({ ws }, cellPrice, true, false)));
			}
		}

		// kit 
		rectangleAllPrice.setPosition(Vector2f(defaultSizeCell * 2.25f, modifiersIcon.m_kitRectangleGUI.rbegin()->getRectangle().getPosition().y + modifiersIcon.m_kitRectangleGUI.rbegin()->getRectangle().getSize().y + yOffset));
		xPosRAP = rectangleAllPrice.getPosition().x;
		for (const auto& [fl, ws] : allocation)
		{
			if (!arrKitPrice.empty())
			{
				xPosRAP += arrKitPrice.rbegin()->getRectangle().getSize().x + xOffset;
			}

			RectangleShape cellPrice;
			cellPrice.setPosition(Vector2f(xPosRAP, rectangleAllPrice.getPosition().y));
			cellPrice.setSize(Vector2f(rectangleAllPrice.getSize().x * fl, rectangleAllPrice.getSize().y));


			if (ws == wstrPrice)
			{
				wstring wstrValuePrice = L"0";
				arrKitPrice.push_back(move(GUITextAndRectangle({ wstrValuePrice, L"₽" }, cellPrice, false, false)));
			}
			else
			{
				arrKitPrice.push_back(move(GUITextAndRectangle({ ws }, cellPrice, true, false)));
			}
			currentPrice.m_priceKit = 0;

		}

	}
	// skill
	rectangleAllPrice.setPosition(Vector2f(defaultSizeCell * 2.25f, modifiersIcon.m_skillRectangleGUI.rbegin()->getRectangle().getPosition().y + modifiersIcon.m_skillRectangleGUI.rbegin()->getRectangle().getSize().y + defaultSizeCell * 0.05f));
	GUITextAndRectangle skillPrice({ L"0", L"₽" }, move(rectangleAllPrice), false, false);

	/// устанавливаем настройки по умолчанию
	// по умолчанию старый инструмент
	for (size_t it = 0; it < modifiersIcon.m_toolRectangleGUI.size(); it++)
	{
		auto name = *modifiersIcon.m_toolRectangleGUI[it].getKeyStringTextWstring().begin();

		if (holds_alternative<string>(name))
		{
			string sName = get<string>(name);

			if (sName == GunStats::Modifiers::getToolName(currentModifiers.m_tool))
			{
				modifiersIcon.m_toolRectangleGUI[it].setAsPressedButton();
				break;
			}
		}
	}

	// по умолчанию без набора
	for (size_t it = 0; it < modifiersIcon.m_kitRectangleGUI.size(); it++)
	{
		auto name = *modifiersIcon.m_kitRectangleGUI[it].getKeyStringTextWstring().begin();

		if (holds_alternative<string>(name))
		{
			string sName = get<string>(name);

			if (sName == GunStats::Modifiers::GetKitName(currentModifiers.m_kit))
			{
				modifiersIcon.m_kitRectangleGUI[it].setAsPressedButton();
				break;
			}
		}
	}

	// игрок мастер 5
	for (size_t it = 0; it < modifiersIcon.m_skillRectangleGUI.size(); it++)
	{
		auto name = modifiersIcon.m_skillRectangleGUI[it].getKeyStringTextWstring().front();
		if (holds_alternative<string>(name))
		{
			string sName = get<string>(name);
			if (sName == GunStats::Modifiers::getSkillName(currentModifiers.m_skill))
			{
				modifiersIcon.m_skillRectangleGUI[it].setAsPressedButton();
				break;
			}
		}
	}

	//////////////////////// ИКОНКИ ОРУЖИЯ  И ВЫБРАННОГО ОРУЖИЯ ////////////////////////

	vector<Weapon> vectorGun;

	vector<path> searchTextureGun = searchFile(statGunFolder, ".png");
	if (searchTextureGun.empty())
	{
		OUTPUT_LOG("main -> Texture Gun not found");
		return -1;
	}

	unsigned int itVectorGun = 0;
	int countGunInList = 22;

	for (const auto& currentGun : searchTextureGun)
	{
		shared_ptr<Texture> ptrTextureWeapon;
		string nameWeapon = wstringToString(currentGun.stem());

		RectangleShape rectangleWeaponCurrent;
		rectangleWeaponCurrent.setSize(Vector2f(defaultSizeCell * 2, defaultSizeCell / 2));

		vectorGun.emplace_back(GUITextAndRectangle({ nameWeapon }, rectangleWeaponCurrent, false, false), nameWeapon, ptrTextureWeapon);
	}

	// для списка
	GUITextAndRectangle allGunShapeScrollGUI = [defaultSizeCell, countGunInList]()
		{
			RectangleShape allGunShapeScroll;
			allGunShapeScroll.setSize(Vector2f(defaultSizeCell * 2, defaultSizeCell / 2 * countGunInList));
			allGunShapeScroll.setPosition(Vector2f(0, 0));

			return GUITextAndRectangle({}, allGunShapeScroll, false, false);
		}();
=======
	path gunFolder = L"Assets/gun";

	path toolFolder = L"Assets/Tool";
	path kitFolder = L"Assets/Kit";
	path skillFolder = L"Assets/Skill";

	path fileStat = L"Attribute/AllGunStat/weaponstat.json";
	path pathToPriceModifiersJS = L"Attribute/PriceModifiers/PriceModifiers.json";

	path backgroundPathDiagram = L"Assets/Standart/background_graf.png";
	path backgroundPatchMain = L"Assets/Standart/background.png";

	// колличество повторений сборок мода
	unsigned int totalIterations = 5000;

	// записанные шаги модификаций 
	vector<Method> methodMod;

	// текущее оружие
	string nameGun = "empty";

	// текущие модификаторы улучающие шанс прока
	Select_Modifiers currentModifiers =
	{
		GunStats::Modifiers::ToolType::old,
		GunStats::Modifiers::KitType::noKit,
		GunStats::Modifiers::SkillType::playerMaster5
	};

	PriceModifiers currentPrice =
	{
		0,
		0,
		0
	};

	LANG_SETTING.loadLangInFolder(langFolder);

	vector<GUIModdingGun> vectorLangGUI;
	{

		auto lang = LANG_SETTING.getLoadedLanguages();
		float sizeLangCell = sizeCellDefault * 0.5f;

		for (size_t i = 0; i < LANG_SETTING.getCountlang(); i++)
		{
			RectangleShape shapeLang;
			shapeLang.setSize({ sizeLangCell, sizeLangCell });
			shapeLang.setPosition(Vector2f(lengthWindowMain - sizeLangCell * i - sizeCellDefault, heightWindowMain - sizeLangCell * 2.7f));

			bool setAsPressedButton;
			i == 1 ? setAsPressedButton = false : setAsPressedButton = true;

			vectorLangGUI.emplace_back(GUIModdingGun({ lang[i] }, shapeLang, true, setAsPressedButton));
		}
		LANG_SETTING.setLanguage(1);
	}

	vector<GUIModdingGun> vectorThemeGUI;

	{
		float ySizeLangCell = sizeCellDefault * 0.5f;
		float xSizeLangCell = sizeCellDefault * 1.5f;

		int itTheme = 0;

		for (const auto& theme : ThemeGui::allTheme)
		{
			auto itbeginTheme = ThemeGui::allTheme.begin();

			RectangleShape shapeLang;
			shapeLang.setSize({ xSizeLangCell ,  ySizeLangCell });
			shapeLang.setPosition(Vector2f(lengthWindowMain - xSizeLangCell * itTheme - sizeCellDefault * 1.7f, heightWindowMain - ySizeLangCell * 1.5f));


			bool setAsPressedButton;
			itTheme == 0 ? setAsPressedButton = false : setAsPressedButton = true;

			vectorThemeGUI.emplace_back(GUIModdingGun({ string(theme.first) }, shapeLang, true, setAsPressedButton));
			itTheme++;
		}
	}

	//////////////////////// МОДИФИКАТОРЫ ////////////////////////

	vector<shared_ptr<Texture>> kitTextures;
	vector<shared_ptr<Texture>> toolTextures;
	vector<shared_ptr<Texture>> skillTextures;

	vector<path> toolpath = searchFile(toolFolder, ".png");
	vector<path> kitpath = searchFile(kitFolder, ".png");
	vector<path> skillpath = searchFile(skillFolder, ".png");
>>>>>>> main

	struct RectangleModifiers
	{
		vector<GUIModdingGun> m_toolRectangleGUI;
		vector<GUIModdingGun> m_kitRectangleGUI;
		vector<GUIModdingGun> m_skillRectangleGUI;

<<<<<<< HEAD
	// - выбранное оружие
	GUITextAndRectangle selectWeaponGUI = [defaultSizeCell]()
		{
			RectangleShape shapeSelectedGun;
			shapeSelectedGun.setSize(Vector2f(defaultSizeCell * 2.5f, defaultSizeCell * 0.5f));
			shapeSelectedGun.setPosition(Vector2f(defaultSizeCell * 2.5f, defaultSizeCell * 0.5f));
			//shapeSelectedGun.setTexture(TexturePoint_Empty.get(), true);

			return GUITextAndRectangle({ "Gun" }, shapeSelectedGun, false, false);
		}();


	GUITextAndRectangle topScrollToolTip = [defaultSizeCell, &vectorGun]()
		{
			auto position = vectorGun[0].m_weapoGUI.getRectangle().getPosition() + Vector2f(vectorGun[0].m_weapoGUI.getRectangle().getSize().x, 0);
			auto size = vectorGun[0].m_weapoGUI.getRectangle().getSize().y;

			RectangleShape toolTip;
			toolTip.setSize(Vector2f(size, size));
			toolTip.setPosition(position);

			return GUITextAndRectangle({}, toolTip, false, false);
=======
	};

	RectangleModifiers modifiersIcon;

	auto LoadTexturesAndShapes = [&](const vector<path>& paths, float Ypos, vector<shared_ptr<Texture>>& textures, vector<GUIModdingGun>& shapes)
		{
			float i = 0;
			float j = 0;

			float size = sizeCellDefault * 0.55f;

			for (const auto& path : paths)
			{
				auto tex = make_shared<Texture>();

				if (!tex->loadFromFile(path))
				{
					continue;
				}

				textures.push_back(tex);

				RectangleShape rectangleCurrentType;
				rectangleCurrentType.setSize(Vector2f(size, size));
				rectangleCurrentType.setPosition(Vector2f(sizeCellDefault * 2.25f + size * i, Ypos + j * size));
				rectangleCurrentType.setTexture(tex.get());

				string nameModifiers = path.stem().string();
				nameModifiers.erase(0, 3);

				shapes.push_back(GUIModdingGun({ nameModifiers }, rectangleCurrentType, true, true));

				i++;
				if (i > 4)
				{
					j = 1.f; i = 0.f;
				}
			}
		};

	// Загружаем всё:
	LoadTexturesAndShapes(toolpath, sizeCellDefault * 1.5f, toolTextures, modifiersIcon.m_toolRectangleGUI);
	LoadTexturesAndShapes(kitpath, sizeCellDefault * 3.0f, kitTextures, modifiersIcon.m_kitRectangleGUI);
	LoadTexturesAndShapes(skillpath, sizeCellDefault * 4.5f, skillTextures, modifiersIcon.m_skillRectangleGUI);

	/// стоимость
	vector<GUIModdingGun> toolPriceAll;
	vector<GUIModdingGun> kitPrice;

	RectangleShape rectangleAllPrice;
	rectangleAllPrice.setSize(Vector2f(sizeCellDefault * 3.5f, sizeCellDefault * 0.4f));

	{
		wstring namePrice = L"price";
		const vector<pair<const float, const wstring>> allocation =
		{
			{0.13f, L"--"},
			{0.11f, L"-" },
			{0.5f, namePrice},
			{0.11f, L"+" },
			{0.13f, L"++"}
		};

		const float yOffset = sizeCellDefault * 0.05f;
		const float xOffset = sizeCellDefault * 0.02f;

		// tool 

		rectangleAllPrice.setPosition(Vector2f(sizeCellDefault * 2.25f, modifiersIcon.m_toolRectangleGUI.rbegin()->getRectangle().getPosition().y + modifiersIcon.m_toolRectangleGUI.rbegin()->getRectangle().getSize().y + yOffset));

		float xPos = rectangleAllPrice.getPosition().x;
		for (const auto& [fl, ws] : allocation)
		{
			if (!toolPriceAll.empty())
			{
				xPos += toolPriceAll.rbegin()->getRectangle().getSize().x + xOffset;
			}


			RectangleShape cellPrice;
			cellPrice.setPosition(Vector2f(xPos, rectangleAllPrice.getPosition().y));
			cellPrice.setSize(Vector2f(rectangleAllPrice.getSize().x * fl, rectangleAllPrice.getSize().y));

			if (ws == namePrice)
			{
				wstring strValuePrice = L"0";

				ifstream inJsonFilePrice(pathToPriceModifiersJS);
				if (!inJsonFilePrice)
				{
					OUTPUT_LOG("CharacteristicGun -> Failed to open file: " + wstringToString(pathToPriceModifiersJS));
				}
				else
				{
					nlohmann::json jsonPrice;
					inJsonFilePrice >> jsonPrice;
					inJsonFilePrice.close();

					unsigned valPrice = 0;

					if (jsonPrice.contains("MD_Tool") && jsonPrice["MD_Tool"].contains(GunStats::Modifiers::GetToolName(currentModifiers.Tool)))
					{
						valPrice = jsonPrice["MD_Tool"][GunStats::Modifiers::GetToolName(currentModifiers.Tool)].get<unsigned>();
						strValuePrice = format(L"{:L}", valPrice);

					}
					else
					{
						jsonPrice["MD_Tool"][GunStats::Modifiers::GetToolName(currentModifiers.Tool)] = 0;

						ofstream inJsonFilePrice(pathToPriceModifiersJS);
						inJsonFilePrice << jsonPrice;
						inJsonFilePrice.close();
					}
					currentPrice.PriceTool = valPrice;

				}
				toolPriceAll.push_back(move(GUIModdingGun({ strValuePrice, L"₽" }, cellPrice, false, false)));
			}
			else
			{
				toolPriceAll.push_back(move(GUIModdingGun({ ws }, cellPrice, true, false)));
			}
		}

		// kit 
		rectangleAllPrice.setPosition(Vector2f(sizeCellDefault * 2.25f, modifiersIcon.m_kitRectangleGUI.rbegin()->getRectangle().getPosition().y + modifiersIcon.m_kitRectangleGUI.rbegin()->getRectangle().getSize().y + yOffset));
		xPos = rectangleAllPrice.getPosition().x;
		for (const auto& [fl, ws] : allocation)
		{
			if (!kitPrice.empty())
			{
				xPos += kitPrice.rbegin()->getRectangle().getSize().x + xOffset;
			}

			RectangleShape cellPrice;
			cellPrice.setPosition(Vector2f(xPos, rectangleAllPrice.getPosition().y));
			cellPrice.setSize(Vector2f(rectangleAllPrice.getSize().x * fl, rectangleAllPrice.getSize().y));


			if (ws == namePrice)
			{
				wstring strValuePrice = L"0";
				kitPrice.push_back(move(GUIModdingGun({ strValuePrice, L"₽" }, cellPrice, false, false)));
			}
			else
			{
				kitPrice.push_back(move(GUIModdingGun({ ws }, cellPrice, true, false)));
			}
			currentPrice.PriceKit = 0;

		}

	}

	// skill
	rectangleAllPrice.setPosition(Vector2f(sizeCellDefault * 2.25f, modifiersIcon.m_skillRectangleGUI.rbegin()->getRectangle().getPosition().y + modifiersIcon.m_skillRectangleGUI.rbegin()->getRectangle().getSize().y + sizeCellDefault * 0.05f));
	GUIModdingGun skillPrice({ L"0", L"₽" }, move(rectangleAllPrice), false, false);

	/// устанавливаем настройки по умолчанию
	// по умолчанию старый инструмент
	for (size_t it = 0; it < modifiersIcon.m_toolRectangleGUI.size(); it++)
	{
		auto name = *modifiersIcon.m_toolRectangleGUI[it].getKeyStringAndTextWstring().begin();

		if (holds_alternative<string>(name))
		{
			string SName = get<string>(name);

			if (SName == GunStats::Modifiers::GetToolName(currentModifiers.Tool))
			{
				modifiersIcon.m_toolRectangleGUI[it].setAsPressedButton();
				break;
			}
		}
	}

	// по умолчанию без набора
	for (size_t it = 0; it < modifiersIcon.m_kitRectangleGUI.size(); it++)
	{
		auto name = *modifiersIcon.m_kitRectangleGUI[it].getKeyStringAndTextWstring().begin();

		if (holds_alternative<string>(name))
		{
			string sName = get<string>(name);

			if (sName == GunStats::Modifiers::GetKitName(currentModifiers.Kit))
			{
				modifiersIcon.m_kitRectangleGUI[it].setAsPressedButton();
				break;
			}
		}
	}

	// игрок мастер 5
	for (size_t it = 0; it < modifiersIcon.m_skillRectangleGUI.size(); it++)
	{
		auto name = *modifiersIcon.m_skillRectangleGUI[it].getKeyStringAndTextWstring().begin();

		if (holds_alternative<string>(name))
		{
			string sName = get<string>(name);

			if (sName == GunStats::Modifiers::GetSkillName(currentModifiers.Skill))
			{
				modifiersIcon.m_skillRectangleGUI[it].setAsPressedButton();
				break;
			}
		}
	}

	//////////////////////// ИКОНКИ ОРУЖИЯ  И ВЫБРАННОГО ОРУЖИЯ ////////////////////////

	struct VectorWeapon
	{
		shared_ptr<Texture> m_WTexture;
		string m_name;
		GUIModdingGun m_WeapoGUI;
	};

	vector<VectorWeapon> arrayGun;

	/////////////////////////////////////////////////////////////////////////////
	vector<path> searchTextureGun = searchFile(gunFolder, ".png");
	if (searchTextureGun.empty())
	{
		OUTPUT_LOG("main -> Texture gun not found");
		return -1;
	}
>>>>>>> main

		}();

	/////////////////////////////////////////////////////////////////////////////
	unsigned int itArrayGun = 0;
	int countGunList = 22;

<<<<<<< HEAD
	GUITextAndRectangle downScrollToolTip = [defaultSizeCell, &vectorGun, countGunInList]()
		{
			auto position = Vector2f(vectorGun[0].m_weapoGUI.getRectangle().getPosition().x + vectorGun[0].m_weapoGUI.getRectangle().getSize().x, vectorGun[0].m_weapoGUI.getRectangle().getSize().y * (countGunInList - 1));

			auto size = vectorGun[0].m_weapoGUI.getRectangle().getSize().y;
=======
	for (const auto gun : searchTextureGun)
	{
		shared_ptr<Texture> ptrTextureWeapon;
		string NameWeapon = wstringToString(gun.stem());

		RectangleShape RectangleWeaponCurrent;
		RectangleWeaponCurrent.setSize(Vector2f(sizeCellDefault * 2, sizeCellDefault / 2));
>>>>>>> main

			RectangleShape toolTip;
			toolTip.setSize(Vector2f(size, size));
			toolTip.setPosition(position);

<<<<<<< HEAD
			return GUITextAndRectangle({ to_wstring(vectorGun.size() - countGunInList) + L"+" }, toolTip, false, false);

		}();

	//////////////////////// КНОПКА СБОРКИ ////////////////////////

	GUITextAndRectangle calculation = [defaultSizeCell, countCellOnHeight]()
		{
			RectangleShape rectangleCalculate;
			rectangleCalculate.setSize(Vector2f(defaultSizeCell * 2.f, defaultSizeCell / 2.f));
			rectangleCalculate.setPosition(Vector2f(defaultSizeCell * 13.5f, defaultSizeCell * static_cast<float>(countCellOnHeight + 3)));
=======
		arrayGun.push_back({ ptrTextureWeapon, NameWeapon, GUIModdingGun({NameWeapon}, RectangleWeaponCurrent, false, false) });
	}

	// для списка
	GUIModdingGun allGunShapeScrollGUI = [sizeCellDefault, countGunList]()
		{
			RectangleShape ALLGunShapeScroll;
			ALLGunShapeScroll.setSize(Vector2f(sizeCellDefault * 2, sizeCellDefault / 2 * countGunList));
			ALLGunShapeScroll.setPosition(Vector2f(0, 0));

			return GUIModdingGun({}, ALLGunShapeScroll, false, false);
		}();

	// - выбранное оружие
	GUIModdingGun selectWeaponGUI = [sizeCellDefault]()
		{
			RectangleShape shapeSelectedGun;
			shapeSelectedGun.setSize(Vector2f(sizeCellDefault * 2.5f, sizeCellDefault * 0.5f));
			shapeSelectedGun.setPosition(Vector2f(sizeCellDefault * 2.5f, sizeCellDefault * 0.5f));
			//shapeSelectedGun.setTexture(texturePointEmpty.get(), true);
>>>>>>> main

			return GUIModdingGun({ "gun" }, shapeSelectedGun, false, false);
		}();

<<<<<<< HEAD
			return GUITextAndRectangle({ "Build" }, rectangleCalculate, true, false);

		}();

	//////////////////////// ОКНО ПАРАМЕТРОВ МОДА ////////////////////////

	GUITextAndRectangle shaheGeneralMod = [defaultSizeCell]()
		{
			RectangleShape generalMod;
			generalMod.setPosition(Vector2f(defaultSizeCell * 6.f, defaultSizeCell * 0.5f));
			generalMod.setSize(Vector2f(defaultSizeCell * 13.8f, defaultSizeCell * 5.15f));
=======
	GUIModdingGun topScrollToolTip = [sizeCellDefault, &arrayGun]()
		{
			auto position = arrayGun[0].m_WeapoGUI.getRectangle().getPosition() + Vector2f(arrayGun[0].m_WeapoGUI.getRectangle().getSize().x, 0);
			auto size = arrayGun[0].m_WeapoGUI.getRectangle().getSize().y;

			RectangleShape toolTip;
			toolTip.setSize(Vector2f(size, size));
			toolTip.setPosition(position);

			return GUIModdingGun({}, toolTip, false, false);

		}();
>>>>>>> main

			return GUITextAndRectangle({}, generalMod, false, false);

<<<<<<< HEAD
		}();

	float ySizeAll = defaultSizeCell * 0.39f;

	vector<GeneralStat> generalModOption = [defaultSizeCell, TexturePoint_GreenMark, TexturePoint_RedMark, &shaheGeneralMod, ySizeAll]()
		{
			vector<GeneralStat> result;

			float ySizeGeneral = shaheGeneralMod.getRectangle().getSize().y;
			float xSizeGeneral = shaheGeneralMod.getRectangle().getSize().x;
			float xPosGeneral = shaheGeneralMod.getRectangle().getPosition().x;

			// отступ сверху || снизу
			float yOffsetOne = ySizeGeneral * 0.15f;

			// отступ между
			float yOffsetRelated = ySizeGeneral * 0.03f;

			// позиция начальной не считая YOffsetOne
			float yPosOneCharacteristic = (ySizeGeneral - yOffsetOne * 2.f) / (GunStats::countCharacteristic + 2);

			// процентарное соотношение x размера (не учитывая кнопку)            
			const array<float, 5> percentSizeCell = { 0.12f, 0.06f, 0.06f, 0.72f, 0.04f }; // sum = ~1  |||  => const 0.04

			for (size_t it = 0; it < GunStats::countCharacteristic; it++)
			{
				RectangleShape rectangleTemp;

				// имя характеристики 
				float XsizeName = xSizeGeneral * percentSizeCell[0];

				rectangleTemp.setPosition({ xPosGeneral , yOffsetOne + yPosOneCharacteristic * it + yOffsetRelated * it });
				rectangleTemp.setSize({ XsizeName, ySizeAll });

				GUITextAndRectangle nameCharacteristic({ string(GunStats::indexCharacteristicName[it]) }, rectangleTemp, false, false);

				/// текущее отображение характеристики \\ темп пробитие ... 
				float XsizeVisualCharacteristic = xSizeGeneral * percentSizeCell[1];

				rectangleTemp.setPosition(rectangleTemp.getPosition() + Vector2f(XsizeName, 0));
				rectangleTemp.setSize({ XsizeVisualCharacteristic, ySizeAll });

				GUITextAndRectangle visualCharacteristic({ L"0" }, rectangleTemp, false, false);

				/// визуальное отображения процента апгрейда
				float XsizeVisualPercent = xSizeGeneral * percentSizeCell[2];

				rectangleTemp.setPosition(rectangleTemp.getPosition() + Vector2f(XsizeVisualCharacteristic, 0));
				rectangleTemp.setSize({ XsizeVisualPercent, ySizeAll });

				GUITextAndRectangle visualPercentUpgrade({ L"0.0%" }, rectangleTemp, false, false);

				// Общий "прямоугольник" в котором распалагаютися все юниты
				float XsizeRectangleALLUnits = xSizeGeneral * percentSizeCell[3];

				RectangleShape rectanpleForUnits;
				rectanpleForUnits.setPosition({ rectangleTemp.getPosition() + Vector2f(XsizeVisualPercent, 0) });
				rectanpleForUnits.setSize(Vector2f(XsizeRectangleALLUnits, ySizeAll));

				// размещаем едины в RectanpleForUnits
				vector<RectangleShape> resultShapeUnits;

				// максимальное колво единиц для текущей характеристики
				int   countUnits = GunStats::getCountUnitsForCharacteristic[it];

				float xPosUnits = rectanpleForUnits.getPosition().x;
				float yPosUnits = rectanpleForUnits.getPosition().y;
				float xSizeUnits = rectanpleForUnits.getSize().x / countUnits;
				float ySizeUnits = rectanpleForUnits.getSize().y;

				for (int currentUnit = 0; currentUnit < countUnits; currentUnit++)
				{
					RectangleShape unitShahe;
					unitShahe.setSize({ xSizeUnits, ySizeUnits });
					unitShahe.setPosition({ xPosUnits + xSizeUnits * currentUnit, yPosUnits });
					unitShahe.setFillColor(Color(66, 66, 66));
					unitShahe.setOutlineColor(Color(10, 10, 10));
					unitShahe.setOutlineThickness(-1);

					resultShapeUnits.push_back(unitShahe);
				}

				/// кнопка для выбора
				float resizeMark = 0.7f; // размер кнопки от максимума %

				// размер кнопки
				float sizeCellButton = rectangleTemp.getSize().y * resizeMark;

				// смещение для центрирования
				float offsetSizeCellButton = rectangleTemp.getSize().y * ((1.f - resizeMark) / 2.f);

				rectangleTemp.setPosition(Vector2f(rectanpleForUnits.getPosition().x + rectanpleForUnits.getSize().x, rectanpleForUnits.getPosition().y) + Vector2f(offsetSizeCellButton * 2, offsetSizeCellButton));

				rectangleTemp.setSize(Vector2f(sizeCellButton, sizeCellButton));

				GUITextAndRectangle buttonCharacteristic({}, rectangleTemp, true, false);
				buttonCharacteristic.setTextureButton(TexturePoint_GreenMark, TexturePoint_RedMark);

				if (it == 0) { buttonCharacteristic.setAsPressedButton(); }

				result.emplace_back(nameCharacteristic, visualCharacteristic, visualPercentUpgrade, buttonCharacteristic, rectanpleForUnits, resultShapeUnits);
			}
			return result;
		}();

	float yDownOffset = 0.7f;
	// между
	float xOffsetRelated = defaultSizeCell * 0.25f;

	/// отображение шанса на улучшение (внизу)
	GUITextAndRectangle textGeneralModChance = [defaultSizeCell, &shaheGeneralMod, ySizeAll, yDownOffset]()
		{
			float Xsize = defaultSizeCell * 4.f;

			RectangleShape RectangleGeneralModChance;
			RectangleGeneralModChance.setSize({ Xsize , ySizeAll });

			auto& TempRectangle = shaheGeneralMod.getRectangle();
			RectangleGeneralModChance.setPosition(TempRectangle.getPosition() + Vector2f(0, TempRectangle.getSize().y - (defaultSizeCell * yDownOffset)));

			return GUITextAndRectangle({ "Chahce_upgrade" , string(GunStats::characteristicNameInGunStat[0]) ,  L" 0.0%" }, RectangleGeneralModChance, false, false);
		}();

	// кнопка применить как начальный
	GUITextAndRectangle shapeGeneralModApplyStatInitial = [defaultSizeCell, &textGeneralModChance, ySizeAll, yDownOffset, xOffsetRelated]()
		{
			float xSize = defaultSizeCell * 3;

			auto shapeGMC = textGeneralModChance.getRectangle();

			RectangleShape tempRec;
			tempRec.setSize(Vector2f(xSize, ySizeAll));
			tempRec.setPosition(Vector2f(shapeGMC.getPosition().x + shapeGMC.getSize().x + xOffsetRelated, shapeGMC.getPosition().y));

			return GUITextAndRectangle({ "Apply_as_initial_mod" }, tempRec, true, false);
		}();

	// кнопка применить как мод
	GUITextAndRectangle buttonGeneralModApplyStat = [defaultSizeCell, &shapeGeneralModApplyStatInitial, ySizeAll, xOffsetRelated]()
		{
			float Xsize = defaultSizeCell * 1.8f;

			auto& Rec = shapeGeneralModApplyStatInitial.getRectangle();

			RectangleShape tempRec;
			tempRec.setSize(Vector2f(Xsize, ySizeAll));
			tempRec.setPosition(Vector2f(Rec.getPosition().x + Rec.getSize().x + xOffsetRelated, Rec.getPosition().y));

			return GUITextAndRectangle({ "Apply" }, tempRec, true, false);
		}();

	// кнопка сбросить
	GUITextAndRectangle shapeGeneralModReset = [defaultSizeCell, &buttonGeneralModApplyStat, ySizeAll, xOffsetRelated]()
		{

			float Xsize = defaultSizeCell * 1.8f;

			auto& Rec = buttonGeneralModApplyStat.getRectangle();

			RectangleShape TempRec;
			TempRec.setSize(Vector2f(Xsize, ySizeAll));
			TempRec.setPosition(Vector2f(Rec.getPosition().x + Rec.getSize().x + xOffsetRelated, Rec.getPosition().y));


			return GUITextAndRectangle({ "Reset" }, TempRec, true, false);
		}();

	// кнопка назад
	GUITextAndRectangle shapeGeneralModBack = [defaultSizeCell, &shapeGeneralModReset, ySizeAll, xOffsetRelated]()
		{

			float Xsize = defaultSizeCell * 1.6f;

			auto& Rec = shapeGeneralModReset.getRectangle();
=======
	GUIModdingGun downScrollToolTip = [sizeCellDefault, &arrayGun, countGunList]()
		{
			auto position = Vector2f(arrayGun[0].m_WeapoGUI.getRectangle().getPosition().x + arrayGun[0].m_WeapoGUI.getRectangle().getSize().x, arrayGun[0].m_WeapoGUI.getRectangle().getSize().y * (countGunList - 1));

			auto size = arrayGun[0].m_WeapoGUI.getRectangle().getSize().y;

			RectangleShape toolTip;
			toolTip.setSize(Vector2f(size, size));
			toolTip.setPosition(position);

			return GUIModdingGun({ to_wstring(arrayGun.size() - countGunList) + L"+" }, toolTip, false, false);

		}();

	//////////////////////// КНОПКА СБОРКИ ////////////////////////

	GUIModdingGun calculation = [sizeCellDefault, countCellOnHeight]()
		{
			RectangleShape rectangleCalculate;
			rectangleCalculate.setSize(Vector2f(sizeCellDefault * 2.f, sizeCellDefault / 2.f));
			rectangleCalculate.setPosition(Vector2f(sizeCellDefault * 13.5f, sizeCellDefault * static_cast<float>(countCellOnHeight + 3)));

			return GUIModdingGun({ "Build" }, rectangleCalculate, true, false);

		}();

	//////////////////////// ОКНО ПАРАМЕТРОВ МОДА ////////////////////////

	GUIModdingGun shaheGeneralMod = [sizeCellDefault]()
		{
			RectangleShape generalMod;
			generalMod.setPosition(Vector2f(sizeCellDefault * 6.f, sizeCellDefault * 0.5f));
			generalMod.setSize(Vector2f(sizeCellDefault * 13.8f, sizeCellDefault * 5.15f));

			return GUIModdingGun({}, generalMod, false, false);

		}();

	float ySizeAll = sizeCellDefault * 0.39f;


	vector<GeneralStat> generalModOption = [sizeCellDefault, texturePointGreenMark, texturePointRedMark, &shaheGeneralMod, ySizeAll]()
		{
			vector<GeneralStat> result;

			float ySizeGeneral = shaheGeneralMod.getRectangle().getSize().y;
			float xSizeGeneral = shaheGeneralMod.getRectangle().getSize().x;
			float xPosGeneral = shaheGeneralMod.getRectangle().getPosition().x;

			// отступ сверху && снизу
			float yOffsetOne = ySizeGeneral * 0.15f;

			// отступ между
			float yOffsetRelated = ySizeGeneral * 0.03f;

			// позиция начальной не считая YOffsetOne
			float yPosOneCharacteristic = (ySizeGeneral - yOffsetOne * 2.f) / (GunStats::countCharacteristic + 2);

			// процентарное соотношение x размера (не учитывая кнопку)            
			const array<float, 5> percentSizeCell = { 0.12f, 0.06f, 0.06f, 0.72f, 0.04f }; // sum = ~1  |||  => const 0.04

			for (size_t it = 0; it < GunStats::countCharacteristic; it++)
			{
				RectangleShape rectangleTemp;

				// имя характеристики 
				float xSizeName = xSizeGeneral * percentSizeCell[0];

				rectangleTemp.setPosition({ xPosGeneral , yOffsetOne + yPosOneCharacteristic * it + yOffsetRelated * it });
				rectangleTemp.setSize({ xSizeName, ySizeAll });

				GUIModdingGun nameCharacteristic({ string(GunStats::mapCharacteristicNameIndex[it]) }, rectangleTemp, false, false);

				/// текущее отображение характеристики \\ темп пробитие ... 
				float XsizeVisualCharacteristic = xSizeGeneral * percentSizeCell[1];

				rectangleTemp.setPosition(rectangleTemp.getPosition() + Vector2f(xSizeName, 0));
				rectangleTemp.setSize({ XsizeVisualCharacteristic, ySizeAll });

				GUIModdingGun visualCharacteristic({ L"0" }, rectangleTemp, false, false);

				/// визуальное отображения процента апгрейда
				float XsizeVisualPercent = xSizeGeneral * percentSizeCell[2];

				rectangleTemp.setPosition(rectangleTemp.getPosition() + Vector2f(XsizeVisualCharacteristic, 0));
				rectangleTemp.setSize({ XsizeVisualPercent, ySizeAll });

				GUIModdingGun visualPercentUpgrade({ L"0.0%" }, rectangleTemp, false, false);

				// Общий "прямоугольник" в котором распалагаютися все юниты
				float xSizeRectangleALLUnits = xSizeGeneral * percentSizeCell[3];

				RectangleShape rectanpleForUnits;
				rectanpleForUnits.setPosition({ rectangleTemp.getPosition() + Vector2f(XsizeVisualPercent, 0) });
				rectanpleForUnits.setSize(Vector2f(xSizeRectangleALLUnits, ySizeAll));

				// размещаем едины в RectanpleForUnits
				vector<RectangleShape> resultShapeUnits;

				// максимальное колво единиц для текущей характеристики
				int   countUnits = GunStats::CountUnitsForCharacteristic[it];

				float xPosUnits = rectanpleForUnits.getPosition().x;
				float yPosUnits = rectanpleForUnits.getPosition().y;
				float xSizeUnits = rectanpleForUnits.getSize().x / countUnits;
				float ySizeUnits = rectanpleForUnits.getSize().y;

				for (int curUnit = 0; curUnit < countUnits; curUnit++)
				{
					RectangleShape unitShahe;
					unitShahe.setSize({ xSizeUnits, ySizeUnits });
					unitShahe.setPosition({ xPosUnits + xSizeUnits * curUnit, yPosUnits });
					unitShahe.setFillColor(Color(66, 66, 66));
					unitShahe.setOutlineColor(Color(10, 10, 10));
					unitShahe.setOutlineThickness(-1);

					resultShapeUnits.push_back(unitShahe);
				}

				/// кнопка для выбора
				float resizeMark = 0.7f; // размер кнопки от максимума %

				// размер кнопки
				float sizeCellButton = rectangleTemp.getSize().y * resizeMark;

				// смещение для центрирования
				float offsetSizeCellButton = rectangleTemp.getSize().y * ((1.f - resizeMark) / 2.f);

				rectangleTemp.setPosition(Vector2f(rectanpleForUnits.getPosition().x + rectanpleForUnits.getSize().x, rectanpleForUnits.getPosition().y) + Vector2f(offsetSizeCellButton * 2, offsetSizeCellButton));
				rectangleTemp.setSize(Vector2f(sizeCellButton, sizeCellButton));

				GUIModdingGun buttonCharacteristic({}, rectangleTemp, true, false);
				buttonCharacteristic.setTextureButton(texturePointGreenMark, texturePointRedMark);

				if (it == 0)
				{
					buttonCharacteristic.setAsPressedButton();
				}
				result.emplace_back(nameCharacteristic, visualCharacteristic, visualPercentUpgrade, buttonCharacteristic, rectanpleForUnits, resultShapeUnits);
			}
			return result;
		}();

	float yDownOffset = 0.7f;

	// между
	float xOffsetRelated = sizeCellDefault * 0.25f;

	/// отображение шанса на улучшение (внизу)
	GUIModdingGun textGeneralModChance = [sizeCellDefault, &shaheGeneralMod, ySizeAll, yDownOffset]()
		{
			float xSize = sizeCellDefault * 4.f;

			RectangleShape rectangleGeneralModChance;
			rectangleGeneralModChance.setSize({ xSize , ySizeAll });

			auto tempRectangle = shaheGeneralMod.getRectangle();
			rectangleGeneralModChance.setPosition(tempRectangle.getPosition() + Vector2f(0, tempRectangle.getSize().y - (sizeCellDefault * yDownOffset)));

			return GUIModdingGun({ "Chahce_upgrade" , string(GunStats::mapCharacteristicNameGunStat[0]) ,  L" 0.0%" }, rectangleGeneralModChance, false, false);
		}();


	// кнопка применить как начальный
	GUIModdingGun shapeGeneralModApplyStatInitial = [sizeCellDefault, &textGeneralModChance, ySizeAll, yDownOffset, xOffsetRelated]()
		{
			float xSize = sizeCellDefault * 3;

			auto rec = textGeneralModChance.getRectangle();

			RectangleShape tempRec;
			tempRec.setSize(Vector2f(xSize, ySizeAll));
			tempRec.setPosition(Vector2f(rec.getPosition().x + rec.getSize().x + xOffsetRelated, rec.getPosition().y));

			return GUIModdingGun({ "Apply_as_initial_mod" }, tempRec, true, false);
		}();
>>>>>>> main

			RectangleShape TempRec;
			TempRec.setSize(Vector2f(Xsize, ySizeAll));
			TempRec.setPosition(Vector2f(Rec.getPosition().x + Rec.getSize().x + xOffsetRelated, Rec.getPosition().y));

	// кнопка применить как мод
	GUIModdingGun buttonGeneralModApplyStat = [sizeCellDefault, &shapeGeneralModApplyStatInitial, ySizeAll, xOffsetRelated]()
		{
			float xSize = sizeCellDefault * 1.8f;

<<<<<<< HEAD
			return GUITextAndRectangle({ "Step_back" }, TempRec, true, false);
		}();

	//////////////////////// ОТОБРАЖЕНИЕ ИНФОРМАЦИИ ///////////////////////////////////

	GUITextAndRectangle textInfoDebug = [defaultSizeCell, countCellOnHeight]()
		{

			RectangleShape shapeInfoDebug;
			shapeInfoDebug.setSize(Vector2f(defaultSizeCell * 8, defaultSizeCell / 2));
			shapeInfoDebug.setPosition(Vector2f(static_cast<float>(defaultSizeCell * 11.5), static_cast<float>(defaultSizeCell * (countCellOnHeight + 1))));

			return GUITextAndRectangle({}, shapeInfoDebug, false, false);
		}();

	//////////////////////// ОТОБРАЖЕНИЕ ПОВТОРНЫХ СБОРОК ////////////////////////

	GUITextAndRectangle shapeTargetIter = [defaultSizeCell, countCellOnHeight, totalIterations]()
		{
=======
			auto rec = shapeGeneralModApplyStatInitial.getRectangle();

			RectangleShape tempRec;
			tempRec.setSize(Vector2f(xSize, ySizeAll));
			tempRec.setPosition(Vector2f(rec.getPosition().x + rec.getSize().x + xOffsetRelated, rec.getPosition().y));

			return GUIModdingGun({ "Apply" }, tempRec, true, false);
		}();

	// кнопка сбросить
	GUIModdingGun shapeGeneralModReset = [sizeCellDefault, &buttonGeneralModApplyStat, ySizeAll, xOffsetRelated]()
		{
			float xSize = sizeCellDefault * 1.8f;

			auto rec = buttonGeneralModApplyStat.getRectangle();

			RectangleShape tempRec;
			tempRec.setSize(Vector2f(xSize, ySizeAll));
			tempRec.setPosition(Vector2f(rec.getPosition().x + rec.getSize().x + xOffsetRelated, rec.getPosition().y));

			return GUIModdingGun({ "Reset" }, tempRec, true, false);
		}();
>>>>>>> main

			RectangleShape ShapeTargetIter;
			ShapeTargetIter.setSize(Vector2f(defaultSizeCell * 4, defaultSizeCell / 2));
			ShapeTargetIter.setPosition(Vector2f(defaultSizeCell * 13.5f, defaultSizeCell * static_cast<float>(countCellOnHeight + 2)));

<<<<<<< HEAD
			return GUITextAndRectangle({ "Number_weapon_assemblies" , to_wstring(totalIterations) }, ShapeTargetIter, true, false);
		}();

	//////////////////////// ЗАДНИЙ ФОН ////////////////////////////////////////////////

	RectangleShape background;
	background.setSize(Vector2f(lengthWindowMain, heightWindowMain));
	background.setPosition(Vector2f(0, 0));

	Texture backgroundTexture;

	if (!backgroundTexture.loadFromFile(backgroundPatchMain))
	{
		OUTPUT_LOG("main -> background.png not loaded");
	}
	else
	{
		background.setTexture(&backgroundTexture);
	}

	//////////////////////////////////////////////////////////////////////////////////////

	RenderWindow mainWindow(VideoMode({ static_cast<unsigned int>(lengthWindowMain), static_cast<unsigned int>(heightWindowMain) }), "ModdingGun");
	RenderWindow diagramWindow;

	mainWindow.setFramerateLimit(150);
	diagramWindow.setFramerateLimit(150);

	/////////////////////// ПРОЧИЕ ПЕРЕМЕННЫЕ ////////////////////////

	//диаграмма

	// используем отрисовку кривой только один раз
	// с помощью diagramBool указывает какие окна отрисовали
	// false = отрисованно -> больше не будет повторяться
	vector<bool> diagramBool;

	// вектор окон диаграм с отрисованными графикам
	// графики отрисовываются один раз -> DiagramBool
	vector<RenderWindow> arrWindowsDiagram;

	//вектор для кривых 
	vector<AllPackGraf> arrDiagramPack;

	RectangleShape backgroundDiagram;
	backgroundDiagram.setSize(Vector2f(static_cast<float>(lengthWindowDiagram), static_cast<float>(heightWindowDiagram)));
	backgroundDiagram.setPosition(Vector2f(0.f, 0.f));

	Texture backgroundTextureDiagram;

	// задний фон
	if (!backgroundTextureDiagram.loadFromFile(backgroundPathDiagram))
	{
		OUTPUT_LOG("main -> background_graf.png not loaded");
	}
	else
	{
		backgroundDiagram.setTexture(&backgroundTextureDiagram);
	}

	int errorNum = 0;

	// дефолт статы для 
	// START_CHARACTERISTIC
	// inititialCharacteristicStat
	const array<double, GunStats::countCharacteristic> kDefaultCharacteristic = { GunStats::countCharacteristic , 0.0 };


	// указанный мод
	// использовать только в области сборки calculate / vector для передачи в drawNormalGraph
	array<double, GunStats::countCharacteristic> currentModCharacteristic = kDefaultCharacteristic;

	// начальный мод 
	// использовать только в области сборки calculate / vector для передачи в drawNormalGraph
	array<double, GunStats::countCharacteristic> inititialCharacteristicStat = kDefaultCharacteristic;

	vector<int> maxStatPosition = { 60, 48, 40, 40,40,40,40, };

	// выгрузка данных из файла оружия
	CharacteristicGun currentCharacteristicGun(maxStatPosition);

	const vector<int> kNullPosition = { 0,0,0, 0,0,0,0 };

	// текущая позиция мода
	// 0 куч
	// 1 темп
	// 2 пробитие
	// 3 отдача
	// 4 качание
	// 6 сост
	// 5 грязь
	vector<int> positionMod = kNullPosition;

	// начальная позиция мода
	// 0 куч
	// 1 темп
	// 2 пробитие
	// 3 отдача
	// 4 качание
	// 6 сост
	// 5 грязь
	vector<int> initialPosition = kNullPosition;

	vector<pair <vector<int>, vector<int>>> historyPosition;

	bool pressedApplyAsMod = false;

	for (int itWindow = 0; mainWindow.isOpen(); itWindow++)
	{
		if (itWindow > 100) { itWindow = 0; }

		positionMouseMainWindow = Mouse::getPosition(mainWindow);
		postionMouseGlobal = mainWindow.mapPixelToCoords(positionMouseMainWindow);

		while (const optional event = mainWindow.pollEvent())
		{
			if (event->is<Event::Closed>())
			{
				mainWindow.close();
				for (int i = 0; i < arrWindowsDiagram.size(); i++)
				{
					arrWindowsDiagram[i].close();
				}
			}

			// выбор
			else if (const auto& mouseButtonPressed = event->getIf<Event::MouseButtonPressed>())
			{
				if (mouseButtonPressed->button == Mouse::Button::Left)
				{
					/// нажали на оружие в списке)
					for (size_t it = 0; it < vectorGun.size(); it++)
					{
						if (vectorGun[it].m_weapoGUI.getRectangle().getGlobalBounds().contains(postionMouseGlobal))
						{
							historyPosition.clear();
							methodStepMod.clear();

							pressedApplyAsMod = false;

							inititialCharacteristicStat = kDefaultCharacteristic;
							currentModCharacteristic = kDefaultCharacteristic;

							// ставим стандартный цвет на юнитах
							for (int it1 = 0; it1 < generalModOption.size(); it1++)
							{
								for (int it2 = 0; it2 < generalModOption[it1].m_shaheUnitsInCharacteristic.size(); it2++)
								{
									generalModOption[it1].m_shaheUnitsInCharacteristic[it2].setFillColor(Color(66, 66, 66));
								}
							}

							// получаем имя оружия
							nameGun = vectorGun[it].m_name;

							if (!currentCharacteristicGun.load(fileAttributeWeapon, stringToWString(nameGun)))
							{
								nameGun = "Empty";
								textInfoDebug.setKeyStringTextWstring({ "Error_File" });
								positionMod = kNullPosition;
								break;
							}

							selectWeaponGUI.setKeyStringTextWstring({ nameGun });

							textInfoDebug.setKeyStringTextWstring({ "Loaded",  nameGun });

							initialPosition = currentCharacteristicGun.getCurrentPosition();
							moveElement(initialPosition, 4, 2);
							positionMod = initialPosition;

							editShaheUnits(generalModOption, currentCharacteristicGun, initialPosition, positionMod);

							for (int it = 0; it < generalModOption.size(); it++)
							{
								if (generalModOption[it].m_markButtonCharacteristic.isPressed())
								{
									setChahceUpgrade(currentCharacteristicGun, currentModifiers, textGeneralModChance, it);
								}

								//generalModOption[it].m_textCharacteristicVisualPercent.setFillColor(ColorText);
								generalModOption[it].m_textCharacteristicVisualPercent.setKeyStringTextWstring({ L"0.0%" });

								// размета значений характеристики
								auto val = currentCharacteristicGun.getValueCharacteristic(GunStats::transIndexToIndexGunStat[it]);
								wstring wstrVal = it ? to_wstring(static_cast<int>(val)) : format(L"{:.1f}", val);
								generalModOption[it].m_textVisualCharacteristic.setKeyStringTextWstring({ wstrVal });
							}

							/// обновляем стоимость KitPrice
							/// tool уже выведен
							/// skill comming soon
							/////////////////////////////////////////////////////////

							unsigned valuePrice = getPrice(filePriceModifiersWeapon, currentModifiers, nameGun);
							currentPrice.m_priceKit = valuePrice;
							for (auto& tp : arrKitPrice)
							{
								if (!tp.isButton())
								{
									tp.setKeyStringTextWstring({ format(L"{:L}", valuePrice), L"₽" });
									break;
								}
							}
							break;
						}
					}

					// languages

					size_t idx = 0;
					for (auto& gui : arrLangGUI)
					{
						if (gui.getRectangle().getGlobalBounds().contains(postionMouseGlobal))
						{
							for (auto& g : arrLangGUI)
							{
								g.setAsReleasedButton();
							}

							gui.setAsPressedButton();

							LANG_SETTING.setLanguage(idx);

							GUITextAndRectangle::refreshTranslateGui();
							break;
						}
						++idx;
					}

					idx = 0;
					for (auto& gui : VectorThemeGUI)
					{
						if (gui.getRectangle().getGlobalBounds().contains(postionMouseGlobal))
						{
							for (auto& g : VectorThemeGUI)
								g.setAsPressedButton();

							gui.setAsReleasedButton();

							auto theme = gui.getKeyStringTextWstring();

							if (std::holds_alternative<std::string>(theme[0]))
							{
								const auto& stringTheme = std::get<std::string>(theme[0]);
								GUITextAndRectangle::refreshThemeGui(stringTheme);
							}
							else
							{
								OUTPUT_LOG("Main -> VectorThemeGUI - no string specified");
							}

							break;
						}

						++idx;
					}

					// нажали рассчет мода
					if (calculation.getRectangle().getGlobalBounds().contains(postionMouseGlobal))
					{
						calculation.setAsReleasedButton();

						// загружен ли файл?
						if (!currentCharacteristicGun.isEmpty())
						{
							textInfoDebug.setKeyStringTextWstring({ "calculation" });

							resultMainModifiers.clear();
							resultRandomAttemptUsed.clear();

							for (int i = 0; i < GunStats::countCharacteristic; i++)
							{
								currentModCharacteristic[i] = currentCharacteristicGun.getVisualPercentUpgradeCharacteristic(GunStats::transIndexToIndexGunStat[i]);
							}

							// если не совпадает c начальным 
							if (positionMod != initialPosition)
							{
								mainWindow.draw(background);
								mainWindow.draw(calculation.getRectangle());
								mainWindow.draw(calculation.getText());
								mainWindow.draw(textInfoDebug.getRectangle());
								mainWindow.draw(textInfoDebug.getText());
								mainWindow.display();

								vector<thread> threadsV;
								atomic<int> currentIndex = 0;

								// защита от деления на 0
								if (threadCount == 0) threadCount = 1;

								int chunkSize = totalIterations / threadCount;
								int remainder = totalIterations % threadCount;

								int start = 0;

								for (int i = 0; i < threadCount; ++i)
								{
									int end = start + chunkSize + (i < remainder ? 1 : 0);  // равномерно распределяем остаток

									threadsV.emplace_back([=]()
										{
											for (int j = start; j < end; ++j)
											{
												CharacteristicGun gunCopy = currentCharacteristicGun;
												RunSingleAssembly(j, methodStepMod);
											}
										});
									start = end;
								}

								// Ждём завершения всех потоков
								for (auto& t : threadsV) { t.join(); }

								errorNum = 0;
								switch (RunSingleAssembly(-1, methodStepMod))
								{
								case 0:
									textInfoDebug.setKeyStringTextWstring({ "Success_assemble" });
									break;

								case -1:
									textInfoDebug.setKeyStringTextWstring({ "Empty_vector_method" });
									break;

								case -2:
									textInfoDebug.setKeyStringTextWstring({ "ChanceUpgrade_<=_0" });
									break;

								case -3:
									textInfoDebug.setKeyStringTextWstring({ "Key_Tool_not_found" });
									break;

								case -4:
									textInfoDebug.setKeyStringTextWstring({ "Key_Kit_not_found" });
									break;

								case -5:
									textInfoDebug.setKeyStringTextWstring({ "Key_Skill_not_found" });
									break;

								case -6:
									textInfoDebug.setKeyStringTextWstring({ "modifiers_noKit_noTool" });
									errorNum = -6;
									break;

								default:
									textInfoDebug.setKeyStringTextWstring({ "Unknown_error_RunSingleAssembly" });
									break;
								}

								diagramWindow.create(VideoMode({ lengthWindowDiagram, heightWindowDiagram }), "Diagram");

								arrDiagramPack.emplace_back();
								arrWindowsDiagram.push_back(move(diagramWindow));
								diagramBool.push_back(false);
							}
							else
							{
								textInfoDebug.setKeyStringTextWstring({ "mod_matches" });
							}
						}
						else
						{
							textInfoDebug.setKeyStringTextWstring({ "File_not_uploaded" });
							positionMod = initialPosition = kNullPosition;
						}
					}

					// окно смены колличества повторений
					else if (shapeTargetIter.getRectangle().getGlobalBounds().contains(postionMouseGlobal))
					{
						shapeTargetIter.setAsReleasedButton();
						totalIterations < 10000 ? totalIterations += 1000 : totalIterations = 1000;
						shapeTargetIter.setKeyStringTextWstring({ "Number_weapon_assemblies", to_wstring(totalIterations) });
					}

					// Окно параметров
					else if (shaheGeneralMod.getRectangle().getGlobalBounds().contains(postionMouseGlobal))
					{
						// нажали по кнопке 
						for (auto& mod : generalModOption)
						{
							if (mod.m_markButtonCharacteristic.getRectangle().getGlobalBounds().contains(postionMouseGlobal))
							{
								for (auto& m : generalModOption)
								{
									m.m_markButtonCharacteristic.setAsReleasedButton();
								}
								mod.m_markButtonCharacteristic.setAsPressedButton();
								break;
							}
						}

						// нажали применить как мод
						if (buttonGeneralModApplyStat.getRectangle().getGlobalBounds().contains(postionMouseGlobal))
						{
							buttonGeneralModApplyStat.setAsReleasedButton();

							if (!currentCharacteristicGun.isEmpty())
							{
								for (int it = 0; it < generalModOption.size(); it++)
								{
									if (generalModOption[it].m_markButtonCharacteristic.isPressed())
									{
										if (positionMod[it] < currentCharacteristicGun.getMaxPositionCharacteristic()[GunStats::transIndexToIndexGunStat[it]])
										{
											auto RealPercent = calculateSuccessChance(
												currentCharacteristicGun.getChanceForNextStat(GunStats::transIndexToIndexGunStat[it]),
												GunStats::Modifiers::getToolModifier(currentModifiers.m_tool),
												GunStats::Modifiers::getKitModifier(currentModifiers.m_kit),
												GunStats::Modifiers::getSkillModifier(currentModifiers.m_skill)
											);

											if (!pressedApplyAsMod)
											{
												for (int i = 0; i < GunStats::countCharacteristic; i++)
												{
													inititialCharacteristicStat[i] = currentCharacteristicGun.getVisualPercentUpgradeCharacteristic(GunStats::transIndexToIndexGunStat[i]);
												}
											}

											if (currentCharacteristicGun.upgradeStat(GunStats::transIndexToIndexGunStat[it]))
											{
												positionMod = currentCharacteristicGun.getCurrentPosition();
												moveElement(positionMod, 4, 2);

												// вектор уменьшения характеристик по GunStats
												auto Des = currentCharacteristicGun.getDecreaseForCurrentStat(GunStats::transIndexToIndexGunStat[it]);
												moveElement(Des, 4, 2);

												for (int i = 0; i < GunStats::countCharacteristic; i++)
												{
													initialPosition[i] -= Des[i];
												}

												historyPosition.emplace_back(positionMod, initialPosition);

												Method m;
												m.m_modifiersThisChance = currentModifiers;
												m.m_price = currentPrice;
												m.m_chanceUpgrade = RealPercent;
												m.m_visualPercent = currentCharacteristicGun.getVisualPercentUpgradeCharacteristic(GunStats::transIndexToIndexGunStat[it]);
												m.m_characteristicGunstat = GunStats::transIndexToIndexGunStat[it];

												methodStepMod.push_back(m);

												pressedApplyAsMod = true;
												shapeGeneralModApplyStatInitial.setAsReleasedButton();

												textInfoDebug.setKeyStringTextWstring({ "Applied_mod_step_recorded" });
											}
										}
										else
										{
											textInfoDebug.setKeyStringTextWstring({ "Cannot_improved_beyond_maximum" });
										}
										break;
									}
								}
							}
							else
							{
								textInfoDebug.setKeyStringTextWstring({ "Data_not_loaded" });
							}
						}

						// нажали применить как начальную
						else if (shapeGeneralModApplyStatInitial.getRectangle().getGlobalBounds().contains(postionMouseGlobal))
						{
							if (!pressedApplyAsMod)
							{
								shapeGeneralModApplyStatInitial.setAsReleasedButton();

								if (!currentCharacteristicGun.isEmpty())
								{
									for (int it = 0; it < generalModOption.size(); it++)
									{
										if (generalModOption[it].m_markButtonCharacteristic.isPressed())
										{
											if (currentCharacteristicGun.upgradeStat(GunStats::transIndexToIndexGunStat[it]))
											{

												initialPosition = currentCharacteristicGun.getCurrentPosition();
												moveElement(initialPosition, 4, 2);

												positionMod = initialPosition;

												for (int i = 0; i < GunStats::countCharacteristic; i++)
												{
													inititialCharacteristicStat[i] = currentCharacteristicGun.getVisualPercentUpgradeCharacteristic(GunStats::transIndexToIndexGunStat[i]);
												}

												historyPosition.push_back({ positionMod,  initialPosition });

												textInfoDebug.setKeyStringTextWstring({ "Marked_as_initial" });
											}
											else
											{
												textInfoDebug.setKeyStringTextWstring({ "Cannot_improved_beyond_maximum" });
											}
											break;
										}
									}
								}
								else
								{
									textInfoDebug.setKeyStringTextWstring({ "Data_not_loaded" });
								}
							}
							else
							{
								textInfoDebug.setKeyStringTextWstring({ "Prohibited_after_applying_mod" });
							}
						}

						// нажали сбросить
						else if (shapeGeneralModReset.getRectangle().getGlobalBounds().contains(postionMouseGlobal))
						{
							historyPosition.clear();
							methodStepMod.clear();

							shapeGeneralModReset.setAsReleasedButton();
							currentCharacteristicGun.returnDefaultPosition();

							initialPosition = currentCharacteristicGun.getCurrentPosition();
							moveElement(initialPosition, 4, 2);
							positionMod = initialPosition;

							pressedApplyAsMod = false;

							inititialCharacteristicStat = kDefaultCharacteristic;
							currentModCharacteristic = kDefaultCharacteristic;

							textInfoDebug.setKeyStringTextWstring({ "Mod_reset" });
						}

						//нажали вернуться на шаг  назад
						else if (shapeGeneralModBack.getRectangle().getGlobalBounds().contains(postionMouseGlobal))
						{
							shapeGeneralModBack.setAsReleasedButton();

							//возвращаем шаг назад
							if (!currentCharacteristicGun.isEmpty())
							{
								if (currentCharacteristicGun.stepBack())
								{
									textInfoDebug.setKeyStringTextWstring({ "Step_back" });

									// если была нажата принять как мод, удаляем последний элемент
									if (pressedApplyAsMod)
									{
										if (!methodStepMod.empty())
										{
											methodStepMod.pop_back();
										}
										else
										{
											textInfoDebug.setKeyStringTextWstring({ "Error - vector was not reduced, it was empty" });
										}

										if (methodStepMod.empty()) { pressedApplyAsMod = false; }
									}
									if (!historyPosition.empty())
									{
										historyPosition.pop_back();

										auto cp_temp = currentCharacteristicGun.getCurrentPosition();
										moveElement(cp_temp, 4, 2);

										if (!historyPosition.empty())
										{
											positionMod = historyPosition[historyPosition.size() - 1].first;
											initialPosition = historyPosition[historyPosition.size() - 1].second;
										}
										else
										{
											positionMod = initialPosition = cp_temp;
										}

										// проверка всегда по PositionMod
										if (positionMod != cp_temp) { textInfoDebug.setKeyStringTextWstring({ L"ERROR 'Position != Check' " }); }
									}
									else { textInfoDebug.setKeyStringTextWstring({ L"No more steps ERROR 'HistoryPosition' " }); }
								}
								else { textInfoDebug.setKeyStringTextWstring({ "No_steps" }); }
							}
							else { textInfoDebug.setKeyStringTextWstring({ "Cant_take_step_back_there_no_data" }); }
						}

						// размечаем процент
						for (int it = 0; it < generalModOption.size(); it++)
						{

							generalModOption[it].m_textCharacteristicVisualPercent.resetColorText();

							if (!currentCharacteristicGun.isEmpty())
							{
								auto VPercent = currentCharacteristicGun.getVisualPercentUpgradeCharacteristic(GunStats::transIndexToIndexGunStat[it]);

								if (VPercent > 0) { generalModOption[it].m_textCharacteristicVisualPercent.setColorText(Color::Green); }
								else if (VPercent < 0) { generalModOption[it].m_textCharacteristicVisualPercent.setColorText(Color::Red); }

								generalModOption[it].m_textCharacteristicVisualPercent.setKeyStringTextWstring({ format(L"{:.1f}", VPercent), L"%" });

								// если не нажата
								if (!pressedApplyAsMod)
								{
									inititialCharacteristicStat[it] = VPercent;
									currentModCharacteristic[it] = VPercent;
								}
								else
								{
									currentModCharacteristic[it] = VPercent;
								}
							}
							else
							{
								generalModOption[it].m_textCharacteristicVisualPercent.setKeyStringTextWstring({ L"0.0%" });
							}

							// разметка шанса улучшения процента
							if (generalModOption[it].m_markButtonCharacteristic.isPressed())
							{
								setChahceUpgrade(currentCharacteristicGun, currentModifiers, textGeneralModChance, it);
							}

							// размета значений характеристики
							auto val = currentCharacteristicGun.getValueCharacteristic(GunStats::transIndexToIndexGunStat[it]);

							wstring WSval = it ? to_wstring(static_cast<int>(val)) : format(L"{:.1f}", val);

							generalModOption[it].m_textVisualCharacteristic.setKeyStringTextWstring({ WSval });
						}

						editShaheUnits(generalModOption, currentCharacteristicGun, initialPosition, positionMod);
					}

					//МОДИЦИФКАТОРЫ
					// инструменты
					for (int i = 0; i < modifiersIcon.m_toolRectangleGUI.size(); i++)
					{
						if (modifiersIcon.m_toolRectangleGUI[i].getRectangle().getGlobalBounds().contains(postionMouseGlobal))
						{
							for (auto& sh : modifiersIcon.m_toolRectangleGUI)
							{
								sh.setAsReleasedButton();
							}
							modifiersIcon.m_toolRectangleGUI[i].setAsPressedButton();

							string STip = toolpath[i].stem().string().erase(0, 3);

							auto tool = GunStats::Modifiers::toolTypeBegin;

							for (tool++; tool < GunStats::Modifiers::ToolTypeEnd; tool++)
							{
								if (GunStats::Modifiers::getToolName(tool) == STip)
								{
									break;
								}
							}
							currentModifiers.m_tool = tool;
							textInfoDebug.setKeyStringTextWstring({ "Select",  GunStats::Modifiers::getToolName(tool) });

							for (int it = 0; it < generalModOption.size(); it++)
							{
								if (generalModOption[it].m_markButtonCharacteristic.isPressed())
								{
									setChahceUpgrade(currentCharacteristicGun, currentModifiers, textGeneralModChance, it);
									break;
								}
							}

							/// стоимость ToolPrice
							unsigned value_price = getPrice(filePriceModifiersWeapon, currentModifiers);
							currentPrice.m_priceTool = value_price;

							for (auto& tp : arrToolPrice)
							{
								if (!tp.isButton())
								{
									tp.setKeyStringTextWstring({ format(L"{:L}", value_price), L"₽" });
									break;
								}
							}

							break;
						}
					}

					// наборы
					for (int i = 0; i < modifiersIcon.m_kitRectangleGUI.size(); i++)
					{
						if (modifiersIcon.m_kitRectangleGUI[i].getRectangle().getGlobalBounds().contains(postionMouseGlobal))
						{
							for (auto& sh : modifiersIcon.m_kitRectangleGUI)
							{
								sh.setAsReleasedButton();
							}
							modifiersIcon.m_kitRectangleGUI[i].setAsPressedButton();

							string STip = kitpath[i].stem().string().erase(0, 3);

							auto KitSelect = GunStats::Modifiers::kitTypeBegin;
							for (KitSelect++; KitSelect < GunStats::Modifiers::kitTypeEnd; KitSelect++)
							{
								if (GunStats::Modifiers::GetKitName(KitSelect) == STip)
								{
									break;
								}
							}

							currentModifiers.m_kit = KitSelect;
							textInfoDebug.setKeyStringTextWstring({ "Select", GunStats::Modifiers::GetKitName(KitSelect) });

							for (int it = 0; it < generalModOption.size(); it++)
							{
								if (generalModOption[it].m_markButtonCharacteristic.isPressed())
								{
									setChahceUpgrade(currentCharacteristicGun, currentModifiers, textGeneralModChance, it);
									break;
								}
							}

							/// стоимость KitPrice
							unsigned value_price = getPrice(filePriceModifiersWeapon, currentModifiers, nameGun);
							currentPrice.m_priceKit = value_price;

							for (auto& kp : arrKitPrice)
							{
								if (!kp.isButton())
								{
									kp.setKeyStringTextWstring({ format(L"{:L}", value_price), L"₽" });
									break;
								}
							}
							break;
						}
					}
					// скилы
					for (int i = 0; i < modifiersIcon.m_skillRectangleGUI.size(); i++)
					{
						if (modifiersIcon.m_skillRectangleGUI[i].getRectangle().getGlobalBounds().contains(postionMouseGlobal))
						{
							for (auto& sh : modifiersIcon.m_skillRectangleGUI)
							{
								sh.setAsReleasedButton();
							}
							modifiersIcon.m_skillRectangleGUI[i].setAsPressedButton();

							string STip = skillpath[i].stem().string().erase(0, 3);
							auto SkillSelect = GunStats::Modifiers::skillTypeBegin;
							for (SkillSelect++; SkillSelect < GunStats::Modifiers::skillTypeEnd; SkillSelect++)
							{
								if (GunStats::Modifiers::getSkillName(SkillSelect) == STip)
								{
									break;
								}
							}
							currentModifiers.m_skill = SkillSelect;
							textInfoDebug.setKeyStringTextWstring({ "Select", GunStats::Modifiers::getSkillName(SkillSelect) });

							for (int it = 0; it < generalModOption.size(); it++)
							{
								if (generalModOption[it].m_markButtonCharacteristic.isPressed())
								{
									setChahceUpgrade(currentCharacteristicGun, currentModifiers, textGeneralModChance, it);
									break;
								}
							}

							currentPrice.m_priceSkill = 0;
							break;
						}
					}

					/// стоимость      
					for (auto& CurrentToolPrice : arrToolPrice)
					{
						if (CurrentToolPrice.getRectangle().getGlobalBounds().contains(postionMouseGlobal) && CurrentToolPrice.isButton())
						{
							auto vvstr = CurrentToolPrice.getKeyStringTextWstring().front();
							if (holds_alternative<wstring>(vvstr))
							{
								if (get<wstring>(vvstr) == L"--")
								{
									CurrentToolPrice.setAsReleasedButton();
									setPriceTool(filePriceModifiersWeapon, currentModifiers.m_tool, currentPrice.m_priceTool - 100'000);
								}
								else if (get<wstring>(vvstr) == L"-")
								{
									CurrentToolPrice.setAsReleasedButton();

									setPriceTool(filePriceModifiersWeapon, currentModifiers.m_tool, currentPrice.m_priceTool - 5'000);
								}
								else if (get<wstring>(vvstr) == L"+")
								{
									CurrentToolPrice.setAsReleasedButton();
									setPriceTool(filePriceModifiersWeapon, currentModifiers.m_tool, currentPrice.m_priceTool + 5'000);
								}
								else if (get<wstring>(vvstr) == L"++")
								{
									CurrentToolPrice.setAsReleasedButton();
									setPriceTool(filePriceModifiersWeapon, currentModifiers.m_tool, currentPrice.m_priceTool + 100'000);
								}

								/// стоимость ToolPrice
								unsigned value_price = getPrice(filePriceModifiersWeapon, currentModifiers);
								currentPrice.m_priceTool = value_price;

								for (auto& tp : arrToolPrice)
								{
									if (!tp.isButton())
									{
										tp.setKeyStringTextWstring({ format(L"{:L}", value_price), L"₽" });
										break;
									}
								}
							}
						}
					}

					for (auto& GUIKitPrice : arrKitPrice)
					{
						if (GUIKitPrice.getRectangle().getGlobalBounds().contains(postionMouseGlobal) && GUIKitPrice.isButton())
						{
							auto vvstr = *GUIKitPrice.getKeyStringTextWstring().begin();
							if (holds_alternative<wstring>(vvstr))
							{
								if (get<wstring>(vvstr) == L"--")
								{
									GUIKitPrice.setAsReleasedButton();

									setPriceKit(filePriceModifiersWeapon, currentModifiers.m_kit, nameGun, currentPrice.m_priceKit - 100'000);
								}
								else if (get<wstring>(vvstr) == L"-")
								{
									GUIKitPrice.setAsReleasedButton();
									setPriceKit(filePriceModifiersWeapon, currentModifiers.m_kit, nameGun, currentPrice.m_priceKit - 5'000);
								}
								else if (get<wstring>(vvstr) == L"+")
								{
									GUIKitPrice.setAsReleasedButton();
									setPriceKit(filePriceModifiersWeapon, currentModifiers.m_kit, nameGun, currentPrice.m_priceKit + 5'000);
								}
								else if (get<wstring>(vvstr) == L"++")
								{
									GUIKitPrice.setAsReleasedButton();
									setPriceKit(filePriceModifiersWeapon, currentModifiers.m_kit, nameGun, currentPrice.m_priceKit + 100'000);
								}

								/// стоимость KitPrice
								unsigned value_price = getPrice(filePriceModifiersWeapon, currentModifiers, nameGun);
								currentPrice.m_priceKit = value_price;
								for (auto& kp : arrKitPrice)
								{
									if (!kp.isButton())
									{
										kp.setKeyStringTextWstring({ format(L"{:L}", value_price), L"₽" });
										break;
									}
								}
							}
						}
					}
				}
			}

			// прокрутка
			else if (const auto& mouseScroll = event->getIf<Event::MouseWheelScrolled>())
			{
				if (allGunShapeScrollGUI.getRectangle().getGlobalBounds().contains(postionMouseGlobal))
				{
					if (mouseScroll->delta < 0 && itVectorGun < vectorGun.size() - countGunInList)
					{
						itVectorGun++;

						wstring stlStr = L"";
						if (itVectorGun != 0)
						{
							stlStr = to_wstring(itVectorGun) + L"+";
						}
						topScrollToolTip.setKeyStringTextWstring({ stlStr });


						stlStr = L"";
						if (vectorGun.size() - (itVectorGun + countGunInList) > 0)
						{
							stlStr = to_wstring(vectorGun.size() - (itVectorGun + countGunInList)) + L"+";
						}
						downScrollToolTip.setKeyStringTextWstring({ stlStr });

					}
					else if (mouseScroll->delta > 0 && itVectorGun > 0)
					{
						itVectorGun--;

						wstring stlStr = L"";
						if (itVectorGun != 0)
						{
							stlStr = to_wstring(itVectorGun) + L"+";
						}
						topScrollToolTip.setKeyStringTextWstring({ stlStr });

						stlStr = L"";
						if (vectorGun.size() - (itVectorGun + countGunInList) > 0)
						{
							stlStr = to_wstring(vectorGun.size() - (itVectorGun + countGunInList)) + L"+";
						}
						downScrollToolTip.setKeyStringTextWstring({ stlStr });

					}
				}
			}

		}
		mainWindow.clear(Color::White);
		mainWindow.draw(background);

		for (auto& GUI : arrLangGUI)
		{
			mainWindow.draw(GUI.getRectangle());
			mainWindow.draw(GUI.getText());
		}

		for (auto& GUI : VectorThemeGUI)
		{
			mainWindow.draw(GUI.getRectangle());
			mainWindow.draw(GUI.getText());
		}

		// модификаторы
		for (auto& GUI : modifiersIcon.m_toolRectangleGUI)
		{
			mainWindow.draw(GUI.getRectangle());
			//mainWindow.draw(GUI.getText());
		}
		for (auto& GUI : modifiersIcon.m_kitRectangleGUI)
		{
			mainWindow.draw(GUI.getRectangle());
			//mainWindow.draw(GUI.getText());
		}
		for (auto& GUI : modifiersIcon.m_skillRectangleGUI)
		{
			mainWindow.draw(GUI.getRectangle());
			//mainWindow.draw(GUI.getText());
		}

		/// стоимость
		for (auto& GUI : arrToolPrice)
		{
			mainWindow.draw(GUI.getRectangle());
			mainWindow.draw(GUI.getText());
		}
		for (auto& GUI : arrKitPrice)
		{
			mainWindow.draw(GUI.getRectangle());
			mainWindow.draw(GUI.getText());
		}
		mainWindow.draw(skillPrice.getRectangle());
		mainWindow.draw(skillPrice.getText());

		mainWindow.draw(allGunShapeScrollGUI.getRectangle());
		//mainWindow.draw(allGunShapeScrollGUI.getText());

		mainWindow.draw(selectWeaponGUI.getRectangle());
		mainWindow.draw(selectWeaponGUI.getText());

		for (int it1 = 0, it_VectorGun2 = itVectorGun; it1 < countGunInList; it1++, it_VectorGun2++)
		{
			vectorGun[it_VectorGun2].m_weapoGUI.setPositionRectangle(Vector2f(0, defaultSizeCell / 2 * it1));

			mainWindow.draw(vectorGun[it_VectorGun2].m_weapoGUI.getRectangle());
			mainWindow.draw(vectorGun[it_VectorGun2].m_weapoGUI.getText());
		}

		mainWindow.draw(topScrollToolTip.getRectangle());
		mainWindow.draw(topScrollToolTip.getText());

		mainWindow.draw(downScrollToolTip.getRectangle());
		mainWindow.draw(downScrollToolTip.getText());

		mainWindow.draw(calculation.getRectangle());
		mainWindow.draw(calculation.getText());

		mainWindow.draw(shaheGeneralMod.getRectangle());
		mainWindow.draw(shaheGeneralMod.getText());

		for (int it1 = 0; it1 < generalModOption.size(); it1++)
		{
			mainWindow.draw(generalModOption[it1].m_textCharacteristicName.getRectangle());
			mainWindow.draw(generalModOption[it1].m_textCharacteristicName.getText());

			mainWindow.draw(generalModOption[it1].m_textVisualCharacteristic.getRectangle());
			mainWindow.draw(generalModOption[it1].m_textVisualCharacteristic.getText());

			mainWindow.draw(generalModOption[it1].m_textCharacteristicVisualPercent.getRectangle());
			mainWindow.draw(generalModOption[it1].m_textCharacteristicVisualPercent.getText());

			mainWindow.draw(generalModOption[it1].m_shapeCharacteristic);

			mainWindow.draw(generalModOption[it1].m_markButtonCharacteristic.getRectangle());
			//mainWindow.draw(generalModOption[it1].m_markButtonCharacteristic.getText()); // текста нет

			bool selectModOptions = generalModOption[it1].m_markButtonCharacteristic.isPressed();

			// метка юнита
			size_t ps = positionMod[it1];

			if (ps >= generalModOption[it1].m_shaheUnitsInCharacteristic.size())
			{
				ps = generalModOption[it1].m_shaheUnitsInCharacteristic.size() - 1;
			}
			else if (selectModOptions && ps >= currentCharacteristicGun.getMaxPositionCharacteristic()[GunStats::transIndexToIndexGunStat[it1]])
			{
				generalModOption[it1].m_shaheUnitsInCharacteristic[ps].setOutlineColor(Color(128, 128, 128));
			}
			else if (selectModOptions)
			{
				generalModOption[it1].m_shaheUnitsInCharacteristic[ps].setOutlineColor(Color::Green);
			}
			for (int it2 = 0; it2 < generalModOption[it1].m_shaheUnitsInCharacteristic.size(); it2++)
			{
				mainWindow.draw(generalModOption[it1].m_shaheUnitsInCharacteristic[it2]);
			};
			if (selectModOptions)
			{
				generalModOption[it1].m_shaheUnitsInCharacteristic[ps].setOutlineColor(Color::Black);
			};
		}

		//mainWindow.draw(textGeneralModChance.getRectangle());
		mainWindow.draw(textGeneralModChance.getText());

		mainWindow.draw(shapeGeneralModApplyStatInitial.getRectangle());
		mainWindow.draw(shapeGeneralModApplyStatInitial.getText());

		mainWindow.draw(buttonGeneralModApplyStat.getRectangle());
		mainWindow.draw(buttonGeneralModApplyStat.getText());

		mainWindow.draw(shapeGeneralModReset.getRectangle());
		mainWindow.draw(shapeGeneralModReset.getText());

		mainWindow.draw(shapeGeneralModBack.getRectangle());
		mainWindow.draw(shapeGeneralModBack.getText());

		mainWindow.draw(textInfoDebug.getRectangle());
		mainWindow.draw(textInfoDebug.getText());

		mainWindow.draw(shapeTargetIter.getRectangle());
		mainWindow.draw(shapeTargetIter.getText());

		// инструменты
		for (int i = 0; i < modifiersIcon.m_toolRectangleGUI.size(); i++)
		{
			if (modifiersIcon.m_toolRectangleGUI[i].getRectangle().getGlobalBounds().contains(postionMouseGlobal))
			{
				string sTip = toolpath[i].stem().string().erase(0, 3);
				wstring tip = LANG_TRANSLATE(sTip);

				RectangleShape shapeToolTip;
				shapeToolTip.setOutlineColor(Color::Black);
				shapeToolTip.setOutlineThickness(-2);
				shapeToolTip.setSize(Vector2f(static_cast<float>(15.f + 7.f * tip.size()), defaultSizeCell / 3.f));
				shapeToolTip.setPosition(Vector2f(postionMouseGlobal.x, postionMouseGlobal.y - defaultSizeCell / 3.f));

				Text textToolTip(ThemeGui::currentFontGUI);
				textToolTip.setString(tip);
				textToolTip.setCharacterSize(static_cast<unsigned>(ThemeGui::characterSizeGUI * 0.8f));
				textToolTip.setFillColor(Color::Black);

				auto rectPos = shapeToolTip.getPosition();
				auto rectSize = shapeToolTip.getSize();
				auto textBounds = textToolTip.getLocalBounds();

				textToolTip.setOrigin(Vector2f(0.f, textBounds.position.y + textBounds.size.y / 2.0f));
				textToolTip.setPosition(Vector2f(rectPos.x + 10, rectPos.y + rectSize.y / 2.0f));

				mainWindow.draw(shapeToolTip);
				mainWindow.draw(textToolTip);
			}
		}
		// наборы
		for (int i = 0; i < modifiersIcon.m_kitRectangleGUI.size(); i++)
		{
			if (modifiersIcon.m_kitRectangleGUI[i].getRectangle().getGlobalBounds().contains(postionMouseGlobal))
			{
				string sTip = kitpath[i].stem().string().erase(0, 3);
				wstring tip = LANG_TRANSLATE(sTip);

				RectangleShape shapeKitTooltip;
				shapeKitTooltip.setOutlineColor(Color::Black);
				shapeKitTooltip.setOutlineThickness(-2);
				shapeKitTooltip.setSize(Vector2f(static_cast<float>(15.f + 7.f * tip.size()), defaultSizeCell / 3.f));
				shapeKitTooltip.setPosition(Vector2f(postionMouseGlobal.x, postionMouseGlobal.y - defaultSizeCell / 3.f));

				Text textKitTooltip(ThemeGui::currentFontGUI);
				textKitTooltip.setString(tip);
				textKitTooltip.setCharacterSize(static_cast<unsigned>(ThemeGui::characterSizeGUI * 0.8f));
				textKitTooltip.setFillColor(Color::Black);

				auto textBounds = textKitTooltip.getLocalBounds();
				auto rectPos = shapeKitTooltip.getPosition();
				auto rectSize = shapeKitTooltip.getSize();

				textKitTooltip.setOrigin(Vector2f(0.f, textBounds.position.y + textBounds.size.y / 2.0f));
				textKitTooltip.setPosition(Vector2f(rectPos.x + 10, rectPos.y + rectSize.y / 2.0f));

				mainWindow.draw(shapeKitTooltip);
				mainWindow.draw(textKitTooltip);
			}
		}
		// скиллы
		for (int i = 0; i < modifiersIcon.m_skillRectangleGUI.size(); i++)
		{
			if (modifiersIcon.m_skillRectangleGUI[i].getRectangle().getGlobalBounds().contains(postionMouseGlobal))
			{
				string STip = skillpath[i].stem().string().erase(0, 3);
				wstring Tip = LANG_TRANSLATE(STip);

				RectangleShape ShapeSkillTooltip;
				ShapeSkillTooltip.setOutlineColor(Color::Black);
				ShapeSkillTooltip.setOutlineThickness(-2);
				ShapeSkillTooltip.setPosition(Vector2f(postionMouseGlobal.x, postionMouseGlobal.y - defaultSizeCell / 3.f));
				ShapeSkillTooltip.setSize(Vector2f(static_cast<float>(15.f + 7.f * Tip.size()), defaultSizeCell / 3.f));

				Text TextSkillTooltip(ThemeGui::currentFontGUI);
				TextSkillTooltip.setString(Tip);
				TextSkillTooltip.setCharacterSize(static_cast<unsigned>(ThemeGui::characterSizeGUI * 0.8f));
				TextSkillTooltip.setFillColor(Color::Black);

				auto rectPos = ShapeSkillTooltip.getPosition();
				auto rectSize = ShapeSkillTooltip.getSize();
				auto textBounds = TextSkillTooltip.getLocalBounds();

				TextSkillTooltip.setOrigin(Vector2f(0.f, textBounds.position.y + textBounds.size.y / 2.0f));
				TextSkillTooltip.setPosition(Vector2f(rectPos.x + 10, rectPos.y + rectSize.y / 2.0f));

				mainWindow.draw(ShapeSkillTooltip);
				mainWindow.draw(TextSkillTooltip);
			}
		}

		mainWindow.display();

		// отрисовка диаграмм
		for (int iterGraf = static_cast<int>(arrWindowsDiagram.size()) - 1; iterGraf >= 0; --iterGraf)
		{
			if (arrWindowsDiagram[iterGraf].isOpen())
			{
				auto positionMouseForGraf = Mouse::getPosition(arrWindowsDiagram[iterGraf]);
				auto mouseWorldPosForGraf = mainWindow.mapPixelToCoords(positionMouseForGraf);
				unsigned int MethodList = 22;

				while (const optional event = arrWindowsDiagram[iterGraf].pollEvent())
				{
					if (event->is<Event::Closed>())
					{
						arrWindowsDiagram[iterGraf].close();
						diagramBool.erase(diagramBool.begin() + iterGraf);
						arrWindowsDiagram.erase(arrWindowsDiagram.begin() + iterGraf);
						arrDiagramPack.erase(arrDiagramPack.begin() + iterGraf);

						break; // чтобы не обращаться к уже удалённому i
					}
					else if (const auto& mouseButtonPressed = event->getIf<Event::MouseButtonPressed>())
					{
						if (mouseButtonPressed->button == Mouse::Button::Left)
						{
							// если прокрутка доступна
							if (arrDiagramPack[iterGraf].m_GUIButtonTop.get() != nullptr && arrDiagramPack[iterGraf].m_GUIButtonDown.get() != nullptr)
							{
								auto size = arrDiagramPack[iterGraf].m_GUIMethodModifiers.size();

								// кнопка прокрутки верхняя
								if (arrDiagramPack[iterGraf].m_GUIButtonTop.get()->getRectangle().getGlobalBounds().contains(mouseWorldPosForGraf))
								{
									// но не более размера 
									if (arrDiagramPack[iterGraf].m_initialIndexMethod > 0)
									{
										arrDiagramPack[iterGraf].m_initialIndexMethod--;
									}
								}

								//кнопка прокрутки вниз
								else if (arrDiagramPack[iterGraf].m_GUIButtonDown.get()->getRectangle().getGlobalBounds().contains(mouseWorldPosForGraf))
								{
									// но не более размера 
									if (arrDiagramPack[iterGraf].m_initialIndexMethod < size)
									{
										arrDiagramPack[iterGraf].m_initialIndexMethod++;
									}
								}

								// корректировка 
								auto sizeRec = arrDiagramPack[iterGraf].m_GUIMethodModifiers.begin()->getRectangle().getSize();
								auto xSizeWindow = arrWindowsDiagram[iterGraf].getSize().x;

								for (size_t i = arrDiagramPack[iterGraf].m_initialIndexMethod; i < MethodList || i < size; i++)
								{
									arrDiagramPack[iterGraf].m_GUIMethodModifiers[i].setPositionRectangle(Vector2f(xSizeWindow - sizeRec.x, sizeRec.y * (i - arrDiagramPack[iterGraf].m_initialIndexMethod)));
								}
							}

							bool needRefresh = false;
							float sizeValBN = 0.010f;
							float sizeValSM = 0.005f;
							for (auto& GUIBinBt : arrDiagramPack[iterGraf].m_GUIPercentBinBt)
							{
								auto vbt = GUIBinBt.getKeyStringTextWstring().front();
								if (GUIBinBt.getRectangle().getGlobalBounds().contains(mouseWorldPosForGraf) && GUIBinBt.isButton() && holds_alternative<wstring>(vbt) && get<wstring>(vbt) == L"-")
								{
									GUIBinBt.setAsReleasedButton();
									if (arrDiagramPack[iterGraf].m_percentBin - sizeValBN >= 0.f)
									{
										arrDiagramPack[iterGraf].m_percentBin -= sizeValBN;
										needRefresh = true;
									}
									break;
								}
								else if (GUIBinBt.getRectangle().getGlobalBounds().contains(mouseWorldPosForGraf) && GUIBinBt.isButton() && holds_alternative<wstring>(vbt) && get<wstring>(vbt) == L"+")
								{
									GUIBinBt.setAsReleasedButton();
									if (arrDiagramPack[iterGraf].m_percentBin + sizeValBN <= 1.f)
									{
										arrDiagramPack[iterGraf].m_percentBin += sizeValBN;
										needRefresh = true;
									}
									break;
								}

							}
							for (auto& GUISmoothBt : arrDiagramPack[iterGraf].m_GUIPercentSmoothBt)
							{
								auto vbt = GUISmoothBt.getKeyStringTextWstring().front();
								if (GUISmoothBt.getRectangle().getGlobalBounds().contains(mouseWorldPosForGraf) && GUISmoothBt.isButton() && holds_alternative<wstring>(vbt) && get<wstring>(vbt) == L"-")
								{
									GUISmoothBt.setAsReleasedButton();

									if (arrDiagramPack[iterGraf].m_percentSmooth - sizeValSM >= 0.f)
									{
										arrDiagramPack[iterGraf].m_percentSmooth -= sizeValSM;
										needRefresh = true;
									}
									break;
								}
								else if (GUISmoothBt.getRectangle().getGlobalBounds().contains(mouseWorldPosForGraf) && GUISmoothBt.isButton() && holds_alternative<wstring>(vbt) && get<wstring>(vbt) == L"+")
								{
									GUISmoothBt.setAsReleasedButton();

									if (arrDiagramPack[iterGraf].m_percentSmooth + sizeValSM <= 1.f)
									{
										arrDiagramPack[iterGraf].m_percentSmooth += sizeValSM;
										needRefresh = true;
									}
									break;
								}
							}
							if (needRefresh)
							{
								for (auto& GUISmoothBt : arrDiagramPack[iterGraf].m_GUIPercentSmoothBt)
								{
									auto vbt = GUISmoothBt.getKeyStringTextWstring().front();
									if (holds_alternative<wstring>(vbt) && !GUISmoothBt.isButton())
									{
										GUISmoothBt.setKeyStringTextWstring({ format(L"{:.3f}", arrDiagramPack[iterGraf].m_percentSmooth) });
										break;

									}
								}
								for (auto& GUIBinBt : arrDiagramPack[iterGraf].m_GUIPercentBinBt)
								{
									auto vbt = GUIBinBt.getKeyStringTextWstring().front();
									if (holds_alternative<wstring>(vbt) && !GUIBinBt.isButton())
									{
										GUIBinBt.setKeyStringTextWstring({ format(L"{:.3f}", arrDiagramPack[iterGraf].m_percentBin) });
										break;
									}
								}

								auto BinnedFreqMap = getBinedFreqModifiers(arrDiagramPack[iterGraf].m_currentSourceFreqModifiers, arrDiagramPack[iterGraf].m_percentBin); // сколько % оставим? (%)
								auto SmoothedFreqMap = getSmoothFreqModifiers(BinnedFreqMap, arrDiagramPack[iterGraf].m_percentSmooth); // насколько сильно (%)

								arrDiagramPack[iterGraf].m_currentSmoothFreqModifiers = SmoothedFreqMap;

								arrDiagramPack[iterGraf].m_currentCurvePack.m_curve = curveFun(arrDiagramPack[iterGraf].m_currentCurvePack.m_mainRectangleCurve, arrDiagramPack[iterGraf].m_colorCurve, arrDiagramPack[iterGraf].m_currentSmoothFreqModifiers);
							}
						}
					}
				}

				if (iterGraf < arrWindowsDiagram.size())
				{

					if (!diagramBool[iterGraf])
					{
						////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
						if (resultMainModifiers.empty())
						{
							OUTPUT_LOG("main -> resultMainModifiers empty");
							continue;
						}

						if (resultRandomAttemptUsed.empty())
						{
							OUTPUT_LOG("main -> resultRandomAttemptUsed empty");
							continue;
						}

						// стандартный вид
						arrDiagramPack[iterGraf].m_percentBin = 1.0f;
						arrDiagramPack[iterGraf].m_percentSmooth = 0.05f;

						// на сколько делим цену ? 
						float priceValueDegree = 1'000'000; // до KK
						arrDiagramPack[iterGraf].m_colorCurve = Color::Red;

						// Получаем позицию главного окна
						Vector2i mainPos = arrWindowsDiagram[iterGraf].getPosition();
						Vector2u mainSize = arrWindowsDiagram[iterGraf].getSize();

						// Центрируем текущий diagramWindow поверх главного окна
						int xPosDiagram = mainPos.x + (static_cast<int>(mainSize.x) - static_cast<int>(lengthWindowDiagram)) / 2;
						int yPosDiagram = mainPos.y + (static_cast<int>(mainSize.y) - static_cast<int>(heightWindowDiagram)) / 2;

						arrWindowsDiagram[iterGraf].setPosition(Vector2i(xPosDiagram, yPosDiagram));

						// оформление кривой
						float xPosPack = arrWindowsDiagram[iterGraf].getSize().x * 0.1f;
						float yPosPack = arrWindowsDiagram[iterGraf].getSize().y * 0.5f;

						float xSizePack = arrWindowsDiagram[iterGraf].getSize().x * 0.4f;
						float ySizePack = arrWindowsDiagram[iterGraf].getSize().y * 0.4f;

						RectangleShape RectangleForCurve;
						RectangleForCurve.setSize({ xSizePack, ySizePack });
						RectangleForCurve.setPosition({ xPosPack, yPosPack });
						RectangleForCurve.setFillColor(Color(255, 255, 255, 220));
						RectangleForCurve.setOutlineColor(Color::Black);
						RectangleForCurve.setOutlineThickness(-1);

						//"сырые" частоты
						auto SourceFreqMap = getFreqModifiers(resultMainModifiers);
						auto BinnedFreqMap = getBinedFreqModifiers(SourceFreqMap, arrDiagramPack[iterGraf].m_percentBin); // сколько % оставим? (%)
						auto SmoothedFreqMap = getSmoothFreqModifiers(BinnedFreqMap, arrDiagramPack[iterGraf].m_percentSmooth); // насколько сильно (%)

						arrDiagramPack[iterGraf].m_currentSmoothFreqModifiers = SmoothedFreqMap;
						arrDiagramPack[iterGraf].m_currentSourceFreqModifiers = SourceFreqMap;

						string nameCurve;
						switch (errorNum)
						{
						case 0:
							nameCurve = "modifiers";
							break;

						case -6:
							nameCurve = "modifiers_noKit_noTool";
							break;

						default:
							nameCurve = "Error";
							break;
						}

						arrDiagramPack[iterGraf].m_currentCurvePack = getCurveFrequrency(RectangleForCurve, arrDiagramPack[iterGraf].m_currentSmoothFreqModifiers, arrDiagramPack[iterGraf].m_colorCurve, nameCurve);

						// оформление 50-ого перцентиля

						xPosPack = arrWindowsDiagram[iterGraf].getSize().x * 0.01f;
						yPosPack = arrWindowsDiagram[iterGraf].getSize().y * 0.01f;

						xSizePack = defaultSizeCell * 3.5f;
						ySizePack = defaultSizeCell * 0.3f;

						auto infoPercentile50 = GetInfoForModifiers(getPercentile(arrDiagramPack[iterGraf].m_currentSourceFreqModifiers, 50.0));
						{
							vector<variant<string, wstring>> vResultWS = { "Percentile_50" };

							GUITextAndRectangle percentileGuiName = [xPosPack, yPosPack, xSizePack, ySizePack, vResultWS]()
								{
									RectangleShape rectangleTemp;
									rectangleTemp.setSize({ xSizePack * 2, ySizePack });
									rectangleTemp.setPosition({ xPosPack, yPosPack });
									return GUITextAndRectangle(vResultWS, rectangleTemp, false, false);

								}();
							arrDiagramPack[iterGraf].m_GUIPercentile50.push_back(percentileGuiName);

							for (const auto& infoItem : infoPercentile50)
							{
								xPosPack = arrDiagramPack[iterGraf].m_GUIPercentile50.begin()->getRectangle().getPosition().x;
								yPosPack = arrDiagramPack[iterGraf].m_GUIPercentile50.rbegin()->getRectangle().getPosition().y + ySizePack;

								vResultWS = { infoItem.first };

								GUITextAndRectangle percentileGuiInfo = [xPosPack, yPosPack, xSizePack, ySizePack, vResultWS]()
									{
										RectangleShape rectangleTemp;
										rectangleTemp.setSize({ xSizePack, ySizePack });
										rectangleTemp.setPosition({ xPosPack, yPosPack });
										return GUITextAndRectangle(vResultWS, rectangleTemp, false, false);

									}();

								arrDiagramPack[iterGraf].m_GUIPercentile50.push_back(percentileGuiInfo);

								percentileGuiInfo.setPositionRectangle(percentileGuiInfo.getPositionRectangle() + Vector2f(percentileGuiInfo.getRectangle().getSize().x, 0));

								vector<variant<string, wstring>> nameVal = { format(L"{:L}", infoItem.second) };

								if (infoItem.first == "AllPrice")
								{
									nameVal.push_back(L"->");
									nameVal.push_back(format(L"{:L}", static_cast<float>(infoItem.second / priceValueDegree)));
									nameVal.push_back("kk");
								}
								else
								{
									nameVal.push_back("units");
								}

								percentileGuiInfo.setKeyStringTextWstring(nameVal);
								arrDiagramPack[iterGraf].m_GUIPercentile50.push_back(percentileGuiInfo);
							}
						}

						// оформление начального и конечного мода
						//////////////////////////////////////////////////////////////////////////////////////////////////////////

						auto size = arrDiagramPack[iterGraf].m_GUIPercentile50.rbegin()->getRectangle().getSize();
						xSizePack = size.x * 2.3f;
						ySizePack = size.y;

						xPosPack = arrDiagramPack[iterGraf].m_GUIPercentile50.begin()->getPositionRectangle().x;
						yPosPack = arrDiagramPack[iterGraf].m_GUIPercentile50.rbegin()->getPositionRectangle().y + size.y;

						GUITextAndRectangle GUInitialMod = [xSizePack, ySizePack, xPosPack, yPosPack, inititialCharacteristicStat]()
							{
								RectangleShape rectangleTemp;
								rectangleTemp.setSize({ xSizePack, ySizePack });
								rectangleTemp.setPosition({ xPosPack, yPosPack });
								wstring result;
								for (const auto& ch : inititialCharacteristicStat)
								{
									result += format(L"{:.1f}", ch) + L" |\t";
								}
								return GUITextAndRectangle({ "Initial_mod", L"\t\t" ,result }, rectangleTemp, false, false);
							}();

						arrDiagramPack[iterGraf].m_GUIInitialMod = make_shared<GUITextAndRectangle>(GUInitialMod);

						yPosPack += ySizePack;

						GUITextAndRectangle endMod = [xSizePack, ySizePack, xPosPack, yPosPack, currentModCharacteristic]()
							{
								RectangleShape RectangleTemp;
								RectangleTemp.setSize({ xSizePack, ySizePack });
								RectangleTemp.setPosition({ xPosPack, yPosPack });
								wstring result;

								for (const auto& ch : currentModCharacteristic)
								{
									result += format(L"{:.1f}", ch) + L" |\t";
								}

								return GUITextAndRectangle({ "End_mod", L"\t\t", result }, RectangleTemp, false, false);
=======
	// кнопка назад
	GUIModdingGun shapeGeneralModBack = [sizeCellDefault, &shapeGeneralModReset, ySizeAll, xOffsetRelated]()
		{
			float xSize = sizeCellDefault * 1.6f;

			auto rec = shapeGeneralModReset.getRectangle();

			RectangleShape tempRec;
			tempRec.setSize(Vector2f(xSize, ySizeAll));
			tempRec.setPosition(Vector2f(rec.getPosition().x + rec.getSize().x + xOffsetRelated, rec.getPosition().y));


			return GUIModdingGun({ "Step_back" }, tempRec, true, false);
		}();


	//////////////////////// ОТОБРАЖЕНИЕ ИНФОРМАЦИИ ///////////////////////////////////

	GUIModdingGun textInfoDebug = [sizeCellDefault, countCellOnHeight]()
		{

			RectangleShape ShapeInfoDebug;
			ShapeInfoDebug.setSize(Vector2f(sizeCellDefault * 8, sizeCellDefault / 2));
			ShapeInfoDebug.setPosition(Vector2f(static_cast<float>(sizeCellDefault * 11.5), static_cast<float>(sizeCellDefault * (countCellOnHeight + 1))));

			return GUIModdingGun({}, ShapeInfoDebug, false, false);
		}();

	//////////////////////// ОТОБРАЖЕНИЕ ПОВТОРНЫХ СБОРОК ////////////////////////

	GUIModdingGun shapeTargetIter = [sizeCellDefault, countCellOnHeight, totalIterations]()
		{

			RectangleShape shapeTargetIter;
			shapeTargetIter.setSize(Vector2f(sizeCellDefault * 4, sizeCellDefault / 2));
			shapeTargetIter.setPosition(Vector2f(sizeCellDefault * 13.5f, sizeCellDefault * static_cast<float>(countCellOnHeight + 2)));

			return GUIModdingGun({ "Number_weapon_assemblies" , to_wstring(totalIterations) }, shapeTargetIter, true, false);
		}();

	//////////////////////// ЗАДНИЙ ФОН ////////////////////////////////////////////////

	RectangleShape background;
	background.setSize(Vector2f(lengthWindowMain, heightWindowMain));
	background.setPosition(Vector2f(0, 0));

	Texture backgroundTexture;
	if (backgroundTexture.loadFromFile(backgroundPatchMain))
	{
		background.setTexture(&backgroundTexture);
	}
	else
	{
		OUTPUT_LOG("main -> background.png not loaded");
	}

	// создаем окно
	RenderWindow window(VideoMode({ static_cast<unsigned int>(lengthWindowMain), static_cast<unsigned int>(heightWindowMain) }), "ModdingGun");

	RenderWindow diagram;

	window.setFramerateLimit(150);
	diagram.setFramerateLimit(150);

	/////////////////////// ПРОЧИЕ ПЕРЕМЕННЫЕ ////////////////////////
	// 
	//диаграмма

	// используем отрисовку кривой только один раз
	// с помощью diagramBool указывает какие окна отрисовали
	// false = отрисованно -> больше не будет повторяться
	vector<bool>         diagramBool;

	// вектор окон диаграм с отрисованными графикам
	// графики отрисовываются один раз -> diagramBool
	vector<RenderWindow> vectorWindowsDiagram;


	struct AllPackGraf
	{
		vector< GUIModdingGun> m_GUIPercentBinBt;
		vector< GUIModdingGun> m_GUIPercentSmoothBt;
		float m_percentBin;
		float m_percentSmooth;

		map<CountModifiers, float> CurrentSmoothFreqModifiers;
		map<CountModifiers, float> CurrentSourceFreqModifiers;
		Color       ColorCurve;
		CurvePack   CurrentCurvePack;

		vector<GUIModdingGun>     m_GUIPercentile50;
		shared_ptr<GUIModdingGun> m_GUIInitialMod;
		shared_ptr<GUIModdingGun> m_GUIEndMod;

		shared_ptr<GUIModdingGun> m_GUIMethod;
		vector<GUIModdingGun>     m_GUIMethodModifiers;
		shared_ptr<GUIModdingGun> m_GUIButtonTop;
		shared_ptr<GUIModdingGun> m_GUIButtonDown;

		size_t m_initialIndexMethod;
	};

	//вектор для кривых 
	vector<AllPackGraf> vDiagramPack;

	RectangleShape backgroundDiagram;
	backgroundDiagram.setSize(Vector2f(static_cast<float>(lengthWindowDiagram), static_cast<float>(heightWindowDiagram)));
	backgroundDiagram.setPosition(Vector2f(0.f, 0.f));

	Texture backgroundTextureDiagram;

	// задний фон
	if (!backgroundTextureDiagram.loadFromFile(backgroundPathDiagram))
	{
		OUTPUT_LOG("main -> background_graf.png not loaded");
	}
	else
	{
		backgroundDiagram.setTexture(&backgroundTextureDiagram);
	}

	int errorCase = 0;


	// дефолт статы для 
	// START_CHARACTERISTIC
	// initialCharacteristic
	const array<double, GunStats::countCharacteristic> defaultCharacteristic = { GunStats::countCharacteristic , 0.0 };

	// указанный мод
	// использовать только в области сборки calculate / vector для передачи в drawNormalGraph
	array<double, GunStats::countCharacteristic> modCharacteristic = defaultCharacteristic;

	// начальный мод 
	// использовать только в области сборки calculate / vector для передачи в drawNormalGraph
	array<double, GunStats::countCharacteristic> initialCharacteristic = defaultCharacteristic;

	vector<int> maxStatPosition = { 60, 48, 40,    40,40,40,40, };

	// выгрузка данных из файла оружия
	CharacteristicGun currentCharacteristicGun(maxStatPosition);

	const vector<int> nullPosition = { 0,0,0, 0,0,0,0 };

	// текущая позиция мода
	// 0 куч
	// 1 темп
	// 2 пробитие
	// 3 отдача
	// 4 качание
	// 6 сост
	// 5 грязь
	vector<int> positionMod = nullPosition;

	// начальная позиция мода
	// 0 куч
	// 1 темп
	// 2 пробитие
	// 3 отдача
	// 4 качание
	// 6 сост
	// 5 грязь
	vector<int> initialPosition = nullPosition;

	vector<pair <vector<int>, vector<int>>> historyPosition;

	bool pressedApplyAsMod = false;

	for (int itW = 0; window.isOpen(); itW++)
	{
		if (itW > 100) { itW = 0; }

		positionMouseForMain = Mouse::getPosition(window);
		mouseWorldPosForMain = window.mapPixelToCoords(positionMouseForMain);

		while (const optional event = window.pollEvent())
		{
			if (event->is<Event::Closed>())
			{
				window.close();
				for (int i = 0; i < vectorWindowsDiagram.size(); i++)
				{
					vectorWindowsDiagram[i].close();
				}
			}

			// выбор
			else if (const auto& mouseButtonPressed = event->getIf<Event::MouseButtonPressed>())
			{
				if (mouseButtonPressed->button == Mouse::Button::Left)
				{
					/// нажали на оружие в списке)
					for (size_t it = 0; it < arrayGun.size(); it++)
					{
						if (arrayGun[it].m_WeapoGUI.getRectangle().getGlobalBounds().contains(mouseWorldPosForMain))
						{
							historyPosition.clear();
							methodMod.clear();

							pressedApplyAsMod = false;

							initialCharacteristic = defaultCharacteristic;
							modCharacteristic = defaultCharacteristic;

							// ставим стандартный цвет на юнитах
							for (auto& mod : generalModOption)
							{
								for (auto& unit : mod.m_shaheUnitsInCharacteristic)
								{
									unit.setFillColor(Color(66, 66, 66));
								}
							}

							// получаем имя оружия
							nameGun = arrayGun[it].m_name;

							if (!currentCharacteristicGun.load(fileStat, stringToWString(nameGun)))
							{
								nameGun = "empty";
								textInfoDebug.setKeyStringAndTextWstring({ "Error_File" });
								positionMod = nullPosition;
								break;
							}

							selectWeaponGUI.setKeyStringAndTextWstring({ nameGun });

							textInfoDebug.setKeyStringAndTextWstring({ "Loaded",  nameGun });

							initialPosition = currentCharacteristicGun.getCurrentPosition();
							moveElement(initialPosition, 4, 2);
							positionMod = initialPosition;

							editShaheUnits(generalModOption, currentCharacteristicGun, initialPosition, positionMod);

							for (int itMod = 0; itMod < generalModOption.size(); itMod++)
							{
								if (generalModOption[itMod].m_markButtonCharacteristic.isPressed())
								{
									setChahceUpgrade(currentCharacteristicGun, currentModifiers, textGeneralModChance, itMod);
								}

								//generalModOption[itMod].m_textCharacteristicVisualPercent.setFillColor(ColorText);
								generalModOption[itMod].m_textCharacteristicVisualPercent.setKeyStringAndTextWstring({ L"0.0%" });

								// размета значений характеристики
								auto val = currentCharacteristicGun.getValueCharacteristic(GunStats::transIndexToIndexGunStat[itMod]);
								wstring WSval = itMod ? to_wstring(static_cast<int>(val)) : format(L"{:.1f}", val);
								generalModOption[itMod].m_textVisualCharacteristic.setKeyStringAndTextWstring({ WSval });

							}

							/// обновляем стоимость KitPrice
							/// tool уже выведен
							/// skill comming soon
							/////////////////////////////////////////////////////////

							unsigned value_price = getPrice(pathToPriceModifiersJS, currentModifiers, nameGun);
							currentPrice.PriceKit = value_price;
							for (auto& tp : kitPrice)
							{
								if (!tp.isButton())
								{
									tp.setKeyStringAndTextWstring({ format(L"{:L}", value_price), L"₽" });
									break;
								}
							}
							break;
						}
					}

					// languages
					size_t index = 0;
					for (auto& elem : vectorLangGUI)
					{
						if (elem.getRectangle().getGlobalBounds().contains(mouseWorldPosForMain))
						{
							// снимаем выделение всех
							for (auto& lGui : vectorLangGUI)
							{
								lGui.setAsReleasedButton();
							}

							elem.setAsPressedButton();

							LANG_SETTING.setLanguage(index);

							GUIModdingGun::refreshTranslateGUI();
						}
						++index;
					}

					for (auto& elem : vectorThemeGUI)
					{
						if (elem.getRectangle().getGlobalBounds().contains(mouseWorldPosForMain))
						{
							// снимаем выделение всех
							for (auto& e : vectorThemeGUI)
							{
								e.setAsPressedButton();
							}

							elem.setAsReleasedButton();

							/// Refresh всех GUI;

							auto theme = elem.getKeyStringAndTextWstring();

							if (std::holds_alternative<std::string>(theme[0]))
							{
								auto sTheme = std::get<std::string>(theme[0]);
								GUIModdingGun::refreshThemeGUI(sTheme);
							}
							else
							{
								OUTPUT_LOG("Main -> vectorThemeGUI - no string specified");
							}
						}
					}

					// нажали рассчет мода
					if (calculation.getRectangle().getGlobalBounds().contains(mouseWorldPosForMain))
					{
						calculation.setAsReleasedButton();
						// загружен ли файл?
						if (!currentCharacteristicGun.is_Empty())
						{
							textInfoDebug.setKeyStringAndTextWstring({ "calculation" });

							allModifiers.clear();
							allRandomAttemptUsed.clear();

							for (int i = 0; i < GunStats::countCharacteristic; i++)
							{
								modCharacteristic[i] = currentCharacteristicGun.getVisualPercentUpgradeCharacteristic(GunStats::transIndexToIndexGunStat[i]);
							}

							// если не совпадает c начальным 
							if (positionMod != initialPosition)
							{
								window.draw(background);
								window.draw(calculation.getRectangle());
								window.draw(calculation.getText());
								window.draw(textInfoDebug.getRectangle());
								window.draw(textInfoDebug.getText());
								window.display();

								vector<thread> threads_v;
								atomic<int> currentIndex = 0;

								// защита от деления на 0
								if (threadCount == 0) threadCount = 1;

								int chunkSize = totalIterations / threadCount;
								int remainder = totalIterations % threadCount;

								int start = 0;

								for (int i = 0; i < threadCount; ++i)
								{
									int end = start + chunkSize + (i < remainder ? 1 : 0);  // равномерно распределяем остаток

									threads_v.emplace_back([=]()
										{

											for (int j = start; j < end; ++j)
											{
												CharacteristicGun gunCopy = currentCharacteristicGun;
												RunSingleAssembly(j, methodMod);
											}
										});
									start = end;
								}

								// Ждём завершения всех потоков
								for (auto& t : threads_v) t.join();

								errorCase = 0;
								switch (RunSingleAssembly(-1, methodMod))
								{
								case 0:
									textInfoDebug.setKeyStringAndTextWstring({ "Success_assemble" });
									break;

								case -1:
									textInfoDebug.setKeyStringAndTextWstring({ "Empty_vector_method" });
									break;

								case -2:
									textInfoDebug.setKeyStringAndTextWstring({ "ChanceUpgrade_<=_0" });
									break;

								case -3:
									textInfoDebug.setKeyStringAndTextWstring({ "Key_Tool_not_found" });
									break;

								case -4:
									textInfoDebug.setKeyStringAndTextWstring({ "Key_Kit_not_found" });
									break;

								case -5:
									textInfoDebug.setKeyStringAndTextWstring({ "Key_Skill_not_found" });
									break;

								case -6:
									textInfoDebug.setKeyStringAndTextWstring({ "modifiers_noKit_noTool" });
									errorCase = -6;
									break;

								default:
									textInfoDebug.setKeyStringAndTextWstring({ "Unknown_error_RunSingleAssembly" });
									break;
								}
								diagram.create(VideoMode({ lengthWindowDiagram, heightWindowDiagram }), "diagram");

								vDiagramPack.emplace_back();
								vectorWindowsDiagram.push_back(move(diagram));
								diagramBool.push_back(false);
							}
							else
							{
								textInfoDebug.setKeyStringAndTextWstring({ "mod_matches" });
							}
						}
						else
						{
							textInfoDebug.setKeyStringAndTextWstring({ "File_not_uploaded" });
							positionMod = initialPosition = nullPosition;
						}
					}

					// окно смены колличества повторений
					else if (shapeTargetIter.getRectangle().getGlobalBounds().contains(mouseWorldPosForMain))
					{
						shapeTargetIter.setAsReleasedButton();
						totalIterations < 10000 ? totalIterations += 1000 : totalIterations = 1000;


						shapeTargetIter.setKeyStringAndTextWstring({ "Number_weapon_assemblies", to_wstring(totalIterations) });
					}

					// Окно параметров
					else if (shaheGeneralMod.getRectangle().getGlobalBounds().contains(mouseWorldPosForMain))
					{
						// нажали по кнопке 
						for (int it = 0; it < generalModOption.size(); it++)
						{
							if (generalModOption[it].m_markButtonCharacteristic.getRectangle().getGlobalBounds().contains(mouseWorldPosForMain))
							{

								for (int it2 = 0; it2 < generalModOption.size(); it2++)
								{
									generalModOption[it2].m_markButtonCharacteristic.setAsReleasedButton();
								}

								generalModOption[it].m_markButtonCharacteristic.setAsPressedButton();
								break;
							}
						}

						// нажали применить как мод
						if (buttonGeneralModApplyStat.getRectangle().getGlobalBounds().contains(mouseWorldPosForMain))
						{

							buttonGeneralModApplyStat.setAsReleasedButton();

							if (!currentCharacteristicGun.is_Empty())
							{
								for (int it = 0; it < generalModOption.size(); it++)
								{
									if (generalModOption[it].m_markButtonCharacteristic.isPressed())
									{
										if (positionMod[it] < currentCharacteristicGun.getMaxPositionCharacteristic()[GunStats::transIndexToIndexGunStat[it]])
										{
											auto RealPercent =
												calculateSuccessChance(
													currentCharacteristicGun.getChanceFor_NEXT_Stat(GunStats::transIndexToIndexGunStat[it]),
													GunStats::Modifiers::GetToolModifier(currentModifiers.Tool),
													GunStats::Modifiers::GetKitModifier(currentModifiers.Kit),
													GunStats::Modifiers::GetSkillModifier(currentModifiers.Skill)
												);

											if (!pressedApplyAsMod)
											{
												for (int i = 0; i < GunStats::countCharacteristic; i++)
												{
													initialCharacteristic[i] = currentCharacteristicGun.getVisualPercentUpgradeCharacteristic(GunStats::transIndexToIndexGunStat[i]);
												}
											}

											if (currentCharacteristicGun.upgradeStat(GunStats::transIndexToIndexGunStat[it]))
											{
												positionMod = currentCharacteristicGun.getCurrentPosition();
												moveElement(positionMod, 4, 2);

												// вектор уменьшения характеристик по GunStats
												auto Des = currentCharacteristicGun.getDecreaseForCurrentStat(GunStats::transIndexToIndexGunStat[it]);
												moveElement(Des, 4, 2);

												for (int i = 0; i < GunStats::countCharacteristic; i++)
												{
													initialPosition[i] -= Des[i];
												}

												historyPosition.push_back({ positionMod,  initialPosition });
												methodMod.push_back({ GunStats::transIndexToIndexGunStat[it], RealPercent, {currentModifiers}, currentCharacteristicGun.getVisualPercentUpgradeCharacteristic(GunStats::transIndexToIndexGunStat[it]), currentPrice });

												pressedApplyAsMod = true;
												shapeGeneralModApplyStatInitial.setAsReleasedButton();

												textInfoDebug.setKeyStringAndTextWstring({ "Applied_mod_step_recorded" });
											}
										}
										else
										{
											textInfoDebug.setKeyStringAndTextWstring({ "Cannot_improved_beyond_maximum" });
										}
										break;
									}

								}
							}
							else
							{
								textInfoDebug.setKeyStringAndTextWstring({ "Data_not_loaded" });
							}
						}

						// нажали применить как начальную
						else if (shapeGeneralModApplyStatInitial.getRectangle().getGlobalBounds().contains(mouseWorldPosForMain))
						{
							if (!pressedApplyAsMod)
							{
								shapeGeneralModApplyStatInitial.setAsReleasedButton();

								if (!currentCharacteristicGun.is_Empty())
								{
									for (int it = 0; it < generalModOption.size(); it++)
									{

										if (generalModOption[it].m_markButtonCharacteristic.isPressed())
										{

											if (currentCharacteristicGun.upgradeStat(GunStats::transIndexToIndexGunStat[it]))
											{

												initialPosition = currentCharacteristicGun.getCurrentPosition();
												moveElement(initialPosition, 4, 2);

												positionMod = initialPosition;

												for (int i = 0; i < GunStats::countCharacteristic; i++)
												{
													initialCharacteristic[i] = currentCharacteristicGun.getVisualPercentUpgradeCharacteristic(GunStats::transIndexToIndexGunStat[i]);
												}

												historyPosition.push_back({ positionMod,  initialPosition });

												textInfoDebug.setKeyStringAndTextWstring({ "Marked_as_initial" });
											}
											else
											{
												textInfoDebug.setKeyStringAndTextWstring({ "Cannot_improved_beyond_maximum" });
											}
											break;
										}
									}
								}
								else
								{
									textInfoDebug.setKeyStringAndTextWstring({ "Data_not_loaded" });
								}
							}
							else
							{
								textInfoDebug.setKeyStringAndTextWstring({ "Prohibited_after_applying_mod" });
							}
						}

						// нажали сбросить
						else if (shapeGeneralModReset.getRectangle().getGlobalBounds().contains(mouseWorldPosForMain))
						{
							historyPosition.clear();
							methodMod.clear();

							shapeGeneralModReset.setAsReleasedButton();
							currentCharacteristicGun.returnDefaultPosition();

							initialPosition = currentCharacteristicGun.getCurrentPosition();
							moveElement(initialPosition, 4, 2);
							positionMod = initialPosition;

							pressedApplyAsMod = false;

							initialCharacteristic = defaultCharacteristic;
							modCharacteristic = defaultCharacteristic;

							textInfoDebug.setKeyStringAndTextWstring({ "Mod_reset" });
						}

						//нажали вернуться на шаг  назад
						else if (shapeGeneralModBack.getRectangle().getGlobalBounds().contains(mouseWorldPosForMain))
						{
							shapeGeneralModBack.setAsReleasedButton();

							//возвращаем шаг назад
							if (!currentCharacteristicGun.is_Empty())
							{
								if (currentCharacteristicGun.stepBack())
								{
									textInfoDebug.setKeyStringAndTextWstring({ "Step_back" });

									// если была нажата принять как мод, удаляем последний элемент
									if (pressedApplyAsMod)
									{
										if (!methodMod.empty())
										{
											methodMod.pop_back();
										}
										else
										{
											textInfoDebug.setKeyStringAndTextWstring({ "errorCase - vector was not reduced, it was empty" });
										}

										if (methodMod.empty()) { pressedApplyAsMod = false; }
									}
									if (!historyPosition.empty())
									{
										historyPosition.pop_back();

										auto cp_temp = currentCharacteristicGun.getCurrentPosition();
										moveElement(cp_temp, 4, 2);

										if (!historyPosition.empty())
										{
											positionMod = historyPosition[historyPosition.size() - 1].first;
											initialPosition = historyPosition[historyPosition.size() - 1].second;
										}
										else
										{
											positionMod = initialPosition = cp_temp;
										}

										// проверка всегда по positionMod
										if (positionMod != cp_temp) { textInfoDebug.setKeyStringAndTextWstring({ L"ERROR 'Position != Check' " }); }
									}
									else { textInfoDebug.setKeyStringAndTextWstring({ L"No more steps ERROR 'historyPosition' " }); }
								}
								else { textInfoDebug.setKeyStringAndTextWstring({ "No_steps" }); }
							}
							else { textInfoDebug.setKeyStringAndTextWstring({ "Cant_take_step_back_there_no_data" }); }
						}

						// размечаем процент
						for (int it = 0; it < generalModOption.size(); it++)
						{
							generalModOption[it].m_textCharacteristicVisualPercent.resetColorText();

							if (!currentCharacteristicGun.is_Empty())
							{
								auto vPercent = currentCharacteristicGun.getVisualPercentUpgradeCharacteristic(GunStats::transIndexToIndexGunStat[it]);

								if (vPercent > 0) { generalModOption[it].m_textCharacteristicVisualPercent.setColorText(Color::Green); }
								else if (vPercent < 0) { generalModOption[it].m_textCharacteristicVisualPercent.setColorText(Color::Red); }

								generalModOption[it].m_textCharacteristicVisualPercent.setKeyStringAndTextWstring({ format(L"{:.1f}", vPercent), L"%" });

								// если не нажата
								if (!pressedApplyAsMod)
								{
									initialCharacteristic[it] = vPercent;
									modCharacteristic[it] = vPercent;
								}
								else
								{
									modCharacteristic[it] = vPercent;
								}

							}
							else
							{
								generalModOption[it].m_textCharacteristicVisualPercent.setKeyStringAndTextWstring({ L"0.0%" });
							}

							// разметка шанса улучшения процента
							if (generalModOption[it].m_markButtonCharacteristic.isPressed())
							{
								setChahceUpgrade(currentCharacteristicGun, currentModifiers, textGeneralModChance, it);
							}

							// размета значений характеристики
							auto val = currentCharacteristicGun.getValueCharacteristic(GunStats::transIndexToIndexGunStat[it]);
							wstring WSval = it ? to_wstring(static_cast<int>(val)) : format(L"{:.1f}", val);
							generalModOption[it].m_textVisualCharacteristic.setKeyStringAndTextWstring({ WSval });
						}
						editShaheUnits(generalModOption, currentCharacteristicGun, initialPosition, positionMod);
					}

					//МОДИЦИФКАТОРЫ

					// инструменты
					for (int i = 0; i < modifiersIcon.m_toolRectangleGUI.size(); i++)
					{
						if (modifiersIcon.m_toolRectangleGUI[i].getRectangle().getGlobalBounds().contains(mouseWorldPosForMain))
						{
							for (auto& sh : modifiersIcon.m_toolRectangleGUI)
							{
								sh.setAsReleasedButton();
							}
							modifiersIcon.m_toolRectangleGUI[i].setAsPressedButton();

							string tipName = toolpath[i].stem().string().erase(0, 3);

							auto tool = GunStats::Modifiers::toolTypeBegin;

							for (tool++; tool < GunStats::Modifiers::toolTypeEnd; tool++)
							{
								if (GunStats::Modifiers::GetToolName(tool) == tipName)
								{
									break;
								}
							}
							currentModifiers.Tool = tool;
							textInfoDebug.setKeyStringAndTextWstring({ "select",  GunStats::Modifiers::GetToolName(tool) });

							for (int it = 0; it < generalModOption.size(); it++)
							{
								if (generalModOption[it].m_markButtonCharacteristic.isPressed())
								{
									setChahceUpgrade(currentCharacteristicGun, currentModifiers, textGeneralModChance, it);
									break;
								}
							}

							/// стоимость ToolPrice
							unsigned valuePrice = getPrice(pathToPriceModifiersJS, currentModifiers);
							currentPrice.PriceTool = valuePrice;

							for (auto& tp : toolPriceAll)
							{
								if (!tp.isButton())
								{
									tp.setKeyStringAndTextWstring({ format(L"{:L}", valuePrice), L"₽" });
									break;
								}
							}
							break;
						}
					}

					// наборы
					for (int i = 0; i < modifiersIcon.m_kitRectangleGUI.size(); i++)
					{
						if (modifiersIcon.m_kitRectangleGUI[i].getRectangle().getGlobalBounds().contains(mouseWorldPosForMain))
						{
							for (auto& sh : modifiersIcon.m_kitRectangleGUI)
							{
								sh.setAsReleasedButton();
							}
							modifiersIcon.m_kitRectangleGUI[i].setAsPressedButton();

							string STip = kitpath[i].stem().string().erase(0, 3);

							auto kitSelect = GunStats::Modifiers::kitTypeBegin;
							for (kitSelect++; kitSelect < GunStats::Modifiers::kitTypeEnd; kitSelect++)
							{
								if (GunStats::Modifiers::GetKitName(kitSelect) == STip)
								{
									break;
								}
							}
							currentModifiers.Kit = kitSelect;
							textInfoDebug.setKeyStringAndTextWstring({ "select", GunStats::Modifiers::GetKitName(kitSelect) });


							for (int it = 0; it < generalModOption.size(); it++)
							{
								if (generalModOption[it].m_markButtonCharacteristic.isPressed())
								{
									setChahceUpgrade(currentCharacteristicGun, currentModifiers, textGeneralModChance, it);
									break;
								}
							}

							/// стоимость KitPrice
							unsigned valuePrice = getPrice(pathToPriceModifiersJS, currentModifiers, nameGun);
							currentPrice.PriceKit = valuePrice;
							for (auto& kp : kitPrice)
							{
								if (!kp.isButton())
								{
									kp.setKeyStringAndTextWstring({ format(L"{:L}", valuePrice), L"₽" });
									break;
								}
							}
							break;
						}
					}
					// скилы
					for (int i = 0; i < modifiersIcon.m_skillRectangleGUI.size(); i++)
					{
						if (modifiersIcon.m_skillRectangleGUI[i].getRectangle().getGlobalBounds().contains(mouseWorldPosForMain))
						{
							for (auto& sh : modifiersIcon.m_skillRectangleGUI)
							{
								sh.setAsReleasedButton();
							}
							modifiersIcon.m_skillRectangleGUI[i].setAsPressedButton();

							string STip = skillpath[i].stem().string().erase(0, 3);
							auto skillSelect = GunStats::Modifiers::skillTypeBegin;
							for (skillSelect++; skillSelect < GunStats::Modifiers::skillTypeEnd; skillSelect++)
							{
								if (GunStats::Modifiers::GetSkillName(skillSelect) == STip)
								{
									break;
								}
							}
							currentModifiers.Skill = skillSelect;
							textInfoDebug.setKeyStringAndTextWstring({ "select", GunStats::Modifiers::GetSkillName(skillSelect) });

							for (int it = 0; it < generalModOption.size(); it++)
							{
								if (generalModOption[it].m_markButtonCharacteristic.isPressed())
								{
									setChahceUpgrade(currentCharacteristicGun, currentModifiers, textGeneralModChance, it);
									break;
								}
							}

							currentPrice.PriceSkill = 0;
							break;
						}
					}

					/// стоимость      
					for (auto& GUIToolPrice : toolPriceAll)
					{
						if (GUIToolPrice.getRectangle().getGlobalBounds().contains(mouseWorldPosForMain) && GUIToolPrice.isButton())
						{
							auto vvstr = *GUIToolPrice.getKeyStringAndTextWstring().begin();
							if (holds_alternative<wstring>(vvstr))
							{
								if (get<wstring>(vvstr) == L"--")
								{
									GUIToolPrice.setAsReleasedButton();
									setPriceTool(pathToPriceModifiersJS, currentModifiers.Tool, currentPrice.PriceTool - 100'000);
								}
								else if (get<wstring>(vvstr) == L"-")
								{
									GUIToolPrice.setAsReleasedButton();

									setPriceTool(pathToPriceModifiersJS, currentModifiers.Tool, currentPrice.PriceTool - 5'000);
								}
								else if (get<wstring>(vvstr) == L"+")
								{
									GUIToolPrice.setAsReleasedButton();
									setPriceTool(pathToPriceModifiersJS, currentModifiers.Tool, currentPrice.PriceTool + 5'000);
								}
								else if (get<wstring>(vvstr) == L"++")
								{
									GUIToolPrice.setAsReleasedButton();
									setPriceTool(pathToPriceModifiersJS, currentModifiers.Tool, currentPrice.PriceTool + 100'000);
								}

								/// стоимость ToolPrice
								unsigned valuePrice = getPrice(pathToPriceModifiersJS, currentModifiers);
								currentPrice.PriceTool = valuePrice;
								for (auto& tp : toolPriceAll)
								{
									if (!tp.isButton())
									{
										tp.setKeyStringAndTextWstring({ format(L"{:L}", valuePrice), L"₽" });
										break;
									}
								}
							}
						}
					}

					for (auto& GUIKitPrice : kitPrice)
					{
						if (GUIKitPrice.getRectangle().getGlobalBounds().contains(mouseWorldPosForMain) && GUIKitPrice.isButton())
						{
							auto vvstr = *GUIKitPrice.getKeyStringAndTextWstring().begin();
							if (holds_alternative<wstring>(vvstr))
							{
								if (get<wstring>(vvstr) == L"--")
								{
									GUIKitPrice.setAsReleasedButton();

									setsPriceKit(pathToPriceModifiersJS, currentModifiers.Kit, nameGun, currentPrice.PriceKit - 100'000);
								}
								else if (get<wstring>(vvstr) == L"-")
								{
									GUIKitPrice.setAsReleasedButton();
									setsPriceKit(pathToPriceModifiersJS, currentModifiers.Kit, nameGun, currentPrice.PriceKit - 5'000);
								}
								else if (get<wstring>(vvstr) == L"+")
								{
									GUIKitPrice.setAsReleasedButton();
									setsPriceKit(pathToPriceModifiersJS, currentModifiers.Kit, nameGun, currentPrice.PriceKit + 5'000);
								}
								else if (get<wstring>(vvstr) == L"++")
								{
									GUIKitPrice.setAsReleasedButton();
									setsPriceKit(pathToPriceModifiersJS, currentModifiers.Kit, nameGun, currentPrice.PriceKit + 100'000);
								}

								/// стоимость KitPrice
								unsigned value_price = getPrice(pathToPriceModifiersJS, currentModifiers, nameGun);
								currentPrice.PriceKit = value_price;
								for (auto& kp : kitPrice)
								{
									if (!kp.isButton())
									{
										kp.setKeyStringAndTextWstring({ format(L"{:L}", value_price), L"₽" });
										break;
									}
								}
							}
						}
					}
				}
			}

			// прокрутка
			else if (const auto& mouseScroll = event->getIf<Event::MouseWheelScrolled>())
			{
				if (allGunShapeScrollGUI.getRectangle().getGlobalBounds().contains(mouseWorldPosForMain))
				{
					if (mouseScroll->delta < 0 && itArrayGun < arrayGun.size() - countGunList)
					{
						itArrayGun++;

						wstring stlStr = L"";
						if (itArrayGun != 0)
						{
							stlStr = to_wstring(itArrayGun) + L"+";
						}
						topScrollToolTip.setKeyStringAndTextWstring({ stlStr });

						stlStr = L"";
						if (arrayGun.size() - (itArrayGun + countGunList) > 0)
						{
							stlStr = to_wstring(arrayGun.size() - (itArrayGun + countGunList)) + "+";
						}
						downScrollToolTip.setKeyStringAndTextWstring({ stlStr });

					}
					else if (mouseScroll->delta > 0 && itArrayGun > 0)
					{
						itArrayGun--;

						wstring stlStr = L"";
						if (itArrayGun != 0)
						{
							stlStr = to_wstring(itArrayGun) + L"+";
						}
						topScrollToolTip.setKeyStringAndTextWstring({ stlStr });

						stlStr = L"";
						if (arrayGun.size() - (itArrayGun + countGunList) > 0)
						{
							stlStr = to_wstring(arrayGun.size() - (itArrayGun + countGunList)) + "+";
						}
						downScrollToolTip.setKeyStringAndTextWstring({ stlStr });

					}
				}
			}

		}

		window.clear(Color::White);
		window.draw(background);

		for (auto& GUI : vectorLangGUI)
		{
			window.draw(GUI.getRectangle());
			window.draw(GUI.getText());
		}

		for (auto& GUI : vectorThemeGUI)
		{
			window.draw(GUI.getRectangle());
			window.draw(GUI.getText());
		}

		// модификаторы
		for (auto& GUI : modifiersIcon.m_toolRectangleGUI)
		{
			window.draw(GUI.getRectangle());
			//window.draw(GUI.getText());
		}
		for (auto& GUI : modifiersIcon.m_kitRectangleGUI)
		{
			window.draw(GUI.getRectangle());
			//window.draw(GUI.getText());
		}
		for (auto& GUI : modifiersIcon.m_skillRectangleGUI)
		{
			window.draw(GUI.getRectangle());
			//window.draw(GUI.getText());
		}

		/// стоимость
		for (auto& GUI : toolPriceAll)
		{
			window.draw(GUI.getRectangle());
			window.draw(GUI.getText());
		}
		for (auto& GUI : kitPrice)
		{
			window.draw(GUI.getRectangle());
			window.draw(GUI.getText());
		}

		window.draw(skillPrice.getRectangle());
		window.draw(skillPrice.getText());

		window.draw(allGunShapeScrollGUI.getRectangle());
		//window.draw(allGunShapeScrollGUI.getText());

		window.draw(selectWeaponGUI.getRectangle());
		window.draw(selectWeaponGUI.getText());

		for (int it1 = 0, it_VectorGun2 = itArrayGun; it1 < countGunList; it1++, it_VectorGun2++)
		{
			arrayGun[it_VectorGun2].m_WeapoGUI.setPositionRectangle(Vector2f(0, sizeCellDefault / 2 * it1));

			window.draw(arrayGun[it_VectorGun2].m_WeapoGUI.getRectangle());
			window.draw(arrayGun[it_VectorGun2].m_WeapoGUI.getText());
		}

		window.draw(topScrollToolTip.getRectangle());
		window.draw(topScrollToolTip.getText());

		window.draw(downScrollToolTip.getRectangle());
		window.draw(downScrollToolTip.getText());

		window.draw(calculation.getRectangle());
		window.draw(calculation.getText());

		window.draw(shaheGeneralMod.getRectangle());
		window.draw(shaheGeneralMod.getText());

		for (int it1 = 0; it1 < generalModOption.size(); it1++)
		{

			window.draw(generalModOption[it1].m_textCharacteristicName.getRectangle());
			window.draw(generalModOption[it1].m_textCharacteristicName.getText());


			window.draw(generalModOption[it1].m_textVisualCharacteristic.getRectangle());
			window.draw(generalModOption[it1].m_textVisualCharacteristic.getText());


			window.draw(generalModOption[it1].m_textCharacteristicVisualPercent.getRectangle());
			window.draw(generalModOption[it1].m_textCharacteristicVisualPercent.getText());

			window.draw(generalModOption[it1].m_shapeCharacteristic);

			window.draw(generalModOption[it1].m_markButtonCharacteristic.getRectangle());
			//window.draw(generalModOption[it1].m_markButtonCharacteristic.getText()); // текста нет

			bool select = generalModOption[it1].m_markButtonCharacteristic.isPressed();

			// метка юнита
			size_t ps = positionMod[it1];

			if (ps >= generalModOption[it1].m_shaheUnitsInCharacteristic.size())
			{
				ps = generalModOption[it1].m_shaheUnitsInCharacteristic.size() - 1;
			}
			else if (select && ps >= currentCharacteristicGun.getMaxPositionCharacteristic()[GunStats::transIndexToIndexGunStat[it1]])
			{
				generalModOption[it1].m_shaheUnitsInCharacteristic[ps].setOutlineColor(Color(128, 128, 128));
			}
			else if (select)
			{
				generalModOption[it1].m_shaheUnitsInCharacteristic[ps].setOutlineColor(Color::Green);
			}
			for (int it2 = 0; it2 < generalModOption[it1].m_shaheUnitsInCharacteristic.size(); it2++)
			{
				window.draw(generalModOption[it1].m_shaheUnitsInCharacteristic[it2]);
			};
			if (select)
			{
				generalModOption[it1].m_shaheUnitsInCharacteristic[ps].setOutlineColor(Color::Black);
			};
		}

		//window.draw(textGeneralModChance.getRectangle());
		window.draw(textGeneralModChance.getText());

		window.draw(shapeGeneralModApplyStatInitial.getRectangle());
		window.draw(shapeGeneralModApplyStatInitial.getText());

		window.draw(buttonGeneralModApplyStat.getRectangle());
		window.draw(buttonGeneralModApplyStat.getText());

		window.draw(shapeGeneralModReset.getRectangle());
		window.draw(shapeGeneralModReset.getText());

		window.draw(shapeGeneralModBack.getRectangle());
		window.draw(shapeGeneralModBack.getText());

		window.draw(textInfoDebug.getRectangle());
		window.draw(textInfoDebug.getText());

		window.draw(shapeTargetIter.getRectangle());
		window.draw(shapeTargetIter.getText());

		// тоолтипы   

		// инструменты
		for (int i = 0; i < modifiersIcon.m_toolRectangleGUI.size(); i++)
		{
			if (modifiersIcon.m_toolRectangleGUI[i].getRectangle().getGlobalBounds().contains(mouseWorldPosForMain))
			{
				string STip = toolpath[i].stem().string().erase(0, 3);
				wstring tip = LANG_TRANSLATE(STip);

				RectangleShape shapeToolTooltip;
				shapeToolTooltip.setOutlineColor(Color::Black);
				shapeToolTooltip.setOutlineThickness(-2);
				shapeToolTooltip.setSize(Vector2f(static_cast<float>(15.f + 7.f * tip.size()), sizeCellDefault / 3.f));
				shapeToolTooltip.setPosition(Vector2f(mouseWorldPosForMain.x, mouseWorldPosForMain.y - sizeCellDefault / 3.f));

				Text textToolTooltip(ThemeGui::currentFont);
				textToolTooltip.setString(tip);
				textToolTooltip.setCharacterSize(static_cast<unsigned>(ThemeGui::characterSize * 0.8f));
				textToolTooltip.setFillColor(Color::Black);

				auto rectPos = shapeToolTooltip.getPosition();
				auto rectSize = shapeToolTooltip.getSize();
				auto textBounds = textToolTooltip.getLocalBounds();

				textToolTooltip.setOrigin(Vector2f(0.f, textBounds.position.y + textBounds.size.y / 2.0f));
				textToolTooltip.setPosition(Vector2f(rectPos.x + 10, rectPos.y + rectSize.y / 2.0f));

				window.draw(shapeToolTooltip);
				window.draw(textToolTooltip);
			}
		}

		// наборы
		for (int i = 0; i < modifiersIcon.m_kitRectangleGUI.size(); i++)
		{
			if (modifiersIcon.m_kitRectangleGUI[i].getRectangle().getGlobalBounds().contains(mouseWorldPosForMain))
			{
				string STip = kitpath[i].stem().string().erase(0, 3);
				wstring Tip = LANG_TRANSLATE(STip);

				RectangleShape shapeKitTooltip;
				shapeKitTooltip.setOutlineColor(Color::Black);
				shapeKitTooltip.setOutlineThickness(-2);
				shapeKitTooltip.setSize(Vector2f(static_cast<float>(15.f + 7.f * Tip.size()), sizeCellDefault / 3.f));
				shapeKitTooltip.setPosition(Vector2f(mouseWorldPosForMain.x, mouseWorldPosForMain.y - sizeCellDefault / 3.f));

				Text textKitTooltip(ThemeGui::currentFont);
				textKitTooltip.setString(Tip);
				textKitTooltip.setCharacterSize(static_cast<unsigned>(ThemeGui::characterSize * 0.8f));
				textKitTooltip.setFillColor(Color::Black);

				auto textBounds = textKitTooltip.getLocalBounds();
				auto rectPos = shapeKitTooltip.getPosition();
				auto rectSize = shapeKitTooltip.getSize();

				textKitTooltip.setOrigin(Vector2f(0.f, textBounds.position.y + textBounds.size.y / 2.0f));
				textKitTooltip.setPosition(Vector2f(rectPos.x + 10, rectPos.y + rectSize.y / 2.0f));

				window.draw(shapeKitTooltip);
				window.draw(textKitTooltip);
			}
		}
		// скиллы
		for (int i = 0; i < modifiersIcon.m_skillRectangleGUI.size(); i++)
		{
			if (modifiersIcon.m_skillRectangleGUI[i].getRectangle().getGlobalBounds().contains(mouseWorldPosForMain))
			{
				string STip = skillpath[i].stem().string().erase(0, 3);
				wstring tip = LANG_TRANSLATE(STip);

				RectangleShape shapeSkill_tooltip;
				shapeSkill_tooltip.setOutlineColor(Color::Black);
				shapeSkill_tooltip.setOutlineThickness(-2);
				shapeSkill_tooltip.setPosition(Vector2f(mouseWorldPosForMain.x, mouseWorldPosForMain.y - sizeCellDefault / 3.f));
				shapeSkill_tooltip.setSize(Vector2f(static_cast<float>(15.f + 7.f * tip.size()), sizeCellDefault / 3.f));

				Text textSkillTooltip(ThemeGui::currentFont);
				textSkillTooltip.setString(tip);
				textSkillTooltip.setCharacterSize(static_cast<unsigned>(ThemeGui::characterSize * 0.8f));
				textSkillTooltip.setFillColor(Color::Black);

				auto rectPos = shapeSkill_tooltip.getPosition();
				auto rectSize = shapeSkill_tooltip.getSize();
				auto textBounds = textSkillTooltip.getLocalBounds();

				textSkillTooltip.setOrigin(Vector2f(0.f, textBounds.position.y + textBounds.size.y / 2.0f));
				textSkillTooltip.setPosition(Vector2f(rectPos.x + 10, rectPos.y + rectSize.y / 2.0f));

				window.draw(shapeSkill_tooltip);
				window.draw(textSkillTooltip);

			}
		}

		window.display();

		// отрисовка диаграмм
		for (int iterGrafF = static_cast<int>(vectorWindowsDiagram.size()) - 1; iterGrafF >= 0; --iterGrafF)
		{
			if (vectorWindowsDiagram[iterGrafF].isOpen())
			{
				auto positionMouseForGraf = Mouse::getPosition(vectorWindowsDiagram[iterGrafF]);
				auto mouseWorldPosForGraf = window.mapPixelToCoords(positionMouseForGraf);
				unsigned int methodList = 22;

				while (const optional event = vectorWindowsDiagram[iterGrafF].pollEvent())
				{
					if (event->is<Event::Closed>())
					{
						vectorWindowsDiagram[iterGrafF].close();
						diagramBool.erase(diagramBool.begin() + iterGrafF);
						vectorWindowsDiagram.erase(vectorWindowsDiagram.begin() + iterGrafF);
						vDiagramPack.erase(vDiagramPack.begin() + iterGrafF);

						break; // чтобы не обращаться к уже удалённому i
					}
					else if (const auto& mouseButtonPressed = event->getIf<Event::MouseButtonPressed>())
					{
						if (mouseButtonPressed->button == Mouse::Button::Left)
						{
							// если прокрутка доступна
							if (vDiagramPack[iterGrafF].m_GUIButtonTop.get() != nullptr && vDiagramPack[iterGrafF].m_GUIButtonDown.get() != nullptr)
							{
								auto size = vDiagramPack[iterGrafF].m_GUIMethodModifiers.size();

								// кнопка прокрутки верхняя
								if (vDiagramPack[iterGrafF].m_GUIButtonTop.get()->getRectangle().getGlobalBounds().contains(mouseWorldPosForGraf))
								{
									// но не более размера 
									if (vDiagramPack[iterGrafF].m_initialIndexMethod > 0)
									{
										vDiagramPack[iterGrafF].m_initialIndexMethod--;
									}
								}

								//кнопка прокрутки вниз
								else if (vDiagramPack[iterGrafF].m_GUIButtonDown.get()->getRectangle().getGlobalBounds().contains(mouseWorldPosForGraf))
								{
									// но не более размера 
									if (vDiagramPack[iterGrafF].m_initialIndexMethod < size)
									{
										vDiagramPack[iterGrafF].m_initialIndexMethod++;
									}
								}

								// корректировка 
								auto SizeRec = vDiagramPack[iterGrafF].m_GUIMethodModifiers.begin()->getRectangle().getSize();
								auto XSizeWindow = vectorWindowsDiagram[iterGrafF].getSize().x;

								for (size_t i = vDiagramPack[iterGrafF].m_initialIndexMethod; i < methodList || i < size; i++)
								{
									vDiagramPack[iterGrafF].m_GUIMethodModifiers[i].setPositionRectangle(Vector2f(XSizeWindow - SizeRec.x, SizeRec.y * (i - vDiagramPack[iterGrafF].m_initialIndexMethod)));
								}
							}

							bool needRefresh = false;
							float sizeValBN = 0.010f;
							float sizeValSM = 0.005f;

							for (auto& GUIBinBt : vDiagramPack[iterGrafF].m_GUIPercentBinBt)
							{
								auto vbt = *GUIBinBt.getKeyStringAndTextWstring().begin();
								if (GUIBinBt.getRectangle().getGlobalBounds().contains(mouseWorldPosForGraf) && GUIBinBt.isButton() && holds_alternative<wstring>(vbt) && get<wstring>(vbt) == L"-")
								{
									GUIBinBt.setAsReleasedButton();
									if (vDiagramPack[iterGrafF].m_percentBin - sizeValBN >= 0.f)
									{
										vDiagramPack[iterGrafF].m_percentBin -= sizeValBN;
										needRefresh = true;
									}
									break;
								}
								else if (GUIBinBt.getRectangle().getGlobalBounds().contains(mouseWorldPosForGraf) && GUIBinBt.isButton() && holds_alternative<wstring>(vbt) && get<wstring>(vbt) == L"+")
								{
									GUIBinBt.setAsReleasedButton();
									if (vDiagramPack[iterGrafF].m_percentBin + sizeValBN <= 1.f)
									{
										vDiagramPack[iterGrafF].m_percentBin += sizeValBN;
										needRefresh = true;
									}
									break;
								}
							}
							for (auto& GUISmoothBt : vDiagramPack[iterGrafF].m_GUIPercentSmoothBt)
							{
								auto vbt = *GUISmoothBt.getKeyStringAndTextWstring().begin();
								if (GUISmoothBt.getRectangle().getGlobalBounds().contains(mouseWorldPosForGraf) && GUISmoothBt.isButton() && holds_alternative<wstring>(vbt) && get<wstring>(vbt) == L"-")
								{
									GUISmoothBt.setAsReleasedButton();

									if (vDiagramPack[iterGrafF].m_percentSmooth - sizeValSM >= 0.f)
									{
										vDiagramPack[iterGrafF].m_percentSmooth -= sizeValSM;
										needRefresh = true;
									}
									break;
								}
								else if (GUISmoothBt.getRectangle().getGlobalBounds().contains(mouseWorldPosForGraf) && GUISmoothBt.isButton() && holds_alternative<wstring>(vbt) && get<wstring>(vbt) == L"+")
								{
									GUISmoothBt.setAsReleasedButton();

									if (vDiagramPack[iterGrafF].m_percentSmooth + sizeValSM <= 1.f)
									{
										vDiagramPack[iterGrafF].m_percentSmooth += sizeValSM;
										needRefresh = true;
									}
									break;
								}
							}
							if (needRefresh)
							{
								for (auto& GUISmoothBt : vDiagramPack[iterGrafF].m_GUIPercentSmoothBt)
								{
									auto vbt = *GUISmoothBt.getKeyStringAndTextWstring().begin();
									if (holds_alternative<wstring>(vbt) && !GUISmoothBt.isButton())
									{
										GUISmoothBt.setKeyStringAndTextWstring({ format(L"{:.3f}", vDiagramPack[iterGrafF].m_percentSmooth) });
										break;

									}
								}
								for (auto& GUIBinBt : vDiagramPack[iterGrafF].m_GUIPercentBinBt)
								{
									auto vbt = *GUIBinBt.getKeyStringAndTextWstring().begin();
									if (holds_alternative<wstring>(vbt) && !GUIBinBt.isButton())
									{
										GUIBinBt.setKeyStringAndTextWstring({ format(L"{:.3f}", vDiagramPack[iterGrafF].m_percentBin) });
										break;
									}
								}

								auto binnedFreqMap = getBinedFreqModifiers(vDiagramPack[iterGrafF].CurrentSourceFreqModifiers, vDiagramPack[iterGrafF].m_percentBin); // сколько % оставим? (%)
								auto smoothedFreqMap = getSmoothFreqModifiers(binnedFreqMap, vDiagramPack[iterGrafF].m_percentSmooth); // насколько сильно (%)

								vDiagramPack[iterGrafF].CurrentSmoothFreqModifiers = smoothedFreqMap;

								vDiagramPack[iterGrafF].CurrentCurvePack.m_curve = curveFun(vDiagramPack[iterGrafF].CurrentCurvePack.m_mainRectangleCurve, vDiagramPack[iterGrafF].ColorCurve, vDiagramPack[iterGrafF].CurrentSmoothFreqModifiers);
							}
						}
					}
				}

				if (iterGrafF < vectorWindowsDiagram.size())
				{
					if (!diagramBool[iterGrafF])
					{
						if (allModifiers.empty())
						{
							OUTPUT_LOG("main -> allModifiers empty");
							continue;
						}

						if (allRandomAttemptUsed.empty())
						{
							OUTPUT_LOG("main -> allRandomAttemptUsed empty");
							continue;
						}

						// стандартный вид
						vDiagramPack[iterGrafF].m_percentBin = 1.0f;
						vDiagramPack[iterGrafF].m_percentSmooth = 0.05f;

						// на сколько делим цену ? 
						float priceValueDegree = 1'000'000; // до KK
						vDiagramPack[iterGrafF].ColorCurve = Color::Red;

						// Получаем позицию главного окна
						Vector2i mainPos = vectorWindowsDiagram[iterGrafF].getPosition();
						Vector2u mainSize = vectorWindowsDiagram[iterGrafF].getSize();

						// Центрируем текущий diagram поверх главного окна
						int xPosDiagram = mainPos.x + (static_cast<int>(mainSize.x) - static_cast<int>(lengthWindowDiagram)) / 2;
						int yPosDiagram = mainPos.y + (static_cast<int>(mainSize.y) - static_cast<int>(heightWindowDiagram)) / 2;

						vectorWindowsDiagram[iterGrafF].setPosition(Vector2i(xPosDiagram, yPosDiagram));

						// оформление кривой

						float xPosPack = vectorWindowsDiagram[iterGrafF].getSize().x * 0.1f;
						float yPosPack = vectorWindowsDiagram[iterGrafF].getSize().y * 0.5f;

						float xSizePack = vectorWindowsDiagram[iterGrafF].getSize().x * 0.4f;
						float ySizePack = vectorWindowsDiagram[iterGrafF].getSize().y * 0.4f;

						RectangleShape rectangleForCurve;
						rectangleForCurve.setSize({ xSizePack, ySizePack });
						rectangleForCurve.setPosition({ xPosPack, yPosPack });
						rectangleForCurve.setFillColor(Color(255, 255, 255, 220));
						rectangleForCurve.setOutlineColor(Color::Black);
						rectangleForCurve.setOutlineThickness(-1);

						//"сырые" частоты
						auto sourceFreqMap = getFreqModifiers(allModifiers);
						auto binnedFreqMap = getBinedFreqModifiers(sourceFreqMap, vDiagramPack[iterGrafF].m_percentBin); // сколько % оставим? (%)
						auto smoothedFreqMap = getSmoothFreqModifiers(binnedFreqMap, vDiagramPack[iterGrafF].m_percentSmooth); // насколько сильно (%)

						vDiagramPack[iterGrafF].CurrentSmoothFreqModifiers = smoothedFreqMap;
						vDiagramPack[iterGrafF].CurrentSourceFreqModifiers = sourceFreqMap;

						string nameCurveE;
						switch (errorCase)
						{
						case 0:
							nameCurveE = "modifiers";
							break;


						case -6:
							nameCurveE = "modifiers_noKit_noTool";
							break;


						default:
							nameCurveE = "errorCase";
							break;
						}

						vDiagramPack[iterGrafF].CurrentCurvePack = getCurveFrequrency(rectangleForCurve, vDiagramPack[iterGrafF].CurrentSmoothFreqModifiers, vDiagramPack[iterGrafF].ColorCurve, nameCurveE);

						// оформление 50-ого перцентиля
						xPosPack = vectorWindowsDiagram[iterGrafF].getSize().x * 0.01f;
						yPosPack = vectorWindowsDiagram[iterGrafF].getSize().y * 0.01f;

						xSizePack = sizeCellDefault * 3.5f;
						ySizePack = sizeCellDefault * 0.3f;

						auto infoPercentile50 = getInfoForModifiers(getPercentile(vDiagramPack[iterGrafF].CurrentSourceFreqModifiers, 50.0));
						{
							vector<variant<string, wstring>> vResultWS = { "Percentile_50" };

							GUIModdingGun percentileGuiName = [xPosPack, yPosPack, xSizePack, ySizePack, vResultWS]()
								{
									RectangleShape rectangleTemp;
									rectangleTemp.setSize({ xSizePack * 2, ySizePack });
									rectangleTemp.setPosition({ xPosPack, yPosPack });
									return GUIModdingGun(vResultWS, rectangleTemp, false, false);

								}();
							vDiagramPack[iterGrafF].m_GUIPercentile50.push_back(percentileGuiName);
>>>>>>> main

							}();

<<<<<<< HEAD
						arrDiagramPack[iterGraf].m_GUIEndMod = make_shared<GUITextAndRectangle>(endMod);

						// оформление метода
						xSizePack = defaultSizeCell * 8.0f;
						ySizePack = defaultSizeCell * 0.4f;

						vector< GUITextAndRectangle> resultGuiMethodMod;

						for (size_t i = 0; i < methodStepMod.size(); i++)
						{
							xPosPack = arrWindowsDiagram[iterGraf].getSize().x - xSizePack;
							yPosPack = ySizePack * i;

							GUITextAndRectangle GUIMethodMod = [xSizePack, ySizePack, xPosPack, yPosPack, &methodStepMod, i, priceValueDegree]()
								{
									RectangleShape rectangleTemp;
									rectangleTemp.setSize({ xSizePack, ySizePack });
									rectangleTemp.setPosition({ xPosPack, yPosPack });

									const auto& [tool, kit, skill] = methodStepMod[i].m_modifiersThisChance;

									auto cancheUp = methodStepMod[i].m_chanceUpgrade;

									float priceTool = methodStepMod[i].m_price.m_priceTool / priceValueDegree;
									float priceKit = methodStepMod[i].m_price.m_priceKit / priceValueDegree;
									float priceSkill = methodStepMod[i].m_price.m_priceSkill / priceValueDegree;

									GUITextAndRectangle result(
										{
										   to_wstring(i + 1), L">",

										   string(GunStats::characteristicNameInGunStat[methodStepMod[i].m_characteristicGunstat]),
										   format(L"{:.1f} ({:.2f}%)", methodStepMod[i].m_visualPercent , cancheUp), L"- (",
										   GunStats::Modifiers::getToolName(tool),   format(L"{:.1f}", priceTool) + LANG_TRANSLATE("kk"), L"|",
										   GunStats::Modifiers::GetKitName(kit),     format(L"{:.1f}", priceKit) + LANG_TRANSLATE("kk"), L"|",
										   GunStats::Modifiers::getSkillName(skill), format(L"{:.1f}", priceSkill) + LANG_TRANSLATE("kk"), L")"

										},
										rectangleTemp,
										false,
										false
									);

									if (cancheUp >= 100.0)
									{
										result.setColorText(Color::Green);
									}

									return result;

								}();

							resultGuiMethodMod.push_back(GUIMethodMod);

						}
						arrDiagramPack[iterGraf].m_GUIMethodModifiers = resultGuiMethodMod;

						// оформление метода

						xPosPack = arrWindowsDiagram[iterGraf].getSize().x - xSizePack;
						yPosPack = 0;

						xSizePack = defaultSizeCell * 2;
						ySizePack = defaultSizeCell * 0.4f;

						xPosPack -= xSizePack;

						GUITextAndRectangle GUIMethod = [xSizePack, ySizePack, xPosPack, yPosPack]()
=======
							for (const auto& infoItem : infoPercentile50)
							{
								xPosPack = vDiagramPack[iterGrafF].m_GUIPercentile50.begin()->getRectangle().getPosition().x;
								yPosPack = vDiagramPack[iterGrafF].m_GUIPercentile50.rbegin()->getRectangle().getPosition().y + ySizePack;

								vResultWS = { infoItem.first };

								GUIModdingGun percentileGuiInfo = [xPosPack, yPosPack, xSizePack, ySizePack, vResultWS]()
									{
										RectangleShape rectangleTemp;
										rectangleTemp.setSize({ xSizePack, ySizePack });
										rectangleTemp.setPosition({ xPosPack, yPosPack });
										return GUIModdingGun(vResultWS, rectangleTemp, false, false);

									}();

								vDiagramPack[iterGrafF].m_GUIPercentile50.push_back(percentileGuiInfo);

								percentileGuiInfo.setPositionRectangle(percentileGuiInfo.getPositionRectangle() + Vector2f(percentileGuiInfo.getRectangle().getSize().x, 0));

								vector<variant<string, wstring>> nameValInfo = { format(L"{:L}", infoItem.second) };

								if (infoItem.first == "m_allPrice")
								{
									nameValInfo.push_back(L"->");
									nameValInfo.push_back(format(L"{:L}", static_cast<float>(infoItem.second / priceValueDegree)));
									nameValInfo.push_back("kk");
								}
								else
								{
									nameValInfo.push_back("units");
								}
								percentileGuiInfo.setKeyStringAndTextWstring(nameValInfo);
								vDiagramPack[iterGrafF].m_GUIPercentile50.push_back(percentileGuiInfo);
							}
						}

						// оформление начального и конечного мода

						auto size = vDiagramPack[iterGrafF].m_GUIPercentile50.rbegin()->getRectangle().getSize();
						xSizePack = size.x * 2.3f;
						ySizePack = size.y;

						xPosPack = vDiagramPack[iterGrafF].m_GUIPercentile50.begin()->getPositionRectangle().x;
						yPosPack = vDiagramPack[iterGrafF].m_GUIPercentile50.rbegin()->getPositionRectangle().y + size.y;

						GUIModdingGun GInitialMod = [xSizePack, ySizePack, xPosPack, yPosPack, initialCharacteristic]()
							{
								RectangleShape rectangleTemp;
								rectangleTemp.setSize({ xSizePack, ySizePack });
								rectangleTemp.setPosition({ xPosPack, yPosPack });
								wstring result;

								for (const auto& ch : initialCharacteristic)
								{
									result += format(L"{:.1f}", ch) + L" |\t";
								}

								return GUIModdingGun({ "Initial_mod", L"\t\t" ,result }, rectangleTemp, false, false);

							}();

						vDiagramPack[iterGrafF].m_GUIInitialMod = make_shared<GUIModdingGun>(GInitialMod);

						yPosPack += ySizePack;

						GUIModdingGun GEndMod = [xSizePack, ySizePack, xPosPack, yPosPack, modCharacteristic]()
>>>>>>> main
							{
								RectangleShape rectangleTemp;
								rectangleTemp.setSize({ xSizePack, ySizePack });
								rectangleTemp.setPosition({ xPosPack, yPosPack });
<<<<<<< HEAD
								return GUITextAndRectangle({ "Method" }, rectangleTemp, false, false);

							}();

						arrDiagramPack[iterGraf].m_GUIMethod = make_shared<GUITextAndRectangle>(GUIMethod);

						// добавляем стрелочки для прокрутки если выходим за указанный предел
						if (resultGuiMethodMod.size() >= MethodList)
						{
							// верхняя
							xSizePack = defaultSizeCell * 0.4f;
							ySizePack = defaultSizeCell * 0.4f;
=======
								wstring result;

								for (const auto& ch : modCharacteristic)
								{
									result += format(L"{:.1f}", ch) + L" |\t";
								}

								return GUIModdingGun({ "End_mod", L"\t\t", result }, rectangleTemp, false, false);

							}();

						vDiagramPack[iterGrafF].m_GUIEndMod = make_shared<GUIModdingGun>(GEndMod);

						// оформление метода
						xSizePack = sizeCellDefault * 8.0f;
						ySizePack = sizeCellDefault * 0.4f;

						vector< GUIModdingGun> resultGuiMethodMod;

						for (size_t i = 0; i < methodMod.size(); i++)
						{
							xPosPack = vectorWindowsDiagram[iterGrafF].getSize().x - xSizePack;
							yPosPack = ySizePack * i;

							GUIModdingGun guiMethodMod = [xSizePack, ySizePack, xPosPack, yPosPack, &methodMod, i, priceValueDegree]()
								{
									RectangleShape rectangleTemp;
									rectangleTemp.setSize({ xSizePack, ySizePack });
									rectangleTemp.setPosition({ xPosPack, yPosPack });

									const auto& [tool, kit, skill] = methodMod[i].ModifiersThisChance;

									auto cancheUp = methodMod[i].chanceUpgrade;

									float pricet = methodMod[i].Price.PriceTool / priceValueDegree;
									float pricek = methodMod[i].Price.PriceKit / priceValueDegree;
									float prices = methodMod[i].Price.PriceSkill / priceValueDegree;

									GUIModdingGun result(
										{
										   to_wstring(i + 1), L">",

										   string(GunStats::mapCharacteristicNameGunStat[methodMod[i].Characteristic_Gunstat]),
										   format(L"{:.1f} ({:.2f}%)", methodMod[i].VisualPercent , cancheUp), L"- (",
										   GunStats::Modifiers::GetToolName(tool),   format(L"{:.1f}", pricet) + LANG_TRANSLATE("kk"), L"|",
										   GunStats::Modifiers::GetKitName(kit),     format(L"{:.1f}", pricek) + LANG_TRANSLATE("kk"), L"|",
										   GunStats::Modifiers::GetSkillName(skill), format(L"{:.1f}", prices) + LANG_TRANSLATE("kk"), L")"

										},
										rectangleTemp,
										false,
										false
									);

									if (cancheUp >= 100.0) { result.setColorText(Color::Green); }
									return result;

								}();
							resultGuiMethodMod.push_back(guiMethodMod);
						}
						vDiagramPack[iterGrafF].m_GUIMethodModifiers = resultGuiMethodMod;

						// оформление метода
						xPosPack = vectorWindowsDiagram[iterGrafF].getSize().x - xSizePack;
						yPosPack = 0;

						xSizePack = sizeCellDefault * 2;
						ySizePack = sizeCellDefault * 0.4f;

						xPosPack -= xSizePack;

						GUIModdingGun GuiMethod = [xSizePack, ySizePack, xPosPack, yPosPack]()
							{
								RectangleShape rectangleTemp;
								rectangleTemp.setSize({ xSizePack, ySizePack });
								rectangleTemp.setPosition({ xPosPack, yPosPack });
								return GUIModdingGun({ "Method" }, rectangleTemp, false, false);

							}();

						vDiagramPack[iterGrafF].m_GUIMethod = make_shared<GUIModdingGun>(GuiMethod);

						// добавляем стрелочки для прокрутки если выходим за указанный предел
						if (resultGuiMethodMod.size() >= methodList)
						{
							// верхняя
							xSizePack = sizeCellDefault * 0.4f;
							ySizePack = sizeCellDefault * 0.4f;
>>>>>>> main

							xPosPack = resultGuiMethodMod[1].getRectangle().getPosition().x - xSizePack;
							yPosPack = resultGuiMethodMod[1].getRectangle().getPosition().y;

<<<<<<< HEAD
							GUITextAndRectangle buttonTop = [xSizePack, ySizePack, xPosPack, yPosPack]()
=======
							GUIModdingGun Btop = [xSizePack, ySizePack, xPosPack, yPosPack]()
>>>>>>> main
								{
									RectangleShape rectangleTemp;
									rectangleTemp.setSize({ xSizePack, ySizePack });
									rectangleTemp.setPosition({ xPosPack, yPosPack });


<<<<<<< HEAD
									return GUITextAndRectangle({ L"↑" }, rectangleTemp, false, false);

								}();
							arrDiagramPack[iterGraf].m_GUIButtonTop = make_shared<GUITextAndRectangle>(buttonTop);

							// нижняя
							xSizePack = defaultSizeCell * 0.4f;
							ySizePack = defaultSizeCell * 0.4f;

							xPosPack = static_cast<float>(resultGuiMethodMod[MethodList - 1].getRectangle().getPosition().x - xSizePack);
							yPosPack = static_cast<float>(resultGuiMethodMod[MethodList - 1].getRectangle().getPosition().y);

							GUITextAndRectangle buttonDown = [xSizePack, ySizePack, xPosPack, yPosPack]()
								{
									RectangleShape RectangleTemp;
									RectangleTemp.setSize({ xSizePack, ySizePack });
									RectangleTemp.setPosition({ xPosPack, yPosPack });


									return GUITextAndRectangle({ L"↓" }, RectangleTemp, false, false);

								}();
							arrDiagramPack[iterGraf].m_GUIButtonDown = make_shared<GUITextAndRectangle>(buttonDown);
						}

						arrDiagramPack[iterGraf].m_initialIndexMethod = 0;
=======
									return GUIModdingGun({ L"↑" }, rectangleTemp, false, false);

								}();
							vDiagramPack[iterGrafF].m_GUIButtonTop = make_shared<GUIModdingGun>(Btop);

							// нижняя
							xSizePack = sizeCellDefault * 0.4f;
							ySizePack = sizeCellDefault * 0.4f;

							xPosPack = resultGuiMethodMod[methodList - 1].getRectangle().getPosition().x - xSizePack;
							yPosPack = resultGuiMethodMod[methodList - 1].getRectangle().getPosition().y;

							GUIModdingGun Bdown = [xSizePack, ySizePack, xPosPack, yPosPack]()
								{
									RectangleShape rectangleTemp;
									rectangleTemp.setSize({ xSizePack, ySizePack });
									rectangleTemp.setPosition({ xPosPack, yPosPack });


									return GUIModdingGun({ L"↓" }, rectangleTemp, false, false);

								}();
							vDiagramPack[iterGrafF].m_GUIButtonDown = make_shared<GUIModdingGun>(Bdown);
						}

						vDiagramPack[iterGrafF].m_initialIndexMethod = 0;
>>>>>>> main

						// сглаживание графика
						vector<pair<wstring, float>> allocationBt =
						{
							{L"name",0.55f},
							{L"-",   0.12f},
							{L"val", 0.30f},
							{L"+",   0.12f}
						};

						int itbt1 = 0;
						for (const auto& [wstrbt, SizeP] : allocationBt)
						{
<<<<<<< HEAD
							GUITextAndRectangle GUI_BinBt = [&wstrbt, &SizeP, &arrDiagramPack, iterGraf, defaultSizeCell, itbt1]()
								{
									RectangleShape binBt;
									binBt.setSize(Vector2f((defaultSizeCell * 3.f) * SizeP, defaultSizeCell * 0.35f));

									auto posR = arrDiagramPack[iterGraf].m_currentCurvePack.m_mainRectangleCurve.getPosition();
									auto SizeR = arrDiagramPack[iterGraf].m_currentCurvePack.m_mainRectangleCurve.getSize();
									if (itbt1 == 1)
									{
										posR = arrDiagramPack[iterGraf].m_GUIPercentBinBt.rbegin()->getPositionRectangle();
										SizeR = arrDiagramPack[iterGraf].m_GUIPercentBinBt.rbegin()->getRectangle().getSize();
=======
							GUIModdingGun GUIBinBt = [&wstrbt, &SizeP, &vDiagramPack, iterGrafF, sizeCellDefault, itbt1]()
								{
									RectangleShape binBt;
									binBt.setSize(Vector2f((sizeCellDefault * 3.f) * SizeP, sizeCellDefault * 0.35f));

									auto posR = vDiagramPack[iterGrafF].CurrentCurvePack.m_mainRectangleCurve.getPosition();
									auto SizeR = vDiagramPack[iterGrafF].CurrentCurvePack.m_mainRectangleCurve.getSize();
									if (itbt1 == 1)
									{
										posR = vDiagramPack[iterGrafF].m_GUIPercentBinBt.rbegin()->getPositionRectangle();
										SizeR = vDiagramPack[iterGrafF].m_GUIPercentBinBt.rbegin()->getRectangle().getSize();
>>>>>>> main
										binBt.setPosition(Vector2f(posR.x, posR.y + SizeR.y));

									}
									else if (itbt1)
									{
<<<<<<< HEAD
										posR = arrDiagramPack[iterGraf].m_GUIPercentBinBt.rbegin()->getPositionRectangle();
										SizeR = arrDiagramPack[iterGraf].m_GUIPercentBinBt.rbegin()->getRectangle().getSize();
=======
										posR = vDiagramPack[iterGrafF].m_GUIPercentBinBt.rbegin()->getPositionRectangle();
										SizeR = vDiagramPack[iterGrafF].m_GUIPercentBinBt.rbegin()->getRectangle().getSize();
>>>>>>> main
										binBt.setPosition(Vector2f(posR.x + SizeR.x, posR.y));
									}
									else
									{
										binBt.setPosition(Vector2f(posR.x + SizeR.x, posR.y));
									}

									if (wstrbt == L"val")
									{
<<<<<<< HEAD
										return GUITextAndRectangle({ format(L"{:.1f}", arrDiagramPack[iterGraf].m_percentBin) }, binBt, false, false);
									}
									else if (wstrbt == L"name")
									{
										return GUITextAndRectangle({ "PercentBin" }, binBt, false, false);
									}
									return GUITextAndRectangle({ wstrbt }, binBt, true, false);
								}();
							arrDiagramPack[iterGraf].m_GUIPercentBinBt.push_back(GUI_BinBt);
=======
										return GUIModdingGun({ format(L"{:.1f}", vDiagramPack[iterGrafF].m_percentBin) }, binBt, false, false);
									}
									else if (wstrbt == L"name")
									{
										return GUIModdingGun({ "m_percentBin" }, binBt, false, false);
									}
									return GUIModdingGun({ wstrbt }, binBt, true, false);

								}();

							vDiagramPack[iterGrafF].m_GUIPercentBinBt.push_back(GUIBinBt);
>>>>>>> main
							itbt1++;
						}

						itbt1 = 0;
						for (const auto& [wstrbt, SizeP] : allocationBt)
						{
<<<<<<< HEAD
							GUITextAndRectangle GUISmoothBt = [&wstrbt, &SizeP, &arrDiagramPack, iterGraf, defaultSizeCell, itbt1]()
								{
									RectangleShape smBt;
									smBt.setSize(Vector2f((defaultSizeCell * 3.f) * SizeP, defaultSizeCell * 0.35f));

									auto posR = arrDiagramPack[iterGraf].m_currentCurvePack.m_mainRectangleCurve.getPosition() + Vector2f(0, defaultSizeCell * 0.35f * 3.f);
									auto SizeR = arrDiagramPack[iterGraf].m_currentCurvePack.m_mainRectangleCurve.getSize();
									if (itbt1 == 1)
									{
										posR = arrDiagramPack[iterGraf].m_GUIPercentSmoothBt.rbegin()->getPositionRectangle();
										SizeR = arrDiagramPack[iterGraf].m_GUIPercentSmoothBt.rbegin()->getRectangle().getSize();
										smBt.setPosition(Vector2f(posR.x, posR.y + SizeR.y));
=======
							GUIModdingGun GUISmoothBt = [&wstrbt, &SizeP, &vDiagramPack, iterGrafF, sizeCellDefault, itbt1]()
								{
									RectangleShape SmBt;
									SmBt.setSize(Vector2f((sizeCellDefault * 3.f) * SizeP, sizeCellDefault * 0.35f));

									auto posR = vDiagramPack[iterGrafF].CurrentCurvePack.m_mainRectangleCurve.getPosition() + Vector2f(0, sizeCellDefault * 0.35f * 3.f);
									auto SizeR = vDiagramPack[iterGrafF].CurrentCurvePack.m_mainRectangleCurve.getSize();
									if (itbt1 == 1)
									{
										posR = vDiagramPack[iterGrafF].m_GUIPercentSmoothBt.rbegin()->getPositionRectangle();
										SizeR = vDiagramPack[iterGrafF].m_GUIPercentSmoothBt.rbegin()->getRectangle().getSize();
										SmBt.setPosition(Vector2f(posR.x, posR.y + SizeR.y));
>>>>>>> main

									}
									else if (itbt1)
									{
<<<<<<< HEAD
										posR = arrDiagramPack[iterGraf].m_GUIPercentSmoothBt.rbegin()->getPositionRectangle();
										SizeR = arrDiagramPack[iterGraf].m_GUIPercentSmoothBt.rbegin()->getRectangle().getSize();
										smBt.setPosition(Vector2f(posR.x + SizeR.x, posR.y));
									}
									else
									{
										smBt.setPosition(Vector2f(posR.x + SizeR.x, posR.y));
=======
										posR = vDiagramPack[iterGrafF].m_GUIPercentSmoothBt.rbegin()->getPositionRectangle();
										SizeR = vDiagramPack[iterGrafF].m_GUIPercentSmoothBt.rbegin()->getRectangle().getSize();
										SmBt.setPosition(Vector2f(posR.x + SizeR.x, posR.y));
									}
									else
									{
										SmBt.setPosition(Vector2f(posR.x + SizeR.x, posR.y));
>>>>>>> main
									}

									if (wstrbt == L"val")
									{
<<<<<<< HEAD
										return GUITextAndRectangle({ format(L"{:.2f}", arrDiagramPack[iterGraf].m_percentSmooth) }, smBt, false, false);
									}
									else if (wstrbt == L"name")
									{
										return GUITextAndRectangle({ "PercentSmooth" }, smBt, false, false);
									}
									return GUITextAndRectangle({ wstrbt }, smBt, true, false);
								}();
							arrDiagramPack[iterGraf].m_GUIPercentSmoothBt.push_back(GUISmoothBt);
							itbt1++;
						}
						// end
						diagramBool[iterGraf] = true;
						OUTPUT_LOG("Main -> Graph setting is finished: " + to_string(iterGraf));
					}

					arrWindowsDiagram[iterGraf].clear(Color::White);
					arrWindowsDiagram[iterGraf].draw(backgroundDiagram);

					for (auto& GUI : arrDiagramPack[iterGraf].m_GUIPercentBinBt)
					{
						arrWindowsDiagram[iterGraf].draw(GUI.getRectangle());
						arrWindowsDiagram[iterGraf].draw(GUI.getText());
					}

					for (auto& GUI : arrDiagramPack[iterGraf].m_GUIPercentSmoothBt)
					{
						arrWindowsDiagram[iterGraf].draw(GUI.getRectangle());
						arrWindowsDiagram[iterGraf].draw(GUI.getText());
					}

					for (auto& GUI : arrDiagramPack[iterGraf].m_GUIPercentile50)
					{
						arrWindowsDiagram[iterGraf].draw(GUI.getRectangle());
						arrWindowsDiagram[iterGraf].draw(GUI.getText());
					}

					arrWindowsDiagram[iterGraf].draw(arrDiagramPack[iterGraf].m_GUIMethod.get()->getRectangle());
					arrWindowsDiagram[iterGraf].draw(arrDiagramPack[iterGraf].m_GUIMethod.get()->getText());

					for (size_t it = arrDiagramPack[iterGraf].m_initialIndexMethod; it < arrDiagramPack[iterGraf].m_GUIMethodModifiers.size(); it++)
					{
						arrWindowsDiagram[iterGraf].draw(arrDiagramPack[iterGraf].m_GUIMethodModifiers[it].getRectangle());
						arrWindowsDiagram[iterGraf].draw(arrDiagramPack[iterGraf].m_GUIMethodModifiers[it].getText());
					}

					if (arrDiagramPack[iterGraf].m_GUIButtonTop.get() != nullptr && arrDiagramPack[iterGraf].m_GUIButtonDown.get() != nullptr)
					{
						arrWindowsDiagram[iterGraf].draw(arrDiagramPack[iterGraf].m_GUIButtonTop.get()->getRectangle());
						arrWindowsDiagram[iterGraf].draw(arrDiagramPack[iterGraf].m_GUIButtonTop.get()->getText());

						arrWindowsDiagram[iterGraf].draw(arrDiagramPack[iterGraf].m_GUIButtonDown.get()->getRectangle());
						arrWindowsDiagram[iterGraf].draw(arrDiagramPack[iterGraf].m_GUIButtonDown.get()->getText());
					}

					if (arrDiagramPack[iterGraf].m_GUIInitialMod.get() != nullptr && arrDiagramPack[iterGraf].m_GUIEndMod.get() != nullptr)
					{
						arrWindowsDiagram[iterGraf].draw(arrDiagramPack[iterGraf].m_GUIInitialMod.get()->getRectangle());
						arrWindowsDiagram[iterGraf].draw(arrDiagramPack[iterGraf].m_GUIInitialMod.get()->getText());

						arrWindowsDiagram[iterGraf].draw(arrDiagramPack[iterGraf].m_GUIEndMod.get()->getRectangle());
						arrWindowsDiagram[iterGraf].draw(arrDiagramPack[iterGraf].m_GUIEndMod.get()->getText());
					}

					arrWindowsDiagram[iterGraf].draw(arrDiagramPack[iterGraf].m_currentCurvePack.m_mainRectangleCurve);

					for (size_t it2 = 0; it2 < arrDiagramPack[iterGraf].m_currentCurvePack.m_arrayGUITextCurve.size(); it2++)
					{
						arrWindowsDiagram[iterGraf].draw(arrDiagramPack[iterGraf].m_currentCurvePack.m_arrayGUITextCurve[it2].getRectangle());
						arrWindowsDiagram[iterGraf].draw(arrDiagramPack[iterGraf].m_currentCurvePack.m_arrayGUITextCurve[it2].getText());
					}
					arrWindowsDiagram[iterGraf].draw(arrDiagramPack[iterGraf].m_currentCurvePack.m_curve);

					/// если мы навелись на кривую ->  выведем инфу о колличестваx на текущей позиции
					if (arrDiagramPack[iterGraf].m_currentCurvePack.m_mainRectangleCurve.getGlobalBounds().contains(mouseWorldPosForGraf))
					{
						// вертикальная линия
						auto posLineTop = arrDiagramPack[iterGraf].m_currentCurvePack.m_mainRectangleCurve.getPosition();
						auto posLineDown = arrDiagramPack[iterGraf].m_currentCurvePack.m_mainRectangleCurve.getPosition() + Vector2f(0, arrDiagramPack[iterGraf].m_currentCurvePack.m_mainRectangleCurve.getSize().y);

						// поправка линии на позицию мышки
						float PosMouseRelativeRectangleCure = mouseWorldPosForGraf.x - arrDiagramPack[iterGraf].m_currentCurvePack.m_mainRectangleCurve.getPosition().x;
						posLineTop += Vector2f(PosMouseRelativeRectangleCure, 0);
						posLineDown += Vector2f(PosMouseRelativeRectangleCure, 0);
=======
										return GUIModdingGun({ format(L"{:.2f}", vDiagramPack[iterGrafF].m_percentSmooth) }, SmBt, false, false);
									}
									else if (wstrbt == L"name")
									{
										return GUIModdingGun({ "m_percentSmooth" }, SmBt, false, false);
									}
									return GUIModdingGun({ wstrbt }, SmBt, true, false);

								}();

							vDiagramPack[iterGrafF].m_GUIPercentSmoothBt.push_back(GUISmoothBt);
							itbt1++;
						}
						diagramBool[iterGrafF] = true;
						OUTPUT_LOG("Main -> Graph setting is finished: " + to_string(iterGrafF));
					}

					vectorWindowsDiagram[iterGrafF].clear(Color::White);
					vectorWindowsDiagram[iterGrafF].draw(backgroundDiagram);

					for (auto& GUI : vDiagramPack[iterGrafF].m_GUIPercentBinBt)
					{
						vectorWindowsDiagram[iterGrafF].draw(GUI.getRectangle());
						vectorWindowsDiagram[iterGrafF].draw(GUI.getText());
					}

					for (auto& GUI : vDiagramPack[iterGrafF].m_GUIPercentSmoothBt)
					{
						vectorWindowsDiagram[iterGrafF].draw(GUI.getRectangle());
						vectorWindowsDiagram[iterGrafF].draw(GUI.getText());
					}

					for (auto& GUI : vDiagramPack[iterGrafF].m_GUIPercentile50)
					{
						vectorWindowsDiagram[iterGrafF].draw(GUI.getRectangle());
						vectorWindowsDiagram[iterGrafF].draw(GUI.getText());
					}

					vectorWindowsDiagram[iterGrafF].draw(vDiagramPack[iterGrafF].m_GUIMethod.get()->getRectangle());
					vectorWindowsDiagram[iterGrafF].draw(vDiagramPack[iterGrafF].m_GUIMethod.get()->getText());

					for (size_t it = vDiagramPack[iterGrafF].m_initialIndexMethod; it < vDiagramPack[iterGrafF].m_GUIMethodModifiers.size(); it++)
					{
						vectorWindowsDiagram[iterGrafF].draw(vDiagramPack[iterGrafF].m_GUIMethodModifiers[it].getRectangle());
						vectorWindowsDiagram[iterGrafF].draw(vDiagramPack[iterGrafF].m_GUIMethodModifiers[it].getText());
					}

					if (vDiagramPack[iterGrafF].m_GUIButtonTop.get() != nullptr && vDiagramPack[iterGrafF].m_GUIButtonDown.get() != nullptr)
					{
						vectorWindowsDiagram[iterGrafF].draw(vDiagramPack[iterGrafF].m_GUIButtonTop.get()->getRectangle());
						vectorWindowsDiagram[iterGrafF].draw(vDiagramPack[iterGrafF].m_GUIButtonTop.get()->getText());

						vectorWindowsDiagram[iterGrafF].draw(vDiagramPack[iterGrafF].m_GUIButtonDown.get()->getRectangle());
						vectorWindowsDiagram[iterGrafF].draw(vDiagramPack[iterGrafF].m_GUIButtonDown.get()->getText());
					}

					if (vDiagramPack[iterGrafF].m_GUIInitialMod.get() != nullptr && vDiagramPack[iterGrafF].m_GUIEndMod.get() != nullptr)
					{
						vectorWindowsDiagram[iterGrafF].draw(vDiagramPack[iterGrafF].m_GUIInitialMod.get()->getRectangle());
						vectorWindowsDiagram[iterGrafF].draw(vDiagramPack[iterGrafF].m_GUIInitialMod.get()->getText());

						vectorWindowsDiagram[iterGrafF].draw(vDiagramPack[iterGrafF].m_GUIEndMod.get()->getRectangle());
						vectorWindowsDiagram[iterGrafF].draw(vDiagramPack[iterGrafF].m_GUIEndMod.get()->getText());
					}

					vectorWindowsDiagram[iterGrafF].draw(vDiagramPack[iterGrafF].CurrentCurvePack.m_mainRectangleCurve);

					for (size_t it2 = 0; it2 < vDiagramPack[iterGrafF].CurrentCurvePack.m_vGUITextCurve.size(); it2++)
					{
						vectorWindowsDiagram[iterGrafF].draw(vDiagramPack[iterGrafF].CurrentCurvePack.m_vGUITextCurve[it2].getRectangle());
						vectorWindowsDiagram[iterGrafF].draw(vDiagramPack[iterGrafF].CurrentCurvePack.m_vGUITextCurve[it2].getText());
					}
					vectorWindowsDiagram[iterGrafF].draw(vDiagramPack[iterGrafF].CurrentCurvePack.m_curve);

					/// если мы навелись на кривую ->  выведем инфу о колличестваx на текущей позиции
					if (vDiagramPack[iterGrafF].CurrentCurvePack.m_mainRectangleCurve.getGlobalBounds().contains(mouseWorldPosForGraf))
					{
						// вертикальная линия
						auto posLineTop = vDiagramPack[iterGrafF].CurrentCurvePack.m_mainRectangleCurve.getPosition();
						auto posLineDown = vDiagramPack[iterGrafF].CurrentCurvePack.m_mainRectangleCurve.getPosition() + Vector2f(0, vDiagramPack[iterGrafF].CurrentCurvePack.m_mainRectangleCurve.getSize().y);

						// поправка линии на позицию мышки
						float posMouseRelativeRectangleCure = mouseWorldPosForGraf.x - vDiagramPack[iterGrafF].CurrentCurvePack.m_mainRectangleCurve.getPosition().x;
						posLineTop += Vector2f(posMouseRelativeRectangleCure, 0);
						posLineDown += Vector2f(posMouseRelativeRectangleCure, 0);
>>>>>>> main

						//делаем прямую y - 0
						VertexArray verticalLine(PrimitiveType::Lines, 2);

						verticalLine[0].position = posLineTop;
						verticalLine[0].color = Color::Black;

						verticalLine[1].position = posLineDown;
						verticalLine[1].color = Color::Black;

<<<<<<< HEAD
						arrWindowsDiagram[iterGraf].draw(verticalLine);

						// перебираем отрезки кривой для вывода информации
						size_t countSectionCurve = arrDiagramPack[iterGraf].m_currentCurvePack.m_curve.getVertexCount();
=======
						vectorWindowsDiagram[iterGrafF].draw(verticalLine);

						// перебираем отрезки кривой для вывода информации
						size_t countSectionCurve = vDiagramPack[iterGrafF].CurrentCurvePack.m_curve.getVertexCount();
>>>>>>> main
						if (countSectionCurve == 0)
						{
							countSectionCurve = 1;
						}
<<<<<<< HEAD
						auto lengthCurve = arrDiagramPack[iterGraf].m_currentCurvePack.m_mainRectangleCurve.getSize().x;

						auto xPosRectangleDP = arrDiagramPack[iterGraf].m_currentCurvePack.m_mainRectangleCurve.getPosition().x;
						auto lengthOneSectionCurve = lengthCurve / countSectionCurve;
						auto xPosLineDiagram = posLineTop.x; // || posLineDown.x
=======

						auto LengthCurve = vDiagramPack[iterGrafF].CurrentCurvePack.m_mainRectangleCurve.getSize().x;

						auto sPosRectangle = vDiagramPack[iterGrafF].CurrentCurvePack.m_mainRectangleCurve.getPosition().x;
						auto lengthOneSectionCurve = LengthCurve / countSectionCurve;
						auto xPosLine = posLineTop.x; // || posLineDown.x
>>>>>>> main

						//имя инструмента \ кол-во
						vector<pair<string, unsigned long long>> infoCountModifiers;

						// перебираем все отрезки
						unsigned itMax = 0;
<<<<<<< HEAD
						float currentFreq = 0.f;
=======
						float curFreq = 0.f;
>>>>>>> main

						for (itMax = 0; itMax < countSectionCurve; itMax++)
						{
							// делаем допуск
							float epsilon = 0.01f;
<<<<<<< HEAD
							float start = xPosRectangleDP + lengthOneSectionCurve * itMax;
							float end = xPosRectangleDP + lengthOneSectionCurve * (itMax + 1);

							// проверяем позиция x вертикальной линии входит в секцию кривой?
							if (xPosLineDiagram >= start - epsilon && xPosLineDiagram < end + epsilon)
							{
								auto iteratorMap = arrDiagramPack[iterGraf].m_currentSmoothFreqModifiers.begin();
								advance(iteratorMap, itMax);

								CountModifiers modifiersM = iteratorMap->first;

								currentFreq = iteratorMap->second;

								if (iteratorMap != arrDiagramPack[iterGraf].m_currentSmoothFreqModifiers.end())
								{
									modifiersM = iteratorMap->first;
									currentFreq = iteratorMap->second;
								}
								infoCountModifiers = GetInfoForModifiers(modifiersM);
=======
							float start = sPosRectangle + lengthOneSectionCurve * itMax;
							float end = sPosRectangle + lengthOneSectionCurve * (itMax + 1);

							// проверяем позиция x вертикальной линии входит в секцию кривой?
							if (xPosLine >= start - epsilon && xPosLine < end + epsilon)
							{
								auto iteratorMap = vDiagramPack[iterGrafF].CurrentSmoothFreqModifiers.begin();
								advance(iteratorMap, itMax);

								CountModifiers modifiersM = iteratorMap->first;
								curFreq = iteratorMap->second;

								if (iteratorMap != vDiagramPack[iterGrafF].CurrentSmoothFreqModifiers.end())
								{
									modifiersM = iteratorMap->first;
									curFreq = iteratorMap->second;
								}

								infoCountModifiers = getInfoForModifiers(modifiersM);
>>>>>>> main
								break;
							}
						}

						// информация о секции
<<<<<<< HEAD
						Text nameModifier(ThemeGui::currentFontGUI);
						nameModifier.setFillColor(Color::Black);
						nameModifier.setCharacterSize(static_cast<unsigned>(ThemeGui::characterSizeGUI * 0.9f));

						RectangleShape rectangleForGraf;
						rectangleForGraf.setSize(Vector2f(defaultSizeCell * 2.7f, defaultSizeCell * 0.23f * static_cast<float>(infoCountModifiers.size() + 1)));
						rectangleForGraf.setPosition({ posLineTop + Vector2f(defaultSizeCell * 0.3f, 0) });
=======
						Text nameModifier(ThemeGui::currentFont);
						nameModifier.setFillColor(Color::Black);
						nameModifier.setCharacterSize(static_cast<unsigned>(ThemeGui::characterSize * 0.9f));

						RectangleShape rectangleForGraf;
						rectangleForGraf.setSize(Vector2f(sizeCellDefault * 2.7f, sizeCellDefault * 0.23f * static_cast<float>(infoCountModifiers.size() + 1)));
						rectangleForGraf.setPosition({ posLineTop + Vector2f(sizeCellDefault * 0.3f, 0) });
>>>>>>> main
						rectangleForGraf.setFillColor(Color(230, 230, 230, 200));
						rectangleForGraf.setOutlineColor(Color::Black);
						rectangleForGraf.setOutlineThickness(-1);

<<<<<<< HEAD
						arrWindowsDiagram[iterGraf].draw(rectangleForGraf);
=======
						vectorWindowsDiagram[iterGrafF].draw(rectangleForGraf);
>>>>>>> main

						size_t it4;
						for (it4 = 0; it4 < infoCountModifiers.size(); it4++)
						{
<<<<<<< HEAD
							auto pos1 = posLineTop + Vector2f(defaultSizeCell * 0.4f, defaultSizeCell * 0.2f * it4);
=======
							auto pos1 = posLineTop + Vector2f(sizeCellDefault * 0.4f, sizeCellDefault * 0.2f * it4);
>>>>>>> main
							nameModifier.setPosition(pos1);

							if (it4 == infoCountModifiers.size() - 1)
							{
								nameModifier.setString(format(L"{:L}", static_cast<float>(infoCountModifiers[it4].second / 1'000'000)) + LANG_TRANSLATE("kk"));
							}
							else
							{
								nameModifier.setString(to_wstring(infoCountModifiers[it4].second));
							}
<<<<<<< HEAD
							arrWindowsDiagram[iterGraf].draw(nameModifier);

							auto pos2 = posLineTop + Vector2f(defaultSizeCell * 0.85f, defaultSizeCell * 0.2f * it4);
=======
							vectorWindowsDiagram[iterGrafF].draw(nameModifier);
							auto pos2 = posLineTop + Vector2f(sizeCellDefault * 0.85f, sizeCellDefault * 0.2f * it4);
>>>>>>> main

							nameModifier.setPosition(pos2);
							nameModifier.setString(L" - " + LANG_TRANSLATE(infoCountModifiers[it4].first));

<<<<<<< HEAD
							arrWindowsDiagram[iterGraf].draw(nameModifier);
						}

						nameModifier.setPosition(posLineTop + Vector2f(defaultSizeCell * 0.4f, defaultSizeCell * 0.2f * it4));
						nameModifier.setString(LANG_TRANSLATE("Section") + to_wstring(itMax + 1) + L" | " + LANG_TRANSLATE("Freq") + format(L"{:.2f}", currentFreq));

						arrWindowsDiagram[iterGraf].draw(nameModifier);
					}

					arrWindowsDiagram[iterGraf].display();

					// возврат к дефолт цвету
					if (itWindow % 50 == 10)
					{
						for (auto& button : arrDiagramPack[iterGraf].m_GUIPercentBinBt)
						{
							button.setAsPressedButton();
						}
						for (auto& button : arrDiagramPack[iterGraf].m_GUIPercentSmoothBt)
=======
							vectorWindowsDiagram[iterGrafF].draw(nameModifier);
						}
						nameModifier.setPosition(posLineTop + Vector2f(sizeCellDefault * 0.4f, sizeCellDefault * 0.2f * it4));
						nameModifier.setString(LANG_TRANSLATE("Section") + to_wstring(itMax + 1) + L" | " + LANG_TRANSLATE("Freq") + format(L"{:.2f}", curFreq));

						vectorWindowsDiagram[iterGrafF].draw(nameModifier);
					}
					vectorWindowsDiagram[iterGrafF].display();

					// возврат к дефолт цвету
					if (itW % 50 == 10)
					{
						for (auto& button : vDiagramPack[iterGrafF].m_GUIPercentBinBt)
						{
							button.setAsPressedButton();
						}
						for (auto& button : vDiagramPack[iterGrafF].m_GUIPercentSmoothBt)
>>>>>>> main
						{
							button.setAsPressedButton();
						}
					}
				}
			}
		}

		// возврат к дефолт цвету
<<<<<<< HEAD
		if (itWindow % 50 == 10)
=======
		if (itW % 50 == 10)
>>>>>>> main
		{
			calculation.setAsPressedButton();
			shapeTargetIter.setAsPressedButton();
			buttonGeneralModApplyStat.setAsPressedButton();
			shapeGeneralModReset.setAsPressedButton();
			shapeGeneralModBack.setAsPressedButton();

			if (!pressedApplyAsMod) { shapeGeneralModApplyStatInitial.setAsPressedButton(); }

			/// стоимость
<<<<<<< HEAD
			for (auto& button : arrToolPrice)
			{
				button.setAsPressedButton();
			}
			for (auto& button : arrKitPrice)
=======
			for (auto& button : toolPriceAll)
			{
				button.setAsPressedButton();
			}
			for (auto& button : kitPrice)
>>>>>>> main
			{
				button.setAsPressedButton();
			}
		}
	}
	return 0;
}

<<<<<<< HEAD
//-----------------------------------------------------------------------------------------------------------------------

vector<pair<string, unsigned long long>> GetInfoForModifiers(const CountModifiers& modifiers)
=======
//-------------------------------------------------------------------------------------------------------------------

vector<pair<string, unsigned long long>> getInfoForModifiers(const CountModifiers& modifiers)
>>>>>>> main
{
	vector<pair<string, unsigned long long>> result;
	// выводим информацию о 
	// CountModifiers

	for (const auto& [currentTool, countTool] : modifiers.m_countTool)
	{
		if (countTool) // != 0
		{
			if (currentTool != GunStats::Modifiers::ToolType::noTool)
			{
<<<<<<< HEAD
				result.emplace_back(GunStats::Modifiers::getToolName(currentTool), countTool);
=======
				result.emplace_back(GunStats::Modifiers::GetToolName(currentTool), countTool);
>>>>>>> main
			}
		}
	}
	for (const auto& [currentkit, countKit] : modifiers.m_countKit)
	{
		if (countKit) // != 0
		{
			if (currentkit != GunStats::Modifiers::KitType::noKit)
			{
				result.emplace_back(GunStats::Modifiers::GetKitName(currentkit), countKit);
			}
		}
	}

	for (const auto& [currentSkill, countSkill] : modifiers.m_countSkill)
	{
		if (countSkill) // != 0 
		{
			// не учитываем
<<<<<<< HEAD
			if (currentSkill != GunStats::Modifiers::SkillType::playerMasterLvl1 &&
				currentSkill != GunStats::Modifiers::SkillType::playerMasterLvl2 &&
				currentSkill != GunStats::Modifiers::SkillType::playerMasterLvl3 &&
				currentSkill != GunStats::Modifiers::SkillType::playerMasterLvl4 &&
				currentSkill != GunStats::Modifiers::SkillType::playerMasterLvl5 &&
				currentSkill != GunStats::Modifiers::SkillType::playerNoSkill
				)
			{
				result.emplace_back(GunStats::Modifiers::getSkillName(currentSkill), countSkill);
			}
		}
	}
	result.emplace_back("AllPrice", modifiers.m_allPrice);
	return result;
}

//-----------------------------------------------------------------------------------------------------------------------

void editShaheUnits(std::vector<GeneralStat>& generalModOption, CharacteristicGun& currentCharacteristicGun, std::vector<int> initialPosition, std::vector<int> modPosition)
{
	auto defaultPosition = currentCharacteristicGun.getDefaultPosition();
	moveElement(defaultPosition, 4, 2);

	auto maxPosition = currentCharacteristicGun.getMaxPositionCharacteristic();
	moveElement(maxPosition, 4, 2);

	auto current = currentCharacteristicGun.getCurrentPosition();
	moveElement(current, 4, 2);

	for (size_t it1 = 0; it1 < generalModOption.size(); it1++)
	{
		auto& units = generalModOption[it1].m_shaheUnitsInCharacteristic;

		for (size_t it2 = 0; it2 < units.size(); it2++)
		{
			
			if (current[it1] > it2 && it2 < defaultPosition[it1]) //отрисовали  текущие 
			{
				units[it2].setFillColor(Color(14, 88, 28));
			}			
			else if (initialPosition[it1] > it2 && it2 < current[it1]) //отрисовали  начальные мод статы 
			{
				units[it2].setFillColor(Color(13, 38, 13));
			}			
			else if (it2 < modPosition[it1]) //отрисовали  мод статы
			{
				units[it2].setFillColor(Color::Green);
			}			
			else if (it2 < defaultPosition[it1]) // отрисовали  недостающие до дефолт статов
			{
				units[it2].setFillColor(Color(128, 128, 128));
			}			
			else if (it2 < maxPosition[it1]) //отрисовали возможные до макс
			{
				units[it2].setFillColor(Color(66, 66, 66));
			}			
			else // отрисовали невозможные
			{
				units[it2].setFillColor(Color(30, 30, 30));
=======
			if (currentSkill != GunStats::Modifiers::SkillType::playerMaster1 &&
				currentSkill != GunStats::Modifiers::SkillType::playerMaster2 &&
				currentSkill != GunStats::Modifiers::SkillType::playerMaster3 &&
				currentSkill != GunStats::Modifiers::SkillType::playerMaster4 &&
				currentSkill != GunStats::Modifiers::SkillType::playerMaster5 &&
				currentSkill != GunStats::Modifiers::SkillType::playerNoSkill
				)
			{
				result.emplace_back(GunStats::Modifiers::GetSkillName(currentSkill), countSkill);
			}
		}
	}
	result.emplace_back("m_allPrice", modifiers.m_allPrice);

	return result;
}

//-------------------------------------------------------------------------------------------------------------------

void editShaheUnits(vector<GeneralStat>& GeneralModOption, CharacteristicGun& currentCharacteristicGun, vector<int> initialPosition, vector<int> modPosition)
{
	auto DefaultPosition = currentCharacteristicGun.getDefaultPosition();
	moveElement(DefaultPosition, 4, 2);

	auto MaxPosition = currentCharacteristicGun.getMaxPositionCharacteristic();
	moveElement(MaxPosition, 4, 2);

	auto Current = currentCharacteristicGun.getCurrentPosition();
	moveElement(Current, 4, 2);


	for (int it1 = 0; it1 < GeneralModOption.size(); it1++)
	{
		for (int it2 = 0; it2 < GeneralModOption[it1].m_shaheUnitsInCharacteristic.size(); it2++)
		{
			//отрисовали  текущие 
			if (Current[it1] > it2 && it2 < DefaultPosition[it1])
			{
				GeneralModOption[it1].m_shaheUnitsInCharacteristic[it2].setFillColor(Color(14, 88, 28));
			}

			//отрисовали  начальные мод статы 
			else if (initialPosition[it1] > it2 && it2 < Current[it1])
			{
				GeneralModOption[it1].m_shaheUnitsInCharacteristic[it2].setFillColor(Color(13, 38, 13));
			}

			//отрисовали  мод статы
			else if (it2 < modPosition[it1])
			{
				GeneralModOption[it1].m_shaheUnitsInCharacteristic[it2].setFillColor(Color::Green);
			}

			// отрисовали  недостающие до дефолт статов
			else if (it2 < DefaultPosition[it1])
			{
				GeneralModOption[it1].m_shaheUnitsInCharacteristic[it2].setFillColor(Color(128, 128, 128));
			}

			//отрисовали возможные до макс
			else if (it2 < MaxPosition[it1])
			{
				GeneralModOption[it1].m_shaheUnitsInCharacteristic[it2].setFillColor(Color(66, 66, 66));
			}

			// отрисовали невозможные
			else
			{
				GeneralModOption[it1].m_shaheUnitsInCharacteristic[it2].setFillColor(Color(30, 30, 30));
>>>>>>> main
			}
		}
	}
}

<<<<<<< HEAD
//-----------------------------------------------------------------------------------------------------------------------

bool setChahceUpgrade(const CharacteristicGun& сurrentCharacteristicGun, const SelectModifiers& сurrentModifiers, GUITextAndRectangle& GUIChance, size_t it)
{
	GUIChance.resetColorText();

	if (сurrentCharacteristicGun.isEmpty())
=======
//-------------------------------------------------------------------------------------------------------------------

bool setChahceUpgrade(const CharacteristicGun& currentCharacteristicGun, const Select_Modifiers& currentModifiers, GUIModdingGun& GUIChance, size_t it)
{
	GUIChance.resetColorText();

	if (currentCharacteristicGun.is_Empty())
>>>>>>> main
	{
		return false;
	}

	auto tempChance = calculateSuccessChance(
		сurrentCharacteristicGun.getChanceForNextStat(GunStats::transIndexToIndexGunStat[it]),

<<<<<<< HEAD
		GunStats::Modifiers::getToolModifier(сurrentModifiers.m_tool),
		GunStats::Modifiers::getKitModifier(сurrentModifiers.m_kit),
		GunStats::Modifiers::getSkillModifier(сurrentModifiers.m_skill)
	);

	wstring percent = L" ";

	if (!сurrentCharacteristicGun.isEmpty())
	{
		percent += format(L"{:.2f}", tempChance);
	}
	else
	{
		percent += L"0.0";
	}
	percent += L"%";

	auto nameCharacteristic = string(GunStats::indexCharacteristicName[it]);

	GUIChance.setKeyStringTextWstring({ "Chahce_upgrade" , nameCharacteristic ,  percent });

	if (tempChance >= 100.0f)
=======
	auto TempChance = calculateSuccessChance(
		currentCharacteristicGun.getChanceFor_NEXT_Stat(GunStats::transIndexToIndexGunStat[it]),

		GunStats::Modifiers::GetToolModifier(currentModifiers.Tool),
		GunStats::Modifiers::GetKitModifier(currentModifiers.Kit),
		GunStats::Modifiers::GetSkillModifier(currentModifiers.Skill)
	);

	wstring Percent = L" ";

	if (!currentCharacteristicGun.is_Empty())
	{
		Percent += format(L"{:.2f}", TempChance);
	}
	else
	{
		Percent += L"0.0";
	}
	Percent += L"%";

	auto NameCharacteristic = string(GunStats::mapCharacteristicNameIndex[it]);

	GUIChance.setKeyStringAndTextWstring({ "Chahce_upgrade" , NameCharacteristic ,  Percent });

	if (TempChance >= 100.0f)
>>>>>>> main
	{
		GUIChance.setColorText(Color::Green);
	}
	return true;
}

<<<<<<< HEAD
//-----------------------------------------------------------------------------------------------------------------------
=======
//-------------------------------------------------------------------------------------------------------------------
>>>>>>> main

template<typename T>
void moveElement(vector<T>& vec, size_t from, size_t to)
{
	if (from == to || from >= vec.size() || to > vec.size())
	{
		return;
	}

	auto itFrom = vec.begin() + from;
	T val = move(*itFrom);
	vec.erase(itFrom);

	if (to > from)
	{
		--to;  // учёт сдвига после erase
	}
	vec.insert(vec.begin() + to, move(val));
}

<<<<<<< HEAD
//-----------------------------------------------------------------------------------------------------------------------

unsigned getPrice(const path& pathToPriceModifiers, const SelectModifiers& currentModifiers)
{
	int result = -1;
	
	ifstream inJsonFilePrice(pathToPriceModifiers);
	if (!inJsonFilePrice)
	{
		OUTPUT_LOG("CharacteristicGun -> Failed to open file: " + wstringToString(pathToPriceModifiers));
	}
	else
	{
		nlohmann::json jsonPrice;
		inJsonFilePrice >> jsonPrice;
		inJsonFilePrice.close();
=======
//-------------------------------------------------------------------------------------------------------------------

unsigned getPrice(const path& pathToPriceModifiersJS, const Select_Modifiers& currentModifiers)
{
	int result = -1;
>>>>>>> main

		string nameTool = GunStats::Modifiers::getToolName(currentModifiers.m_tool);

<<<<<<< HEAD
		if (jsonPrice.contains("MD_Tool") && jsonPrice["MD_Tool"].contains(nameTool))
		{
			result = jsonPrice["MD_Tool"][nameTool].get<unsigned>();
		}
		else
		{
			jsonPrice["MD_Tool"][nameTool] = 0;
		}

		if (result == -1)
		{
			ofstream outJsonFile(pathToPriceModifiers);
			outJsonFile << jsonPrice.dump(2);
			outJsonFile.close();
=======
	ifstream inJsonFilePrice(pathToPriceModifiersJS);
	if (!inJsonFilePrice)
	{
		OUTPUT_LOG("CharacteristicGun -> Failed to open file: " + wstringToString(pathToPriceModifiersJS));
	}
	else
	{
		nlohmann::json jsPrice;
		inJsonFilePrice >> jsPrice;
		inJsonFilePrice.close();

		string nameTool = GunStats::Modifiers::GetToolName(currentModifiers.Tool);

		if (jsPrice.contains("MD_Tool") && jsPrice["MD_Tool"].contains(nameTool))
		{
			result = jsPrice["MD_Tool"][nameTool].get<unsigned>();
		}
		else
		{
			jsPrice["MD_Tool"][nameTool] = 0;
		}

		if (result == -1)
		{
			ofstream outJsonFilePrice(pathToPriceModifiersJS);
			outJsonFilePrice << jsPrice.dump(2);
			outJsonFilePrice.close();

>>>>>>> main
			result = 0;
		}
	}
	return static_cast<unsigned>(result);
}

<<<<<<< HEAD
//-----------------------------------------------------------------------------------------------------------------------

unsigned getPrice(const path& pathToPriceModifiersJson, const SelectModifiers& currentModifiers, string nameGun)
{
	int result = -1;

	ifstream inJjsonFilePrice(pathToPriceModifiersJson);
	if (!inJjsonFilePrice)
	{
		OUTPUT_LOG("CharacteristicGun -> Failed to open file: " + wstringToString(pathToPriceModifiersJson));
	}
	else
	{
		nlohmann::json jsonPrice;
		inJjsonFilePrice >> jsonPrice;
		inJjsonFilePrice.close();

		if (jsonPrice.contains("MD_Kit") && jsonPrice["MD_Kit"].contains(nameGun) && jsonPrice["MD_Kit"][nameGun].contains(GunStats::Modifiers::GetKitName(currentModifiers.m_kit)))
		{
			result = jsonPrice["MD_Kit"][nameGun][GunStats::Modifiers::GetKitName(currentModifiers.m_kit)].get<unsigned>();
		}
		else
		{
			jsonPrice["MD_Kit"][nameGun][GunStats::Modifiers::GetKitName(currentModifiers.m_kit)] = 0;
=======
//-------------------------------------------------------------------------------------------------------------------

unsigned getPrice(const path& pathToPriceModifiersJS, const Select_Modifiers& currentModifiers, string nameGun)
{
	int result = -1;

	ifstream inJsonFilePrice(pathToPriceModifiersJS);
	if (!inJsonFilePrice)
	{
		OUTPUT_LOG("CharacteristicGun -> Failed to open file: " + wstringToString(pathToPriceModifiersJS));
	}
	else
	{
		nlohmann::json jsPrice;
		inJsonFilePrice >> jsPrice;
		inJsonFilePrice.close();

		if (jsPrice.contains("MD_Kit") && jsPrice["MD_Kit"].contains(nameGun) && jsPrice["MD_Kit"][nameGun].contains(GunStats::Modifiers::GetKitName(currentModifiers.Kit)))
		{
			result = jsPrice["MD_Kit"][nameGun][GunStats::Modifiers::GetKitName(currentModifiers.Kit)].get<unsigned>();
		}
		else
		{
			jsPrice["MD_Kit"][nameGun][GunStats::Modifiers::GetKitName(currentModifiers.Kit)] = 0;
>>>>>>> main
		}

		if (result == -1)
		{
<<<<<<< HEAD
			ofstream outJson(pathToPriceModifiersJson);
			outJson << jsonPrice.dump(2);
			outJson.close();
=======
			ofstream outJsonFilePrice(pathToPriceModifiersJS);
			outJsonFilePrice << jsPrice.dump(2);
			outJsonFilePrice.close();
>>>>>>> main

			result = 0;
		}
	}
	return static_cast<unsigned>(result);
}

<<<<<<< HEAD
//-----------------------------------------------------------------------------------------------------------------------

bool setPriceTool(const path& pathToPriceModifiersJson, GunStats::Modifiers::ToolType tool, unsigned price)
=======
//-------------------------------------------------------------------------------------------------------------------

bool setPriceTool(const path& pathToPriceModifiersJS, GunStats::Modifiers::ToolType tool, unsigned price)
>>>>>>> main
{
	if (tool == GunStats::Modifiers::ToolType::noTool)
	{
		return false;
	}

<<<<<<< HEAD
	ifstream inJjsonFilePrice(pathToPriceModifiersJson);
	if (!inJjsonFilePrice)
	{
		OUTPUT_LOG("CharacteristicGun -> Failed to open file: " + wstringToString(pathToPriceModifiersJson));
		return false;
	}
	else
	{
		nlohmann::json jsonPrice;
		inJjsonFilePrice >> jsonPrice;
		inJjsonFilePrice.close();

		jsonPrice["MD_Tool"][GunStats::Modifiers::getToolName(tool)] = price;

		ofstream outJson(pathToPriceModifiersJson);
		outJson << jsonPrice.dump(2);
		outJson.close();
	}
	return true;
}

//-----------------------------------------------------------------------------------------------------------------------

bool setPriceKit(const path& pathToPriceModifiersJson, GunStats::Modifiers::KitType kit, string nameGun, unsigned price)
{
	if (kit == GunStats::Modifiers::KitType::noKit)
	{
		return false;
	}
	if (nameGun == "Empty")
	{
		return false;
	}

	ifstream inJjsonFilePrice(pathToPriceModifiersJson);
	if (!inJjsonFilePrice)
	{
		OUTPUT_LOG("CharacteristicGun -> Failed to open file: " + wstringToString(pathToPriceModifiersJson));
=======
	ifstream inJsonFilePrice(pathToPriceModifiersJS);
	if (!inJsonFilePrice)
	{
		OUTPUT_LOG("CharacteristicGun -> Failed to open file: " + wstringToString(pathToPriceModifiersJS));
		return false;
	}

	else
	{
		nlohmann::json jsPrice;
		inJsonFilePrice >> jsPrice;
		inJsonFilePrice.close();

		jsPrice["MD_Tool"][GunStats::Modifiers::GetToolName(tool)] = price;

		ofstream outJsonFilePrice(pathToPriceModifiersJS);
		outJsonFilePrice << jsPrice.dump(2);
		outJsonFilePrice.close();
	}
	return true;
}

//-------------------------------------------------------------------------------------------------------------------

bool setsPriceKit(const path& pathToPriceModifiersJS, GunStats::Modifiers::KitType kit, string nameGun, unsigned price)
{
	if (kit == GunStats::Modifiers::KitType::noKit || nameGun == "empty")
	{
		return false;
	}

	ifstream inJsonFilePrice(pathToPriceModifiersJS);
	if (!inJsonFilePrice)
	{
		OUTPUT_LOG("CharacteristicGun -> Failed to open file: " + wstringToString(pathToPriceModifiersJS));
>>>>>>> main
		return false;
	}
	else
	{
<<<<<<< HEAD
		nlohmann::json jsonPrice;
		inJjsonFilePrice >> jsonPrice;
		inJjsonFilePrice.close();

		jsonPrice["MD_Kit"][nameGun][GunStats::Modifiers::GetKitName(kit)] = price;

		ofstream outJson(pathToPriceModifiersJson);
		outJson << jsonPrice.dump(2);
		outJson.close();
	}
	return true;
}

//-----------------------------------------------------------------------------------------------------------------------
=======
		nlohmann::json jsPrice;
		inJsonFilePrice >> jsPrice;
		inJsonFilePrice.close();

		jsPrice["MD_Kit"][nameGun][GunStats::Modifiers::GetKitName(kit)] = price;

		ofstream outJsonFilePrice(pathToPriceModifiersJS);
		outJsonFilePrice << jsPrice.dump(2);
		outJsonFilePrice.close();
	}

	return true;
}

//-------------------------------------------------------------------------------------------------------------------
>>>>>>> main
