#include "ModdingGun.h"

int main()
{
	// создаем вывод в логи
	permissions("Log/log.txt", perms::all); remove("Log/log.txt"); OUTPUT_LOG("Start!");
	locale::global(std::locale("en_US.UTF-8"));

	/// высота окна, от нее зависит длина окна
	const float  heightWindowMain = 700.f;
	const float  lengthWindowMain = heightWindowMain * (16.0f / 9.0f);

	// создаем диалоговое окно графика
	const unsigned int heightWindowDiagram = static_cast<unsigned>(heightWindowMain * 0.8);
	const unsigned int lengthWindowDiagram = static_cast<unsigned>(heightWindowDiagram * (20.0f / 9.0f));

	// количество клеток по сторонам
	// устарело
	[[maybe_unused]]
	const unsigned int countCellOnLength = 20;
	// устарело
	[[maybe_unused]]
	const unsigned int countCellOnHeight = 5;

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

		}();


	GUITextAndRectangle downScrollToolTip = [defaultSizeCell, &vectorGun, countGunInList]()
		{
			auto position = Vector2f(vectorGun[0].m_weapoGUI.getRectangle().getPosition().x + vectorGun[0].m_weapoGUI.getRectangle().getSize().x, vectorGun[0].m_weapoGUI.getRectangle().getSize().y * (countGunInList - 1));

			auto size = vectorGun[0].m_weapoGUI.getRectangle().getSize().y;

			RectangleShape toolTip;
			toolTip.setSize(Vector2f(size, size));
			toolTip.setPosition(position);

			return GUITextAndRectangle({ to_wstring(vectorGun.size() - countGunInList) + L"+" }, toolTip, false, false);

		}();

	//////////////////////// КНОПКА СБОРКИ ////////////////////////

	GUITextAndRectangle calculation = [defaultSizeCell, countCellOnHeight]()
		{
			RectangleShape rectangleCalculate;
			rectangleCalculate.setSize(Vector2f(defaultSizeCell * 2.f, defaultSizeCell / 2.f));
			rectangleCalculate.setPosition(Vector2f(defaultSizeCell * 13.5f, defaultSizeCell * static_cast<float>(countCellOnHeight + 3)));


			return GUITextAndRectangle({ "Build" }, rectangleCalculate, true, false);

		}();

	//////////////////////// ОКНО ПАРАМЕТРОВ МОДА ////////////////////////

	GUITextAndRectangle shaheGeneralMod = [defaultSizeCell]()
		{
			RectangleShape generalMod;
			generalMod.setPosition(Vector2f(defaultSizeCell * 6.f, defaultSizeCell * 0.5f));
			generalMod.setSize(Vector2f(defaultSizeCell * 13.8f, defaultSizeCell * 5.15f));

			return GUITextAndRectangle({}, generalMod, false, false);

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

			RectangleShape TempRec;
			TempRec.setSize(Vector2f(Xsize, ySizeAll));
			TempRec.setPosition(Vector2f(Rec.getPosition().x + Rec.getSize().x + xOffsetRelated, Rec.getPosition().y));


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

			RectangleShape ShapeTargetIter;
			ShapeTargetIter.setSize(Vector2f(defaultSizeCell * 4, defaultSizeCell / 2));
			ShapeTargetIter.setPosition(Vector2f(defaultSizeCell * 13.5f, defaultSizeCell * static_cast<float>(countCellOnHeight + 2)));

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

							}();

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
							{
								RectangleShape rectangleTemp;
								rectangleTemp.setSize({ xSizePack, ySizePack });
								rectangleTemp.setPosition({ xPosPack, yPosPack });
								return GUITextAndRectangle({ "Method" }, rectangleTemp, false, false);

							}();

						arrDiagramPack[iterGraf].m_GUIMethod = make_shared<GUITextAndRectangle>(GUIMethod);

						// добавляем стрелочки для прокрутки если выходим за указанный предел
						if (resultGuiMethodMod.size() >= MethodList)
						{
							// верхняя
							xSizePack = defaultSizeCell * 0.4f;
							ySizePack = defaultSizeCell * 0.4f;

							xPosPack = resultGuiMethodMod[1].getRectangle().getPosition().x - xSizePack;
							yPosPack = resultGuiMethodMod[1].getRectangle().getPosition().y;

							GUITextAndRectangle buttonTop = [xSizePack, ySizePack, xPosPack, yPosPack]()
								{
									RectangleShape rectangleTemp;
									rectangleTemp.setSize({ xSizePack, ySizePack });
									rectangleTemp.setPosition({ xPosPack, yPosPack });


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
										binBt.setPosition(Vector2f(posR.x, posR.y + SizeR.y));

									}
									else if (itbt1)
									{
										posR = arrDiagramPack[iterGraf].m_GUIPercentBinBt.rbegin()->getPositionRectangle();
										SizeR = arrDiagramPack[iterGraf].m_GUIPercentBinBt.rbegin()->getRectangle().getSize();
										binBt.setPosition(Vector2f(posR.x + SizeR.x, posR.y));
									}
									else
									{
										binBt.setPosition(Vector2f(posR.x + SizeR.x, posR.y));
									}

									if (wstrbt == L"val")
									{
										return GUITextAndRectangle({ format(L"{:.1f}", arrDiagramPack[iterGraf].m_percentBin) }, binBt, false, false);
									}
									else if (wstrbt == L"name")
									{
										return GUITextAndRectangle({ "PercentBin" }, binBt, false, false);
									}
									return GUITextAndRectangle({ wstrbt }, binBt, true, false);
								}();
							arrDiagramPack[iterGraf].m_GUIPercentBinBt.push_back(GUI_BinBt);
							itbt1++;
						}

						itbt1 = 0;
						for (const auto& [wstrbt, SizeP] : allocationBt)
						{
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

									}
									else if (itbt1)
									{
										posR = arrDiagramPack[iterGraf].m_GUIPercentSmoothBt.rbegin()->getPositionRectangle();
										SizeR = arrDiagramPack[iterGraf].m_GUIPercentSmoothBt.rbegin()->getRectangle().getSize();
										smBt.setPosition(Vector2f(posR.x + SizeR.x, posR.y));
									}
									else
									{
										smBt.setPosition(Vector2f(posR.x + SizeR.x, posR.y));
									}

									if (wstrbt == L"val")
									{
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

						//делаем прямую y - 0
						VertexArray verticalLine(PrimitiveType::Lines, 2);

						verticalLine[0].position = posLineTop;
						verticalLine[0].color = Color::Black;

						verticalLine[1].position = posLineDown;
						verticalLine[1].color = Color::Black;

						arrWindowsDiagram[iterGraf].draw(verticalLine);

						// перебираем отрезки кривой для вывода информации
						size_t countSectionCurve = arrDiagramPack[iterGraf].m_currentCurvePack.m_curve.getVertexCount();
						if (countSectionCurve == 0)
						{
							countSectionCurve = 1;
						}
						auto lengthCurve = arrDiagramPack[iterGraf].m_currentCurvePack.m_mainRectangleCurve.getSize().x;

						auto xPosRectangleDP = arrDiagramPack[iterGraf].m_currentCurvePack.m_mainRectangleCurve.getPosition().x;
						auto lengthOneSectionCurve = lengthCurve / countSectionCurve;
						auto xPosLineDiagram = posLineTop.x; // || posLineDown.x

						//имя инструмента \ кол-во
						vector<pair<string, unsigned long long>> infoCountModifiers;

						// перебираем все отрезки
						unsigned itMax = 0;
						float currentFreq = 0.f;

						for (itMax = 0; itMax < countSectionCurve; itMax++)
						{
							// делаем допуск
							float epsilon = 0.01f;
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
								break;
							}
						}

						// информация о секции
						Text nameModifier(ThemeGui::currentFontGUI);
						nameModifier.setFillColor(Color::Black);
						nameModifier.setCharacterSize(static_cast<unsigned>(ThemeGui::characterSizeGUI * 0.9f));

						RectangleShape rectangleForGraf;
						rectangleForGraf.setSize(Vector2f(defaultSizeCell * 2.7f, defaultSizeCell * 0.23f * static_cast<float>(infoCountModifiers.size() + 1)));
						rectangleForGraf.setPosition({ posLineTop + Vector2f(defaultSizeCell * 0.3f, 0) });
						rectangleForGraf.setFillColor(Color(230, 230, 230, 200));
						rectangleForGraf.setOutlineColor(Color::Black);
						rectangleForGraf.setOutlineThickness(-1);

						arrWindowsDiagram[iterGraf].draw(rectangleForGraf);

						size_t it4;
						for (it4 = 0; it4 < infoCountModifiers.size(); it4++)
						{
							auto pos1 = posLineTop + Vector2f(defaultSizeCell * 0.4f, defaultSizeCell * 0.2f * it4);
							nameModifier.setPosition(pos1);

							if (it4 == infoCountModifiers.size() - 1)
							{
								nameModifier.setString(format(L"{:L}", static_cast<float>(infoCountModifiers[it4].second / 1'000'000)) + LANG_TRANSLATE("kk"));
							}
							else
							{
								nameModifier.setString(to_wstring(infoCountModifiers[it4].second));
							}
							arrWindowsDiagram[iterGraf].draw(nameModifier);

							auto pos2 = posLineTop + Vector2f(defaultSizeCell * 0.85f, defaultSizeCell * 0.2f * it4);

							nameModifier.setPosition(pos2);
							nameModifier.setString(L" - " + LANG_TRANSLATE(infoCountModifiers[it4].first));

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
						{
							button.setAsPressedButton();
						}
					}
				}
			}
		}

		// возврат к дефолт цвету
		if (itWindow % 50 == 10)
		{
			calculation.setAsPressedButton();
			shapeTargetIter.setAsPressedButton();
			buttonGeneralModApplyStat.setAsPressedButton();
			shapeGeneralModReset.setAsPressedButton();
			shapeGeneralModBack.setAsPressedButton();

			if (!pressedApplyAsMod) { shapeGeneralModApplyStatInitial.setAsPressedButton(); }

			/// стоимость
			for (auto& button : arrToolPrice)
			{
				button.setAsPressedButton();
			}
			for (auto& button : arrKitPrice)
			{
				button.setAsPressedButton();
			}
		}
	}
	return 0;
}

//-----------------------------------------------------------------------------------------------------------------------

vector<pair<string, unsigned long long>> GetInfoForModifiers(const CountModifiers& modifiers)
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
				result.emplace_back(GunStats::Modifiers::getToolName(currentTool), countTool);
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
			}
		}
	}
}

//-----------------------------------------------------------------------------------------------------------------------

bool setChahceUpgrade(const CharacteristicGun& сurrentCharacteristicGun, const SelectModifiers& сurrentModifiers, GUITextAndRectangle& GUIChance, size_t it)
{
	GUIChance.resetColorText();

	if (сurrentCharacteristicGun.isEmpty())
	{
		return false;
	}

	auto tempChance = calculateSuccessChance(
		сurrentCharacteristicGun.getChanceForNextStat(GunStats::transIndexToIndexGunStat[it]),

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
	{
		GUIChance.setColorText(Color::Green);
	}
	return true;
}

//-----------------------------------------------------------------------------------------------------------------------

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

		string nameTool = GunStats::Modifiers::getToolName(currentModifiers.m_tool);

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
			result = 0;
		}
	}
	return static_cast<unsigned>(result);
}

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
		}

		if (result == -1)
		{
			ofstream outJson(pathToPriceModifiersJson);
			outJson << jsonPrice.dump(2);
			outJson.close();

			result = 0;
		}
	}
	return static_cast<unsigned>(result);
}

//-----------------------------------------------------------------------------------------------------------------------

bool setPriceTool(const path& pathToPriceModifiersJson, GunStats::Modifiers::ToolType tool, unsigned price)
{
	if (tool == GunStats::Modifiers::ToolType::noTool)
	{
		return false;
	}

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
		return false;
	}
	else
	{
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
