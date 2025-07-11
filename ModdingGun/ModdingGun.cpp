#include "ModdingGun.h"




int main()
{
    // создаем вывод в логи
    permissions("Log/log.txt", perms::all); remove("Log/log.txt"); OutputLog("Start!");
    locale::global(std::locale("en_US.UTF-8"));

    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    /// высота окна, от нее зависит длина окна
    float  HeightWindowMain = 700.f;
    float  LengthWindowMain = HeightWindowMain * (16.0f / 9.0f);
   
    // создаем диалоговое окно графика
    unsigned int HeightWindowDiagram = static_cast<unsigned>(HeightWindowMain    * 0.8);
    unsigned int LengthWindowDiagram = static_cast<unsigned>(HeightWindowDiagram * (20.0f / 9.0f));



    // количество клеток по сторонам
    // устарело
    unsigned int CountCellOnLength = 20;
    // устарело
    unsigned int CountCellOnHeight = 5;


    float SizeCell_default = static_cast<float>(LengthWindowMain / CountCellOnLength);


    // переменные  мышки
    Vector2i PositionMouseForMain;
    Vector2f MouseWorldPosForMain;


    shared_ptr<Texture> TexturePoint_Empty     = make_shared<Texture>(L"Assets/Standart/Empty.png");
    shared_ptr<Texture> TexturePoint_RedMark   = make_shared<Texture>(L"Assets/Standart/MarkX.png");
    shared_ptr<Texture> TexturePoint_GreenMark = make_shared<Texture>(L"Assets/Standart/MarkV.png");


    path Lang_Folder  = L"Lang/";


    path Gun_Folder   = L"Assets/Gun";


    path Tool_Folder  = L"Assets/Tool";
    path Kit_Folder   = L"Assets/Kit";
    path Skill_Folder = L"Assets/Skill";


    path FileStat                = L"Attribute/AllGunStat/weaponstat.json";
    path PathToPriceModifiers_JS = L"Attribute/PriceModifiers/PriceModifiers.json";


    path backgroundPathDiagram = L"Assets/Standart/background_graf.png";
    path backgroundPatchMain   = L"Assets/Standart/background.png";
    
    /////////////////////////////////////////////////////////////////////////////////////////////////////////





    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    // колличество повторений сборок мода
    unsigned int totalIterations = 5000;

    // записанные шаги модификаций 
    vector<Method> method_MOD;

    // текущее оружие
    string NameGun = "Empty";


    // текущие модификаторы улучающие шанс прока
    Select_Modifiers CurrentModifiers = 
    {
        GunStats::Modifiers::ToolType::OLD_TOOL,
        GunStats::Modifiers::KitType::NO_KIT,
        GunStats::Modifiers::SkillType::PLAYER_MASTER_5
    };

    PriceModifiers CurrentPrice = 
    {
        0,
        0,
        0
    };



    /////////////////////////////////////////////////////////////////////////////////////////////////////////





    /////////////////////////////////////////////////////////////////////////////////////////////////////////







    /////////////////////////////////////////////////////////////////////////////////////////////////////////






















    ///////////////////////////////////////////////////////////////////////////////////
    
    LANG_SETTING.loadLangInFolder(Lang_Folder);

    vector<GUI_TextAndRectangle> VectorLangGUI;
    {

        auto lang = LANG_SETTING.getLoadedLanguages();
        float SizeLangCell = SizeCell_default * 0.5f;

        for (size_t i = 0; i < LANG_SETTING.getCountlang(); i++)
        {
            RectangleShape ShapeLang;
            ShapeLang.setSize({ SizeLangCell, SizeLangCell });
            ShapeLang.setPosition(Vector2f(LengthWindowMain - SizeLangCell * i - SizeCell_default, HeightWindowMain - SizeLangCell * 2.7f));

            bool setAsPressedButton;
            i == 1 ? setAsPressedButton = false : setAsPressedButton = true;

            VectorLangGUI.emplace_back(GUI_TextAndRectangle({ lang[i] }, ShapeLang, true, setAsPressedButton));
        }
        LANG_SETTING.setLanguage(1);
    }
    ///////////////////////////////////////////////////////////////////////////////////














    ///////////////////////////////////////////////////////////////////////////////////

    vector<GUI_TextAndRectangle> VectorThemeGUI;    


    {
        float YSizeLangCell = SizeCell_default * 0.5f;
        float XSizeLangCell = SizeCell_default * 1.5f;



        int itTheme = 0;

        for (const auto& Theme : ThemeGui::AllTheme)
        {
            auto itbeginTheme = ThemeGui::AllTheme.begin();

            RectangleShape ShapeLang;
            ShapeLang.setSize({ XSizeLangCell ,  YSizeLangCell });
            ShapeLang.setPosition(Vector2f(LengthWindowMain - XSizeLangCell * itTheme - SizeCell_default * 1.7f, HeightWindowMain - YSizeLangCell * 1.5f));


            bool setAsPressedButton;
            itTheme == 0 ? setAsPressedButton = false : setAsPressedButton = true;

            VectorThemeGUI.emplace_back(GUI_TextAndRectangle({ string(Theme.first) }, ShapeLang, true, setAsPressedButton));
            itTheme++;
        }
    }
    ///////////////////////////////////////////////////////////////////////////////////



















    //////////////////////// МОДИФИКАТОРЫ ////////////////////////

    ////////////////////////////////////////////////////////////////////////////////////
    vector<shared_ptr<Texture>> KitTextures;
    vector<shared_ptr<Texture>> ToolTextures;
    vector<shared_ptr<Texture>> SkillTextures;

    vector<path> toolpath  = SearchFile(Tool_Folder, ".png");
    vector<path> kitpath   = SearchFile(Kit_Folder, ".png");
    vector<path> skillpath = SearchFile(Skill_Folder, ".png");

    struct RectangleModifiers
    {        
        vector<GUI_TextAndRectangle> ToolRectangleGUI;
        vector<GUI_TextAndRectangle> KitRectangleGUI;
        vector<GUI_TextAndRectangle> SkillRectangleGUI;

    };
    RectangleModifiers ModifiersIcon;


    auto LoadTexturesAndShapes = [&](const vector<path>& paths, float Ypos, vector<shared_ptr<Texture>>& textures, vector<GUI_TextAndRectangle>& shapes)
        {            
            float i = 0;
            float j = 0;

            float Size = SizeCell_default * 0.55f;

            for (const auto& patht : paths)
            {
                auto tex = make_shared<Texture>();

                if (!tex->loadFromFile(patht))
                {
                    continue;
                }

                textures.push_back(tex);

                RectangleShape RectangleCurrentType;
                RectangleCurrentType.setSize(Vector2f(Size, Size));
                RectangleCurrentType.setPosition(Vector2f(SizeCell_default * 2.25f + Size * i, Ypos + j * Size));
                RectangleCurrentType.setTexture(tex.get());

                string NameModifiers = patht.stem().string();
                NameModifiers.erase(0, 3);

                shapes.push_back(GUI_TextAndRectangle( { NameModifiers }, RectangleCurrentType, true, true)  );

                i++;
                if (i > 4)
                { 
                    j = 1.f; i = 0.f;
                }
            }
        };

    // Загружаем всё:
    LoadTexturesAndShapes(toolpath,  SizeCell_default * 1.5f, ToolTextures,  ModifiersIcon.ToolRectangleGUI);
    LoadTexturesAndShapes(kitpath,   SizeCell_default * 3.0f, KitTextures,   ModifiersIcon.KitRectangleGUI);
    LoadTexturesAndShapes(skillpath, SizeCell_default * 4.5f, SkillTextures, ModifiersIcon.SkillRectangleGUI);



    
    /// стоимость
    vector<GUI_TextAndRectangle> ToolPrice;
    vector<GUI_TextAndRectangle> KitPrice;


    RectangleShape RectangleAllPrice;
    RectangleAllPrice.setSize(Vector2f(SizeCell_default * 3.5f, SizeCell_default * 0.4f));



    {

        wstring WsPrice = L"price";

        const vector<pair<const float, const wstring>> allocation = 
        {
            {0.13f, L"--"},
            {0.11f, L"-" },
            {0.5f, WsPrice},
            {0.11f, L"+" },
            {0.13f, L"++"}
        };

        const float Yoffset = SizeCell_default * 0.05f;
        const float Xoffset = SizeCell_default * 0.02f;

        // Tool 

        RectangleAllPrice.setPosition(Vector2f(SizeCell_default * 2.25f, ModifiersIcon.ToolRectangleGUI.rbegin()->getRectangle().getPosition().y + ModifiersIcon.ToolRectangleGUI.rbegin()->getRectangle().getSize().y + Yoffset));

        float Xpos = RectangleAllPrice.getPosition().x;
        for (const auto& [fl, ws] : allocation)
        {
            if (!ToolPrice.empty())
            {
                Xpos += ToolPrice.rbegin()->getRectangle().getSize().x + Xoffset;
            }


            RectangleShape CellPrice;
            CellPrice.setPosition(Vector2f(Xpos , RectangleAllPrice.getPosition().y));
            CellPrice.setSize(Vector2f(RectangleAllPrice.getSize().x * fl, RectangleAllPrice.getSize().y) );

            if (ws == WsPrice)
            {
                wstring Str_ValuePrice = L"0";

                ifstream in_jsonFile_price(PathToPriceModifiers_JS);
                if (!in_jsonFile_price)
                {
                    OutputLog("CharacteristicGun -> Failed to open file: " + WstringToString(PathToPriceModifiers_JS));
                }
                else
                {
                    nlohmann::json JSON_PRICE;
                    in_jsonFile_price >> JSON_PRICE;
                    in_jsonFile_price.close();

                    unsigned val_price = 0;

                    if (JSON_PRICE.contains("MD_Tool") && JSON_PRICE["MD_Tool"].contains(GunStats::Modifiers::GetToolName(CurrentModifiers.Tool)))
                    {
                        val_price = JSON_PRICE["MD_Tool"][GunStats::Modifiers::GetToolName(CurrentModifiers.Tool)].get<unsigned>();
                        Str_ValuePrice = format( L"{:L}", val_price);

                    }
                    else
                    {
                        JSON_PRICE["MD_Tool"][GunStats::Modifiers::GetToolName(CurrentModifiers.Tool)] = 0;

                        ofstream in_jsonFile_price(PathToPriceModifiers_JS);
                        in_jsonFile_price << JSON_PRICE;
                        in_jsonFile_price.close();
                    }
                    CurrentPrice.PriceTool = val_price;

                }
                ToolPrice.push_back(move(GUI_TextAndRectangle( { Str_ValuePrice, L"₽"}, CellPrice, false, false)));
            } 
            else
            {
                ToolPrice.push_back(move(GUI_TextAndRectangle({ ws }, CellPrice, true, false)));
            }

        }

        // Kit 
        RectangleAllPrice.setPosition(Vector2f(SizeCell_default * 2.25f, ModifiersIcon.KitRectangleGUI.rbegin()->getRectangle().getPosition().y + ModifiersIcon.KitRectangleGUI.rbegin()->getRectangle().getSize().y + Yoffset));
        Xpos = RectangleAllPrice.getPosition().x;
        for (const auto& [fl, ws] : allocation)
        {
            if (!KitPrice.empty())
            {
                Xpos += KitPrice.rbegin()->getRectangle().getSize().x + Xoffset;
            }

            RectangleShape CellPrice;
            CellPrice.setPosition(Vector2f(Xpos, RectangleAllPrice.getPosition().y));
            CellPrice.setSize(Vector2f(RectangleAllPrice.getSize().x * fl, RectangleAllPrice.getSize().y));


            if (ws == WsPrice)
            {
                wstring Str_ValuePrice = L"0";
                KitPrice.push_back(move(GUI_TextAndRectangle({ Str_ValuePrice, L"₽" }, CellPrice, false, false)));
            }
            else
            {
                KitPrice.push_back(move(GUI_TextAndRectangle({ ws }, CellPrice, true, false)));
            }
            CurrentPrice.PriceKit = 0;

        }

    }
    // skill
    RectangleAllPrice.setPosition(Vector2f(SizeCell_default * 2.25f, ModifiersIcon.SkillRectangleGUI.rbegin()->getRectangle().getPosition().y + ModifiersIcon.SkillRectangleGUI.rbegin()->getRectangle().getSize().y + SizeCell_default * 0.05f));
    GUI_TextAndRectangle SkillPrice({ L"0", L"₽" }, move(RectangleAllPrice), false, false);




    /// устанавливаем настройки по умолчанию
    // по умолчанию старый инструмент
    for (size_t it = 0; it < ModifiersIcon.ToolRectangleGUI.size(); it++)
    {
        auto name = *ModifiersIcon.ToolRectangleGUI[it].getKeyString_and_TextWstring().begin();


        if (holds_alternative<string>(name))
        {
            string SName = get<string>(name);

            if (SName == GunStats::Modifiers::GetToolName(CurrentModifiers.Tool))
            {
                ModifiersIcon.ToolRectangleGUI[it].setAsPressedButton();

                break;
            }
        }
    }

    // по умолчанию без набора
    for (size_t it = 0; it < ModifiersIcon.KitRectangleGUI.size(); it++)
    {
        auto name = *ModifiersIcon.KitRectangleGUI[it].getKeyString_and_TextWstring().begin();

        if (holds_alternative<string>(name))
        {
            string SName = get<string>(name);           

            if (SName == GunStats::Modifiers::GetKitName(CurrentModifiers.Kit))
            {
                ModifiersIcon.KitRectangleGUI[it].setAsPressedButton();
                break;
            }
        }
    }

    // игрок мастер 5
    for (size_t it = 0; it < ModifiersIcon.SkillRectangleGUI.size(); it++)
    {
        auto name = *ModifiersIcon.SkillRectangleGUI[it].getKeyString_and_TextWstring().begin();

        if (holds_alternative<string>(name))
        {
            string SName = get<string>(name);


            if (SName == GunStats::Modifiers::GetSkillName(CurrentModifiers.Skill))
            {
                ModifiersIcon.SkillRectangleGUI[it].setAsPressedButton();
                break;
            }
        }
    }

    ///////////////////////////////////////////////////////////////////////////////////






















    //////////////////////// ИКОНКИ ОРУЖИЯ  И ВЫБРАННОГО ОРУЖИЯ ////////////////////////





    /////////////////////////////////////////////////////////////////////////////
    struct VectorWeapon
    {
        shared_ptr<Texture> WTexture;

        string Str_Name;

        GUI_TextAndRectangle WeapoGUI;
    };

    vector<VectorWeapon> VectorGun;
    /////////////////////////////////////////////////////////////////////////////





    /////////////////////////////////////////////////////////////////////////////
    vector<path> SearchTextureGun = SearchFile(Gun_Folder, ".png");
    if (SearchTextureGun.empty())
    {
        OutputLog("main -> Texture Gun not found");
        return -1;
    }
    /////////////////////////////////////////////////////////////////////////////




    /////////////////////////////////////////////////////////////////////////////
    unsigned int it_VectorGun = 0;
    int CountGunList = 22;

    
    for (const auto Gun : SearchTextureGun)
    {
        shared_ptr<Texture> PtrTextureWeapon;

        string NameWeapon = WstringToString(Gun.stem());


        RectangleShape RectangleWeaponCurrent;
        RectangleWeaponCurrent.setSize(Vector2f(SizeCell_default * 2, SizeCell_default / 2));


        VectorGun.push_back({ PtrTextureWeapon, NameWeapon, GUI_TextAndRectangle({NameWeapon}, RectangleWeaponCurrent, false, false)});
    }
    

    // для списка
    GUI_TextAndRectangle ALLGunShapeScrollGUI = [SizeCell_default, CountGunList]()
        {
        RectangleShape ALLGunShapeScroll;
        ALLGunShapeScroll.setSize(Vector2f(SizeCell_default * 2, SizeCell_default / 2 * CountGunList));
        ALLGunShapeScroll.setPosition(Vector2f(0, 0));

        return GUI_TextAndRectangle({}, ALLGunShapeScroll, false, false);
        }();

    

    
    // - выбранное оружие
    GUI_TextAndRectangle SelectWeaponGUI = [SizeCell_default]()
        {
            RectangleShape ShapeSelectedGun;
            ShapeSelectedGun.setSize(Vector2f(SizeCell_default * 2.5f, SizeCell_default * 0.5f));
            ShapeSelectedGun.setPosition(Vector2f(SizeCell_default * 2.5f, SizeCell_default * 0.5f));
            //ShapeSelectedGun.setTexture(TexturePoint_Empty.get(), true);

            return GUI_TextAndRectangle({"Gun"}, ShapeSelectedGun, false, false);
        }();



    GUI_TextAndRectangle TopScrollToolTip = [SizeCell_default, &VectorGun]()
        {
            auto position = VectorGun[0].WeapoGUI.getRectangle().getPosition() + Vector2f(VectorGun[0].WeapoGUI.getRectangle().getSize().x , 0) ;
            auto Size = VectorGun[0].WeapoGUI.getRectangle().getSize().y;

            RectangleShape ToolTip;
            ToolTip.setSize(Vector2f(Size, Size));
            ToolTip.setPosition(position);

            return GUI_TextAndRectangle({}, ToolTip, false, false);

        }();


    GUI_TextAndRectangle DownScrollToolTip = [SizeCell_default, &VectorGun, CountGunList]()
        {
            auto position = Vector2f(VectorGun[0].WeapoGUI.getRectangle().getPosition().x + VectorGun[0].WeapoGUI.getRectangle().getSize().x, VectorGun[0].WeapoGUI.getRectangle().getSize().y * (CountGunList- 1));

            auto Size = VectorGun[0].WeapoGUI.getRectangle().getSize().y;

            RectangleShape ToolTip;
            ToolTip.setSize(Vector2f(Size, Size));
            ToolTip.setPosition(position);

            return GUI_TextAndRectangle({ to_wstring(VectorGun.size() - CountGunList) + L"+" }, ToolTip, false, false);

        }();

    ///////////////////////////////////////////////////////////////////////////////////















     //////////////////////// КНОПКА СБОРКИ ////////////////////////
    
    ////////////////////////////////////////////////////////////////////////////////////

    GUI_TextAndRectangle Calculation = [SizeCell_default, CountCellOnHeight]()
        {
            RectangleShape RectangleCalculate;
            RectangleCalculate.setSize(Vector2f(SizeCell_default * 2.f, SizeCell_default / 2.f));
            RectangleCalculate.setPosition(Vector2f(SizeCell_default * 13.5f, SizeCell_default * static_cast<float>(CountCellOnHeight + 3)));


            return GUI_TextAndRectangle({ "Build" }, RectangleCalculate, true, false);

        }();

    ////////////////////////////////////////////////////////////////////////////////////





























    //////////////////////// ОКНО ПАРАМЕТРОВ МОДА ////////////////////////

    ///////////////////////////////////////////////////////////////////////////////////

    GUI_TextAndRectangle ShaheGeneralMod = [SizeCell_default]()
        {
            RectangleShape GeneralMod;
            GeneralMod.setPosition(Vector2f(SizeCell_default * 6.f, SizeCell_default * 0.5f));
            GeneralMod.setSize(Vector2f(SizeCell_default * 13.8f, SizeCell_default * 5.15f));
            
            return GUI_TextAndRectangle({}, GeneralMod, false, false);

        }();
    ///////////////////////////////////////////////////////////////////////////////////







    ///////////////////////////////////////////////////////////////////////////////////

    float YsizeAll = SizeCell_default * 0.39f;


    vector<GeneralStat> GeneralModOption = [SizeCell_default, TexturePoint_GreenMark, TexturePoint_RedMark, &ShaheGeneralMod, YsizeAll]()
        {
            vector<GeneralStat> result;

            float YsizeGeneral = ShaheGeneralMod.getRectangle().getSize().y;
            float XsizeGeneral = ShaheGeneralMod.getRectangle().getSize().x;
            float XposGeneral = ShaheGeneralMod.getRectangle().getPosition().x;



            // отступ сверху || снизу
            float YOffsetOne = YsizeGeneral * 0.15f;

            // отступ между
            float YOffsetRelated = YsizeGeneral * 0.03f;


            // позиция начальной не считая YOffsetOne
            float YposOneCharacteristic = (YsizeGeneral - YOffsetOne * 2.f) / ( GunStats::COUNT_CHARACTERISTIC + 2 );


            // процентарное соотношение x размера (не учитывая кнопку)            
            const array<float, 5> PercentSizeCell = { 0.12f, 0.06f, 0.06f, 0.72f, 0.04f}; // sum = ~1  |||  => const 0.04


            for (size_t it = 0; it < GunStats::COUNT_CHARACTERISTIC; it++)
            {
                RectangleShape RectangleTemp;





                // имя характеристики 
                /////////////////////////////////////////////////////////////////////////////////////
                float XsizeName = XsizeGeneral * PercentSizeCell[0];

                RectangleTemp.setPosition({ XposGeneral , YOffsetOne + YposOneCharacteristic * it + YOffsetRelated * it });
                RectangleTemp.setSize({ XsizeName, YsizeAll });

                GUI_TextAndRectangle NameCharacteristic({ string(GunStats::MapCharacteristicName_Index[it]) }, RectangleTemp, false, false);
                /////////////////////////////////////////////////////////////////////////////////////

               










                /// текущее отображение характеристики \\ темп пробитие ... \\
                /////////////////////////////////////////////////////////////////////////////////////
                float XsizeVisualCharacteristic = XsizeGeneral * PercentSizeCell[1];

                RectangleTemp.setPosition(RectangleTemp.getPosition() + Vector2f(XsizeName, 0));
                RectangleTemp.setSize({ XsizeVisualCharacteristic, YsizeAll });

                GUI_TextAndRectangle VisualCharacteristic({ L"0" }, RectangleTemp, false, false);
                /////////////////////////////////////////////////////////////////////////////////////











                /// визуальное отображения процента апгрейда
                /////////////////////////////////////////////////////////////////////////////////////
                float XsizeVisualPercent = XsizeGeneral * PercentSizeCell[2];

                RectangleTemp.setPosition(RectangleTemp.getPosition() + Vector2f(XsizeVisualCharacteristic, 0 ));
                RectangleTemp.setSize({ XsizeVisualPercent, YsizeAll });

                GUI_TextAndRectangle VisualPercentUpgrade({ L"0.0%" }, RectangleTemp, false, false);
                /////////////////////////////////////////////////////////////////////////////////////










                // Общий "прямоугольник" в котором распалагаютися все юниты
                /////////////////////////////////////////////////////////////////////////////////////
                float XsizeRectangleALLUnits = XsizeGeneral * PercentSizeCell[3];

                RectangleShape RectanpleForUnits;
                RectanpleForUnits.setPosition({ RectangleTemp.getPosition() + Vector2f(XsizeVisualPercent, 0) });
                RectanpleForUnits.setSize(Vector2f(XsizeRectangleALLUnits, YsizeAll));
                
                /////////////////////////////////////////////////////////////////////////////////////











                // размещаем едины в RectanpleForUnits
                /////////////////////////////////////////////////////////////////////////////////////
                vector<RectangleShape> resultShapeUnits;


                // максимальное колво единиц для текущей характеристики
                int   CountUnits = GunStats::GET_COUNT_UNITS_FOR_CHARACTERISTIC[it];

                float XposUnits  = RectanpleForUnits.getPosition().x;
                float YposUnits  = RectanpleForUnits.getPosition().y;
                float XsizeUnits = RectanpleForUnits.getSize().x / CountUnits;
                float YsizeUnits = RectanpleForUnits.getSize().y;


                for (int Cur_Unit = 0; Cur_Unit < CountUnits; Cur_Unit++)
                {
                    RectangleShape UnitShahe;
                    UnitShahe.setSize({ XsizeUnits, YsizeUnits });
                    UnitShahe.setPosition({ XposUnits + XsizeUnits * Cur_Unit, YposUnits });
                    UnitShahe.setFillColor(Color(66, 66, 66));
                    UnitShahe.setOutlineColor(Color(10, 10, 10));
                    UnitShahe.setOutlineThickness(-1);

                    resultShapeUnits.push_back(UnitShahe);
                }
                /////////////////////////////////////////////////////////////////////////////////////













                /// кнопка для выбора
                /////////////////////////////////////////////////////////////////////////////////////

                float ResizeMark = 0.7f; // размер кнопки от максимума %

                // размер кнопки
                float SizeCellButton = RectangleTemp.getSize().y * ResizeMark;

                // смещение для центрирования
                float OffsetSizeCellButton = RectangleTemp.getSize().y * (( 1.f - ResizeMark)  / 2.f );

                RectangleTemp.setPosition(   Vector2f(RectanpleForUnits.getPosition().x + RectanpleForUnits.getSize().x, RectanpleForUnits.getPosition().y) + Vector2f(OffsetSizeCellButton * 2, OffsetSizeCellButton)   );

                RectangleTemp.setSize(Vector2f(SizeCellButton, SizeCellButton));


                GUI_TextAndRectangle ButtonCharacteristic({}, RectangleTemp, true, false);  
                ButtonCharacteristic.setTextureButton(TexturePoint_GreenMark, TexturePoint_RedMark);


                if (it == 0)
                {
                    ButtonCharacteristic.setAsPressedButton();
                }
                /////////////////////////////////////////////////////////////////////////////////////








                result.emplace_back( NameCharacteristic, VisualCharacteristic ,VisualPercentUpgrade, RectanpleForUnits,  resultShapeUnits, ButtonCharacteristic  );
            }

            return result;
        }();

    ///////////////////////////////////////////////////////////////////////////////////


    ///////////////////////////////////////////////////////////////////////////////////
    float YDownOffset = 0.7f;
    // между
    float XOffsetRelated = SizeCell_default * 0.25f;

    /// отображение шанса на улучшение (внизу)
    GUI_TextAndRectangle TextGeneralModChance = [SizeCell_default, &ShaheGeneralMod, YsizeAll, YDownOffset]()
        {
            float Xsize = SizeCell_default * 4.f;


            RectangleShape RectangleGeneralModChance;
            RectangleGeneralModChance.setSize({ Xsize , YsizeAll });

            auto TempRectangle = ShaheGeneralMod.getRectangle();
            RectangleGeneralModChance.setPosition(TempRectangle.getPosition() + Vector2f(0, TempRectangle.getSize().y - (SizeCell_default * YDownOffset)));

            return GUI_TextAndRectangle({ "Chahce_upgrade" , string(GunStats::MapCharacteristicName_GunStat[0]) ,  L" 0.0%" }, RectangleGeneralModChance, false, false);
        }();







    ///////////////////////////////////////////////////////////////////////////////////

    // кнопка применить как начальный
    GUI_TextAndRectangle ShapeGeneralModApplyStatInitial = [SizeCell_default, &TextGeneralModChance, YsizeAll, YDownOffset, XOffsetRelated]()
        {         
            float Xsize = SizeCell_default * 3;

            auto Rec = TextGeneralModChance.getRectangle();

            RectangleShape TempRec;
            TempRec.setSize(Vector2f(Xsize, YsizeAll));
            TempRec.setPosition(Vector2f(Rec.getPosition().x + Rec.getSize().x + XOffsetRelated, Rec.getPosition().y) );

            return GUI_TextAndRectangle({ "Apply_as_initial_mod" }, TempRec, true, false);
        }();








    ///////////////////////////////////////////////////////////////////////////////////

    // кнопка применить как мод
    GUI_TextAndRectangle ButtonGeneralModApplyStat = [SizeCell_default, &ShapeGeneralModApplyStatInitial, YsizeAll, XOffsetRelated]()
        {

            float Xsize = SizeCell_default * 1.8f;

            auto Rec = ShapeGeneralModApplyStatInitial.getRectangle();

            RectangleShape TempRec;
            TempRec.setSize(Vector2f(Xsize, YsizeAll));
            TempRec.setPosition(Vector2f(Rec.getPosition().x + Rec.getSize().x + XOffsetRelated, Rec.getPosition().y));


            return GUI_TextAndRectangle({ "Apply" }, TempRec, true, false);
        }();








    ///////////////////////////////////////////////////////////////////////////////////

    // кнопка сбросить
    GUI_TextAndRectangle ShapeGeneralModReset = [SizeCell_default, &ButtonGeneralModApplyStat, YsizeAll, XOffsetRelated]()
        {

            float Xsize = SizeCell_default * 1.8f;

            auto Rec = ButtonGeneralModApplyStat.getRectangle();

            RectangleShape TempRec;
            TempRec.setSize(Vector2f(Xsize, YsizeAll));
            TempRec.setPosition(Vector2f(Rec.getPosition().x + Rec.getSize().x + XOffsetRelated, Rec.getPosition().y));


            return GUI_TextAndRectangle({ "Reset" }, TempRec, true, false);
        }();











    ///////////////////////////////////////////////////////////////////////////////////

    // кнопка назад
    GUI_TextAndRectangle ShapeGeneralModBack = [SizeCell_default, &ShapeGeneralModReset, YsizeAll, XOffsetRelated]()
        {

            float Xsize = SizeCell_default * 1.6f;

            auto Rec = ShapeGeneralModReset.getRectangle();

            RectangleShape TempRec;
            TempRec.setSize(Vector2f(Xsize, YsizeAll));
            TempRec.setPosition(Vector2f(Rec.getPosition().x + Rec.getSize().x + XOffsetRelated, Rec.getPosition().y));


            return GUI_TextAndRectangle({ "Step_back" }, TempRec, true, false);
        }();









    ///////////////////////////////////////////////////////////////////////////////////























    //////////////////////// ОТОБРАЖЕНИЕ ИНФОРМАЦИИ ///////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////////////
    
    GUI_TextAndRectangle TextInfoDebug = [SizeCell_default, CountCellOnHeight]()
        {

            RectangleShape ShapeInfoDebug;
            ShapeInfoDebug.setSize(Vector2f(SizeCell_default * 8, SizeCell_default / 2));
            ShapeInfoDebug.setPosition(Vector2f(static_cast<float>(SizeCell_default * 11.5), static_cast<float>(SizeCell_default* (CountCellOnHeight + 1))));

            return GUI_TextAndRectangle({}, ShapeInfoDebug, false, false);
        }();
    ///////////////////////////////////////////////////////////////////////////////////




















    //////////////////////// ОТОБРАЖЕНИЕ ПОВТОРНЫХ СБОРОК ////////////////////////

    ///////////////////////////////////////////////////////////////////////////////////


    GUI_TextAndRectangle ShapeTargetIter = [SizeCell_default, CountCellOnHeight, totalIterations]()
        {

            RectangleShape ShapeTargetIter;
            ShapeTargetIter.setSize(Vector2f(SizeCell_default * 4, SizeCell_default / 2));
            ShapeTargetIter.setPosition(Vector2f(SizeCell_default * 13.5f, SizeCell_default* static_cast<float>(CountCellOnHeight + 2)));

            return GUI_TextAndRectangle({ "Number_weapon_assemblies" , to_wstring(totalIterations)}, ShapeTargetIter, true, false);
        }();  

    ///////////////////////////////////////////////////////////////////////////////////



















//////////////////////// ЗАДНИЙ ФОН ////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////

    RectangleShape background;
    background.setSize(Vector2f(LengthWindowMain, HeightWindowMain));
    background.setPosition(Vector2f(0, 0));

    Texture backgroundTexture;

    if (!backgroundTexture.loadFromFile(backgroundPatchMain)) 
    {
        OutputLog("main -> background.png not loaded");
    }
    else 
    {
        background.setTexture(&backgroundTexture); 
    }


///////////////////////////////////////////////////////////////////////////////////
    
    
    




















/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // создаем окно
    RenderWindow window(VideoMode({ static_cast<unsigned int>(LengthWindowMain), static_cast<unsigned int>(HeightWindowMain) }), "ModdingGun");


    RenderWindow Diagram;


    window.setFramerateLimit(150);
    Diagram.setFramerateLimit(150);
////////////////////////////////////////////////////////////////////////////////



















































/////////////////////// ПРОЧИЕ ПЕРЕМЕННЫЕ ////////////////////////




//диаграмма
////////////////////////////////////////////////////////////////////////////////

    // используем отрисовку кривой только один раз
    // с помощью DiagramBool указывает какие окна отрисовали
    // false = отрисованно -> больше не будет повторяться
    vector<bool>         DiagramBool;

    // вектор окон диаграм с отрисованными графикам
    // графики отрисовываются один раз -> DiagramBool
    vector<RenderWindow> VectorWindowsDiagram;



    struct AllPackGraf
    {
        vector< GUI_TextAndRectangle> GUIPercentBinBt;
        vector< GUI_TextAndRectangle> GUIPercentSmoothBt;
        float PercentBin;
        float PercentSmooth;



        map<CountModifiers, float> CurrentSmoothFreqModifiers;
        map<CountModifiers, float> CurrentSourceFreqModifiers;    
        Color       ColorCurve;
        CurvePack   CurrentCurvePack;




        vector<GUI_TextAndRectangle>     GUIPercentile50;
        shared_ptr<GUI_TextAndRectangle> GUIInitialMod;
        shared_ptr<GUI_TextAndRectangle> GUIEndMod;




        shared_ptr<GUI_TextAndRectangle> GUIMethod;
        vector<GUI_TextAndRectangle>     GUIMethodModifiers;
        shared_ptr<GUI_TextAndRectangle> GUIButtonTop;
        shared_ptr<GUI_TextAndRectangle> GUIButtonDown;



        size_t InitialIndexMethod;
    };

    //вектор для кривых 
    vector<AllPackGraf> VectorDiagramPack;








    RectangleShape backgroundDiagram;
    backgroundDiagram.setSize(Vector2f(static_cast<float>(LengthWindowDiagram), static_cast<float>(HeightWindowDiagram)));
    backgroundDiagram.setPosition(Vector2f(0.f, 0.f));

    Texture backgroundTextureDiagram;

    // задний фон
    if (!backgroundTextureDiagram.loadFromFile(backgroundPathDiagram))
    {
        OutputLog("main -> background_graf.png not loaded");
    }
    else
    {
        backgroundDiagram.setTexture(&backgroundTextureDiagram);
    }

    int Error = 0;

////////////////////////////////////////////////////////////////////////////////















////////////////////////////////////////////////////////////////////////////////

    // дефолт статы для 
    // START_CHARACTERISTIC
    // INITIAL_CHARACTERISTIC
    const array<double, GunStats::COUNT_CHARACTERISTIC> DEFAULT_CHARACTERISTIC = { GunStats::COUNT_CHARACTERISTIC , 0.0 };


    // указанный мод
    // использовать только в области сборки calculate / vector для передачи в drawNormalGraph
    array<double, GunStats::COUNT_CHARACTERISTIC> MOD_CHARACTERISTIC = DEFAULT_CHARACTERISTIC;

    // начальный мод 
    // использовать только в области сборки calculate / vector для передачи в drawNormalGraph
    array<double, GunStats::COUNT_CHARACTERISTIC> INITIAL_CHARACTERISTIC = DEFAULT_CHARACTERISTIC;


    vector<int> MaxStatPosition = { 60, 48, 40,    40,40,40,40, };



    // выгрузка данных из файла оружия
    CharacteristicGun CurrentCharacteristicGun(MaxStatPosition);




    const vector<int> NullPosition = { 0,0,0, 0,0,0,0 };   

    // текущая позиция мода
    // 0 куч
    // 1 темп
    // 2 пробитие
    // 3 отдача
    // 4 качание
    // 6 сост
    // 5 грязь
    vector<int> PositionMod = NullPosition;


    // начальная позиция мода
    // 0 куч
    // 1 темп
    // 2 пробитие
    // 3 отдача
    // 4 качание
    // 6 сост
    // 5 грязь
    vector<int> InitialPosition = NullPosition;




    vector<pair <vector<int>, vector<int>>> HistoryPosition;

    bool PressedApplyAsMod = false;

////////////////////////////////////////////////////////////////////////////////
















////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


    for (int WI = 0; window.isOpen() ; WI++)
    {
        if (WI > 100) { WI = 0; }

        PositionMouseForMain = Mouse::getPosition(window);
        MouseWorldPosForMain = window.mapPixelToCoords(PositionMouseForMain);



        while (const optional event = window.pollEvent())
        {
            if (event->is<Event::Closed>())
            {
                window.close();
                for (int i = 0; i < VectorWindowsDiagram.size(); i++)
                {
                    VectorWindowsDiagram[i].close();
                }
            }




            // выбор
            else if (const auto& mouseButtonPressed = event->getIf<Event::MouseButtonPressed>())
            {
                if (mouseButtonPressed->button == Mouse::Button::Left)
                {























                    /// нажали на оружие в списке)
                    for (size_t it = 0; it < VectorGun.size(); it++)
                    {
                        if (VectorGun[it].WeapoGUI.getRectangle().getGlobalBounds().contains(MouseWorldPosForMain))
                        {
                            HistoryPosition.clear();
                            method_MOD.clear();

                            PressedApplyAsMod = false;

                            INITIAL_CHARACTERISTIC = DEFAULT_CHARACTERISTIC;
                            MOD_CHARACTERISTIC     = DEFAULT_CHARACTERISTIC;



                            // ставим стандартный цвет на юнитах
                            for (int it1 = 0; it1 < GeneralModOption.size(); it1++)
                            {
                                for (int it2 = 0; it2 < GeneralModOption[it1].ShaheUnitsInCharacteristic.size(); it2++)
                                {
                                    GeneralModOption[it1].ShaheUnitsInCharacteristic[it2].setFillColor(Color(66, 66, 66));
                                }
                            }







                            ////////////////////////////////////////////////////////////////////////////////////
                            // получаем имя оружия
                            NameGun = VectorGun[it].Str_Name;

                            if (!CurrentCharacteristicGun.load(FileStat, StringToWString( NameGun)))
                            {
                                NameGun = "Empty";
                                TextInfoDebug.setKeyString_and_TextWstring({ "Error_File" });
                                PositionMod = NullPosition;
                                break;
                            }

                            SelectWeaponGUI.setKeyString_and_TextWstring({ NameGun });

                            TextInfoDebug.setKeyString_and_TextWstring({ "Loaded",  NameGun } );
                            ////////////////////////////////////////////////////////////////////////////////////









                            ////////////////////////////////////////////////////////////////////////////////////

                            InitialPosition = CurrentCharacteristicGun.getCurrentPosition();
                            moveElement(InitialPosition, 4, 2);
                            PositionMod = InitialPosition;

                            
                            EditShaheUnits(GeneralModOption, CurrentCharacteristicGun, InitialPosition, PositionMod);


                            ////////////////////////////////////////////////////////////////////////////////////
                            for (int it = 0; it < GeneralModOption.size(); it++)
                            {
                                if (GeneralModOption[it].MarkButtonCharacteristic.isPressed())
                                {
                                    Set_Chahce_upgrade(CurrentCharacteristicGun, CurrentModifiers, TextGeneralModChance, it);
                                }




                                //GeneralModOption[it].TextCharacteristicVisualPercent.setFillColor(ColorText);
                                GeneralModOption[it].TextCharacteristicVisualPercent.setKeyString_and_TextWstring({ L"0.0%" });


                                // размета значений характеристики
                                auto val = CurrentCharacteristicGun.getValueCharacteristic(GunStats::Trans_INDEX_to_INDEXGUNSTAT[it]);
                                wstring WSval = it ? to_wstring(static_cast<int>(val)) : format(L"{:.1f}", val);
                                GeneralModOption[it].TextVisualCharacteristic.setKeyString_and_TextWstring({ WSval });

                            }

                            ////////////////////////////////////////////////////////////////////////////////////





                            /// обновляем стоимость KitPrice
                            /// tool уже выведен
                            /// skill comming soon
                            /////////////////////////////////////////////////////////

                            unsigned value_price = Get_Price(PathToPriceModifiers_JS, CurrentModifiers, NameGun);
                            CurrentPrice.PriceKit = value_price;
                            for (auto& tp : KitPrice)
                            {
                                if (!tp.isButton())
                                {
                                    tp.setKeyString_and_TextWstring({ format(L"{:L}", value_price), L"₽" });
                                    break;
                                }
                            }
                            /////////////////////////////////////////////////////////

                            break;
                        }
                    }







































                    // languages
                    for (size_t it = 0; it < VectorLangGUI.size(); it++)
                    {
                        if (VectorLangGUI[it].getRectangle().getGlobalBounds().contains(MouseWorldPosForMain))
                        {
                            // снимаем выделение всех
                            for (size_t it1 = 0; it1 < VectorLangGUI.size(); it1++) 
                            { 
                                VectorLangGUI[it1].setAsReleasedButton();
                            }

                            VectorLangGUI[it].setAsPressedButton();

                            LANG_SETTING.setLanguage(it);

                            /// Refresh всех GUI;
                            
                            GUI_TextAndRectangle::FREFRESH_TR_GUI();                         

                        }
                    }










































                    for (size_t it = 0; it < VectorThemeGUI.size(); it++)
                    {
                        if (VectorThemeGUI[it].getRectangle().getGlobalBounds().contains(MouseWorldPosForMain))
                        {
                            // снимаем выделение всех
                            for (size_t it1 = 0; it1 < VectorThemeGUI.size(); it1++)
                            {
                                VectorThemeGUI[it1].setAsPressedButton();
                            }

                            VectorThemeGUI[it].setAsReleasedButton();



                            /// Refresh всех GUI;

                            auto Theme = VectorThemeGUI[it].getKeyString_and_TextWstring();

                            if (holds_alternative<string>(Theme[0]))
                            {
                                auto StringTheme = get<string>(Theme[0]);
                                GUI_TextAndRectangle::FREFRESH_THEME_GUI(StringTheme);
                            }
                            else
                            {
                                OutputLog("Main -> VectorThemeGUI - no string specified");
                            }


                        }
                    }








































                    // нажали рассчет мода
                    if (Calculation.getRectangle().getGlobalBounds().contains(MouseWorldPosForMain))
                    {
                        
                        Calculation.setAsReleasedButton();


                        // загружен ли файл?
                        if (!CurrentCharacteristicGun.is_Empty())
                        {
                            TextInfoDebug.setKeyString_and_TextWstring({ "calculation" });


                            ALL_modifiers.clear();
                            All_Random_Attempt_Used.clear();

                            for (int i = 0; i < GunStats::COUNT_CHARACTERISTIC; i++)
                            {
                                MOD_CHARACTERISTIC[i] = CurrentCharacteristicGun.getVisualPercentUpgradeCharacteristic(GunStats::Trans_INDEX_to_INDEXGUNSTAT[i]);
                            }                            


                            // если не совпадает c начальным 
                            if (PositionMod != InitialPosition)
                            {
                                window.draw(background);

                                window.draw(Calculation.getRectangle());
                                window.draw(Calculation.getText());

                                window.draw(TextInfoDebug.getRectangle());
                                window.draw(TextInfoDebug.getText());
                             
                                window.display();


                                vector<thread> threads_v;
                                atomic<int> currentIndex = 0;



                                // защита от деления на 0
                                if (threadCount == 0) threadCount = 1;

                                int chunkSize = totalIterations  / threadCount;
                                int remainder = totalIterations  % threadCount;

                                int start = 0;

                                for (int i = 0; i < threadCount; ++i)
                                {
                                    int end = start + chunkSize + (i < remainder ? 1 : 0);  // равномерно распределяем остаток

                                    threads_v.emplace_back([=]()
                                        {

                                            for (int j = start; j < end; ++j)
                                            {
                                                CharacteristicGun gunCopy = CurrentCharacteristicGun;
                                                RunSingleAssembly(j, method_MOD);
                                            }
                                        });
                                    start = end;
                                }

                                // Ждём завершения всех потоков
                                for (auto& t : threads_v) t.join();

                                Error = 0;
                                switch (RunSingleAssembly(-1, method_MOD))
                                {
                                case 0:
                                    TextInfoDebug.setKeyString_and_TextWstring({ "Success_assemble" } );
                                    break;

                                case -1:
                                    TextInfoDebug.setKeyString_and_TextWstring({ "Empty_vector_method" });
                                    break;

                                case -2:
                                    TextInfoDebug.setKeyString_and_TextWstring({ "ChanceUpgrade_<=_0" });
                                    break;

                                case -3:
                                    TextInfoDebug.setKeyString_and_TextWstring({ "Key_Tool_not_found" });
                                    break;

                                case -4:
                                    TextInfoDebug.setKeyString_and_TextWstring({ "Key_Kit_not_found" });
                                    break;

                                case -5:
                                    TextInfoDebug.setKeyString_and_TextWstring({ "Key_Skill_not_found" });
                                    break;

                                case -6:
                                    TextInfoDebug.setKeyString_and_TextWstring({ "modifiers_noKit_noTool" });
                                    Error = -6;
                                    break;


                                default:
                                    TextInfoDebug.setKeyString_and_TextWstring({ "Unknown_error_RunSingleAssembly" });
                                    break;
                                }



                                Diagram.create(VideoMode({ LengthWindowDiagram, HeightWindowDiagram }), "Diagram");

                                VectorDiagramPack.emplace_back();
                                VectorWindowsDiagram.push_back(move(Diagram));
                                DiagramBool.push_back(false);

                            }
                            else
                            {
                                TextInfoDebug.setKeyString_and_TextWstring({ "mod_matches" });
                            }


                        }
                        else
                        {
                            TextInfoDebug.setKeyString_and_TextWstring({ "File_not_uploaded" });
                            PositionMod = InitialPosition = NullPosition;
                        }
                    }

















































                    // окно смены колличества повторений
                    else if (ShapeTargetIter.getRectangle().getGlobalBounds().contains(MouseWorldPosForMain))
                    {
                        ShapeTargetIter.setAsReleasedButton();
                        totalIterations < 10000 ? totalIterations += 1000 : totalIterations = 1000; 


                        ShapeTargetIter.setKeyString_and_TextWstring({ "Number_weapon_assemblies", to_wstring(totalIterations) });
                    }

























                    








                    // Окно параметров
                    else if (ShaheGeneralMod.getRectangle().getGlobalBounds().contains(MouseWorldPosForMain))
                    {
                        // нажали по кнопке 
                        for ( int it = 0; it < GeneralModOption.size(); it++)
                        {
                            if (GeneralModOption[it].MarkButtonCharacteristic.getRectangle().getGlobalBounds().contains(MouseWorldPosForMain))
                            {
                                
                                for (int it2 = 0; it2 < GeneralModOption.size(); it2++)
                                {
                                    GeneralModOption[it2].MarkButtonCharacteristic.setAsReleasedButton();
                                }

                                GeneralModOption[it].MarkButtonCharacteristic.setAsPressedButton();                                   
                                break;
                            }
                        }



                        

                        // нажали применить как мод
                        if (ButtonGeneralModApplyStat.getRectangle().getGlobalBounds().contains(MouseWorldPosForMain))
                        {                          
                                                        
                            ButtonGeneralModApplyStat.setAsReleasedButton();

                            if (!CurrentCharacteristicGun.is_Empty())
                            {                            
                                for (int it = 0; it < GeneralModOption.size(); it++)
                                {
                                    if (GeneralModOption[it].MarkButtonCharacteristic.isPressed()) 
                                    {
                                        if (PositionMod[it] < CurrentCharacteristicGun.getMaxPositionCharacteristic()[ GunStats::Trans_INDEX_to_INDEXGUNSTAT[it] ] )
                                        {   


                                            //////////////////////////////////////////////////////////////////////////////////////////////
                                            auto RealPercent =
                                                CalculateSuccessChance(
                                                    CurrentCharacteristicGun.getChanceFor_NEXT_Stat(GunStats::Trans_INDEX_to_INDEXGUNSTAT[it]),
                                                    GunStats::Modifiers::GetToolModifier(CurrentModifiers.Tool),
                                                    GunStats::Modifiers::GetKitModifier(CurrentModifiers.Kit),
                                                    GunStats::Modifiers::GetSkillModifier(CurrentModifiers.Skill)
                                                );
                                            //////////////////////////////////////////////////////////////////////////////////////////////

                                            if (!PressedApplyAsMod)
                                            {
                                                for (int i = 0; i < GunStats::COUNT_CHARACTERISTIC; i++)
                                                {
                                                    INITIAL_CHARACTERISTIC[i] = CurrentCharacteristicGun.getVisualPercentUpgradeCharacteristic(GunStats::Trans_INDEX_to_INDEXGUNSTAT[i]);
                                                }
                                            }


                                            if (CurrentCharacteristicGun.upgradeStat(GunStats::Trans_INDEX_to_INDEXGUNSTAT[it]))
                                            {




                                                //////////////////////////////////////////////////////////////////////////////////////////////
                                                PositionMod = CurrentCharacteristicGun.getCurrentPosition();
                                                moveElement(PositionMod, 4, 2);

                                                // вектор уменьшения характеристик по GunStats
                                                auto Des = CurrentCharacteristicGun.getDecreaseForCurrentStat(GunStats::Trans_INDEX_to_INDEXGUNSTAT[it]);
                                                moveElement(Des, 4, 2);

                                                for (int i = 0; i < GunStats::COUNT_CHARACTERISTIC; i++)
                                                { 
                                                    InitialPosition[i] -= Des[i];
                                                }
                                                //////////////////////////////////////////////////////////////////////////////////////////////




                                                



                                                //////////////////////////////////////////////////////////////////////////////////////////////
                                                HistoryPosition.push_back({ PositionMod,  InitialPosition });
                                                method_MOD.push_back({ GunStats::Trans_INDEX_to_INDEXGUNSTAT[it], RealPercent, {CurrentModifiers}, CurrentCharacteristicGun.getVisualPercentUpgradeCharacteristic(GunStats::Trans_INDEX_to_INDEXGUNSTAT[it]), CurrentPrice });
                                                //////////////////////////////////////////////////////////////////////////////////////////////



                                                //////////////////////////////////////////////////////////////////////////////////////////////
                                                PressedApplyAsMod = true;
                                                ShapeGeneralModApplyStatInitial.setAsReleasedButton();
                                                //////////////////////////////////////////////////////////////////////////////////////////////                                              


                                                TextInfoDebug.setKeyString_and_TextWstring({ "Applied_mod_step_recorded" });
                                            }
                                        }
                                        else
                                        {
                                            TextInfoDebug.setKeyString_and_TextWstring({ "Cannot_improved_beyond_maximum" });
                                        }
                                        break;
                                    }

                                }
                            }
                            else
                            {
                                TextInfoDebug.setKeyString_and_TextWstring({ "Data_not_loaded" });
                            }
                        }





                        // нажали применить как начальную
                        else if (ShapeGeneralModApplyStatInitial.getRectangle().getGlobalBounds().contains(MouseWorldPosForMain))
                        {
                            if (!PressedApplyAsMod)
                            {                               

                                ShapeGeneralModApplyStatInitial.setAsReleasedButton();

                                if (!CurrentCharacteristicGun.is_Empty())
                                {

                                    for (int it = 0; it < GeneralModOption.size(); it++)
                                    {

                                        if (GeneralModOption[it].MarkButtonCharacteristic.isPressed())
                                        {

                                            if (CurrentCharacteristicGun.upgradeStat(GunStats::Trans_INDEX_to_INDEXGUNSTAT[it]))
                                            {

                                                InitialPosition = CurrentCharacteristicGun.getCurrentPosition();
                                                moveElement(InitialPosition, 4, 2);

                                                PositionMod = InitialPosition;

                                                for (int i = 0; i < GunStats::COUNT_CHARACTERISTIC; i++)
                                                {
                                                    INITIAL_CHARACTERISTIC[i] = CurrentCharacteristicGun.getVisualPercentUpgradeCharacteristic(GunStats::Trans_INDEX_to_INDEXGUNSTAT[i]);
                                                }

                                                HistoryPosition.push_back({ PositionMod,  InitialPosition });

                                                TextInfoDebug.setKeyString_and_TextWstring({ "Marked_as_initial" });
                                            }
                                            else
                                            {
                                                TextInfoDebug.setKeyString_and_TextWstring({ "Cannot_improved_beyond_maximum" });
                                            }
                                            break;
                                        }



                                    }
                                }
                                else
                                {
                                    TextInfoDebug.setKeyString_and_TextWstring({ "Data_not_loaded" } );
                                }
                            }
                            else
                            {
                                TextInfoDebug.setKeyString_and_TextWstring({ "Prohibited_after_applying_mod" });
                            }
                        }





                        // нажали сбросить
                        else if (ShapeGeneralModReset.getRectangle().getGlobalBounds().contains(MouseWorldPosForMain))
                        {
                            HistoryPosition.clear();
                            method_MOD.clear();

                            ShapeGeneralModReset.setAsReleasedButton();
                            CurrentCharacteristicGun.returnDefaultPosition();


                            InitialPosition = CurrentCharacteristicGun.getCurrentPosition();
                            moveElement(InitialPosition, 4, 2);
                            PositionMod = InitialPosition;

                            PressedApplyAsMod = false;                        


                            INITIAL_CHARACTERISTIC = DEFAULT_CHARACTERISTIC;
                            MOD_CHARACTERISTIC     = DEFAULT_CHARACTERISTIC;


                            TextInfoDebug.setKeyString_and_TextWstring({ "Mod_reset" });

                        }




                        //нажали вернуться на шаг  назад
                        else if (ShapeGeneralModBack.getRectangle().getGlobalBounds().contains(MouseWorldPosForMain))
                        {
                            ShapeGeneralModBack.setAsReleasedButton();
                            
                            //возвращаем шаг назад
                            if (!CurrentCharacteristicGun.is_Empty())
                            {
                                if (CurrentCharacteristicGun.stepBack())
                                {
                                    TextInfoDebug.setKeyString_and_TextWstring({ "Step_back" });

                                    // если была нажата принять как мод, удаляем последний элемент
                                    if (PressedApplyAsMod)
                                    {
                                        if (!method_MOD.empty())
                                        {
                                            method_MOD.pop_back();
                                        }
                                        else 
                                        { 
                                            TextInfoDebug.setKeyString_and_TextWstring({ "Error - vector was not reduced, it was empty" } );
                                        }



                                        if (method_MOD.empty()) { PressedApplyAsMod = false; }
                                    }
                                    if (!HistoryPosition.empty())
                                    {
                                        HistoryPosition.pop_back();

                                        auto cp_temp = CurrentCharacteristicGun.getCurrentPosition();
                                        moveElement(cp_temp, 4, 2);

                                        if (!HistoryPosition.empty())
                                        {
                                            PositionMod = HistoryPosition[HistoryPosition.size() - 1].first;
                                            InitialPosition = HistoryPosition[HistoryPosition.size() - 1].second;
                                        }
                                        else
                                        {
                                            PositionMod = InitialPosition = cp_temp;
                                        }

                                        // проверка всегда по PositionMod
                                        if (PositionMod != cp_temp) { TextInfoDebug.setKeyString_and_TextWstring({ L"ERROR 'Position != Check' " }); }
                                    }
                                    else { TextInfoDebug.setKeyString_and_TextWstring({ L"No more steps ERROR 'HistoryPosition' " }); }
                                }
                                else { TextInfoDebug.setKeyString_and_TextWstring({ "No_steps" }); }
                            }
                            else { TextInfoDebug.setKeyString_and_TextWstring({ "Cant_take_step_back_there_no_data" }); }
                        }




                        // размечаем процент
                        for (int it = 0; it < GeneralModOption.size(); it++)
                        {

                            GeneralModOption[it].TextCharacteristicVisualPercent.resetColorText();

                            if (!CurrentCharacteristicGun.is_Empty())
                            {
                                auto VPercent = CurrentCharacteristicGun.getVisualPercentUpgradeCharacteristic(GunStats::Trans_INDEX_to_INDEXGUNSTAT[it]);

                                if      (VPercent > 0) { GeneralModOption[it].TextCharacteristicVisualPercent.setColorText(Color::Green); }
                                else if (VPercent < 0) { GeneralModOption[it].TextCharacteristicVisualPercent.setColorText(Color::Red);   }

                                GeneralModOption[it].TextCharacteristicVisualPercent.setKeyString_and_TextWstring({ format(L"{:.1f}", VPercent), L"%" });

                                // если не нажата
                                if (!PressedApplyAsMod)
                                {
                                    INITIAL_CHARACTERISTIC[it] = VPercent;
                                    MOD_CHARACTERISTIC[it]     = VPercent;
                                }
                                else
                                {
                                    MOD_CHARACTERISTIC[it] = VPercent;
                                }

                            }
                            else
                            {
                                GeneralModOption[it].TextCharacteristicVisualPercent.setKeyString_and_TextWstring({ L"0.0%" });
                            }

                            // разметка шанса улучшения процента
                            if (GeneralModOption[it].MarkButtonCharacteristic.isPressed())
                            {
                                Set_Chahce_upgrade(CurrentCharacteristicGun, CurrentModifiers, TextGeneralModChance, it);
                            }


                            // размета значений характеристики
                            auto val = CurrentCharacteristicGun.getValueCharacteristic(GunStats::Trans_INDEX_to_INDEXGUNSTAT[ it]);

                            wstring WSval = it ? to_wstring(static_cast<int>(val)) : format(L"{:.1f}", val);

                            GeneralModOption[it].TextVisualCharacteristic.setKeyString_and_TextWstring({ WSval });
                        }

                        EditShaheUnits(GeneralModOption, CurrentCharacteristicGun, InitialPosition, PositionMod);
                    }





























                    //МОДИЦИФКАТОРЫ

                    // инструменты
                    for (int i = 0; i < ModifiersIcon.ToolRectangleGUI.size(); i++)
                    {
                        if (ModifiersIcon.ToolRectangleGUI[i].getRectangle().getGlobalBounds().contains(MouseWorldPosForMain))
                        {
                            for (auto& sh : ModifiersIcon.ToolRectangleGUI)
                            { 
                                sh.setAsReleasedButton();
                            }
                            ModifiersIcon.ToolRectangleGUI[i].setAsPressedButton();






                            string STip = toolpath[i].stem().string().erase(0, 3);

                            auto tool = GunStats::Modifiers::ToolType_begin;

                            for (tool++ ; tool < GunStats::Modifiers::ToolType_end; tool++)
                            {
                                if (GunStats::Modifiers::GetToolName(tool) == STip)
                                {
                                    break;
                                }
                            }
                            CurrentModifiers.Tool = tool;
                            TextInfoDebug.setKeyString_and_TextWstring({ "Select",  GunStats::Modifiers::GetToolName(tool) });



                            for (int it = 0; it < GeneralModOption.size(); it++)
                            {
                                if (GeneralModOption[it].MarkButtonCharacteristic.isPressed())
                                {
                                    Set_Chahce_upgrade(CurrentCharacteristicGun, CurrentModifiers, TextGeneralModChance, it);
                                    break;
                                }
                            }



                            /// стоимость ToolPrice
                            /////////////////////////////////////////////////////////

                            unsigned value_price = Get_Price(PathToPriceModifiers_JS, CurrentModifiers);
                            CurrentPrice.PriceTool = value_price;

                            for (auto& tp : ToolPrice)
                            {
                                if (!tp.isButton())
                                {
                                    tp.setKeyString_and_TextWstring({ format(L"{:L}", value_price), L"₽" });
                                    break;
                                }
                            }
                            /////////////////////////////////////////////////////////

                            break;
                        }
                    }
                    // наборы
                    for (int i = 0; i < ModifiersIcon.KitRectangleGUI.size(); i++)
                        {
                            if (ModifiersIcon.KitRectangleGUI[i].getRectangle().getGlobalBounds().contains(MouseWorldPosForMain))
                            {
                                for (auto& sh : ModifiersIcon.KitRectangleGUI)
                                {
                                    sh.setAsReleasedButton();
                                }
                                ModifiersIcon.KitRectangleGUI[i].setAsPressedButton();


                                string STip = kitpath[i].stem().string().erase(0, 3);

                                auto KitSelect = GunStats::Modifiers::KitType_begin;
                                for (KitSelect++; KitSelect < GunStats::Modifiers::KitType_end; KitSelect++)
                                {
                                    if (GunStats::Modifiers::GetKitName(KitSelect) == STip)
                                    {
                                        break;
                                    }
                                }
                                CurrentModifiers.Kit = KitSelect;
                                TextInfoDebug.setKeyString_and_TextWstring({ "Select", GunStats::Modifiers::GetKitName(KitSelect) });


                                for (int it = 0; it < GeneralModOption.size(); it++)
                                {
                                    if (GeneralModOption[it].MarkButtonCharacteristic.isPressed())
                                    {
                                        Set_Chahce_upgrade(CurrentCharacteristicGun, CurrentModifiers, TextGeneralModChance, it);
                                        break;
                                    }
                                }




                                /// стоимость KitPrice
                                /////////////////////////////////////////////////////////

                                unsigned value_price = Get_Price(PathToPriceModifiers_JS, CurrentModifiers, NameGun);
                                CurrentPrice.PriceKit = value_price;
                                for (auto& kp : KitPrice)
                                {
                                    if (!kp.isButton())
                                    {
                                        kp.setKeyString_and_TextWstring({ format(L"{:L}", value_price), L"₽" });
                                        break;
                                    }
                                }
                                /////////////////////////////////////////////////////////

                                break;
                            }
                        }
                    // скилы
                    for (int i = 0; i < ModifiersIcon.SkillRectangleGUI.size(); i++)
                    {
                        if (ModifiersIcon.SkillRectangleGUI[i].getRectangle().getGlobalBounds().contains(MouseWorldPosForMain))
                        {
                            for (auto& sh : ModifiersIcon.SkillRectangleGUI)
                            {
                                sh.setAsReleasedButton();
                            }
                            ModifiersIcon.SkillRectangleGUI[i].setAsPressedButton();



                            string STip = skillpath[i].stem().string().erase(0, 3);
                            auto SkillSelect = GunStats::Modifiers::SkillType_begin;
                            for (SkillSelect++; SkillSelect < GunStats::Modifiers::SkillType_end; SkillSelect++)
                            {
                                if (GunStats::Modifiers::GetSkillName(SkillSelect) == STip)
                                {                                    
                                    break;
                                }                                
                            }
                            CurrentModifiers.Skill = SkillSelect;
                            TextInfoDebug.setKeyString_and_TextWstring({ "Select", GunStats::Modifiers::GetSkillName(SkillSelect) });


                            for (int it = 0; it < GeneralModOption.size(); it++)
                            {
                                if (GeneralModOption[it].MarkButtonCharacteristic.isPressed())
                                {
                                    Set_Chahce_upgrade(CurrentCharacteristicGun, CurrentModifiers, TextGeneralModChance, it);
                                    break;
                                }
                            }


                            CurrentPrice.PriceSkill = 0;
                            break;
                        }
                    }





                    /// стоимость      
                    for (auto& GUI_ToolPrice : ToolPrice)
                    {
                        if (GUI_ToolPrice.getRectangle().getGlobalBounds().contains(MouseWorldPosForMain) && GUI_ToolPrice.isButton())
                        {
                            auto vvstr = *GUI_ToolPrice.getKeyString_and_TextWstring().begin();
                            if ( holds_alternative<wstring>(vvstr))
                            {
                                if (get<wstring>(vvstr) == L"--")
                                {
                                    GUI_ToolPrice.setAsReleasedButton();
                                    Set_Price_Tool(PathToPriceModifiers_JS, CurrentModifiers.Tool, CurrentPrice.PriceTool - 100'000);
                                }
                                else if (get<wstring>(vvstr) == L"-")
                                {
                                    GUI_ToolPrice.setAsReleasedButton();

                                    Set_Price_Tool(PathToPriceModifiers_JS, CurrentModifiers.Tool, CurrentPrice.PriceTool - 5'000);
                                }
                                else if (get<wstring>(vvstr) == L"+")
                                {
                                    GUI_ToolPrice.setAsReleasedButton();
                                    Set_Price_Tool(PathToPriceModifiers_JS, CurrentModifiers.Tool, CurrentPrice.PriceTool + 5'000);
                                }
                                else if (get<wstring>(vvstr) == L"++")
                                {
                                    GUI_ToolPrice.setAsReleasedButton();
                                    Set_Price_Tool(PathToPriceModifiers_JS, CurrentModifiers.Tool, CurrentPrice.PriceTool + 100'000);
                                }


                                /// стоимость ToolPrice
                                /////////////////////////////////////////////////////////

                                unsigned value_price = Get_Price(PathToPriceModifiers_JS, CurrentModifiers);
                                CurrentPrice.PriceTool = value_price;

                                for (auto& tp : ToolPrice)
                                {
                                    if (!tp.isButton())
                                    {
                                        tp.setKeyString_and_TextWstring({ format(L"{:L}", value_price), L"₽" });
                                        break;
                                    }
                                }
                                /////////////////////////////////////////////////////////
                            }
                        }
                    }


                    for (auto& GUI_KitPrice : KitPrice)
                    {
                        if (GUI_KitPrice.getRectangle().getGlobalBounds().contains(MouseWorldPosForMain) && GUI_KitPrice.isButton())
                        {
                            auto vvstr = *GUI_KitPrice.getKeyString_and_TextWstring().begin();
                            if (holds_alternative<wstring>(vvstr))
                            {
                                if (get<wstring>(vvstr) == L"--")
                                {
                                    GUI_KitPrice.setAsReleasedButton();

                                    Set_Price_Kit(PathToPriceModifiers_JS, CurrentModifiers.Kit, NameGun, CurrentPrice.PriceKit - 100'000);
                                }
                                else if (get<wstring>(vvstr) == L"-")
                                {
                                    GUI_KitPrice.setAsReleasedButton();
                                    Set_Price_Kit(PathToPriceModifiers_JS, CurrentModifiers.Kit, NameGun, CurrentPrice.PriceKit - 5'000);
                                }
                                else if (get<wstring>(vvstr) == L"+")
                                {
                                    GUI_KitPrice.setAsReleasedButton();
                                    Set_Price_Kit(PathToPriceModifiers_JS, CurrentModifiers.Kit, NameGun, CurrentPrice.PriceKit + 5'000);
                                }
                                else if (get<wstring>(vvstr) == L"++")
                                {
                                    GUI_KitPrice.setAsReleasedButton();
                                    Set_Price_Kit(PathToPriceModifiers_JS, CurrentModifiers.Kit, NameGun, CurrentPrice.PriceKit + 100'000);
                                }


                                /// стоимость KitPrice
                                /////////////////////////////////////////////////////////

                                unsigned value_price = Get_Price(PathToPriceModifiers_JS, CurrentModifiers, NameGun);
                                CurrentPrice.PriceKit = value_price;
                                for (auto& kp : KitPrice)
                                {
                                    if (!kp.isButton())
                                    {
                                        kp.setKeyString_and_TextWstring({ format(L"{:L}", value_price), L"₽" });
                                        break;
                                    }
                                }
                                /////////////////////////////////////////////////////////
                            }
                        }
                    }
                }
            }



            // прокрутка
            else if (const auto& mouseScroll = event->getIf<Event::MouseWheelScrolled>())
            {
                if (ALLGunShapeScrollGUI.getRectangle().getGlobalBounds().contains(MouseWorldPosForMain))
                {
                    if (mouseScroll->delta < 0 && it_VectorGun < VectorGun.size() - CountGunList)
                    {
                        it_VectorGun++;


                        wstring STLSTR = L"";
                        if (it_VectorGun != 0)
                        {
                            STLSTR = to_wstring(it_VectorGun) + L"+";
                        }
                        TopScrollToolTip.setKeyString_and_TextWstring({ STLSTR });


                        STLSTR = L"";
                        if (VectorGun.size() - (it_VectorGun + CountGunList) > 0)
                        {
                            STLSTR = to_wstring(VectorGun.size() - (it_VectorGun + CountGunList )) + "+";
                        }
                        DownScrollToolTip.setKeyString_and_TextWstring({ STLSTR });

                    }
                    else if (mouseScroll->delta > 0 && it_VectorGun > 0)
                    {
                        it_VectorGun--;

                        wstring STLSTR = L"";
                        if (it_VectorGun != 0)
                        {
                            STLSTR = to_wstring(it_VectorGun) + L"+";
                        }
                        TopScrollToolTip.setKeyString_and_TextWstring({ STLSTR });


                        STLSTR = L"";
                        if (VectorGun.size() - (it_VectorGun + CountGunList ) > 0)
                        {
                            STLSTR = to_wstring(VectorGun.size() - (it_VectorGun + CountGunList )) + "+";
                        }
                        DownScrollToolTip.setKeyString_and_TextWstring({ STLSTR });

                    }
                }
            }

        }



        ////////////////////////////////////////////////////////////////////////////////////

        window.clear(Color::White);
        window.draw(background);
        ////////////////////////////////////////////////////////////////////////////////////







        ////////////////////////////////////////////////////////////////////////////////////
        for (auto& GUI : VectorLangGUI)
        {
            window.draw(GUI.getRectangle());
            window.draw(GUI.getText());
        }
        ////////////////////////////////////////////////////////////////////////////////////





        ////////////////////////////////////////////////////////////////////////////////////       


        for (auto& GUI : VectorThemeGUI)
        {
            window.draw(GUI.getRectangle());
            window.draw(GUI.getText());
        }

        ////////////////////////////////////////////////////////////////////////////////////





        // модификаторы
        ////////////////////////////////////////////////////////////////////////////////////
        for (auto& GUI : ModifiersIcon.ToolRectangleGUI)
        {
            window.draw(GUI.getRectangle());
            //window.draw(GUI.getText());
        }
        for (auto& GUI : ModifiersIcon.KitRectangleGUI)
        {
            window.draw(GUI.getRectangle());
            //window.draw(GUI.getText());
        }
        for (auto& GUI : ModifiersIcon.SkillRectangleGUI)
        {
            window.draw(GUI.getRectangle());
            //window.draw(GUI.getText());
        }


        
        /// стоимость
        for (auto& GUI : ToolPrice)
        {
            window.draw(GUI.getRectangle());
            window.draw(GUI.getText());
        }
        for (auto& GUI : KitPrice)
        {
            window.draw(GUI.getRectangle());
            window.draw(GUI.getText());
        }
        window.draw(SkillPrice.getRectangle());
        window.draw(SkillPrice.getText());
        
        
        ////////////////////////////////////////////////////////////////////////////////////









        ////////////////////////////////////////////////////////////////////////////////////

        window.draw(ALLGunShapeScrollGUI.getRectangle());
        //window.draw(ALLGunShapeScrollGUI.getText());


        window.draw(SelectWeaponGUI.getRectangle());
        window.draw(SelectWeaponGUI.getText());


        for (int it1 = 0, it_VectorGun2 = it_VectorGun; it1 < CountGunList; it1++, it_VectorGun2++)
        {

            VectorGun[it_VectorGun2].WeapoGUI.setPositionRectangle(Vector2f(0, SizeCell_default / 2 * it1));

            window.draw(VectorGun[it_VectorGun2].WeapoGUI.getRectangle());
            window.draw(VectorGun[it_VectorGun2].WeapoGUI.getText());            
        }     

        window.draw(TopScrollToolTip.getRectangle());
        window.draw(TopScrollToolTip.getText());


        window.draw(DownScrollToolTip.getRectangle());
        window.draw(DownScrollToolTip.getText());

        ////////////////////////////////////////////////////////////////////////////////////








        ////////////////////////////////////////////////////////////////////////////////////

        window.draw(Calculation.getRectangle());
        window.draw(Calculation.getText());

        ////////////////////////////////////////////////////////////////////////////////////











        ////////////////////////////////////////////////////////////////////////////////////

        window.draw(ShaheGeneralMod.getRectangle());
        window.draw(ShaheGeneralMod.getText());


        for (int it1 = 0; it1 < GeneralModOption.size(); it1++)
        {

            window.draw(GeneralModOption[it1].TextCharacteristicName.getRectangle());
            window.draw(GeneralModOption[it1].TextCharacteristicName.getText());


            window.draw(GeneralModOption[it1].TextVisualCharacteristic.getRectangle());
            window.draw(GeneralModOption[it1].TextVisualCharacteristic.getText());


            window.draw(GeneralModOption[it1].TextCharacteristicVisualPercent.getRectangle());
            window.draw(GeneralModOption[it1].TextCharacteristicVisualPercent.getText());






            window.draw(GeneralModOption[it1].ShapeCharacteristic);

            window.draw(GeneralModOption[it1].MarkButtonCharacteristic.getRectangle());
            //window.draw(GeneralModOption[it1].MarkButtonCharacteristic.getText()); // текста нет


            bool Select = GeneralModOption[it1].MarkButtonCharacteristic.isPressed();

            // метка юнита
            size_t ps = PositionMod[it1];

            if (ps >= GeneralModOption[it1].ShaheUnitsInCharacteristic.size())
            {
                ps = GeneralModOption[it1].ShaheUnitsInCharacteristic.size() - 1;
            }

            else if (Select && ps >= CurrentCharacteristicGun.getMaxPositionCharacteristic()[GunStats::Trans_INDEX_to_INDEXGUNSTAT[it1]])
            {
                GeneralModOption[it1].ShaheUnitsInCharacteristic[ps].setOutlineColor(Color(128, 128, 128));
            }
            else if (Select)
            {
                GeneralModOption[it1].ShaheUnitsInCharacteristic[ps].setOutlineColor(Color::Green);
            }


            for (int it2 = 0; it2 < GeneralModOption[it1].ShaheUnitsInCharacteristic.size(); it2++)
            {
                window.draw(GeneralModOption[it1].ShaheUnitsInCharacteristic[it2]);
            };


            if (Select)
            {
                GeneralModOption[it1].ShaheUnitsInCharacteristic[ps].setOutlineColor(Color::Black);
            };
        }


        //window.draw(TextGeneralModChance.getRectangle());
        window.draw(TextGeneralModChance.getText());


        window.draw(ShapeGeneralModApplyStatInitial.getRectangle());
        window.draw(ShapeGeneralModApplyStatInitial.getText());


        window.draw(ButtonGeneralModApplyStat.getRectangle());
        window.draw(ButtonGeneralModApplyStat.getText());


        window.draw(ShapeGeneralModReset.getRectangle());
        window.draw(ShapeGeneralModReset.getText());


        window.draw(ShapeGeneralModBack.getRectangle());
        window.draw(ShapeGeneralModBack.getText());
        
        ////////////////////////////////////////////////////////////////////////////////////













        ////////////////////////////////////////////////////////////////////////////////////

        window.draw(TextInfoDebug.getRectangle());
        window.draw(TextInfoDebug.getText());

        ////////////////////////////////////////////////////////////////////////////////////














        ////////////////////////////////////////////////////////////////////////////////////

        window.draw(ShapeTargetIter.getRectangle());
        window.draw(ShapeTargetIter.getText());

        ////////////////////////////////////////////////////////////////////////////////////






     

        ///////////////////////////     тоолтипы       /////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////

        // инструменты
        for (int i = 0; i < ModifiersIcon.ToolRectangleGUI.size(); i++)
        {
            if (ModifiersIcon.ToolRectangleGUI[i].getRectangle().getGlobalBounds().contains(MouseWorldPosForMain))
            {

                string STip = toolpath[i].stem().string().erase(0, 3);
                wstring Tip = LANG_TRANSLATE(STip);

                RectangleShape ShapeTool_tooltip;
                ShapeTool_tooltip.setOutlineColor(Color::Black);
                ShapeTool_tooltip.setOutlineThickness(-2);
                ShapeTool_tooltip.setSize(Vector2f(static_cast<float>(15.f + 7.f * Tip.size()), SizeCell_default / 3.f));
                ShapeTool_tooltip.setPosition(Vector2f(MouseWorldPosForMain.x, MouseWorldPosForMain.y - SizeCell_default / 3.f));



                Text TextTool_tooltip(ThemeGui::CurrentFont);
                TextTool_tooltip.setString(Tip);
                TextTool_tooltip.setCharacterSize(static_cast<unsigned>(ThemeGui::CharacterSize * 0.8f));
                TextTool_tooltip.setFillColor(Color::Black);


                auto rectPos = ShapeTool_tooltip.getPosition();
                auto rectSize = ShapeTool_tooltip.getSize();
                auto textBounds = TextTool_tooltip.getLocalBounds();


                TextTool_tooltip.setOrigin(Vector2f(0.f, textBounds.position.y + textBounds.size.y / 2.0f));
                TextTool_tooltip.setPosition(Vector2f(rectPos.x + 10, rectPos.y + rectSize.y / 2.0f));
               

                window.draw(ShapeTool_tooltip);
                window.draw(TextTool_tooltip);

            }
        }
        // наборы
        for (int i = 0; i < ModifiersIcon.KitRectangleGUI.size(); i++)
        {
            if (ModifiersIcon.KitRectangleGUI[i].getRectangle().getGlobalBounds().contains(MouseWorldPosForMain))
            {
                string STip = kitpath[i].stem().string().erase(0, 3);
                wstring Tip = LANG_TRANSLATE(STip);


                RectangleShape ShapeKit_tooltip;
                ShapeKit_tooltip.setOutlineColor(Color::Black);
                ShapeKit_tooltip.setOutlineThickness(-2);
                ShapeKit_tooltip.setSize(Vector2f(static_cast<float>(15.f + 7.f * Tip.size()), SizeCell_default / 3.f));
                ShapeKit_tooltip.setPosition(Vector2f(MouseWorldPosForMain.x, MouseWorldPosForMain.y - SizeCell_default / 3.f));



                Text TextKit_tooltip(ThemeGui::CurrentFont);
                TextKit_tooltip.setString(Tip);
                TextKit_tooltip.setCharacterSize(static_cast<unsigned>(ThemeGui::CharacterSize * 0.8f));
                TextKit_tooltip.setFillColor(Color::Black);



                auto textBounds = TextKit_tooltip.getLocalBounds();
                auto rectPos = ShapeKit_tooltip.getPosition();
                auto rectSize = ShapeKit_tooltip.getSize();

                TextKit_tooltip.setOrigin(Vector2f(0.f, textBounds.position.y + textBounds.size.y / 2.0f));
                TextKit_tooltip.setPosition(Vector2f(rectPos.x + 10, rectPos.y + rectSize.y / 2.0f));


                window.draw(ShapeKit_tooltip);
                window.draw(TextKit_tooltip);
            }
        }
        // скиллы
        for (int i = 0; i < ModifiersIcon.SkillRectangleGUI.size(); i++)
        {
            if (ModifiersIcon.SkillRectangleGUI[i].getRectangle().getGlobalBounds().contains(MouseWorldPosForMain))
            {
                string STip = skillpath[i].stem().string().erase(0, 3);
                wstring Tip = LANG_TRANSLATE(STip);

                RectangleShape ShapeSkill_tooltip;
                ShapeSkill_tooltip.setOutlineColor(Color::Black);
                ShapeSkill_tooltip.setOutlineThickness(-2);
                ShapeSkill_tooltip.setPosition(Vector2f(MouseWorldPosForMain.x, MouseWorldPosForMain.y - SizeCell_default / 3.f));
                ShapeSkill_tooltip.setSize(Vector2f(static_cast<float>(15.f + 7.f * Tip.size()), SizeCell_default / 3.f));



                Text TextSkill_tooltip(ThemeGui::CurrentFont);
                TextSkill_tooltip.setString(Tip);
                TextSkill_tooltip.setCharacterSize(static_cast<unsigned>(ThemeGui::CharacterSize * 0.8f));
                TextSkill_tooltip.setFillColor(Color::Black);


                
                auto rectPos = ShapeSkill_tooltip.getPosition();
                auto rectSize = ShapeSkill_tooltip.getSize();
                auto textBounds = TextSkill_tooltip.getLocalBounds();

                TextSkill_tooltip.setOrigin(Vector2f(0.f, textBounds.position.y + textBounds.size.y / 2.0f));
                TextSkill_tooltip.setPosition(Vector2f(rectPos.x + 10, rectPos.y + rectSize.y / 2.0f));



                window.draw(ShapeSkill_tooltip);
                window.draw(TextSkill_tooltip);

            }
        }

        ////////////////////////////////////////////////////////////////////////////////







        ////////////////////////////////////////////////////////////////////////////////


        window.display();


        ////////////////////////////////////////////////////////////////////////////////






























































        ////////////////////////////////////////////////////////////////////////////////
        // отрисовка диаграмм
        for (int iter_graf = static_cast<int>(VectorWindowsDiagram.size()) - 1; iter_graf >= 0; --iter_graf)
        {
            if (VectorWindowsDiagram[iter_graf].isOpen())
            {
                auto PositionMouseForGraf = Mouse::getPosition(VectorWindowsDiagram[iter_graf]);
                auto MouseWorldPosForGraf = window.mapPixelToCoords(PositionMouseForGraf);
                unsigned int MethodList = 22;

                while (const optional event = VectorWindowsDiagram[iter_graf].pollEvent())
                {
                    if (event->is<Event::Closed>())
                    {
                        VectorWindowsDiagram[iter_graf].close();
                        DiagramBool.erase(DiagramBool.begin() + iter_graf);
                        VectorWindowsDiagram.erase(VectorWindowsDiagram.begin() + iter_graf);
                        VectorDiagramPack.erase(VectorDiagramPack.begin() + iter_graf);

                        break; // чтобы не обращаться к уже удалённому i
                    }
                    else if (const auto& mouseButtonPressed = event->getIf<Event::MouseButtonPressed>())
                    {
                        if (mouseButtonPressed->button == Mouse::Button::Left)
                        {

                            // если прокрутка доступна
                            if (VectorDiagramPack[iter_graf].GUIButtonTop.get() != nullptr && VectorDiagramPack[iter_graf].GUIButtonDown.get() != nullptr)
                            {

                                auto size = VectorDiagramPack[iter_graf].GUIMethodModifiers.size();

                                // кнопка прокрутки верхняя
                                if (VectorDiagramPack[iter_graf].GUIButtonTop.get()->getRectangle().getGlobalBounds().contains(MouseWorldPosForGraf))
                                {
                                    // но не более размера 
                                    if (VectorDiagramPack[iter_graf].InitialIndexMethod > 0)
                                    {
                                        VectorDiagramPack[iter_graf].InitialIndexMethod--;
                                    }
                                }

                                //кнопка прокрутки вниз
                                else if (VectorDiagramPack[iter_graf].GUIButtonDown.get()->getRectangle().getGlobalBounds().contains(MouseWorldPosForGraf))
                                {
                                    // но не более размера 
                                    if (VectorDiagramPack[iter_graf].InitialIndexMethod < size)
                                    {
                                        VectorDiagramPack[iter_graf].InitialIndexMethod++;
                                    }
                                }

                                // корректировка 
                                auto SizeRec = VectorDiagramPack[iter_graf].GUIMethodModifiers.begin()->getRectangle().getSize();
                                auto XSizeWindow = VectorWindowsDiagram[iter_graf].getSize().x;

                                for (size_t i = VectorDiagramPack[iter_graf].InitialIndexMethod; i < MethodList || i < size; i++)
                                {
                                    VectorDiagramPack[iter_graf].GUIMethodModifiers[i].setPositionRectangle(Vector2f(XSizeWindow - SizeRec.x, SizeRec.y * (i - VectorDiagramPack[iter_graf].InitialIndexMethod)));
                                }
                            }







                            bool NeedRefresh = false;
                            float SizeValBN = 0.010f;
                            float SizeValSM = 0.005f;
                            for (auto& GUIBinBt : VectorDiagramPack[iter_graf].GUIPercentBinBt)
                            {
                                auto vbt = *GUIBinBt.getKeyString_and_TextWstring().begin();
                                if (GUIBinBt.getRectangle().getGlobalBounds().contains(MouseWorldPosForGraf) && GUIBinBt.isButton() && holds_alternative<wstring>(vbt) && get<wstring>(vbt) == L"-")
                                {
                                    GUIBinBt.setAsReleasedButton();
                                    if (VectorDiagramPack[iter_graf].PercentBin - SizeValBN >= 0.f)
                                    {
                                        VectorDiagramPack[iter_graf].PercentBin -= SizeValBN;
                                        NeedRefresh = true;
                                    }
                                    break;
                                }
                                else if (GUIBinBt.getRectangle().getGlobalBounds().contains(MouseWorldPosForGraf) && GUIBinBt.isButton() && holds_alternative<wstring>(vbt) && get<wstring>(vbt) == L"+")
                                {
                                    GUIBinBt.setAsReleasedButton();
                                    if (VectorDiagramPack[iter_graf].PercentBin + SizeValBN <= 1.f)
                                    {
                                        VectorDiagramPack[iter_graf].PercentBin += SizeValBN;
                                        NeedRefresh = true;
                                    }  
                                    break;
                                }

                            }
                            for (auto& GUISmoothBt : VectorDiagramPack[iter_graf].GUIPercentSmoothBt)
                            {

                                auto vbt = *GUISmoothBt.getKeyString_and_TextWstring().begin();
                                if (GUISmoothBt.getRectangle().getGlobalBounds().contains(MouseWorldPosForGraf) && GUISmoothBt.isButton() && holds_alternative<wstring>(vbt) && get<wstring>(vbt) == L"-")
                                {
                                    GUISmoothBt.setAsReleasedButton();

                                    if (VectorDiagramPack[iter_graf].PercentSmooth - SizeValSM >= 0.f)
                                    {
                                        VectorDiagramPack[iter_graf].PercentSmooth -= SizeValSM;
                                        NeedRefresh = true;
                                    }
                                    break;
                                }
                                else if (GUISmoothBt.getRectangle().getGlobalBounds().contains(MouseWorldPosForGraf) && GUISmoothBt.isButton() && holds_alternative<wstring>(vbt) && get<wstring>(vbt) == L"+")
                                {
                                    GUISmoothBt.setAsReleasedButton();

                                    if (VectorDiagramPack[iter_graf].PercentSmooth + SizeValSM <= 1.f)
                                    {
                                        VectorDiagramPack[iter_graf].PercentSmooth += SizeValSM;
                                        NeedRefresh = true;
                                    }
                                    break;
                                }
                            }
                            if (NeedRefresh)
                            {
                                for (auto& GUISmoothBt : VectorDiagramPack[iter_graf].GUIPercentSmoothBt)
                                {
                                    auto vbt = *GUISmoothBt.getKeyString_and_TextWstring().begin();
                                    if (holds_alternative<wstring>(vbt) && !GUISmoothBt.isButton())
                                    {
                                        GUISmoothBt.setKeyString_and_TextWstring({ format( L"{:.3f}", VectorDiagramPack[iter_graf].PercentSmooth)});
                                        break;

                                    }
                                }
                                for (auto& GUIBinBt : VectorDiagramPack[iter_graf].GUIPercentBinBt)
                                {
                                    auto vbt = *GUIBinBt.getKeyString_and_TextWstring().begin();
                                    if (holds_alternative<wstring>(vbt) && !GUIBinBt.isButton())
                                    {
                                        GUIBinBt.setKeyString_and_TextWstring({ format(L"{:.3f}", VectorDiagramPack[iter_graf].PercentBin) });
                                        break;
                                    }
                                }

                                auto BinnedFreqMap = GetBinedFreqModifiers(VectorDiagramPack[iter_graf].CurrentSourceFreqModifiers, VectorDiagramPack[iter_graf].PercentBin); // сколько % оставим? (%)
                                auto SmoothedFreqMap = GetSmoothFreqModifiers(BinnedFreqMap, VectorDiagramPack[iter_graf].PercentSmooth); // насколько сильно (%)

                                VectorDiagramPack[iter_graf].CurrentSmoothFreqModifiers = SmoothedFreqMap;

                                VectorDiagramPack[iter_graf].CurrentCurvePack.Curve = CurveFun(VectorDiagramPack[iter_graf].CurrentCurvePack.MainRectangleCurve, VectorDiagramPack[iter_graf].ColorCurve, VectorDiagramPack[iter_graf].CurrentSmoothFreqModifiers);
                            }
                        }
                    }
                }

                if (iter_graf < VectorWindowsDiagram.size() )
                {

                    if (!DiagramBool[iter_graf])
                    {
                        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                        if (ALL_modifiers.empty())
                        {
                            OutputLog("main -> ALL_modifiers empty");
                            continue;
                        }

                        if (All_Random_Attempt_Used.empty())
                        {
                            OutputLog("main -> All_Random_Attempt_Used empty");
                            continue;
                        }
                        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


                        // стандартный вид
                        VectorDiagramPack[iter_graf].PercentBin = 1.0f;
                        VectorDiagramPack[iter_graf].PercentSmooth = 0.05f;



                        // на сколько делим цену ? 
                        float PriceValueDegree = 1'000'000; // до KK
                        VectorDiagramPack[iter_graf].ColorCurve = Color::Red;


                        // Получаем позицию главного окна
                        Vector2i mainPos = VectorWindowsDiagram[iter_graf].getPosition();
                        Vector2u mainSize = VectorWindowsDiagram[iter_graf].getSize();

                        // Центрируем текущий Diagram поверх главного окна
                        int xPosDiagram = mainPos.x + (static_cast<int>(mainSize.x) - static_cast<int>(LengthWindowDiagram)) / 2;
                        int yPosDiagram = mainPos.y + (static_cast<int>(mainSize.y) - static_cast<int>(HeightWindowDiagram)) / 2;

                        VectorWindowsDiagram[iter_graf].setPosition(Vector2i(xPosDiagram, yPosDiagram));


































                        // оформление кривой
                        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

                        float XposPack = VectorWindowsDiagram[iter_graf].getSize().x * 0.1f;
                        float YposPack = VectorWindowsDiagram[iter_graf].getSize().y * 0.5f;

                        float XsizePack = VectorWindowsDiagram[iter_graf].getSize().x * 0.4f;
                        float YsizePack = VectorWindowsDiagram[iter_graf].getSize().y * 0.4f;



                        RectangleShape RectangleForCurve;
                        RectangleForCurve.setSize({ XsizePack, YsizePack });
                        RectangleForCurve.setPosition({ XposPack, YposPack });
                        RectangleForCurve.setFillColor(Color(255, 255, 255, 220));
                        RectangleForCurve.setOutlineColor(Color::Black);
                        RectangleForCurve.setOutlineThickness(-1);
                        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


                        //"сырые" частоты
                        auto SourceFreqMap = GetFreqModifiers(ALL_modifiers);

                        auto BinnedFreqMap = GetBinedFreqModifiers(SourceFreqMap, VectorDiagramPack[iter_graf].PercentBin); // сколько % оставим? (%)

                        auto SmoothedFreqMap = GetSmoothFreqModifiers(BinnedFreqMap, VectorDiagramPack[iter_graf].PercentSmooth); // насколько сильно (%)



                        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

                        VectorDiagramPack[iter_graf].CurrentSmoothFreqModifiers = SmoothedFreqMap;
                        VectorDiagramPack[iter_graf].CurrentSourceFreqModifiers = SourceFreqMap;

                        string NameCurve;
                        switch (Error)
                        {
                        case 0:
                            NameCurve = "modifiers";
                            break;


                        case -6:
                            NameCurve = "modifiers_noKit_noTool";
                            break;


                        default:
                            NameCurve = "Error";
                            break;
                        }

                        VectorDiagramPack[iter_graf].CurrentCurvePack = GetCurveFrequrency(RectangleForCurve, VectorDiagramPack[iter_graf].CurrentSmoothFreqModifiers, VectorDiagramPack[iter_graf].ColorCurve, NameCurve);


                        //////////////////////////////////////////////////////////////////////////////////////////////////////////




































                        // оформление 50-ого перцентиля
                        //////////////////////////////////////////////////////////////////////////////////////////////////////////


                        XposPack = VectorWindowsDiagram[iter_graf].getSize().x * 0.01f;
                        YposPack = VectorWindowsDiagram[iter_graf].getSize().y * 0.01f;

                        XsizePack = SizeCell_default * 3.5f;
                        YsizePack = SizeCell_default * 0.3f;

                        auto InfoPercentile_50 = GetInfoForModifiers(GetPercentile(VectorDiagramPack[iter_graf].CurrentSourceFreqModifiers, 50.0));
                        {
                            vector<variant<string, wstring>> VResultWS = { "Percentile_50" };

                            GUI_TextAndRectangle PercentileGuiName = [XposPack, YposPack, XsizePack, YsizePack, VResultWS]()
                                {
                                    RectangleShape RectangleTemp;
                                    RectangleTemp.setSize({ XsizePack * 2, YsizePack });
                                    RectangleTemp.setPosition({ XposPack, YposPack });
                                    return GUI_TextAndRectangle(VResultWS, RectangleTemp, false, false);

                                }();
                            VectorDiagramPack[iter_graf].GUIPercentile50.push_back(PercentileGuiName);


                            for (const auto& info_item : InfoPercentile_50)
                            {                                
                                XposPack = VectorDiagramPack[iter_graf].GUIPercentile50.begin()->getRectangle().getPosition().x;
                                YposPack = VectorDiagramPack[iter_graf].GUIPercentile50.rbegin()->getRectangle().getPosition().y + YsizePack;

                                VResultWS = { info_item.first };

                                GUI_TextAndRectangle PercentileGuiInfo = [XposPack, YposPack, XsizePack, YsizePack, VResultWS]()
                                    {
                                        RectangleShape RectangleTemp;
                                        RectangleTemp.setSize({ XsizePack, YsizePack });
                                        RectangleTemp.setPosition({ XposPack, YposPack });
                                        return GUI_TextAndRectangle(VResultWS, RectangleTemp, false, false);

                                    }();

                                VectorDiagramPack[iter_graf].GUIPercentile50.push_back(PercentileGuiInfo);

                                PercentileGuiInfo.setPositionRectangle(PercentileGuiInfo.getPositionRectangle() + Vector2f( PercentileGuiInfo.getRectangle().getSize().x, 0 ));

                                vector<variant<string, wstring>> NameVal = { format(L"{:L}", info_item.second) } ;

                                if (info_item.first == "AllPrice")
                                {
                                    NameVal.push_back(L"->");
                                    NameVal.push_back(format(L"{:L}", static_cast<float>(info_item.second / PriceValueDegree)));
                                    NameVal.push_back("kk");
                                }
                                else
                                {
                                    NameVal.push_back("units");
                                }

                                PercentileGuiInfo.setKeyString_and_TextWstring(NameVal);
                                VectorDiagramPack[iter_graf].GUIPercentile50.push_back(PercentileGuiInfo);

                            }
                        }
                         
                        //////////////////////////////////////////////////////////////////////////////////////////////////////////







































                       // оформление начального и конечного мода
                       //////////////////////////////////////////////////////////////////////////////////////////////////////////

                        auto size = VectorDiagramPack[iter_graf].GUIPercentile50.rbegin()->getRectangle().getSize();
                        XsizePack = size.x * 2.3f;
                        YsizePack = size.y;

                        XposPack = VectorDiagramPack[iter_graf].GUIPercentile50.begin()->getPositionRectangle().x;
                        YposPack = VectorDiagramPack[iter_graf].GUIPercentile50.rbegin()->getPositionRectangle().y + size.y;


                        GUI_TextAndRectangle GInitialMod = [XsizePack, YsizePack, XposPack, YposPack, INITIAL_CHARACTERISTIC]()
                            {
                                RectangleShape RectangleTemp;
                                RectangleTemp.setSize({ XsizePack, YsizePack });
                                RectangleTemp.setPosition({ XposPack, YposPack });
                                wstring result;

                                for (const auto& ch : INITIAL_CHARACTERISTIC)
                                {
                                    result += format(L"{:.1f}", ch) + L" |\t";
                                }

                                return GUI_TextAndRectangle({ "Initial_mod", L"\t\t" ,result }, RectangleTemp, false, false);

                            }();


                        VectorDiagramPack[iter_graf].GUIInitialMod = make_shared<GUI_TextAndRectangle>(GInitialMod);





                        YposPack += YsizePack;

                        GUI_TextAndRectangle GEndMod = [XsizePack, YsizePack, XposPack, YposPack, MOD_CHARACTERISTIC]()
                            {
                                RectangleShape RectangleTemp;
                                RectangleTemp.setSize({ XsizePack, YsizePack });
                                RectangleTemp.setPosition({ XposPack, YposPack });
                                wstring result;

                                for (const auto& ch : MOD_CHARACTERISTIC)
                                {
                                    result += format(L"{:.1f}", ch) + L" |\t";
                                }

                                return GUI_TextAndRectangle({ "End_mod", L"\t\t", result }, RectangleTemp, false, false);

                            }();


                        VectorDiagramPack[iter_graf].GUIEndMod = make_shared<GUI_TextAndRectangle>(GEndMod);

                        //////////////////////////////////////////////////////////////////////////////////////////////////////////















































                        // оформление метода
                         //////////////////////////////////////////////////////////////////////////////////////////////////////////
                         XsizePack = SizeCell_default * 8.0f;
                         YsizePack = SizeCell_default * 0.4f;

                         vector< GUI_TextAndRectangle> ResultGuiMethodMod;

                         for (size_t i = 0; i < method_MOD.size(); i++)
                         {
                             XposPack = VectorWindowsDiagram[iter_graf].getSize().x - XsizePack;
                             YposPack = YsizePack * i;

                             GUI_TextAndRectangle GuiMethodMod = [XsizePack, YsizePack, XposPack, YposPack, &method_MOD, i, PriceValueDegree]()
                                 {
                                     RectangleShape RectangleTemp;
                                     RectangleTemp.setSize({ XsizePack, YsizePack });
                                     RectangleTemp.setPosition({ XposPack, YposPack });

                                     const auto& [tool, kit, skill] = method_MOD[i].ModifiersThisChance;



                                     auto cancheUp = method_MOD[i].ChanceUpgrade;

                                     float pricet = method_MOD[i].Price.PriceTool  / PriceValueDegree;
                                     float pricek = method_MOD[i].Price.PriceKit   / PriceValueDegree;
                                     float prices = method_MOD[i].Price.PriceSkill / PriceValueDegree;



                                     GUI_TextAndRectangle result(
                                         {
                                            to_wstring(i + 1), L">",

                                            string(GunStats::MapCharacteristicName_GunStat[method_MOD[i].Characteristic_Gunstat]),
                                            format(L"{:.1f} ({:.2f}%)", method_MOD[i].VisualPercent , cancheUp), L"- (",                                            
                                            GunStats::Modifiers::GetToolName(tool),   format(L"{:.1f}", pricet) + LANG_TRANSLATE("kk"), L"|",
                                            GunStats::Modifiers::GetKitName(kit),     format(L"{:.1f}", pricek) + LANG_TRANSLATE("kk"), L"|",
                                            GunStats::Modifiers::GetSkillName(skill), format(L"{:.1f}", prices) + LANG_TRANSLATE("kk"), L")"

                                         },
                                         RectangleTemp, 
                                         false,
                                         false
                                     );


                                     if (cancheUp >= 100.0)
                                     {
                                         result.setColorText(Color::Green);
                                     }

                                     return result;

                                 }();

                             ResultGuiMethodMod.push_back(GuiMethodMod);

                         }
                         VectorDiagramPack[iter_graf].GUIMethodModifiers = ResultGuiMethodMod;

                        //////////////////////////////////////////////////////////////////////////////////////////////////////////
                         
                        // оформление метода
                        //////////////////////////////////////////////////////////////////////////////////////////////////////////



                         XposPack = VectorWindowsDiagram[iter_graf].getSize().x - XsizePack;
                         YposPack = 0;

                         XsizePack = SizeCell_default * 2;
                         YsizePack = SizeCell_default * 0.4f;

                         XposPack -= XsizePack;

                         GUI_TextAndRectangle GuiMethod = [XsizePack, YsizePack, XposPack, YposPack]()
                             {
                                 RectangleShape RectangleTemp;
                                 RectangleTemp.setSize({ XsizePack, YsizePack });
                                 RectangleTemp.setPosition({ XposPack, YposPack });
                                 return GUI_TextAndRectangle( {"Method"}, RectangleTemp, false, false);

                             }();
                         
                         VectorDiagramPack[iter_graf].GUIMethod = make_shared<GUI_TextAndRectangle>(GuiMethod);
                         //////////////////////////////////////////////////////////////////////////////////////////////////////////








                         //////////////////////////////////////////////////////////////////////////////////////////////////////////

                         // добавляем стрелочки для прокрутки если выходим за указанный предел
                         if (ResultGuiMethodMod.size() >= MethodList)
                         {
                             //////////////////////////////////////////////////////////////////////////////////////////////////////////
                             // верхняя
                             XsizePack = SizeCell_default * 0.4f;
                             YsizePack = SizeCell_default * 0.4f;

                             XposPack = ResultGuiMethodMod[1].getRectangle().getPosition().x - XsizePack;
                             YposPack = ResultGuiMethodMod[1].getRectangle().getPosition().y;

                             GUI_TextAndRectangle Btop = [XsizePack, YsizePack, XposPack, YposPack]()
                                 {
                                     RectangleShape RectangleTemp;
                                     RectangleTemp.setSize({ XsizePack, YsizePack });
                                     RectangleTemp.setPosition({ XposPack, YposPack });


                                     return GUI_TextAndRectangle({ L"↑" }, RectangleTemp, false, false);

                                 }();
                             VectorDiagramPack[iter_graf].GUIButtonTop = make_shared<GUI_TextAndRectangle>(Btop);

                             //////////////////////////////////////////////////////////////////////////////////////////////////////////


                             //////////////////////////////////////////////////////////////////////////////////////////////////////////
                             // нижняя
                             XsizePack = SizeCell_default * 0.4f;
                             YsizePack = SizeCell_default * 0.4f;

                             XposPack = ResultGuiMethodMod[MethodList - 1].getRectangle().getPosition().x - XsizePack;
                             YposPack = ResultGuiMethodMod[MethodList - 1].getRectangle().getPosition().y;

                             GUI_TextAndRectangle Bdown = [XsizePack, YsizePack, XposPack, YposPack]()
                                 {
                                     RectangleShape RectangleTemp;
                                     RectangleTemp.setSize({ XsizePack, YsizePack });
                                     RectangleTemp.setPosition({ XposPack, YposPack });


                                     return GUI_TextAndRectangle({ L"↓" }, RectangleTemp, false, false);

                                 }();
                             VectorDiagramPack[iter_graf].GUIButtonDown = make_shared<GUI_TextAndRectangle>(Bdown);

                             //////////////////////////////////////////////////////////////////////////////////////////////////////////

                         }

                         VectorDiagramPack[iter_graf].InitialIndexMethod = 0;
                         //////////////////////////////////////////////////////////////////////////////////////////////////////////










































                         // сглаживание графика
                         //////////////////////////////////////////////////////////////////////////////////////////////////////////

                         vector<pair<wstring, float>> AllocationBt =
                         {
                             {L"name",0.55f},
                             {L"-",   0.12f},
                             {L"val", 0.30f},
                             {L"+",   0.12f}
                         };

                         int itbt1 = 0;
                         for (const auto& [wstrbt, SizeP] : AllocationBt)
                         {
                             GUI_TextAndRectangle GUI_BinBt = [&wstrbt, &SizeP, &VectorDiagramPack, iter_graf, SizeCell_default, itbt1]()
                                 {
                                     RectangleShape BinBt;
                                     BinBt.setSize(Vector2f((SizeCell_default * 3.f) * SizeP, SizeCell_default * 0.35f));






                                     auto posR = VectorDiagramPack[iter_graf].CurrentCurvePack.MainRectangleCurve.getPosition();
                                     auto SizeR = VectorDiagramPack[iter_graf].CurrentCurvePack.MainRectangleCurve.getSize();
                                     if (itbt1 == 1)
                                     {
                                         posR = VectorDiagramPack[iter_graf].GUIPercentBinBt.rbegin()->getPositionRectangle();
                                         SizeR = VectorDiagramPack[iter_graf].GUIPercentBinBt.rbegin()->getRectangle().getSize();
                                         BinBt.setPosition(Vector2f(posR.x, posR.y + SizeR.y));

                                     }
                                     else if (itbt1)
                                     {
                                         posR = VectorDiagramPack[iter_graf].GUIPercentBinBt.rbegin()->getPositionRectangle();
                                         SizeR = VectorDiagramPack[iter_graf].GUIPercentBinBt.rbegin()->getRectangle().getSize();
                                         BinBt.setPosition(Vector2f(posR.x + SizeR.x, posR.y));
                                     }
                                     else
                                     {
                                         BinBt.setPosition(Vector2f(posR.x + SizeR.x, posR.y));
                                     }








                                     if (wstrbt == L"val")
                                     {
                                         return GUI_TextAndRectangle({format(L"{:.1f}", VectorDiagramPack[iter_graf].PercentBin)}, BinBt, false, false);
                                     }
                                     else if (wstrbt == L"name")
                                     {
                                         return GUI_TextAndRectangle({ "PercentBin" }, BinBt, false, false);
                                     }
                                     return GUI_TextAndRectangle({ wstrbt }, BinBt, true, false);

                                 }();


                             VectorDiagramPack[iter_graf].GUIPercentBinBt.push_back(GUI_BinBt);
                             itbt1++;
                         }



                         itbt1 = 0;
                         for (const auto& [wstrbt, SizeP] : AllocationBt)
                         {
                             GUI_TextAndRectangle GUI_SmoothBt = [&wstrbt, &SizeP, &VectorDiagramPack, iter_graf, SizeCell_default, itbt1]()
                                 {
                                     RectangleShape SmBt;
                                     SmBt.setSize(Vector2f((SizeCell_default * 3.f) * SizeP, SizeCell_default * 0.35f));






                                     auto posR = VectorDiagramPack[iter_graf].CurrentCurvePack.MainRectangleCurve.getPosition() + Vector2f(0, SizeCell_default * 0.35f * 3.f );
                                     auto SizeR = VectorDiagramPack[iter_graf].CurrentCurvePack.MainRectangleCurve.getSize();
                                     if (itbt1 == 1)
                                     {
                                         posR = VectorDiagramPack[iter_graf].GUIPercentSmoothBt.rbegin()->getPositionRectangle();
                                         SizeR = VectorDiagramPack[iter_graf].GUIPercentSmoothBt.rbegin()->getRectangle().getSize();
                                         SmBt.setPosition(Vector2f(posR.x, posR.y + SizeR.y));

                                     }
                                     else if (itbt1)
                                     {
                                         posR = VectorDiagramPack[iter_graf].GUIPercentSmoothBt.rbegin()->getPositionRectangle();
                                         SizeR = VectorDiagramPack[iter_graf].GUIPercentSmoothBt.rbegin()->getRectangle().getSize();
                                         SmBt.setPosition(Vector2f(posR.x + SizeR.x, posR.y));
                                     }
                                     else
                                     {
                                         SmBt.setPosition(Vector2f(posR.x + SizeR.x, posR.y));
                                     }



                                     if (wstrbt == L"val")
                                     {
                                         return GUI_TextAndRectangle({ format(L"{:.2f}", VectorDiagramPack[iter_graf].PercentSmooth) }, SmBt, false, false);
                                     }
                                     else if (wstrbt == L"name")
                                     {
                                         return GUI_TextAndRectangle({ "PercentSmooth" }, SmBt, false, false);
                                     }
                                     return GUI_TextAndRectangle({ wstrbt }, SmBt, true, false);

                                 }();


                             VectorDiagramPack[iter_graf].GUIPercentSmoothBt.push_back(GUI_SmoothBt);
                             itbt1++;
                         }

                         //////////////////////////////////////////////////////////////////////////////////////////////////////////





































                        // end
                        //////////////////////////////////////////////////////////////////////////////////////////////////////////
                        DiagramBool[iter_graf] = true;
                        OutputLog("Main -> Graph setting is finished: " + to_string(iter_graf));
                        //////////////////////////////////////////////////////////////////////////////////////////////////////////
                    }
                    //////////////////////////////////////////////////////////////////////////////////////////////////////////
                    //////////////////////////////////////////////////////////////////////////////////////////////////////////
                    //////////////////////////////////////////////////////////////////////////////////////////////////////////
























                    VectorWindowsDiagram[iter_graf].clear(Color::White);
                    VectorWindowsDiagram[iter_graf].draw(backgroundDiagram);



                    ////////////////////////////////////////////////////////////////////////////////
                    for (auto& GUI : VectorDiagramPack[iter_graf].GUIPercentBinBt)
                    {
                        VectorWindowsDiagram[iter_graf].draw(GUI.getRectangle());
                        VectorWindowsDiagram[iter_graf].draw(GUI.getText());
                    }
                    ////////////////////////////////////////////////////////////////////////////////




                                        ////////////////////////////////////////////////////////////////////////////////
                    for (auto& GUI : VectorDiagramPack[iter_graf].GUIPercentSmoothBt)
                    {
                        VectorWindowsDiagram[iter_graf].draw(GUI.getRectangle());
                        VectorWindowsDiagram[iter_graf].draw(GUI.getText());
                    }
                    ////////////////////////////////////////////////////////////////////////////////

                    


                    ////////////////////////////////////////////////////////////////////////////////
                    for (auto& GUI : VectorDiagramPack[iter_graf].GUIPercentile50)
                    {
                        VectorWindowsDiagram[iter_graf].draw(GUI.getRectangle());
                        VectorWindowsDiagram[iter_graf].draw(GUI.getText());
                    }
                    ////////////////////////////////////////////////////////////////////////////////








                    ////////////////////////////////////////////////////////////////////////////////

                    VectorWindowsDiagram[iter_graf].draw(VectorDiagramPack[iter_graf].GUIMethod.get()->getRectangle());
                    VectorWindowsDiagram[iter_graf].draw(VectorDiagramPack[iter_graf].GUIMethod.get()->getText());


                    for (size_t it = VectorDiagramPack[iter_graf].InitialIndexMethod; it < VectorDiagramPack[iter_graf].GUIMethodModifiers.size(); it++)
                    {
                        VectorWindowsDiagram[iter_graf].draw(VectorDiagramPack[iter_graf].GUIMethodModifiers[it].getRectangle());
                        VectorWindowsDiagram[iter_graf].draw(VectorDiagramPack[iter_graf].GUIMethodModifiers[it].getText());
                    }

                    if (VectorDiagramPack[iter_graf].GUIButtonTop.get() != nullptr && VectorDiagramPack[iter_graf].GUIButtonDown.get() != nullptr)
                    {
                        VectorWindowsDiagram[iter_graf].draw(VectorDiagramPack[iter_graf].GUIButtonTop.get()->getRectangle());
                        VectorWindowsDiagram[iter_graf].draw(VectorDiagramPack[iter_graf].GUIButtonTop.get()->getText());

                        VectorWindowsDiagram[iter_graf].draw(VectorDiagramPack[iter_graf].GUIButtonDown.get()->getRectangle());
                        VectorWindowsDiagram[iter_graf].draw(VectorDiagramPack[iter_graf].GUIButtonDown.get()->getText());
                    }

                    if (VectorDiagramPack[iter_graf].GUIInitialMod.get() != nullptr && VectorDiagramPack[iter_graf].GUIEndMod.get() != nullptr)
                    {
                        VectorWindowsDiagram[iter_graf].draw(VectorDiagramPack[iter_graf].GUIInitialMod.get()->getRectangle());
                        VectorWindowsDiagram[iter_graf].draw(VectorDiagramPack[iter_graf].GUIInitialMod.get()->getText());

                        VectorWindowsDiagram[iter_graf].draw(VectorDiagramPack[iter_graf].GUIEndMod.get()->getRectangle());
                        VectorWindowsDiagram[iter_graf].draw(VectorDiagramPack[iter_graf].GUIEndMod.get()->getText());
                    }

                    ////////////////////////////////////////////////////////////////////////////////










                    ////////////////////////////////////////////////////////////////////////////////
                    VectorWindowsDiagram[iter_graf].draw(VectorDiagramPack[iter_graf].CurrentCurvePack.MainRectangleCurve);

                    for (size_t it2 = 0; it2 < VectorDiagramPack[iter_graf].CurrentCurvePack.VGUITextCurve.size(); it2++)
                    {
                        VectorWindowsDiagram[iter_graf].draw(VectorDiagramPack[iter_graf].CurrentCurvePack.VGUITextCurve[it2].getRectangle());
                        VectorWindowsDiagram[iter_graf].draw(VectorDiagramPack[iter_graf].CurrentCurvePack.VGUITextCurve[it2].getText());
                    }
                    VectorWindowsDiagram[iter_graf].draw(VectorDiagramPack[iter_graf].CurrentCurvePack.Curve);

                    ////////////////////////////////////////////////////////////////////////////////



























                    /// если мы навелись на кривую ->  выведем инфу о колличестваx на текущей позиции
                    if (VectorDiagramPack[iter_graf].CurrentCurvePack.MainRectangleCurve.getGlobalBounds().contains(MouseWorldPosForGraf))
                    {

                        // вертикальная линия
                        //////////////////////////////////////////////////////////////////////////////////////////
                        auto posLineTop = VectorDiagramPack[iter_graf].CurrentCurvePack.MainRectangleCurve.getPosition();
                        auto posLineDown = VectorDiagramPack[iter_graf].CurrentCurvePack.MainRectangleCurve.getPosition() + Vector2f( 0, VectorDiagramPack[iter_graf].CurrentCurvePack.MainRectangleCurve.getSize().y );

                        // поправка линии на позицию мышки
                        float PosMouseRelativeRectangleCure = MouseWorldPosForGraf.x - VectorDiagramPack[iter_graf].CurrentCurvePack.MainRectangleCurve.getPosition().x;
                        posLineTop  += Vector2f(PosMouseRelativeRectangleCure, 0);
                        posLineDown += Vector2f(PosMouseRelativeRectangleCure, 0);

                        //делаем прямую y - 0
                        VertexArray VerticalLine(PrimitiveType::Lines, 2);    

                        VerticalLine[0].position = posLineTop;
                        VerticalLine[0].color    = Color::Black;

                        VerticalLine[1].position = posLineDown;
                        VerticalLine[1].color    = Color::Black;

                        VectorWindowsDiagram[iter_graf].draw(VerticalLine);
                        //////////////////////////////////////////////////////////////////////////////////////////






                        // перебираем отрезки кривой для вывода информации
                        //////////////////////////////////////////////////////////////////////////////////////////
                        
                        size_t CountSectionCurve = VectorDiagramPack[iter_graf].CurrentCurvePack.Curve.getVertexCount();
                        if (CountSectionCurve == 0)
                        {
                            CountSectionCurve = 1;
                        }

                        auto LengthCurve = VectorDiagramPack[iter_graf].CurrentCurvePack.MainRectangleCurve.getSize().x;

                        auto XposRectangle = VectorDiagramPack[iter_graf].CurrentCurvePack.MainRectangleCurve.getPosition().x;
                        auto LengthOneSectionCurve = LengthCurve / CountSectionCurve;                        
                        auto XposLine = posLineTop.x; // || posLineDown.x

                        //имя инструмента \ кол-во
                        vector<pair<string, unsigned long long>> InfoCountModifiers;

                        // перебираем все отрезки
                        unsigned it_max;
                        float Cur_Freq;

                        for (it_max = 0; it_max < CountSectionCurve; it_max++)
                        {
                            // делаем допуск
                            float epsilon = 0.01f;
                            float start   = XposRectangle + LengthOneSectionCurve * it_max;
                            float end     = XposRectangle + LengthOneSectionCurve * (it_max + 1);


                            // проверяем позиция x вертикальной линии входит в секцию кривой?
                            if (XposLine >= start - epsilon && XposLine < end + epsilon)
                            {

                                auto iteratorMap = VectorDiagramPack[iter_graf].CurrentSmoothFreqModifiers.begin();
                                advance(iteratorMap, it_max);


                                CountModifiers MODIFIERS_M = iteratorMap->first;

                                Cur_Freq = iteratorMap->second;

                                if (iteratorMap != VectorDiagramPack[iter_graf].CurrentSmoothFreqModifiers.end())
                                {
                                    MODIFIERS_M = iteratorMap->first;
                                    Cur_Freq = iteratorMap->second;
                                }


                                InfoCountModifiers = GetInfoForModifiers(MODIFIERS_M);

                                break;
                            }
                        }
                        //////////////////////////////////////////////////////////////////////////////////////////








                        // информация о секции
                        //////////////////////////////////////////////////////////////////////////////////////////
                        Text NameModifier(ThemeGui::CurrentFont);
                        NameModifier.setFillColor(Color::Black);
                        NameModifier.setCharacterSize(static_cast<unsigned>(ThemeGui::CharacterSize * 0.9f));

                        RectangleShape RectangleForGraf;
                        RectangleForGraf.setSize(Vector2f(SizeCell_default * 2.7f, SizeCell_default * 0.23f * static_cast<float>(InfoCountModifiers.size() + 1)   ));
                        RectangleForGraf.setPosition({ posLineTop + Vector2f(SizeCell_default * 0.3f, 0) });
                        RectangleForGraf.setFillColor(Color(230,230,230, 200));
                        RectangleForGraf.setOutlineColor(Color::Black);
                        RectangleForGraf.setOutlineThickness(-1);


                        VectorWindowsDiagram[iter_graf].draw(RectangleForGraf);

                        size_t it4;
                        for (it4 = 0; it4 < InfoCountModifiers.size(); it4++)
                        {
                            //////////////////////////////////////////////////////////////////////////////////////////
                            auto pos1 = posLineTop + Vector2f(SizeCell_default * 0.4f, SizeCell_default * 0.2f * it4);

                            NameModifier.setPosition(pos1);
                            
                            if (it4 == InfoCountModifiers.size() - 1)
                            {

                                NameModifier.setString(  format(L"{:L}", static_cast<float>(InfoCountModifiers[it4].second / 1'000'000)) + LANG_TRANSLATE("kk"));

                            }
                            else
                            {
                                NameModifier.setString(to_wstring(InfoCountModifiers[it4].second));
                            }
                            

                            VectorWindowsDiagram[iter_graf].draw(NameModifier);
                            //////////////////////////////////////////////////////////////////////////////////////////




                            //////////////////////////////////////////////////////////////////////////////////////////
                            auto pos2 = posLineTop + Vector2f(SizeCell_default * 0.85f, SizeCell_default * 0.2f * it4);

                            NameModifier.setPosition(pos2);
                            NameModifier.setString(L" - " + LANG_TRANSLATE(InfoCountModifiers[it4].first));

                            VectorWindowsDiagram[iter_graf].draw(NameModifier);
                            //////////////////////////////////////////////////////////////////////////////////////////
                        }



                        //////////////////////////////////////////////////////////////////////////////////////////
                        NameModifier.setPosition(posLineTop + Vector2f(SizeCell_default * 0.4f, SizeCell_default * 0.2f * it4));
                        NameModifier.setString(LANG_TRANSLATE("Section") + to_wstring(it_max + 1) + L" | " + LANG_TRANSLATE("Freq") + format(L"{:.2f}", Cur_Freq));

                        VectorWindowsDiagram[iter_graf].draw(NameModifier);
                        //////////////////////////////////////////////////////////////////////////////////////////
                    }
                    ////////////////////////////////////////////////////////////////////////////////




























                    VectorWindowsDiagram[iter_graf].display();








                    // возврат к дефолт цвету
                    if (WI % 50 == 10)
                    {
                        for (auto& button : VectorDiagramPack[iter_graf].GUIPercentBinBt)
                        {
                            button.setAsPressedButton();
                        }
                        for (auto& button : VectorDiagramPack[iter_graf].GUIPercentSmoothBt)
                        {
                            button.setAsPressedButton();
                        }
                    }
                }                
            }
        }
        ////////////////////////////////////////////////////////////////////////////////       
        // возврат к дефолт цвету
        if (WI % 50 == 10)
        {
            Calculation.setAsPressedButton();
            ShapeTargetIter.setAsPressedButton();
            ButtonGeneralModApplyStat.setAsPressedButton();
            ShapeGeneralModReset.setAsPressedButton();
            ShapeGeneralModBack.setAsPressedButton();

            if (!PressedApplyAsMod) { ShapeGeneralModApplyStatInitial.setAsPressedButton(); }

            /// стоимость
            for (auto& button : ToolPrice)
            {
                button.setAsPressedButton();
            }
            for (auto& button : KitPrice)
            {
                button.setAsPressedButton();
            }
        }

    }


    
    return 0;    
}


vector<pair<string, unsigned long long>> GetInfoForModifiers(const CountModifiers& MODIFIERS)
{
    vector<pair<string, unsigned long long>> result;
    // выводим информацию о 
    // CountModifiers



    for (const auto& [CurrentTool, CountTool] : MODIFIERS.CountTool)
    {
        if (CountTool) // != 0
        {
            if (CurrentTool != GunStats::Modifiers::ToolType::NO_TOOL)
            {
                result.emplace_back(GunStats::Modifiers::GetToolName(CurrentTool), CountTool);
            }
        }
    }
    for (const auto& [Currentkit, CountKit] : MODIFIERS.CountKit)
    {
        if (CountKit) // != 0
        {
            if (Currentkit != GunStats::Modifiers::KitType::NO_KIT)
            {
                result.emplace_back(GunStats::Modifiers::GetKitName(Currentkit), CountKit);
            }
        }
    }
    
    for (const auto& [CurrentSkill, CountSkill] : MODIFIERS.CountSkill)
    {
        if (CountSkill) // != 0 
        {
            // не учитываем
            if (CurrentSkill != GunStats::Modifiers::SkillType::PLAYER_MASTER_1 &&
                CurrentSkill != GunStats::Modifiers::SkillType::PLAYER_MASTER_2 &&
                CurrentSkill != GunStats::Modifiers::SkillType::PLAYER_MASTER_3 &&
                CurrentSkill != GunStats::Modifiers::SkillType::PLAYER_MASTER_4 &&
                CurrentSkill != GunStats::Modifiers::SkillType::PLAYER_MASTER_5 &&
                CurrentSkill != GunStats::Modifiers::SkillType::PLAYER_NO_SKILL
                )
            {
                result.emplace_back(GunStats::Modifiers::GetSkillName(CurrentSkill), CountSkill);
            }
        }
    }
    result.emplace_back("AllPrice", MODIFIERS.AllPrice);


    return result;
}











void EditShaheUnits(vector<GeneralStat>& GeneralModOption, CharacteristicGun& CurrentCharacteristicGun, vector<int> InitialPosition, vector<int> ModPosition)
{
    auto DefaultPosition = CurrentCharacteristicGun.getDefaultPosition();
    moveElement(DefaultPosition, 4, 2);

    auto MaxPosition     = CurrentCharacteristicGun.getMaxPositionCharacteristic();
    moveElement(MaxPosition, 4, 2);

    auto Current         = CurrentCharacteristicGun.getCurrentPosition();
    moveElement(Current, 4, 2);
    

    for (int it1 = 0; it1 < GeneralModOption.size(); it1++)
    {
        for (int it2 = 0; it2 < GeneralModOption[it1].ShaheUnitsInCharacteristic.size(); it2++)
        {
            //отрисовали  текущие 
            if (Current[it1] > it2 && it2 < DefaultPosition[it1]) 
            { 
                GeneralModOption[it1].ShaheUnitsInCharacteristic[it2].setFillColor(Color(14, 88, 28)); 
            }

            //отрисовали  начальные мод статы 
            else if ( InitialPosition[it1] > it2 && it2 < Current[it1]) 
            { 
                GeneralModOption[it1].ShaheUnitsInCharacteristic[it2].setFillColor(Color(13, 38, 13));
            }

            //отрисовали  мод статы
            else if (it2 < ModPosition[it1]) 
            {
                GeneralModOption[it1].ShaheUnitsInCharacteristic[it2].setFillColor(Color::Green);
            }

            // отрисовали  недостающие до дефолт статов
            else if (it2 < DefaultPosition[it1]) 
            {
                GeneralModOption[it1].ShaheUnitsInCharacteristic[it2].setFillColor(Color(128, 128, 128));
            }

            //отрисовали возможные до макс
            else if (it2 < MaxPosition[it1]) 
            {
                GeneralModOption[it1].ShaheUnitsInCharacteristic[it2].setFillColor(Color(66, 66, 66)); 
            }

            // отрисовали невозможные
            else 
            {
                GeneralModOption[it1].ShaheUnitsInCharacteristic[it2].setFillColor(Color(30, 30, 30)); 
            }
        }
    }
}

bool Set_Chahce_upgrade(const CharacteristicGun& CurrentCharacteristicGun, const Select_Modifiers& CurrentModifiers, GUI_TextAndRectangle& GUI_Chance, size_t it)
{
    GUI_Chance.resetColorText();

    if (CurrentCharacteristicGun.is_Empty())
    {
        return false;
    }


    auto TempChance = CalculateSuccessChance(
        CurrentCharacteristicGun.getChanceFor_NEXT_Stat(GunStats::Trans_INDEX_to_INDEXGUNSTAT[it]),

        GunStats::Modifiers::GetToolModifier(CurrentModifiers.Tool),
        GunStats::Modifiers::GetKitModifier(CurrentModifiers.Kit),
        GunStats::Modifiers::GetSkillModifier(CurrentModifiers.Skill)
    );

    wstring Percent = L" ";

    if (!CurrentCharacteristicGun.is_Empty())
    {
        Percent += format(L"{:.2f}", TempChance);
    }
    else
    {
        Percent += L"0.0";
    }
    Percent += L"%";

    auto NameCharacteristic = string(GunStats::MapCharacteristicName_Index[it]);

    GUI_Chance.setKeyString_and_TextWstring({ "Chahce_upgrade" , NameCharacteristic ,  Percent });

    if (TempChance >= 100.0f)
    {
        GUI_Chance.setColorText(Color::Green);
    }
    return true;
}



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

unsigned Get_Price(const path& PathToPriceModifiers_JS, const Select_Modifiers& CurrentModifiers)
{
    int result = -1;


    ifstream in_jsonFile_price(PathToPriceModifiers_JS);
    if (!in_jsonFile_price)
    {
        OutputLog("CharacteristicGun -> Failed to open file: " + WstringToString(PathToPriceModifiers_JS));
    }
    else
    {
        nlohmann::json JSON_PRICE;
        in_jsonFile_price >> JSON_PRICE;
        in_jsonFile_price.close();


        string Str_tool = GunStats::Modifiers::GetToolName(CurrentModifiers.Tool);


        if (JSON_PRICE.contains("MD_Tool") && JSON_PRICE["MD_Tool"].contains(Str_tool))
        {
            result = JSON_PRICE["MD_Tool"][Str_tool].get<unsigned>();
        }
        else
        {
            JSON_PRICE["MD_Tool"][Str_tool] = 0;
        }

        if (result == -1)
        {
            ofstream out_jsonFile_price(PathToPriceModifiers_JS);
            out_jsonFile_price << JSON_PRICE.dump(2);
            out_jsonFile_price.close();

            result = 0;
        }
    }
    return static_cast<unsigned>(result);
}
unsigned Get_Price(const path& PathToPriceModifiers_JS, const Select_Modifiers& CurrentModifiers, string NameGun)
{
    int result = -1;


    ifstream in_jsonFile_price(PathToPriceModifiers_JS);
    if (!in_jsonFile_price)
    {
        OutputLog("CharacteristicGun -> Failed to open file: " + WstringToString(PathToPriceModifiers_JS));
    }
    else
    {

        nlohmann::json JSON_PRICE;
        in_jsonFile_price >> JSON_PRICE;
        in_jsonFile_price.close();


        if (JSON_PRICE.contains("MD_Kit") && JSON_PRICE["MD_Kit"].contains(NameGun) && JSON_PRICE["MD_Kit"][NameGun].contains(GunStats::Modifiers::GetKitName(CurrentModifiers.Kit)))
        {
            result = JSON_PRICE["MD_Kit"][NameGun][GunStats::Modifiers::GetKitName(CurrentModifiers.Kit)].get<unsigned>();
        }
        else
        {
            JSON_PRICE["MD_Kit"][NameGun][GunStats::Modifiers::GetKitName(CurrentModifiers.Kit)] = 0;
        }

        if (result == -1)
        {
            ofstream out_jsonFile_price(PathToPriceModifiers_JS);
            out_jsonFile_price << JSON_PRICE.dump(2);
            out_jsonFile_price.close();

            result = 0;
        }
    }
    return static_cast<unsigned>(result);
}




bool Set_Price_Tool(const path& PathToPriceModifiers_JS, GunStats::Modifiers::ToolType Tool, unsigned Price)
{
    if (Tool == GunStats::Modifiers::ToolType::NO_TOOL)
    {
        return false;
    }

    ifstream in_jsonFile_price(PathToPriceModifiers_JS);
    if (!in_jsonFile_price)
    {
        OutputLog("CharacteristicGun -> Failed to open file: " + WstringToString(PathToPriceModifiers_JS));
        return false;
    }
    else
    {
        nlohmann::json JSON_PRICE;
        in_jsonFile_price >> JSON_PRICE;
        in_jsonFile_price.close();

        JSON_PRICE["MD_Tool"][GunStats::Modifiers::GetToolName(Tool)] = Price;

        ofstream out_jsonFile_price(PathToPriceModifiers_JS);
        out_jsonFile_price << JSON_PRICE.dump(2);
        out_jsonFile_price.close();
    }
    return true;
}


bool Set_Price_Kit(const path& PathToPriceModifiers_JS, GunStats::Modifiers::KitType Kit, string nameGun, unsigned Price)
{
    if (Kit == GunStats::Modifiers::KitType::NO_KIT)
    {
        return false;
    }
    if (nameGun == "Empty")
    {
        return false;
    }


    ifstream in_jsonFile_price(PathToPriceModifiers_JS);
    if (!in_jsonFile_price)
    {
        OutputLog("CharacteristicGun -> Failed to open file: " + WstringToString(PathToPriceModifiers_JS));
        return false;
    }
    else
    {
        nlohmann::json JSON_PRICE;
        in_jsonFile_price >> JSON_PRICE;
        in_jsonFile_price.close();


        JSON_PRICE["MD_Kit"][nameGun][GunStats::Modifiers::GetKitName(Kit)] = Price;


        ofstream out_jsonFile_price(PathToPriceModifiers_JS);
        out_jsonFile_price << JSON_PRICE.dump(2);
        out_jsonFile_price.close();
    }
    return true;
}