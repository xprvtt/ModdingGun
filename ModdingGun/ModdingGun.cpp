#include "ModdingGun.h"


// колво повторений мода
unsigned int totalIterations = 500;

vector<int> method = {

    GunStats::ACCURACY,
    GunStats::RATE_OF_FIRE,
    GunStats::PENETRATION,
    GunStats::KICKBACK,
    GunStats::SWAY,
    GunStats::MALFUNCTION_CONDITION,
    GunStats::MALFUNCTION_DIRT

};

tuple Tool_Kit_Skil = 
{
    GunStats::GetToolModifier(GunStats::ToolType::OLD_TOOL),
    GunStats::GetKitModifier(GunStats::KitType::NO_KIT),
    GunStats::GetSkillModifier(GunStats::SkillType::PLAYER_MASTER_5)
};













int main()
{

    Average10000_RANDOM_GEN.resize(200'000'000);
    Average10000.resize(200'000'000);
    // создаем вывод в логи
    permissions("Log/log.txt", perms::all);
    remove("Log/log.txt");
    OutputLog("Запуск!");


















    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// высота окна, от нее зависит длина окна
    float  SizeWindowHeight = 700;
    float  SizeWindowLength = SizeWindowHeight * 1.8f;

    /// количество клеток на одной стороне
    unsigned int CountCellOnLengthWindow = 20;
    unsigned int CountCellOnHeightWindow = 5;

    float SizeCell = static_cast<float>(SizeWindowLength / CountCellOnLengthWindow);

    // создаем диалоговое окно
    unsigned int HeightCharacteristicsInputWindow = 100;
    unsigned int LengthCharacteristicsInputWindow = HeightCharacteristicsInputWindow * 5;
   
    // создаем диалоговое окно графика
    unsigned int sizeXGrafWindow = 1300;
    unsigned int sizeYGrafWindow = 500;

    unsigned int SizeFont = 17;

    // переменные положения мышки
    Vector2i PositionMouse;
    Vector2f MouseWorldPos;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////

















    ///////////////////////////////////  FONT   /////////////////////////////////////// 
    /// обнаруживаем все шрифты в папке 
    vector<path> SearchFont = SearchFile("Font/", ".ttf");
    /// если шрифтов не найдено вых
    if (SearchFont.empty())
    {
        OutputLog("Шрифт не найден, завершение");
        return -1;
    }
    // используем первый в списке шрифт
    OutputLog("Шрифт: " + SearchFont[0].string());
    Font CurrentFont = LoadFont(SearchFont[0]);
    OutputLog("Шрифт загружен - 60" );
    ///////////////////////////////////////////////////////////////////////////////////





















        //////////////////////// ИКОНКИ ОРУЖИЯ  И ВЫБРАННОГО ОРУЖИЯ ////////////////////////

    vector<tuple<RectangleShape, shared_ptr<Texture>, wstring, int>> VectorTextureGun;
    vector<Text> VectorNameGun;


    shared_ptr<Texture> PointTextureEmpty = make_shared<Texture>(L"Assets/Standart/Empty.png");

    for (size_t Tier = 1, MaxTier = CountCellOnHeightWindow; Tier <= MaxTier; Tier++)
    {
        vector<path> SearchTextureGun = SearchFile("Assets/Tier_" + to_string(Tier), ".png");
        if (SearchFont.empty())
        {
            OutputLog("текстуры оружия не найдены");
            return -1;
        }
        vector<path> SearchAttributeGun = SearchFile("Attribute/Tier_" + to_string(Tier), ".txt");
        if (SearchFont.empty())
        {
            OutputLog("текстуры оружия не найдены");
            return -1;
        }

        for (unsigned int it = 0; it < CountCellOnLengthWindow; it++)
        {
            shared_ptr<Texture> TempPoint;

            wstring TempName;
            if (it < SearchTextureGun.size())
            {
                TempPoint = make_shared<Texture>(SearchTextureGun[it]);
                TempName = SearchTextureGun[it].stem();
            }
            else
            {
                TempPoint = PointTextureEmpty;
                TempName  = L" ";
            }


            RectangleShape TempShape;
            TempShape.setSize(Vector2f(SizeCell, SizeCell));
            TempShape.setOutlineThickness(-1);
            TempShape.setOutlineColor(Color::Black);
            TempShape.setTexture(TempPoint.get());
            TempShape.setPosition(Vector2f(SizeCell * it, SizeCell * (Tier - 1) ));

            Text TempNameGun(CurrentFont, TempName, SizeFont);
            TempNameGun.setFillColor(Color::Red);

            for (const auto& AT : SearchAttributeGun)
            {
                if (AT.stem() == TempName)
                {
                    TempNameGun.setFillColor(Color::White);
                }
            }

            TempNameGun.setPosition(Vector2f(SizeCell * static_cast<float>(it), static_cast<float>(SizeCell * (Tier - 1))));
            TempNameGun.setOutlineColor(Color::Black);
            TempNameGun.setOutlineThickness(1);
            TempNameGun.setCharacterSize(14);

            VectorTextureGun.push_back(make_tuple(TempShape, TempPoint, TempName, Tier));
            VectorNameGun.push_back(TempNameGun);
        }
    }



    RectangleShape SelectedGunShape;
    SelectedGunShape.setSize(Vector2f(SizeCell, SizeCell));
    SelectedGunShape.setTexture(PointTextureEmpty.get(), true);
    SelectedGunShape.setOutlineThickness(-2);
    SelectedGunShape.setOutlineColor(Color::Black);
    SelectedGunShape.setPosition(Vector2f(0, SizeCell * (CountCellOnHeightWindow + 1) ));

    OutputLog("загружен vector textrue");
    ///////////////////////////////////////////////////////////////////////////////////






















        //////////////////////// КНОПКА СБОРКИ ////////////////////////
    
    ////////////////////////////////////////////////////////////////////////////////////

    RectangleShape Calculation;
    Calculation.setSize(Vector2f(SizeCell * 2, SizeCell / 2));
    Calculation.setOutlineThickness(-3);
    Calculation.setOutlineColor(Color::Green);
    Calculation.setPosition(Vector2f(SizeCell * 13.5f, SizeCell * static_cast<float>(CountCellOnHeightWindow + 3)));

    

    Text TextCalculation(CurrentFont);
    TextCalculation.setString(L"Собрать");
    TextCalculation.setCharacterSize(SizeFont+3);
    TextCalculation.setFillColor(Color::Black);


    FloatRect textBounds = TextCalculation.getLocalBounds();
    Vector2f rectPos = Calculation.getPosition();
    Vector2f rectSize = Calculation.getSize();

    TextCalculation.setOrigin(Vector2f(textBounds.position.x + textBounds.size.x / 2.0f, textBounds.position.y + textBounds.size.y / 2.0f));
    TextCalculation.setPosition(Vector2f(rectPos.x + rectSize.x / 2.0f,rectPos.y + rectSize.y / 2.0f)  );


    ////////////////////////////////////////////////////////////////////////////////////

    //RectangleShape Calculation;
    //Calculation.setSize(Vector2f(SizeCell * 2, SizeCell / 2));
    //Calculation.setOutlineThickness(-3);
    //Calculation.setOutlineColor(Color::Green);
    //Calculation.setPosition(Vector2f(SizeCell + SizeCell / 2, SizeCell * (CountCellOnHeightWindow + 2)));

    ////Vector2f(SizeCell * 13.5f, SizeCell * static_cast<float>(CountCellOnHeightWindow + 2)));

    //Text TextCalculation(CurrentFont);
    //TextCalculation.setString(L"Calculation->");
    //TextCalculation.setCharacterSize(SizeFont + 3);
    //TextCalculation.setFillColor(Color::Black);


    //FloatRect textBounds = TextCalculation.getLocalBounds();
    //Vector2f rectPos = Calculation.getPosition();
    //Vector2f rectSize = Calculation.getSize();

    //TextCalculation.setOrigin(Vector2f(textBounds.position.x + textBounds.size.x / 2.0f, textBounds.position.y + textBounds.size.y / 2.0f));
    //TextCalculation.setPosition(Vector2f(rectPos.x + rectSize.x / 2.0f, rectPos.y + rectSize.y / 2.0f));

























    //////////////////////// МОДИФИКАТОРЫ ////////////////////////

    ////////////////////////////////////////////////////////////////////////////////////
    vector<shared_ptr<Texture>> KitTextures;
    vector<shared_ptr<Texture>> ToolTextures;
    vector<shared_ptr<Texture>> SkillTextures;

    vector<RectangleShape> ToolShapes;
    vector<RectangleShape> SkillShapes;
    vector<RectangleShape> KitShapes;

    vector<path> toolpath = SearchFile("Assets/Tool/", ".png");
    vector<path> kitpath = SearchFile("Assets/Kit/", ".png");
    vector<path> skillpath = SearchFile("Assets/Skill/", ".png");

    auto LoadTexturesAndShapes = [&](const vector<path>& paths, float yOffset,
        vector<shared_ptr<Texture>>& textures,
        vector<RectangleShape>& shapes)
        {
            int i = 1;
            for (const auto& patht : paths)
            {
                auto tex = make_shared<Texture>();
                if (!tex->loadFromFile(patht)) continue;

                textures.push_back(tex);

                RectangleShape box;
                box.setOutlineThickness(-1);
                box.setOutlineColor(Color::Black);
                box.setSize(Vector2f(SizeCell / 2 + 10, SizeCell / 2 + 10));
                box.setTexture(tex.get());
                box.setPosition(Vector2f(SizeCell / 2 + (SizeCell / 2  + 10) * i , SizeCell * yOffset));

                shapes.push_back(box);
                ++i;
            }
        };

    // Загружаем всё:
    LoadTexturesAndShapes(toolpath, CountCellOnHeightWindow + 1.0f, ToolTextures, ToolShapes);
    LoadTexturesAndShapes(kitpath, CountCellOnHeightWindow + 2.f, KitTextures, KitShapes);
    LoadTexturesAndShapes(skillpath, CountCellOnHeightWindow + 3.0f, SkillTextures, SkillShapes);

    ToolShapes[3].setOutlineColor(Color::Green);
    KitShapes[0].setOutlineColor(Color::Green);
    SkillShapes[5].setOutlineColor(Color::Green);
    ///////////////////////////////////////////////////////////////////////////////////























        //////////////////////// ОТОБРАЖЕНИЕ УКАЗАННОГО МОДА ////////////////////////
        
    ///////////////////////////////////////////////////////////////////////////////////

    RectangleShape СharacteristicsModGun;
    СharacteristicsModGun.setSize(Vector2f(SizeCell * 8, SizeCell / 2));
    СharacteristicsModGun.setOutlineThickness(-3);
    СharacteristicsModGun.setOutlineColor(Color::Green);
    СharacteristicsModGun.setPosition(Vector2f(SizeCell*4, SizeCell * (CountCellOnHeightWindow + 1)));

    Text TextСharacteristicsModGun(CurrentFont);
    TextСharacteristicsModGun.setString(L"мод, который хотим получить");
    TextСharacteristicsModGun.setCharacterSize(SizeFont + 3);
    TextСharacteristicsModGun.setFillColor(Color::Black);

    textBounds = TextСharacteristicsModGun.getLocalBounds();
    rectPos = СharacteristicsModGun.getPosition();
    rectSize = СharacteristicsModGun.getSize();

    TextСharacteristicsModGun.setOrigin(Vector2f(0.f, textBounds.position.y + textBounds.size.y / 2.0f));
    TextСharacteristicsModGun.setPosition(Vector2f(rectPos.x + 10, rectPos.y + rectSize.y / 2.0f));

    OutputLog("загружен СharacteristicsModGun");

    ///////////////////////////////////////////////////////////////////////////////////











 
    //////////////////////// ОТОБРАЖЕНИЕ НАЧАЛЬНОГО МОДА И КНОПКИ ПЕРЕКЛЮЧЕНИЯ ////////////////////////

    ///////////////////////////////////////////////////////////////////////////////////


    shared_ptr<Texture> RedMark = make_shared<Texture>(L"Assets/Standart/MarkX.png");
    shared_ptr<Texture> GreenMark = make_shared<Texture>(L"Assets/Standart/MarkV.png");



    RectangleShape СharacteristicsModGunInitial;
    СharacteristicsModGunInitial.setSize(Vector2f(SizeCell * 8, SizeCell / 2));
    СharacteristicsModGunInitial.setOutlineThickness(-3);
    СharacteristicsModGunInitial.setOutlineColor(Color::Green);
    СharacteristicsModGunInitial.setPosition(Vector2f(SizeCell * 4, SizeCell* (CountCellOnHeightWindow + 2)));

    RectangleShape ButtonСharacteristicsModGunInitial;
    ButtonСharacteristicsModGunInitial.setSize(Vector2f(SizeCell / 2, SizeCell / 2));
    ButtonСharacteristicsModGunInitial.setOutlineThickness(-3);
    ButtonСharacteristicsModGunInitial.setOutlineColor(Color::Black);
    ButtonСharacteristicsModGunInitial.setTexture(RedMark.get(), true);
    ButtonСharacteristicsModGunInitial.setPosition(Vector2f(SizeCell * 12 + SizeCell / 2, SizeCell* (CountCellOnHeightWindow + 2)));

    Text TextСharacteristicsModGunInitial(CurrentFont);
    TextСharacteristicsModGunInitial.setString(L"начальный мод, от корого начнем  |  переключение ->");
    TextСharacteristicsModGunInitial.setCharacterSize(SizeFont + 3); 
    TextСharacteristicsModGunInitial.setFillColor(Color::Black);

    textBounds = TextСharacteristicsModGunInitial.getLocalBounds();
    rectPos = СharacteristicsModGunInitial.getPosition();
    rectSize = СharacteristicsModGunInitial.getSize();

    TextСharacteristicsModGunInitial.setOrigin(Vector2f(0.f, textBounds.position.y + textBounds.size.y / 2.0f));
    TextСharacteristicsModGunInitial.setPosition(Vector2f(rectPos.x + 10, rectPos.y + rectSize.y / 2.0f));

    OutputLog("загружен СharacteristicsModGunInitial");

    ///////////////////////////////////////////////////////////////////////////////////



















     //////////////////////// ОТОБРАЖЕНИЕ ИНФОРМАЦИИ ////////////////////////

    ///////////////////////////////////////////////////////////////////////////////////

    RectangleShape ShapeInfoDebug;
    ShapeInfoDebug.setSize(Vector2f(SizeCell * 7, SizeCell / 2));
    ShapeInfoDebug.setOutlineThickness(-3);
    ShapeInfoDebug.setOutlineColor(Color::Black);
    ShapeInfoDebug.setPosition(Vector2f(static_cast<float>(SizeCell * 12.5), static_cast<float>(SizeCell* (CountCellOnHeightWindow + 1))));

    Text TextInfoDebug(CurrentFont);
    TextInfoDebug.setCharacterSize(SizeFont + 3);
    TextInfoDebug.setFillColor(Color::Black);

    textBounds = TextInfoDebug.getLocalBounds();
    rectPos = ShapeInfoDebug.getPosition();
    rectSize = ShapeInfoDebug.getSize();

    TextInfoDebug.setOrigin(Vector2f(0.f, textBounds.position.y + textBounds.size.y / 2.0f));
    TextInfoDebug.setPosition(Vector2f(rectPos.x + 10, rectPos.y + rectSize.y / 11.5f));

    OutputLog("загружен ShapeInfoDebug");

    ///////////////////////////////////////////////////////////////////////////////////













    //////////////////////// ОТОБРАЖЕНИЕ ПОВТОРНЫХ СБОРОК ////////////////////////

    ///////////////////////////////////////////////////////////////////////////////////

    RectangleShape ShapeTargetIter;
    ShapeTargetIter.setSize(Vector2f(SizeCell * 4, SizeCell / 2));
    ShapeTargetIter.setOutlineThickness(-3);
    ShapeTargetIter.setOutlineColor(Color::Green);
    ShapeTargetIter.setPosition(Vector2f(SizeCell * 13.5f, SizeCell * static_cast<float>(CountCellOnHeightWindow + 2) ));

    Text TextTargetIter(CurrentFont);
    TextTargetIter.setString(L"колличество сборок: " + to_string(totalIterations));
    TextTargetIter.setCharacterSize(SizeFont + 3);
    TextTargetIter.setFillColor(Color::Black);

    textBounds = TextTargetIter.getLocalBounds();
    rectPos = ShapeTargetIter.getPosition();
    rectSize = ShapeTargetIter.getSize();

    TextTargetIter.setOrigin(Vector2f(0.f, textBounds.position.y + textBounds.size.y / 2.0f));
    TextTargetIter.setPosition(Vector2f(rectPos.x + 10, rectPos.y + rectSize.y / 2.f));
    

    ///////////////////////////////////////////////////////////////////////////////////



















//////////////////////// ЗАДНИЙ ФОН ////////////////////////

////////////////////////////////////////////////////////////////////////////////

    Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("Assets/Standart/background.png"))
    {
        // Обработка ошибки — например, лог или аварийный выход
        OutputLog("Не удалось загрузить background.png");
    }
    RectangleShape background;
    background.setSize(Vector2f(SizeWindowLength, SizeWindowHeight));
    background.setPosition(Vector2f(0, 0));
    background.setTexture(&backgroundTexture);
    OutputLog("загружен background");


////////////////////////////////////////////////////////////////////////////////////
    
    
    


















//////////////////////// ДИАЛОГОВОЕ ОКНО ВВОДА ////////////////////////

////////////////////////////////////////////////////////////////////////////////

    
    vector<pair<RectangleShape, Text>> InputCharacteristicsShapeText;
    for (int i = 0, j = 0; i < 7 && j < 3;)
    {
        RectangleShape TempInputCharacteristicsShape;
        TempInputCharacteristicsShape.setSize(Vector2f(static_cast<float>(LengthCharacteristicsInputWindow / 7), static_cast<float>(HeightCharacteristicsInputWindow / 3)));
        TempInputCharacteristicsShape.setPosition(Vector2f(static_cast<float>((LengthCharacteristicsInputWindow / 7) * i), static_cast<float>((HeightCharacteristicsInputWindow / 3) * j)));
        TempInputCharacteristicsShape.setOutlineColor(Color::Red);

        Text TempInputCharacteristicsText(CurrentFont);

        if      (j == 0) { TempInputCharacteristicsText.setString(L"↑"); }
        else if (j == 2) { TempInputCharacteristicsText.setString(L"↓"); }
        else if (j == 1) 
        { 
            TempInputCharacteristicsText.setString("0.00");
        }

        TempInputCharacteristicsText.setCharacterSize(SizeFont + 3);
        TempInputCharacteristicsText.setFillColor(Color::Black);
        TempInputCharacteristicsText.setCharacterSize(HeightCharacteristicsInputWindow / 5);

        FloatRect textBounds3 = TempInputCharacteristicsText.getLocalBounds();
        TempInputCharacteristicsText.setOrigin(Vector2f(textBounds3.position.x + textBounds3.size.x / 2.0f, textBounds3.position.y + textBounds3.size.y / 2.0f));

        Vector2f rectPos3 = TempInputCharacteristicsShape.getPosition();
        Vector2f rectSize3 = TempInputCharacteristicsShape.getSize();
        
        //j == 1 ? TempInputCharacteristicsText.setPosition(Vector2f(rectPos3.x + rectSize3.x / 4.0f, rectPos3.y + rectSize3.y / 2.0f)) : 
        TempInputCharacteristicsText.setPosition(Vector2f(rectPos3.x + rectSize3.x / 2.0f, rectPos3.y + rectSize3.y / 2.0f));


        InputCharacteristicsShapeText.push_back(make_pair(TempInputCharacteristicsShape, TempInputCharacteristicsText));

        ///// условие прохождения
        i++;
        if (i >= 7) {  i = 0;  j++;  }
    }
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // создаем окно
    RenderWindow window(VideoMode({ static_cast<unsigned int>(SizeWindowLength), static_cast<unsigned int>(SizeWindowHeight) }), "ModdingGun");
    RenderWindow Diagram;
    RenderWindow InputWindow;
    window.setFramerateLimit(150);
    InputWindow.setFramerateLimit(150);

////////////////////////////////////////////////////////////////////////////////



















    //////////////////////// ОТОБРАЖЕНИЕ МЕТОДА ////////////////////////

    ///////////////////////////////////////////////////////////////////////////////////
    Text TextCurrentMethod(CurrentFont);
    RectangleShape CurrentMethod;


    CurrentMethod.setSize(Vector2f(SizeCell * 13, SizeCell / 2));
    CurrentMethod.setOutlineThickness(-3);
    CurrentMethod.setOutlineColor(Color::Green);
    CurrentMethod.setPosition(Vector2f(0, SizeCell * (CountCellOnHeightWindow + 4)));


    wstring StandartMethod = L"Метод Standart: кучность -> темп -> пробитие -> отдача -> качание -> отказ -> отказ ";
    wstring MaxModMethod   = L"Метод Max Mod: отказ -> отказ -> отдача  -> кучность -> темп -> пробитие -> качание";

    vector<int> StandartMethodV = {
        GunStats::ACCURACY,
        GunStats::RATE_OF_FIRE,
        GunStats::PENETRATION,
        GunStats::KICKBACK,
        GunStats::SWAY,
        GunStats::MALFUNCTION_CONDITION,
        GunStats::MALFUNCTION_DIRT
    };
    vector<int> MaxModMethodV = {
        GunStats::MALFUNCTION_CONDITION,
        GunStats::MALFUNCTION_DIRT,
        GunStats::KICKBACK,
        GunStats::ACCURACY,
        GunStats::RATE_OF_FIRE,
        GunStats::PENETRATION,
        GunStats::SWAY,
    };

    TextCurrentMethod.setString(StandartMethod);
    TextCurrentMethod.setCharacterSize(SizeFont + 3);
    TextCurrentMethod.setFillColor(Color::Black);

    textBounds = TextCurrentMethod.getLocalBounds();
    rectPos = CurrentMethod.getPosition();
    rectSize = CurrentMethod.getSize();

    TextCurrentMethod.setOrigin(Vector2f(0.f, textBounds.position.y + textBounds.size.y / 2.0f));
    TextCurrentMethod.setPosition(Vector2f(rectPos.x + 10, rectPos.y + rectSize.y / 2.0f));


    ///////////////////////////////////////////////////////////////////////////////////






































/////////////////////// ПРОЧИЕ ПЕРЕМЕННЫЕ ////////////////////////

////////////////////////////////////////////////////////////////////////////////

    // используем отрисовку графика только один раз чтобы не настал пиздец
    // с помощью DiagramGrafBool указывает какие окна отрисовали
    // false = отрисованно -> больше не будет повторяться
    vector<bool>         DiagramBool;

    // вектор окон диаграм с отрисованными графикам
    // графики отрисовываются один раз -> DiagramBool
    vector<RenderWindow> VectorDiagram;




    // выгрузка данных из файла оружия
    CharacteristicGun CurrentCharacteristicGun;





    // для окна ввода (указывает куда будут записанны данные)
    // false = в указанный мод
    // true = в начальный мод
    bool Initial_Input_Window = false;



    // true  = начинаем с указанного начаольного мода
    // false = начинаем с нуля
    // изменяется кнопкой начального мода
    bool InitialMod_ON_or_OFF = false;



    






    // общие дефолт статы для 
    // CharacteristicsModded 
    // CharacteristicsInitial
    // inputVector    
    const vector<int>    Characteristics_DEFAULT{ 60, 48, 40, 40, 40, 40, 40 };

    // характеристики мода
    // оращаемся только через ----- GunStats::      например       GunStats::ACCURACY
    vector<int> CharacteristicsModded = Characteristics_DEFAULT;

    // начальный мод
    // оращаемся только через ----- GunStats::      например      GunStats::KICKBACK
    vector<int> CharacteristicsInitial = Characteristics_DEFAULT;

    // временное хранение характеристик из окна ввода
    // обращаемся по индексам
    // 0 куч
    // 1 темп
    // 2 пробитие
    // 3 отдача
    // 4 качание
    // 6 сост
    // 5 грязь
    vector<int> inputVector = Characteristics_DEFAULT;










    // дефолт статы для 
    // RESULT_CHARACTERISTIC 
    // START_CHARACTERISTIC
    // INITIAL_CHARACTERISTIC
    const vector<double> DEFAULT_RESULT_CHARACTERISTIC = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, };


    // получившийся мод
    // использовать только в области сборки calculate / vector для передачи в drawNormalGraph
    vector<double> RESULT_CHARACTERISTIC = DEFAULT_RESULT_CHARACTERISTIC;

    // указанный мод
    // использовать только в области сборки calculate / vector для передачи в drawNormalGraph
    vector<double> START_CHARACTERISTIC = DEFAULT_RESULT_CHARACTERISTIC;

    // начальный мод 
    // использовать только в области сборки calculate / vector для передачи в drawNormalGraph
    vector<double> INITIAL_CHARACTERISTIC = DEFAULT_RESULT_CHARACTERISTIC;



    // имя передаваемое в drawNormalGraph (?)
    wstring NameGun = L"Empty";



////////////////////////////////////////////////////////////////////////////////















////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


    for (int WI = 0; window.isOpen() ; WI++)
    {
        if (WI > 100) { WI = 0; }

        PositionMouse = Mouse::getPosition(window);
        MouseWorldPos = window.mapPixelToCoords(PositionMouse);



        while (const optional event = window.pollEvent())
        {
            if (event->is<Event::Closed>())
            {
                window.close();
                InputWindow.close();

                for (int i = 0; i < VectorDiagram.size(); i++)
                {
                    VectorDiagram[i].close();
                }

            }







            // выбор
            else if (const auto& mouseButtonPressed = event->getIf<Event::MouseButtonPressed>())
            {
                if (mouseButtonPressed->button == Mouse::Button::Left)
                {










                    /// нажали икнонки оружия
                    for (size_t it = 0; it < VectorTextureGun.size(); it++)
                    {
                        if (get<0>(VectorTextureGun[it]).getGlobalBounds().contains(MouseWorldPos))
                        {
                            SelectedGunShape.setTexture(get<1>(VectorTextureGun[it]).get(), true);

                            // получаем имя пушки
                            NameGun = get<2>(VectorTextureGun[it]);

                            // получаем расположение пушки
                            wstring FileStat = L"Attribute/Tier_" + to_wstring(get<3>(VectorTextureGun[it])) + "/" + NameGun + L".txt";


                            // получаем статы пушки
                            if (!CurrentCharacteristicGun.Load(FileStat))
                            {
                                NameGun = L"Empty";
                                TextInfoDebug.setString(L"ошибка файла");
                                break;
                            }
                            TextInfoDebug.setString(L"файл загружен");
                            break;
                        }
                    }




















                    /// нажали поле ввода мода к которому замодаемся
                    if (СharacteristicsModGun.getGlobalBounds().contains(MouseWorldPos))
                    {
                        СharacteristicsModGun.setOutlineColor(Color::Red);
                        window.draw(СharacteristicsModGun);
                        window.display();



                        inputVector[0] = CharacteristicsModded[GunStats::ACCURACY];             // 0 куч
                        inputVector[1] = CharacteristicsModded[GunStats::RATE_OF_FIRE];         // 1 темп
                        inputVector[2] = CharacteristicsModded[GunStats::PENETRATION];          // 2 пробитие
                        inputVector[3] = CharacteristicsModded[GunStats::KICKBACK];             // 3 отдача
                        inputVector[4] = CharacteristicsModded[GunStats::SWAY];                 // 4 качание
                        inputVector[5] = CharacteristicsModded[GunStats::MALFUNCTION_CONDITION];// 6 сост
                        inputVector[6] = CharacteristicsModded[GunStats::MALFUNCTION_DIRT];     // 5 грязь

                        for (int SelectOption = 0; SelectOption < inputVector.size(); SelectOption++)
                        {
                            if      (SelectOption == 0) { InputCharacteristicsShapeText[SelectOption + 7].second.setString(format("{:.1f}", GunStats::Accuracy_VStat[inputVector[SelectOption]])); }
                            else if (SelectOption == 1) { InputCharacteristicsShapeText[SelectOption + 7].second.setString(format("{:.1f}", GunStats::RateFire_VStat[inputVector[SelectOption]])); }
                            else                        { InputCharacteristicsShapeText[SelectOption + 7].second.setString(format("{:.1f}", GunStats::Other_VStat[inputVector[SelectOption]])); }
                        }



                        InputWindow.create(VideoMode({ LengthCharacteristicsInputWindow, HeightCharacteristicsInputWindow }), "CharacteristicsCurrent");
                        InputWindow.setFramerateLimit(150);

                        Initial_Input_Window = false;
                    }





                    // нажали поле вводе начального мода
                    else if (СharacteristicsModGunInitial.getGlobalBounds().contains(MouseWorldPos))
                    {
                        СharacteristicsModGunInitial.setOutlineColor(Color::Red);
                        window.draw(СharacteristicsModGunInitial);
                        window.display();


                        Initial_Input_Window = true;
                        
                        inputVector[0] = CharacteristicsInitial[GunStats::ACCURACY]              ;     // 0 куч
                        inputVector[1] = CharacteristicsInitial[GunStats::RATE_OF_FIRE]          ;     // 1 темп
                        inputVector[2] = CharacteristicsInitial[GunStats::PENETRATION]           ;     // 2 пробитие
                        inputVector[3] = CharacteristicsInitial[GunStats::KICKBACK]              ;     // 3 отдача
                        inputVector[4] = CharacteristicsInitial[GunStats::SWAY]                  ;     // 4 качание
                        inputVector[5] = CharacteristicsInitial[GunStats::MALFUNCTION_CONDITION] ;     // 6 сост
                        inputVector[6] = CharacteristicsInitial[GunStats::MALFUNCTION_DIRT]      ;     // 5 грязь

                        for (int SelectOption = 0; SelectOption < inputVector.size(); SelectOption++)
                        {
                            if      (SelectOption == 0) { InputCharacteristicsShapeText[SelectOption + 7].second.setString(format("{:.1f}", GunStats::Accuracy_VStat[inputVector[SelectOption]])); }
                            else if (SelectOption == 1) { InputCharacteristicsShapeText[SelectOption + 7].second.setString(format("{:.1f}", GunStats::RateFire_VStat[inputVector[SelectOption]])); }
                            else                        { InputCharacteristicsShapeText[SelectOption + 7].second.setString(format("{:.1f}", GunStats::Other_VStat[inputVector[SelectOption]])); }
                        }

                        InputWindow.create(VideoMode({ LengthCharacteristicsInputWindow, HeightCharacteristicsInputWindow }), "CharacteristicsCurrent");
                        InputWindow.setFramerateLimit(150);
                    }
























                    // нажали рассчет мода
                    else if (Calculation.getGlobalBounds().contains(MouseWorldPos))
                    {



                        Average10000.clear();
                        Average10000_RANDOM_GEN.clear();
                        Calculation.setOutlineColor(Color::Red);
                        window.draw(Calculation);



                        


                        // загружен ли файл?
                        if (!CurrentCharacteristicGun.is_Empty())
                        {
                            TextInfoDebug.setString(L"Расчет");                            
                            
                            CurrentCharacteristicGun.ReturnDefaultstat();
                        


                            // пытаемся занизить мод?
                            bool understatement = false;

                            // работает при выключенном начальном моде InitialMod_ON_or_OFF
                            if (   GunStats::Accuracy_VStat[CharacteristicsModded[GunStats::ACCURACY]] < CurrentCharacteristicGun.GetVisualProcentStat(GunStats::ACCURACY)
                                || GunStats::RateFire_VStat[CharacteristicsModded[GunStats::RATE_OF_FIRE]] < CurrentCharacteristicGun.GetVisualProcentStat(GunStats::RATE_OF_FIRE)
                                || GunStats::Other_VStat[CharacteristicsModded[GunStats::PENETRATION]] < CurrentCharacteristicGun.GetVisualProcentStat(GunStats::PENETRATION)
                                || GunStats::Other_VStat[CharacteristicsModded[GunStats::KICKBACK]] < CurrentCharacteristicGun.GetVisualProcentStat(GunStats::KICKBACK)
                                || GunStats::Other_VStat[CharacteristicsModded[GunStats::SWAY]] < CurrentCharacteristicGun.GetVisualProcentStat(GunStats::SWAY)
                                || GunStats::Other_VStat[CharacteristicsModded[GunStats::MALFUNCTION_DIRT]] < CurrentCharacteristicGun.GetVisualProcentStat(GunStats::MALFUNCTION_DIRT)
                                || GunStats::Other_VStat[CharacteristicsModded[GunStats::MALFUNCTION_CONDITION]] < CurrentCharacteristicGun.GetVisualProcentStat(GunStats::MALFUNCTION_CONDITION)
                                && !InitialMod_ON_or_OFF
                                )
                            {
                                understatement = true;
                            }

                            // если мод не пытаемся занизить 
                            if (!understatement)
                            {


                                // МОД СОВПАДАЕТ?
                                bool ModMatches = false;

                                if (   GunStats::Accuracy_VStat[CharacteristicsModded[GunStats::ACCURACY]]           == CurrentCharacteristicGun.GetVisualProcentStat(GunStats::ACCURACY)
                                    && GunStats::RateFire_VStat[CharacteristicsModded[GunStats::RATE_OF_FIRE]]       == CurrentCharacteristicGun.GetVisualProcentStat(GunStats::RATE_OF_FIRE)
                                    && GunStats::Other_VStat[CharacteristicsModded[GunStats::PENETRATION]]           == CurrentCharacteristicGun.GetVisualProcentStat(GunStats::PENETRATION)
                                    && GunStats::Other_VStat[CharacteristicsModded[GunStats::KICKBACK]]              == CurrentCharacteristicGun.GetVisualProcentStat(GunStats::KICKBACK)
                                    && GunStats::Other_VStat[CharacteristicsModded[GunStats::SWAY]]                  == CurrentCharacteristicGun.GetVisualProcentStat(GunStats::SWAY)
                                    && GunStats::Other_VStat[CharacteristicsModded[GunStats::MALFUNCTION_DIRT]]      == CurrentCharacteristicGun.GetVisualProcentStat(GunStats::MALFUNCTION_DIRT)
                                    && GunStats::Other_VStat[CharacteristicsModded[GunStats::MALFUNCTION_CONDITION]] == CurrentCharacteristicGun.GetVisualProcentStat(GunStats::MALFUNCTION_CONDITION)
                                    && !InitialMod_ON_or_OFF
                                    )
                                {
                                    ModMatches = true;
                                }

                                // если мод совпадает с начальными характеристиками ===>>>>> с включеным начальным модом
                                if (CharacteristicsModded == CharacteristicsInitial && InitialMod_ON_or_OFF)
                                {
                                    ModMatches = true;
                                }




                                // если не совпадает
                                if (!ModMatches)
                                {


                                    // реален ли мод?
                                    bool exists_mod = true;

                                    if (GunStats::Accuracy_VStat[CharacteristicsModded[GunStats::ACCURACY]] > CurrentCharacteristicGun.GetMaxStatVisualProcent(GunStats::ACCURACY)) { TextInfoDebug.setString(L"кучность выше максимума"); exists_mod = false; }
                                    else if (GunStats::RateFire_VStat[CharacteristicsModded[GunStats::RATE_OF_FIRE]] > CurrentCharacteristicGun.GetMaxStatVisualProcent(GunStats::RATE_OF_FIRE)) { TextInfoDebug.setString(L"темп стрельбы выше максимума"); exists_mod = false; }
                                    else if (GunStats::Other_VStat[CharacteristicsModded[GunStats::PENETRATION]] > CurrentCharacteristicGun.GetMaxStatVisualProcent(GunStats::PENETRATION)) { TextInfoDebug.setString(L"пробитие выше максимума"); exists_mod = false; }
                                    else if (GunStats::Other_VStat[CharacteristicsModded[GunStats::KICKBACK]] > CurrentCharacteristicGun.GetMaxStatVisualProcent(GunStats::KICKBACK)) { TextInfoDebug.setString(L"отдача выше максимума"); exists_mod = false; }
                                    else if (GunStats::Other_VStat[CharacteristicsModded[GunStats::SWAY]] > CurrentCharacteristicGun.GetMaxStatVisualProcent(GunStats::SWAY)) { TextInfoDebug.setString(L"качание выше максимума"); exists_mod = false; }
                                    else if (GunStats::Other_VStat[CharacteristicsModded[GunStats::MALFUNCTION_DIRT]] > CurrentCharacteristicGun.GetMaxStatVisualProcent(GunStats::MALFUNCTION_DIRT)) { TextInfoDebug.setString(L"отказ от грязи выше максимума"); exists_mod = false; }
                                    else if (GunStats::Other_VStat[CharacteristicsModded[GunStats::MALFUNCTION_CONDITION]] > CurrentCharacteristicGun.GetMaxStatVisualProcent(GunStats::MALFUNCTION_CONDITION)) { TextInfoDebug.setString(L"отказ от состояния выше максимума"); exists_mod = false; }







                                    /// проверка у нас мод с нуля?

                                    //получилось ли домодать?
                                    bool ReUpgrade = false;

                                    // если включена кнопка начального мода, то есть начинаем с указанного начального мода
                                    if (InitialMod_ON_or_OFF)
                                    {
                                        //значит не с нуля, значит до_модифицируем до нужных статов
                                        for (size_t itm = 0; itm < method.size(); itm++)
                                        {
                                            int option = method[itm];

                                            for (int i = 0; i <= 200; i++)
                                            {


                                                // получаем визуальный процент прибавки указанного мода указанным методом
                                                double ModdedVisualProcent;

                                                if (option == GunStats::ACCURACY) { ModdedVisualProcent = GunStats::Accuracy_VStat[CharacteristicsInitial[GunStats::ACCURACY]]; }
                                                else if (option == GunStats::RATE_OF_FIRE) { ModdedVisualProcent = GunStats::RateFire_VStat[CharacteristicsInitial[GunStats::RATE_OF_FIRE]]; }
                                                else { ModdedVisualProcent = GunStats::Other_VStat[CharacteristicsInitial[option]]; }

                                                if (CurrentCharacteristicGun.GetVisualProcentStat(option) == ModdedVisualProcent)
                                                {
                                                    ReUpgrade = true;
                                                    break;
                                                }
                                                if (i == 200)
                                                {
                                                    TextInfoDebug.setString(L"не получилось домодать до начальнных статов");
                                                    OutputLog(" НЕ ПОЛУЧИЛОСЬ ДОМОДАТЬ ????");
                                                    itm = method.size();
                                                    break;
                                                }

                                                if (GunStats::Accuracy_VStat[CharacteristicsInitial[GunStats::ACCURACY]] > CurrentCharacteristicGun.GetMaxStatVisualProcent(GunStats::ACCURACY)) { TextInfoDebug.setString(L"начальная кучность выше максимума");           ReUpgrade = false; itm = method.size(); break; }
                                                else if (GunStats::RateFire_VStat[CharacteristicsInitial[GunStats::RATE_OF_FIRE]] > CurrentCharacteristicGun.GetMaxStatVisualProcent(GunStats::RATE_OF_FIRE)) { TextInfoDebug.setString(L"начальная темп стрельбы выше максимума");      ReUpgrade = false; itm = method.size(); break; }
                                                else if (GunStats::Other_VStat[CharacteristicsInitial[GunStats::PENETRATION]] > CurrentCharacteristicGun.GetMaxStatVisualProcent(GunStats::PENETRATION)) { TextInfoDebug.setString(L"начальная пробитие выше максимума");           ReUpgrade = false; itm = method.size(); break; }
                                                else if (GunStats::Other_VStat[CharacteristicsInitial[GunStats::KICKBACK]] > CurrentCharacteristicGun.GetMaxStatVisualProcent(GunStats::KICKBACK)) { TextInfoDebug.setString(L"начальная отдача выше максимума");             ReUpgrade = false; itm = method.size(); break; }
                                                else if (GunStats::Other_VStat[CharacteristicsInitial[GunStats::SWAY]] > CurrentCharacteristicGun.GetMaxStatVisualProcent(GunStats::SWAY)) { TextInfoDebug.setString(L"начальная качание выше максимума");            ReUpgrade = false; itm = method.size(); break; }
                                                else if (GunStats::Other_VStat[CharacteristicsInitial[GunStats::MALFUNCTION_DIRT]] > CurrentCharacteristicGun.GetMaxStatVisualProcent(GunStats::MALFUNCTION_DIRT)) { TextInfoDebug.setString(L"начальная отказ от грязи выше максимума");     ReUpgrade = false; itm = method.size(); break; }
                                                else if (GunStats::Other_VStat[CharacteristicsInitial[GunStats::MALFUNCTION_CONDITION]] > CurrentCharacteristicGun.GetMaxStatVisualProcent(GunStats::MALFUNCTION_CONDITION)) { TextInfoDebug.setString(L"начальная отказ от состояния выше максимума"); ReUpgrade = false; itm = method.size(); break; }


                                                CurrentCharacteristicGun.UpgradeStat(option);
                                            }
                                        }
                                    }
                                    else
                                    {
                                        ReUpgrade = true;
                                    }

                                    window.draw(background);
                                    Calculation.setOutlineColor(Color::Red);
                                    window.draw(Calculation);
                                    window.display();


                                    if (ReUpgrade)
                                    {
                                        if (exists_mod)
                                        {









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

                                                            CharacteristicGun gunCopy = CurrentCharacteristicGun;


                                                            RunSingleAssembly(
                                                                j,
                                                                gunCopy,
                                                                method,
                                                                CharacteristicsInitial,
                                                                CharacteristicsModded,
                                                                InitialMod_ON_or_OFF,
                                                                get<0>(Tool_Kit_Skil),
                                                                get<1>(Tool_Kit_Skil),
                                                                get<2>(Tool_Kit_Skil)
                                                            );

                                                        }


                                                    });

                                                start = end;
                                            }

                                            // Ждём завершения всех потоков
                                            for (auto& t : threads_v) t.join();

















                                            CurrentCharacteristicGun = RunSingleAssembly(
                                                -1,
                                                CurrentCharacteristicGun,
                                                method,
                                                CharacteristicsInitial,
                                                CharacteristicsModded,
                                                InitialMod_ON_or_OFF,
                                                get<0>(Tool_Kit_Skil),
                                                get<1>(Tool_Kit_Skil),
                                                get<2>(Tool_Kit_Skil)
                                            );



                                            // чистим переменные чтобы записать в них отображаемые данны для графика
                                            RESULT_CHARACTERISTIC = CurrentCharacteristicGun.GetFullCurrentVisualStat();
                                            START_CHARACTERISTIC.clear();
                                            INITIAL_CHARACTERISTIC.clear();









                                            for (int i = 0; i < CharacteristicsModded.size(); i++)
                                            {
                                                if (i == 0) { START_CHARACTERISTIC.push_back(GunStats::Accuracy_VStat[CharacteristicsModded[i]]); }
                                                else if (i == 1) { START_CHARACTERISTIC.push_back(GunStats::RateFire_VStat[CharacteristicsModded[i]]); }
                                                else { START_CHARACTERISTIC.push_back(GunStats::Other_VStat[CharacteristicsModded[i]]); }
                                            }

                                            if (InitialMod_ON_or_OFF)
                                            {
                                                for (int i = 0; i < CharacteristicsInitial.size(); i++)
                                                {
                                                    if (i == 0) { INITIAL_CHARACTERISTIC.push_back(GunStats::Accuracy_VStat[CharacteristicsInitial[i]]); }
                                                    else if (i == 1) { INITIAL_CHARACTERISTIC.push_back(GunStats::RateFire_VStat[CharacteristicsInitial[i]]); }
                                                    else { INITIAL_CHARACTERISTIC.push_back(GunStats::Other_VStat[CharacteristicsInitial[i]]); }
                                                }
                                            }
                                            else
                                            {
                                                INITIAL_CHARACTERISTIC = DEFAULT_RESULT_CHARACTERISTIC;
                                            }



                                            Diagram.create(VideoMode({ sizeXGrafWindow, sizeYGrafWindow }), "Diagram");
                                            Diagram.setFramerateLimit(150);

                                            VectorDiagram.push_back(move(Diagram));
                                            DiagramBool.push_back(false);

                                            Diagram.close();

                                            if (!CurrentCharacteristicGun.ReturnDefaultstat())
                                            {
                                                // выход
                                                OutputLog("не удалось вернуть начальные статы, почему?");
                                                TextInfoDebug.setString(L"не удалось вернуть начальные статы, почему?");

                                            }
                                        }
                                    }
                                }
                                else
                                {
                                    TextInfoDebug.setString(L"мод совпадает");

                                }
                            }
                            else
                            {
                                TextInfoDebug.setString(L"попытка уменьшить мод");
                            }

                        }
                        else
                        {
                            TextInfoDebug.setString(L"файл не загружен");
                        }
                    }

















                    // кнопка начального мода
                    else if (ButtonСharacteristicsModGunInitial.getGlobalBounds().contains(MouseWorldPos))
                    {
                        if (InitialMod_ON_or_OFF)
                        {
                            InitialMod_ON_or_OFF = false;
                            ButtonСharacteristicsModGunInitial.setTexture(RedMark.get(), true);

                        }
                        else
                        {
                            InitialMod_ON_or_OFF = true;
                            ButtonСharacteristicsModGunInitial.setTexture(GreenMark.get(), true);

                        }
                    }


















                    // окно смены колличества повторений
                    else if (ShapeTargetIter.getGlobalBounds().contains(MouseWorldPos))
                    {
                        ShapeTargetIter.setOutlineColor(Color::Red);
                        totalIterations < 5000 ? totalIterations += 1000 : totalIterations = 500;
                        TextTargetIter.setString(L"колличество сборок: " + to_string(totalIterations));
                    }
















                    // окно метода
                    else if (CurrentMethod.getGlobalBounds().contains(MouseWorldPos))
                    {
                        if (method == StandartMethodV)
                        {
                            method = MaxModMethodV;
                            TextCurrentMethod.setString(MaxModMethod);
                            CurrentMethod.setOutlineColor(Color::Red);
                        }
                        else
                        {
                            method = StandartMethodV;
                            TextCurrentMethod.setString(StandartMethod);
                            CurrentMethod.setOutlineColor(Color::Red);
                        }

                    }





















                    // инструменты
                    for (int i = 0; i < ToolShapes.size(); i++)
                    {
                        if (ToolShapes[i].getGlobalBounds().contains(MouseWorldPos))
                        {
                            for (auto& sh : ToolShapes)
                            {
                                sh.setOutlineColor(Color::Black);
                            }

                            switch (i)
                            {

                            case 0:     ToolShapes[i].setOutlineColor(Color::Green);    get<0>(Tool_Kit_Skil) = GunStats::GetToolModifier(GunStats::ToolType::NO_TOOL);          break;
                            case 1:     ToolShapes[i].setOutlineColor(Color::Green);    get<0>(Tool_Kit_Skil) = GunStats::GetToolModifier(GunStats::ToolType::IMPROVED_TOOL);    break;
                            case 2:     ToolShapes[i].setOutlineColor(Color::Green);    get<0>(Tool_Kit_Skil) = GunStats::GetToolModifier(GunStats::ToolType::NORMAL_TOOL);      break;
                            case 3:     ToolShapes[i].setOutlineColor(Color::Green);    get<0>(Tool_Kit_Skil) = GunStats::GetToolModifier(GunStats::ToolType::OLD_TOOL);         break;

                            default:
                                break;
                            }
                        }
                    }









                    // наборы
                    for (int i = 0; i < KitShapes.size(); i++)
                    {
                        if (KitShapes[i].getGlobalBounds().contains(MouseWorldPos))
                        {
                            for (auto& sh : KitShapes)
                            {
                                sh.setOutlineColor(Color::Black);
                            }

                            switch (i)
                            {

                            case 0:     KitShapes[i].setOutlineColor(Color::Green);    get<1>(Tool_Kit_Skil) = GunStats::GetKitModifier(GunStats::KitType::NO_KIT);        break;
                            case 1:     KitShapes[i].setOutlineColor(Color::Green);    get<1>(Tool_Kit_Skil) = GunStats::GetKitModifier(GunStats::KitType::DETAIL_KIT);    break;
                            case 2:     KitShapes[i].setOutlineColor(Color::Green);    get<1>(Tool_Kit_Skil) = GunStats::GetKitModifier(GunStats::KitType::REPAIR_KIT);    break;
                                 
                            default:
                                break;
                            }
                        }

                    }










                    // скилы
                    for (int i = 0; i < SkillShapes.size(); i++)
                    {
                        if (SkillShapes[i].getGlobalBounds().contains(MouseWorldPos))
                        {
                            for (auto& sh : SkillShapes)
                            {
                                sh.setOutlineColor(Color::Black);
                            }

                            switch (i)
                            {

                            case 0:     SkillShapes[i].setOutlineColor(Color::Green);    get<2>(Tool_Kit_Skil) = GunStats::GetSkillModifier(GunStats::SkillType::PLAYER_NO_SKILL);          break;
                            case 1:     SkillShapes[i].setOutlineColor(Color::Green);    get<2>(Tool_Kit_Skil) = GunStats::GetSkillModifier(GunStats::SkillType::PLAYER_MASTER_1);          break;
                            case 2:     SkillShapes[i].setOutlineColor(Color::Green);    get<2>(Tool_Kit_Skil) = GunStats::GetSkillModifier(GunStats::SkillType::PLAYER_MASTER_2);          break;
                            case 3:     SkillShapes[i].setOutlineColor(Color::Green);    get<2>(Tool_Kit_Skil) = GunStats::GetSkillModifier(GunStats::SkillType::PLAYER_MASTER_3);          break;
                            case 4:     SkillShapes[i].setOutlineColor(Color::Green);    get<2>(Tool_Kit_Skil) = GunStats::GetSkillModifier(GunStats::SkillType::PLAYER_MASTER_4);          break;
                            case 5:     SkillShapes[i].setOutlineColor(Color::Green);    get<2>(Tool_Kit_Skil) = GunStats::GetSkillModifier(GunStats::SkillType::PLAYER_MASTER_5);          break;
                            case 6:     SkillShapes[i].setOutlineColor(Color::Green);    get<2>(Tool_Kit_Skil) = GunStats::GetSkillModifier(GunStats::SkillType::NPC_MASTER_2);          break;
                            case 7:     SkillShapes[i].setOutlineColor(Color::Green);    get<2>(Tool_Kit_Skil) = GunStats::GetSkillModifier(GunStats::SkillType::NPC_MASTER_3);          break;
                            case 8:     SkillShapes[i].setOutlineColor(Color::Green);    get<2>(Tool_Kit_Skil) = GunStats::GetSkillModifier(GunStats::SkillType::NPC_MASTER_4);          break;
                            case 9:     SkillShapes[i].setOutlineColor(Color::Green);    get<2>(Tool_Kit_Skil) = GunStats::GetSkillModifier(GunStats::SkillType::NPC_MASTER_5);          break;


                            default:
                                break;
                            }
                        }
                    }



                }

            }
        }        














        window.clear(Color::White);
        window.draw(background);


        for (int i = 0; i < VectorTextureGun.size(); i++)
        {
            window.draw(get<0>(VectorTextureGun[i]));
            window.draw(VectorNameGun[i]);
        }


        for (int i = 0; i < ToolShapes.size(); i++)
        {
            window.draw(ToolShapes[i]);
        }
        for (int i = 0; i < KitShapes.size(); i++)
        {
            window.draw(KitShapes[i]);
        }
        for (int i = 0; i < SkillShapes.size(); i++)
        {
            window.draw(SkillShapes[i]);
        }

        window.draw(SelectedGunShape);
        window.draw(Calculation);
        window.draw(TextCalculation);

        window.draw(ShapeInfoDebug);
        window.draw(TextInfoDebug);

        window.draw(СharacteristicsModGun);
        window.draw(TextСharacteristicsModGun);

        window.draw(СharacteristicsModGunInitial);
        window.draw(TextСharacteristicsModGunInitial);

        window.draw(ButtonСharacteristicsModGunInitial);

        window.draw(ShapeTargetIter);
        window.draw(TextTargetIter);


        window.draw(CurrentMethod);
        window.draw(TextCurrentMethod);


        window.display();




        if (WI % 50 == 10)
        {
            СharacteristicsModGun.setOutlineColor(Color::Green);
            СharacteristicsModGunInitial.setOutlineColor(Color::Green);
            Calculation.setOutlineColor(Color::Green);
            ShapeTargetIter.setOutlineColor(Color::Green);
            CurrentMethod.setOutlineColor(Color::Green);
        }










        ////////////////////////////////////////////////////////////////////////////////
        // окно ввода мода
        if (InputWindow.isOpen())
        {
            PositionMouse = Mouse::getPosition(InputWindow);
            MouseWorldPos = InputWindow.mapPixelToCoords(PositionMouse);


            if (const optional eventD = InputWindow.pollEvent())
            {
                if (eventD->is<Event::Closed>())
                {

                    //ставим строку в главное окно
                    string Сharacteristics_Input = " | ";

                    for (int i = 7; i < 14; i++)
                    {
                        Сharacteristics_Input += InputCharacteristicsShapeText[i].second.getString();
                        Сharacteristics_Input += " | ";
                    }
                    

                    //// характеристики мода
                    //vector<int> CharacteristicsModded;
                    //// начальный мод
                    //vector<int> CharacteristicsInitial;

                    if (Initial_Input_Window)
                    {
                        TextСharacteristicsModGunInitial.setString(Сharacteristics_Input);                    


                        CharacteristicsInitial[GunStats::ACCURACY]              = inputVector[0];     // 0 куч
                        CharacteristicsInitial[GunStats::RATE_OF_FIRE]          = inputVector[1];     // 1 темп
                        CharacteristicsInitial[GunStats::PENETRATION]           = inputVector[2];     // 2 пробитие
                        CharacteristicsInitial[GunStats::KICKBACK]              = inputVector[3];     // 3 отдача
                        CharacteristicsInitial[GunStats::SWAY]                  = inputVector[4];     // 4 качание
                        CharacteristicsInitial[GunStats::MALFUNCTION_CONDITION] = inputVector[5];     // 6 сост
                        CharacteristicsInitial[GunStats::MALFUNCTION_DIRT]      = inputVector[6];     // 5 грязь
                        // |^||^||^||^||^||^|
                        // 0 куч 
                        // 1 темп
                        // 2 отдача
                        // 3 качание
                        // 4 пробитие
                        // 5 сост
                        // 6 грязь
                        /////////////////////

                    }
                    else
                    {
                        TextСharacteristicsModGun.setString(Сharacteristics_Input);

                        CharacteristicsModded[GunStats::ACCURACY]              = inputVector[0];     // 0 куч
                        CharacteristicsModded[GunStats::RATE_OF_FIRE]          = inputVector[1];     // 1 темп
                        CharacteristicsModded[GunStats::PENETRATION]           = inputVector[2];     // 2 пробитие
                        CharacteristicsModded[GunStats::KICKBACK]              = inputVector[3];     // 3 отдача
                        CharacteristicsModded[GunStats::SWAY]                  = inputVector[4];     // 4 качание
                        CharacteristicsModded[GunStats::MALFUNCTION_CONDITION] = inputVector[5];     // 6 сост
                        CharacteristicsModded[GunStats::MALFUNCTION_DIRT]      = inputVector[6];     // 5 грязь
                        // |^||^||^||^||^||^|
                        // 0 куч 
                        // 1 темп
                        // 2 отдача
                        // 3 качание
                        // 4 пробитие
                        // 5 сост
                        // 6 грязь
                        /////////////////////

                    }




                    InputWindow.close();
                }

                // нажали лкм
                else if (const auto& mouseButtonPressed = eventD->getIf<Event::MouseButtonPressed>())
                {
                    if (mouseButtonPressed->button == Mouse::Button::Left)
                    {

                        for (size_t SelectOption = 0; SelectOption < InputCharacteristicsShapeText.size(); SelectOption++)
                        {
                            // если попали по стрелочкам
                            if (InputCharacteristicsShapeText[SelectOption].first.getGlobalBounds().contains(MouseWorldPos) && (SelectOption < 7 || SelectOption >= 14))
                            {
                                InputCharacteristicsShapeText[SelectOption].first.setOutlineThickness(-3);

                                // нажали повысить
                                if (InputCharacteristicsShapeText[SelectOption].second.getString() == L"↑"){ inputVector[SelectOption]++;}
                                // нажали понизить
                                else if (InputCharacteristicsShapeText[SelectOption].second.getString() == L"↓"){ inputVector[SelectOption - 14]--;}
                               
                                auto clamp = [](int& val, size_t max) {
                                    if (val >= static_cast<int>(max)) val--;
                                    if (val < 0) val++;
                                    };

                                // Для 0 — AccuracyStat
                                clamp(inputVector[0], GunStats::Accuracy_VStat.size());
                                // Для 1 — RateFireStat
                                clamp(inputVector[1], GunStats::RateFire_VStat.size());
                                // Для остальных — тоже OtherStat
                                for (size_t i = 2; i < Characteristics_DEFAULT.size(); ++i) {
                                    clamp(inputVector[i], GunStats::Other_VStat.size());
                                }


                                if (SelectOption >= 14) { SelectOption -= 14; }
                                //вывод процентов в инпут 
                                if      (SelectOption == 0) { InputCharacteristicsShapeText[SelectOption + 7].second.setString(format("{:.1f}", GunStats::Accuracy_VStat[inputVector[SelectOption]])); }
                                else if (SelectOption == 1) { InputCharacteristicsShapeText[SelectOption + 7].second.setString(format("{:.1f}", GunStats::RateFire_VStat[inputVector[SelectOption]])); }
                                else                        { InputCharacteristicsShapeText[SelectOption + 7].second.setString(format("{:.1f}", GunStats::Other_VStat   [inputVector[SelectOption]])); }
                                // выход
                                SelectOption = InputCharacteristicsShapeText.size();
                            }
                        }
                    }
                }
            }
            InputWindow.clear(Color::White);

            for (int i = 0; i < InputCharacteristicsShapeText.size(); i++)
            {
                InputWindow.draw(InputCharacteristicsShapeText[i].first);
                InputWindow.draw(InputCharacteristicsShapeText[i].second);

                InputCharacteristicsShapeText[i].first.setOutlineThickness(0);    

            }
            InputWindow.display();

        }
        ////////////////////////////////////////////////////////////////////////////////
























        ////////////////////////////////////////////////////////////////////////////////
        // отрисовка диаграмм
        for (int i = VectorDiagram.size() - 1; i >= 0; --i)
        {
            if (VectorDiagram[i].isOpen())
            {
                while (const optional event = VectorDiagram[i].pollEvent())
                {
                    if (event->is<Event::Closed>())
                    {
                        VectorDiagram[i].close();
                        DiagramBool.erase(DiagramBool.begin() + i);
                        VectorDiagram.erase(VectorDiagram.begin() + i);
                        break; // чтобы не обращаться к уже удалённому i
                    }
                }

                if (i < VectorDiagram.size() && !DiagramBool[i])
                {
                    VectorDiagram[i].clear(Color::White);
                    drawNormalGraph(VectorDiagram[i], Average10000, 200.f, 50.f, Average10000_RANDOM_GEN, totalIterations, RESULT_CHARACTERISTIC, START_CHARACTERISTIC, INITIAL_CHARACTERISTIC, NameGun, method, Tool_Kit_Skil);
                    
                    DiagramBool[i] = true;

                    OutputLog("отрисовка диаграммы закончена: " + to_string(i));
                    VectorDiagram[i].display();
                }                
            }
        }
        ////////////////////////////////////////////////////////////////////////////////




    }
    
    return 0;
}