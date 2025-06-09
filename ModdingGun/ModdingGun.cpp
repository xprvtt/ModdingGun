#include "ModdingGun.h"
#include <random>
#include <thread>
#include <mutex>

#pragma comment(lib, "User32.lib")

int getRandomInt(int range);

vector<int> drawNormalGraph(RenderWindow& window, const vector<int>& Average10000, float graphHeight, float marginBottom, const vector<int>& Average10000_RANDOM_GEN);


void ChangeCharacteristic(bool Initial_Input_Window, int SelectOption, int Characteriscit, vector<int>& CharacteristicsInitial, vector<int>& CharacteristicsModded, Text& InputCharacteristicsShapeText);



int main()
{
    // создаем вывод в логи
    permissions("Log/log.txt", perms::all);
    remove("Log/log.txt");
    OutputLog("Запуск!");

    ///////////////////////////////////////////////////////////////////////////////////

    /// высота окна, от нее зависит длина окна
    unsigned int SizeWindowHeight = 700;

    /// длина окна
    unsigned int SizeWindowLength = static_cast<unsigned int>(round(SizeWindowHeight * 1.8));

    /// количество клеток на одной стороне
    size_t CountCellOnLengthWindow = 20;
    size_t CountCellOnHeightWindow = 5;

    size_t SizeFont = 17;

    size_t SizeCell = SizeWindowLength / CountCellOnLengthWindow;

    ///////////////////////////////////////////////////////////////////////////////////



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

    /// vector textrue ///
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

        for (size_t it = 0; it < CountCellOnLengthWindow; it++)
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

            TempNameGun.setPosition(Vector2f(SizeCell * it, SizeCell * (Tier - 1)));
            TempNameGun.setOutlineColor(Color::Black);
            TempNameGun.setOutlineThickness(1);

            VectorTextureGun.push_back(make_tuple(TempShape, TempPoint, TempName, Tier));
            VectorNameGun.push_back(TempNameGun);
        }
    }


    RectangleShape SelectedGunShape;
    SelectedGunShape.setSize(Vector2f(SizeCell, SizeCell));
    SelectedGunShape.setOutlineThickness(-1);
    SelectedGunShape.setOutlineColor(Color::Black);
    SelectedGunShape.setPosition(Vector2f(0, SizeCell * (CountCellOnHeightWindow + 1) ));



    OutputLog("загружен vector textrue");

    ///////////////////////////////////////////////////////////////////////////////////


    // переменные характеристик оружия

    ///кучность 
    ///темп огня 
    ///кучность  
    ///отдача
    ///качание
    ///отказ от сост
    ///отказ от грязи                                   get<>           get<>(get<2>)
    ///           v1[]      v1[][]                      ---- процент
    ///     --- куч --- колво варинатов ----- шанс    
    /// v1 ---- темп                                ------------------- корректировка мода \x7\
    ///          --- ...
    vector<vector<tuple<int, int, tuple<int, int, int, int, int, int, int>>>> AllStatMod;


    /// характеристики мода
    vector<int> CharacteristicsModded{ 0, 0, 0, 0, 0, 0, 0 };

    /// начальный мод
    vector<int> CharacteristicsInitial{ 0, 0, 0, 0, 0, 0, 0 };
    /// Мод с нуля? t - да \ f - нет
    bool Initial_OFF = true;
    
    ////////////////////////////////////////////////////////////////////////////////////

    RectangleShape Calculation;
    Calculation.setSize(Vector2f(SizeCell * 2, SizeCell / 2));
    Calculation.setOutlineThickness(-3);
    Calculation.setOutlineColor(Color::Green);
    Calculation.setPosition(Vector2f(SizeCell + SizeCell / 2, SizeCell * (CountCellOnHeightWindow + 2)));

    Text TextCalculation(CurrentFont);
    TextCalculation.setString(L"Calculation->");
    TextCalculation.setCharacterSize(SizeFont+3);
    TextCalculation.setFillColor(Color::Black);


    FloatRect textBounds = TextCalculation.getLocalBounds();
    Vector2f rectPos = Calculation.getPosition();
    Vector2f rectSize = Calculation.getSize();

    TextCalculation.setOrigin(Vector2f(textBounds.position.x + textBounds.size.x / 2.0f, textBounds.position.y + textBounds.size.y / 2.0f));
    TextCalculation.setPosition(Vector2f(rectPos.x + rectSize.x / 2.0f,rectPos.y + rectSize.y / 2.0f)  );


    OutputLog("загружен Calculation");

    ////////////////////////////////////////////////////////////////////////////////////


    RectangleShape Box;
    Box.setSize(Vector2f(SizeCell * 2, SizeCell / 2));
    Box.setOutlineThickness(-3);
    Box.setOutlineColor(Color::Green);
    Box.setPosition(Vector2f(SizeCell + SizeCell / 2, SizeCell * (CountCellOnHeightWindow + 1)));

    Text TextBox(CurrentFont);
    TextBox.setString(L"Old Tool");
    TextBox.setCharacterSize(SizeFont + 3);
    TextBox.setFillColor(Color::Black);


    textBounds = TextBox.getLocalBounds();
    rectPos = Box.getPosition();
    rectSize = Box.getSize();

    TextBox.setOrigin(Vector2f(textBounds.position.x + textBounds.size.x / 2.0f, textBounds.position.y + textBounds.size.y / 2.0f));
    TextBox.setPosition(Vector2f(rectPos.x + rectSize.x / 2.0f, rectPos.y + rectSize.y / 2.0f));


    OutputLog("загружен box");

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

    RectangleShape СharacteristicsModGunInitial;
    СharacteristicsModGunInitial.setSize(Vector2f(SizeCell * 8, SizeCell / 2));
    СharacteristicsModGunInitial.setOutlineThickness(-3);
    СharacteristicsModGunInitial.setOutlineColor(Color::Green);
    СharacteristicsModGunInitial.setPosition(Vector2f(SizeCell * 4, SizeCell* (CountCellOnHeightWindow + 2)));

    RectangleShape ButtonСharacteristicsModGunInitial;
    ButtonСharacteristicsModGunInitial.setSize(Vector2f(SizeCell / 2, SizeCell / 2));
    ButtonСharacteristicsModGunInitial.setOutlineThickness(-3);
    ButtonСharacteristicsModGunInitial.setOutlineColor(Color::Black);
    ButtonСharacteristicsModGunInitial.setFillColor(Color::Red);
    ButtonСharacteristicsModGunInitial.setPosition(Vector2f(SizeCell * 12 + SizeCell / 2, SizeCell* (CountCellOnHeightWindow + 2)));

    Text TextСharacteristicsModGunOriginal(CurrentFont);
    TextСharacteristicsModGunOriginal.setString(L"начальный мод, от корого начнем  |  переключение ->");
    TextСharacteristicsModGunOriginal.setCharacterSize(SizeFont + 3); 
    TextСharacteristicsModGunOriginal.setFillColor(Color::Black);

    textBounds = TextСharacteristicsModGunOriginal.getLocalBounds();
    rectPos = СharacteristicsModGunInitial.getPosition();
    rectSize = СharacteristicsModGunInitial.getSize();

    TextСharacteristicsModGunOriginal.setOrigin(Vector2f(0.f, textBounds.position.y + textBounds.size.y / 2.0f));
    TextСharacteristicsModGunOriginal.setPosition(Vector2f(rectPos.x + 10, rectPos.y + rectSize.y / 2.0f));

    OutputLog("загружен СharacteristicsModGunInitial");

    ///////////////////////////////////////////////////////////////////////////////////

    RectangleShape ShapeInfoDebug;
    ShapeInfoDebug.setSize(Vector2f(SizeCell * 7, SizeCell / 2));
    ShapeInfoDebug.setOutlineThickness(-3);
    ShapeInfoDebug.setOutlineColor(Color::Black);
    ShapeInfoDebug.setPosition(Vector2f(SizeCell * 12.5, SizeCell* (CountCellOnHeightWindow + 1)));

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

    // задний фон

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

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // для диалогового окна


    // создаем диалоговое окно
    unsigned int HeightCharacteristicsInputWindow = 100;
    unsigned int LengthCharacteristicsInputWindow = HeightCharacteristicsInputWindow * 5;


    vector<pair<RectangleShape, Text>> InputCharacteristicsShapeText;


    for (int i = 0, j = 0; i < 7 && j < 3;)
    {
        RectangleShape TempInputCharacteristicsShape;
        TempInputCharacteristicsShape.setSize(Vector2f(LengthCharacteristicsInputWindow / 7, HeightCharacteristicsInputWindow / 3));
        TempInputCharacteristicsShape.setPosition(Vector2f((LengthCharacteristicsInputWindow / 7)* i, (HeightCharacteristicsInputWindow / 3)* j));
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

    OutputLog("загружено диалоговое окно");

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // переменные положения мышки
    Vector2i PositionMouse;
    Vector2f MouseWorldPos;

    // создаем окно
    RenderWindow window(VideoMode({ SizeWindowLength, SizeWindowHeight }), "ModdingGun");
    window.setFramerateLimit(150);

    RenderWindow Diagram;

    // создаем д окно
    RenderWindow CharacteristicsInputWindow;


    //для расчета диаграммы
    bool ScheduleCalculation = false;

    bool Initial_Input_Window = false;

    vector<int> AccuracyTemp
    {
        -6000, -5830, -5670, -5500, -5330, -5170,
        -5000, -4830, -4670, -4500, -4330, -4170,
        -4000, -3830, -3670, -3500, -3330, -3170,
        -3000, -2830, -2670, -2500, -2330, -2170,
        -2000, -1830, -1670, -1500, -1330, -1170,
        -1000,  -830,  -670,  -500,  -330,  -170,
            0,   170,   330,   500,   670,   830,
         1000,  1170,  1330,  1500,  1670,  1830,
         2000,  2170,  2330,  2500,  2670,  2830,
         3000,  3170,  3330,  3500,  3670,  3830,
         4000,  4170,  4330,  4500,  4670,  4830,
         5000,  5170,  5330,  5500,  5670,  5830,
         6000
    };


    vector<int> PaceFireTemp
    {
        -6250, -6040, -5830, -5630, -5420, -5250, -5000,
        -4790, -4580, -4380, -4170, -3960, -3750, -3540, -3330, -3130, -2920, -2710, -2500,
        -2290, -2080, -1880, -1670, -1460, -1250, -1040,  -830,  -630,  -420,  -210,
            0,   210,   420,   630,   830,  1040,  1250,  1460,  1670,  1880,  2080,  2290,
         2500,  2710
    };

    vector<int> Average10000;
    vector<int> Average10000_RANDOM_GEN;

    bool DiagramGraf = false;

    while (window.isOpen())
    {
        PositionMouse = Mouse::getPosition(window);
        MouseWorldPos = window.mapPixelToCoords(PositionMouse);

        while (const optional event = window.pollEvent())
        {
            if (event->is<Event::Closed>())
            {
                window.close();
                CharacteristicsInputWindow.close();
                Diagram.close();

            }

            // выбор
            else if (const auto& mouseButtonPressed = event->getIf<Event::MouseButtonPressed>())
            {
                if (mouseButtonPressed->button == Mouse::Button::Left)
                {
                    /// икнонки оружия
                    for (size_t it = 0; it < VectorTextureGun.size(); it++)
                    {
                        if (get<0>(VectorTextureGun[it]).getGlobalBounds().contains(MouseWorldPos))
                        {
                            SelectedGunShape.setTexture(get<1>(VectorTextureGun[it]).get(), true);

                            wstring NameGun = get<2>(VectorTextureGun[it]);
                            wstring FileStat = L"Attribute/Tier_" + to_wstring(get<3>(VectorTextureGun[it])) + "/" + NameGun + L".txt";

                            string s(NameGun.begin(), NameGun.end());

                            //считываем статы пушки
                            ifstream file(FileStat);
                            if (file.is_open()) 
                            {
                                string line;
                                vector<string> lines;

                                while (getline(file, line)) 
                                {
                                    lines.push_back(line);
                                }
                                if (lines.size() < 7) 
                                {
                                    OutputLog("Ожидалось 7 строк, найдено: " + to_string(lines.size())  );
                                    TextInfoDebug.setString(L"Ожидалось 7 строк, в файле -> ошибка");
                                    AllStatMod.clear();
                                }
                                else
                                {
                                    for (int i = 0; i < 7; i++)
                                    {
                                        vector<tuple<int, int, tuple<int, int, int, int, int, int, int>>> temp;
                                        ParseLine3(lines[i], temp);
                                        AllStatMod.push_back(temp);
                                    }
                                    OutputLog("Файл успешно обработан: " + s);
                                    TextInfoDebug.setString(L"Файл успешно обработан");
                                }
                            }
                            else
                            {
                                OutputLog("Не удалось открыть файл: " + s);
                                TextInfoDebug.setString(L"Не удалось открыть файл");
                                AllStatMod.clear();
                            }
                            break;
                        }
                    }

                    /// поле ввода мода к которому замодаемся
                    if (СharacteristicsModGun.getGlobalBounds().contains(MouseWorldPos))
                    {
                        СharacteristicsModGun.setOutlineColor(Color::Red);
                        window.draw(СharacteristicsModGun);
                        window.display();

                        CharacteristicsModded = { 0,0,0,0,0,0,0 }; 

                        Initial_Input_Window = false;

                        CharacteristicsInputWindow.create(VideoMode({ LengthCharacteristicsInputWindow, HeightCharacteristicsInputWindow }), "CharacteristicsCurrent");
                        CharacteristicsInputWindow.setFramerateLimit(150);
                    }

                    // поле вводе начального мода
                    else if (СharacteristicsModGunInitial.getGlobalBounds().contains(MouseWorldPos))
                    {
                        СharacteristicsModGunInitial.setOutlineColor(Color::Red);
                        window.draw(СharacteristicsModGunInitial);
                        window.display();

                        CharacteristicsInitial = { 0,0,0,0,0,0,0 };

                        Initial_Input_Window = true;
                        
                        CharacteristicsInputWindow.create(VideoMode({ LengthCharacteristicsInputWindow, HeightCharacteristicsInputWindow }), "CharacteristicsCurrent");
                        CharacteristicsInputWindow.setFramerateLimit(150);
                    }

                    // рассчет мода
                    else if (Calculation.getGlobalBounds().contains(MouseWorldPos))
                    {
                        Average10000.clear();
                        Average10000_RANDOM_GEN.clear();

                        Calculation.setOutlineColor(Color::Red);
                        window.draw(Calculation);

                        if (AllStatMod.size())
                        {
                            if (CharacteristicsModded != CharacteristicsInitial)
                            {
                                // колличество найденых параметров
                                int ModStatBool7 = 0;
                                int ErrorOption = 0;



                                //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                                
                                
                                // проверяем реален ли такой мод                          
                                // проходима по параметрам х7
                                for (int optionExist = 0; optionExist < AllStatMod.size(); optionExist++)
                                {
                                    // если характеристика мода 0, она автоматом существует
                                    if (CharacteristicsModded[optionExist] == 0)
                                    {
                                        // добавляем единичку в сумме должно быть 7 \ выходим
                                        ModStatBool7++;
                                    }
                                    else
                                    {
                                        // проходимся по колву вариантам параметра из файла
                                        bool nextCheck = true;
                                        for (int countOptionExist = 0; countOptionExist < AllStatMod[optionExist].size() && nextCheck; countOptionExist++)
                                        {
                                            // если находим такой вариант
                                            if (CharacteristicsModded[optionExist] == get<0>(AllStatMod[optionExist][countOptionExist]))
                                            {
                                                // добавляем единичку в сумме должно быть 7 \ выходим
                                                ModStatBool7++;
                                                nextCheck = false;                                                
                                            }
                                        }
                                        // если не нашли еще один цикл проверки статов в корректировках
                                        if (nextCheck)
                                        {

                                            
                                            
                                            for (int CheckOptionCorrect = 0; CheckOptionCorrect < AllStatMod.size(); CheckOptionCorrect++)
                                            {
                                                for (int variation = 0; variation < AllStatMod[CheckOptionCorrect].size() ; variation++)
                                                {
                                                    vector<int> correctStat = { get<0>(get<2>(AllStatMod[CheckOptionCorrect][variation])),
                                                                                get<1>(get<2>(AllStatMod[CheckOptionCorrect][variation])),
                                                                                get<2>(get<2>(AllStatMod[CheckOptionCorrect][variation])),
                                                                                get<3>(get<2>(AllStatMod[CheckOptionCorrect][variation])),
                                                                                get<4>(get<2>(AllStatMod[CheckOptionCorrect][variation])),
                                                                                get<5>(get<2>(AllStatMod[CheckOptionCorrect][variation])),
                                                                                get<6>(get<2>(AllStatMod[CheckOptionCorrect][variation]))
                                                    };

                                                    if (CharacteristicsModded[optionExist] == correctStat[optionExist])
                                                    {
                                                        // добавляем единичку в сумме должно быть 7 \ выходим
                                                        ModStatBool7++;
                                                        nextCheck = false;
                                                        CheckOptionCorrect = AllStatMod.size();
                                                        break;
                                                    }
                                                }
                                            }
        
                                        }

                                    }
                                }
                                //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


                                //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                                // если все параметры найдены
                                if (ModStatBool7 == AllStatMod.size())
                                {                                  

                                    bool Error = false;

                                    // колво повторений мода
                                    unsigned int Target = 10000;


                                    for (int Average10000iter = 0; Average10000iter < Target; Average10000iter++)
                                    {
                                        //////////////////////////////////////////////////////////////////////////////////////////////
                                        // к этому надо прийти// 
                                        // CharacteristicsModded;

                                        // это у нас сейчас// 
                                        // CharacteristicsCurrentOriginal
                                        vector<int> CharacteristicsCurrentOp;

                                                  // указанны статы и стоит метка ? - начинаем с них        // иначе мод с нуля                              
                                        Initial_OFF ? CharacteristicsCurrentOp = CharacteristicsInitial : CharacteristicsCurrentOp = { 0, 0, 0, 0, 0, 0, 0, } ;

                                        //////////////////////////////////////////////////////////////////////////////////////////////


                                        /// колво коробок
                                        int RESULT = 0;


                                        //////////////////////////////////////////////////////////////////////////////////////////////
                                        
                                        // цикл перепроверки
                                        for (int ReIT = 0; ReIT < 1; ReIT++)
                                        {
                                            //проходимся по всем характеристикам x7
                                            for (int Options = 0; Options < CharacteristicsModded.size(); Options++)
                                            {

                                                // если текущая характеристика x1 мода к которой стремимся больше или равна не заходим || если больше --- ошибка
                                                if (CharacteristicsCurrentOp[Options] < CharacteristicsModded[Options])
                                                {
                                                    ////---////---///---////---////---////---/////----////---////---///---////---////---////---/////
                                                    // 
                                                    //     vector<int> AccuracyTemp
                                                    //     vector<int> PaceFireTemp
                                                    // 
                                                    // 
                                                    // начальный этап
                                                    int it_Initial;

                                                    // находим этот начальный этап
                                                    bool Exist = false;

                                                    if (Options == 0)
                                                    {
                                                        // найдем характеристику на +1 стат больше
                                                        for (int i = 0; i < AccuracyTemp.size(); i++)
                                                        {
                                                            if (CharacteristicsCurrentOp[Options] == AccuracyTemp[i])
                                                            {
                                                                CharacteristicsCurrentOp[Options] = AccuracyTemp[i + 1];

                                                                // найдем текущий этап, который +1 больше
                                                                for (it_Initial = 0; it_Initial < AllStatMod[Options].size(); it_Initial++)
                                                                {
                                                                    if (CharacteristicsCurrentOp[Options] == get<0>(AllStatMod[Options][it_Initial]))
                                                                    {
                                                                        i = AccuracyTemp.size();
                                                                        Exist = true;
                                                                        break;
                                                                    }
                                                                }
                                                                
                                                            }
                                                        }
                                                    }
                                                    else if (Options == 1)
                                                    {
                                                        for (int i = 0; i < PaceFireTemp.size(); i++)
                                                        {
                                                            if (CharacteristicsCurrentOp[Options] == PaceFireTemp[i])
                                                            {
                                                                CharacteristicsCurrentOp[Options] = PaceFireTemp[i + 1];

                                                                for (it_Initial = 0; it_Initial < AllStatMod[Options].size(); it_Initial++)
                                                                {
                                                                    if (CharacteristicsCurrentOp[Options] == get<0>(AllStatMod[Options][it_Initial]))
                                                                    {
                                                                        i = PaceFireTemp.size();
                                                                        Exist = true;
                                                                        break;
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                    else
                                                    {
                                                        CharacteristicsCurrentOp[Options] += 250;

                                                        for (it_Initial = 0; it_Initial < AllStatMod[Options].size(); it_Initial++)
                                                        {

                                                            int Stat = get<0>(AllStatMod[Options][it_Initial]);

                                                            if (CharacteristicsCurrentOp[Options] == Stat)
                                                            {
                                                                Exist = true;
                                                                break;
                                                            }
                                                        }
                                                    }


                                                    ////---////---///---////---////---////---/////----////---////---///---////---////---////---/////


                                                    // если нашли этот вариант проходимся по шансам
                                                    if (Exist)
                                                    {
                                                        // текущий вариант характеристики 
                                                        int iter = it_Initial;

                                                        // цил до тех пор пока не от текущей статы дойдем до целевого

                                                        for (; get<0>(AllStatMod[Options][iter]) < CharacteristicsModded[Options]; iter++ )
                                                        {

                                                            // подлучаем шанс
                                                            int chance = get<1>(AllStatMod[Options][iter]);

                                                            // цикл пока не прокнет шанс
                                                            for (;;)
                                                            {
                                                                // симуляция шанса
                                                                // засчитываем попытку

                                                                RESULT++;

                                                                // рандомное число
                                                                int random_chance = getRandomInt(10000);

                                                                Average10000_RANDOM_GEN.push_back(random_chance);

                                                                // если прокнуло
                                                                if (random_chance <= chance)
                                                                {
                                                                    // выходим 
                                                                    break;
                                                                }

                                                            }
                                                         
                                                        }
                                                        if (get<0>(AllStatMod[Options][iter]) == CharacteristicsModded[Options])
                                                        {
                                                            // подлучаем шанс
                                                            int chance = get<1>(AllStatMod[Options][iter]);

                                                            // цикл пока не прокнет шанс
                                                            for (;;)
                                                            {
                                                                // симуляция шанса
                                                                // засчитываем попытку

                                                                RESULT++;

                                                                // рандомное число
                                                                int random_chance = getRandomInt(10000);

                                                                Average10000_RANDOM_GEN.push_back(random_chance);

                                                                // если прокнуло
                                                                if (random_chance <= chance)
                                                                {
                                                                    // выходим 
                                                                    break;
                                                                }

                                                            }
                                                        }
                                                        else
                                                        {
                                                            RESULT = -9999;
                                                            OutputLog("ошибка последнего цикла мода");
                                                        }
                                                        // последний замод
                                                        // 
                                                        // корректировка текущей характеристики
                                                        CharacteristicsCurrentOp[Options] = CharacteristicsModded[Options];

                                                        auto correct = get<2>(AllStatMod[Options][iter]);

                                                        // корректировка характеристик
                                                        ///////////////// полу костыль ////////////////////////
                                                        CharacteristicsCurrentOp[0] += get<0>(correct);
                                                        CharacteristicsCurrentOp[1] += get<1>(correct);
                                                        CharacteristicsCurrentOp[2] += get<2>(correct);
                                                        CharacteristicsCurrentOp[3] += get<3>(correct);
                                                        CharacteristicsCurrentOp[4] += get<4>(correct);
                                                        CharacteristicsCurrentOp[5] += get<5>(correct);
                                                        CharacteristicsCurrentOp[6] += get<6>(correct);
                                                        ///////////////////////////////////////////////////////

                                                    }
                                                    // если нет варианта выходим
                                                    else
                                                    {
                                                        OutputLog("нет варианта");

                                                        Options = CharacteristicsModded.size();
                                                        Average10000iter = Target;
                                                        RESULT = -1111;
                                                        break;
                                                    }
                                                }
                                                // если характеристика текущего мода выше чем та которую мы хотим, -> ошибка
                                                // текущий мод больше чем нужен
                                                else if (CharacteristicsCurrentOp[Options] > CharacteristicsModded[Options])
                                                {
                                                    OutputLog("CharacteristicsCurrentOp[Options] > CharacteristicsModded[Options]");
                                                    OutputLog("нельзя уменьшить мод // текущий мод больше чем нужен");

                                                    Options = CharacteristicsModded.size();
                                                    Average10000iter = Target;
                                                    RESULT = -2222;
                                                    break;
                                                }

                                            }

                                            if (CharacteristicsCurrentOp != CharacteristicsModded)
                                            {         
                                                OutputLog("CharacteristicsCurrentOp != CharacteristicsModded");


                                                Average10000iter = Target;
                                                RESULT = -3333;
                                                
                                                break;
                                                //ReIT--;
                                            }
                                        }
                                        //////////////////////////////////////////////////////////////////////////////////////////////


                                        Average10000.push_back(RESULT);

                                        TextInfoDebug.setString(L"Колличество собранного мод-оружия: " + to_string(Average10000iter) + L"/" + to_string(Target));

                                        while (const optional event = window.pollEvent())
                                        {
                                            if (event->is<Event::Closed>())
                                            {
                                                window.close();
                                                CharacteristicsInputWindow.close();
                                                Diagram.close();                                                
                                            }
                                        }

                                        window.clear(Color::White);
                                        window.draw(background);

                                        window.draw(ShapeInfoDebug);
                                        window.draw(TextInfoDebug);
                                        window.draw(Calculation);
                                        window.draw(TextCalculation);

                                        window.display();

                                    }
                                    // если у нас нет ошибок
                                    if (!Error)
                                    {
                                        unsigned int sizeX = 1500;
                                        unsigned int sizeY = 500;

                                        Diagram.create(VideoMode({ sizeX, sizeY }), "Diagram");
                                        Diagram.setFramerateLimit(150);
                                    }
                                    else
                                    {
                                        TextInfoDebug.setString(L"не найден вариант процента в : / Options / " + to_wstring(ErrorOption));
                                    }
                                }
                                else
                                {
                                    TextInfoDebug.setString(L"мод неправильный,или не найден параметр в файле");
                                }
                            }
                            else
                            {
                                TextInfoDebug.setString(L"мод совпадает");
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
                        if (Initial_OFF)
                        {
                            Initial_OFF = false;
                            ButtonСharacteristicsModGunInitial.setFillColor(Color::Red);
                        }
                        else
                        {
                            Initial_OFF = true;
                            ButtonСharacteristicsModGunInitial.setFillColor(Color::Green);
                        }
                    }
                }
            }
        }
        

        ////////////////////////////////////////////////////////////////////////////////
        // окно ввода мода 
        if (CharacteristicsInputWindow.isOpen())
        {

            PositionMouse = Mouse::getPosition(CharacteristicsInputWindow);
            MouseWorldPos = CharacteristicsInputWindow.mapPixelToCoords(PositionMouse);

            if (const optional eventD = CharacteristicsInputWindow.pollEvent())
            {
                if (eventD->is<Event::Closed>())
                {

                    string СharacteristicsModGunInput = " | ";

                    for (int i = 7; i < 14; i++)
                    {
                        СharacteristicsModGunInput += InputCharacteristicsShapeText[i].second.getString();
                        InputCharacteristicsShapeText[i].second.setString("0.00");
                        СharacteristicsModGunInput += " | ";

                    }
                    Initial_Input_Window ? TextСharacteristicsModGunOriginal.setString(СharacteristicsModGunInput) : TextСharacteristicsModGun.setString(СharacteristicsModGunInput);

                    CharacteristicsInputWindow.close();
                }
                else if (const auto& mouseButtonPressed = eventD->getIf<Event::MouseButtonPressed>())
                {
                    if (mouseButtonPressed->button == Mouse::Button::Left)
                    {
                        for (size_t SelectOption = 0; SelectOption < InputCharacteristicsShapeText.size(); SelectOption++)
                        {
                            if (InputCharacteristicsShapeText[SelectOption].first.getGlobalBounds().contains(MouseWorldPos) && (SelectOption < 7 || SelectOption >= 14))
                            {
                                InputCharacteristicsShapeText[SelectOption].first.setOutlineThickness(-3);
                                int Characteriscit = 0;                                                                    

                                ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
                                if (InputCharacteristicsShapeText[SelectOption].second.getString() == L"↑")
                                {
                                    Initial_Input_Window ? Characteriscit = CharacteristicsInitial[SelectOption] : Characteriscit = CharacteristicsModded[SelectOption];                                    
                                    ///---///---///---///---///---///---///---///---///---///---///---///---///---///---///---///---///---///---
                                    if (SelectOption == 0)
                                    {
                                        for (int i = 0; i < AccuracyTemp.size(); i++)
                                        {
                                            if (Characteriscit == AccuracyTemp[i])
                                            {
                                                if (Characteriscit != AccuracyTemp[AccuracyTemp.size() - 1])
                                                {
                                                    Characteriscit = AccuracyTemp[i+1];

                                                    ChangeCharacteristic(Initial_Input_Window, SelectOption, Characteriscit, CharacteristicsInitial, CharacteristicsModded,  InputCharacteristicsShapeText[SelectOption + 7].second);
                                                }
                                                SelectOption = InputCharacteristicsShapeText.size();
                                                break;
                                            }
                                        }
                                    }
                                    ///---///---///---///---///---///---///---///---///---///---///---///---///---///---///---///---///---///---
                                    else if (SelectOption == 1)
                                    {
                                        for (int i = 0; i < PaceFireTemp.size(); i++)
                                        {
                                            if (Characteriscit == PaceFireTemp[i])
                                            {
                                                if (Characteriscit != PaceFireTemp[PaceFireTemp.size() - 1])
                                                {
                                                    if (Characteriscit != PaceFireTemp[PaceFireTemp.size() - 1])
                                                    {
                                                        Characteriscit = PaceFireTemp[i + 1];
                                                        ChangeCharacteristic(Initial_Input_Window, SelectOption, Characteriscit, CharacteristicsInitial, CharacteristicsModded, InputCharacteristicsShapeText[SelectOption + 7].second);
                                                    }
                                                }
                                                SelectOption = InputCharacteristicsShapeText.size();
                                                break;
                                            }
                                        }
                                    }
                                    ///---///---///---///---///---///---///---///---///---///---///---///---///---///---///---///---///---///---
                                    else if (Characteriscit < 10000)
                                    {
                                        Characteriscit += 250;
                                        ChangeCharacteristic(Initial_Input_Window, SelectOption, Characteriscit, CharacteristicsInitial, CharacteristicsModded, InputCharacteristicsShapeText[SelectOption + 7].second);
                                        SelectOption = InputCharacteristicsShapeText.size();
                                        break;
                                    }
                                    ///---///---///---///---///---///---///---///---///---///---///---///---///---///---///---///---///---///---
                                    else
                                    {
                                        SelectOption = InputCharacteristicsShapeText.size();
                                        break;
                                    }
                                }
                                /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                                else if (InputCharacteristicsShapeText[SelectOption].second.getString() == L"↓")
                                {
                                    if (SelectOption >= 14) { SelectOption -= 14; }
                                    Initial_Input_Window ? Characteriscit = CharacteristicsInitial[SelectOption] : Characteriscit = CharacteristicsModded[SelectOption];

                                    ///---///---///---///---///---///---///---///---///---///---///---///---///---///---///---///---///---///---
                                    if (SelectOption == 0)
                                    {
                                        for (int i = 0; i < AccuracyTemp.size(); i++)
                                        {
                                            if (Characteriscit == AccuracyTemp[i])
                                            {
                                                if (Characteriscit != AccuracyTemp[0])
                                                {
                                                    if (Characteriscit != AccuracyTemp[AccuracyTemp.size() - 1])
                                                    {
                                                        Characteriscit = AccuracyTemp[i - 1];
                                                        ChangeCharacteristic(Initial_Input_Window, SelectOption, Characteriscit, CharacteristicsInitial, CharacteristicsModded, InputCharacteristicsShapeText[SelectOption + 7].second);
                                                    }
                                                }
                                                SelectOption = InputCharacteristicsShapeText.size();
                                                break;
                                            }
                                        }
                                    }
                                    ///---///---///---///---///---///---///---///---///---///---///---///---///---///---///---///---///---///---
                                    else if (SelectOption == 1)
                                    {
                                        for (int i = 0; i < PaceFireTemp.size(); i++)
                                        {
                                            if (Characteriscit == PaceFireTemp[i])
                                            {
                                                if (Characteriscit != PaceFireTemp[0])
                                                {
                                                    if (Characteriscit != PaceFireTemp[PaceFireTemp.size() - 1])
                                                    {
                                                        Characteriscit = PaceFireTemp[i - 1];
                                                        ChangeCharacteristic(Initial_Input_Window, SelectOption, Characteriscit, CharacteristicsInitial, CharacteristicsModded, InputCharacteristicsShapeText[SelectOption + 7].second);
                                                    }
                                                }
                                                SelectOption = InputCharacteristicsShapeText.size();
                                                break;
                                            }
                                        }
                                    }
                                    ///---///---///---///---///---///---///---///---///---///---///---///---///---///---///---///---///---///---
                                    else if (Characteriscit > -10000)
                                    {
                                        Characteriscit -= 250;
                                        ChangeCharacteristic(Initial_Input_Window, SelectOption, Characteriscit, CharacteristicsInitial, CharacteristicsModded, InputCharacteristicsShapeText[SelectOption + 7].second);
                                        SelectOption = InputCharacteristicsShapeText.size();
                                        break;                                        
                                    }
                                    ///---///---///---///---///---///---///---///---///---///---///---///---///---///---///---///---///---///---
                                    else
                                    {
                                        SelectOption = InputCharacteristicsShapeText.size();
                                        break;
                                    }
                                }
                                ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 

                            }
                        }
                    }
                }
            }
            CharacteristicsInputWindow.clear(Color::White);

            for (int i = 0; i < InputCharacteristicsShapeText.size(); i++)
            {
                CharacteristicsInputWindow.draw(InputCharacteristicsShapeText[i].first);
                CharacteristicsInputWindow.draw(InputCharacteristicsShapeText[i].second);

                InputCharacteristicsShapeText[i].first.setOutlineThickness(0);          
            }
            CharacteristicsInputWindow.display();

        }

        // отрисовка диаграммы
        if (Diagram.isOpen())
        {
            while (const optional event = Diagram.pollEvent())
            {
                if (event->is<Event::Closed>())
                {

                    Diagram.close();
                }                
            } 
            if (!DiagramGraf)
            {
                Diagram.clear(Color::White);

                drawNormalGraph(Diagram, Average10000, 200.f, 50.f, Average10000_RANDOM_GEN);

                ScheduleCalculation = true;

                Diagram.display();

                DiagramGraf = true;
            }
        }
        else
        {
            DiagramGraf = false;
        }


        window.clear(Color::White);

        window.draw(background);       


        for (int i = 0; i < VectorTextureGun.size(); i++)
        {
            window.draw(get<0>(VectorTextureGun[i]));
            window.draw(VectorNameGun[i]);
        }

        window.draw(SelectedGunShape);
        window.draw(Calculation);
        window.draw(TextCalculation);

        window.draw(ShapeInfoDebug);
        window.draw(TextInfoDebug);

        window.draw(Box);
        window.draw(TextBox);

        window.draw(СharacteristicsModGun);
        window.draw(TextСharacteristicsModGun);    

        window.draw(СharacteristicsModGunInitial);
        window.draw(TextСharacteristicsModGunOriginal);
        
        window.draw(ButtonСharacteristicsModGunInitial);
        

        СharacteristicsModGun.setOutlineColor(Color::Green);
        СharacteristicsModGunInitial.setOutlineColor(Color::Green);
        Calculation.setOutlineColor(Color::Green);
        
        window.display();
    }
    
    return 0;
}

int getRandomInt(int range) 
{
    random_device rd;                          
    mt19937 gen(rd());                         
    uniform_int_distribution<> distrib(0, range);
    return distrib(gen);
}

void ChangeCharacteristic(bool Initial_Input_Window, int SelectOption, int Characteriscit, vector<int>& CharacteristicsInitial, vector<int>& CharacteristicsModded,  Text& InputCharacteristicsShapeText)
{

    if (Initial_Input_Window)
    {
        CharacteristicsInitial[SelectOption] = Characteriscit;
        string result = to_string(CharacteristicsInitial[SelectOption] / 100) + "." + to_string(abs(CharacteristicsInitial[SelectOption] % 100));

        InputCharacteristicsShapeText.setString(result);
    }
    else
    {
        CharacteristicsModded[SelectOption] = Characteriscit;
        string result = to_string(CharacteristicsModded[SelectOption] / 100) + "." + to_string(abs(CharacteristicsModded[SelectOption] % 100));

        InputCharacteristicsShapeText.setString(result);
    }
}

vector<int> drawNormalGraph(RenderWindow& window, const vector<int>& Average10000, float graphHeight, float marginBottom, const vector<int>& Average10000_RANDOM_GEN)
{
    if (Average10000.empty() || Average10000_RANDOM_GEN.empty())
    {
        OutputLog("Average10000.empty() || Average10000_RANDOM_GEN.empty()");
        return {};
    }

    map<int, int> freq1, freq2;
    for (int val : Average10000) freq1[val]++;
    for (int val : Average10000_RANDOM_GEN) freq2[val]++;

    auto mode1 = max_element(freq1.begin(), freq1.end(), [](const auto& a, const auto& b) { return a.second < b.second; });
    auto mode2 = max_element(freq2.begin(), freq2.end(), [](const auto& a, const auto& b) { return a.second < b.second; });

    int modeValue1 = mode1->first;
    int maxFreq1 = mode1->second;
    int maxFreq2 = mode2->second;

    float yScale1 = graphHeight / log2(1.f + maxFreq1);
    float yScale2 = 300.f / static_cast<float>(maxFreq2);

    const float smoothWindow = 0.01;
    const float smoothWindow2 = 5;

    vector<pair<int, int>> smoothedFreq1, smoothedFreq2;

    for (auto it = freq1.begin(); it != freq1.end(); ++it)
    {
        int sum = 0, count = 0;
        for (int offset = -smoothWindow; offset <= smoothWindow; ++offset)
        {
            auto neighbor = freq1.find(it->first + offset);
            if (neighbor != freq1.end())
            {
                sum += neighbor->second;
                count++;
            }
        }
        smoothedFreq1.emplace_back(it->first, sum / max(1, count));
    }

    for (auto it = freq2.begin(); it != freq2.end(); ++it)
    {
        int sum = 0, count = 0;
        for (int offset = -smoothWindow2; offset <= smoothWindow2; ++offset)
        {
            auto neighbor = freq2.find(it->first + offset);
            if (neighbor != freq2.end())
            {
                sum += neighbor->second;
                count++;
            }
        }
        smoothedFreq2.emplace_back(it->first, sum / max(1, count));
    }

    float plotWidth = window.getSize().x - 100.f;
    float xStep1 = plotWidth / max(1.f, (float)smoothedFreq1.size());
    float xStep2 = plotWidth / max(1.f, (float)smoothedFreq2.size());

    VertexArray curve2(PrimitiveType::LineStrip);
    float x2 = 70.f;
    for (const auto& [val, count] : smoothedFreq2)
    {
        float y = window.getSize().y - marginBottom - count * yScale2;
        curve2.append(Vertex(Vector2f(x2, y), Color::Blue));
        x2 += xStep2;
    }
    window.draw(curve2);

    VertexArray curve1(PrimitiveType::LineStrip);
    float x1 = 70.f;
    for (const auto& [val, count] : smoothedFreq1)
    {
        float y = window.getSize().y - marginBottom - std::log2(1 + count) * yScale1;
        curve1.append(Vertex(Vector2f(x1, y), Color::Red));
        x1 += xStep1;
    }
    window.draw(curve1);

    vector<path> SearchFont = SearchFile("Font/", ".ttf");
    if (SearchFont.empty())
    {
        OutputLog("Шрифт не найден 2, завершение");
        return {};
    }
    OutputLog("попытка");

    Font font = LoadFont(SearchFont[0]);

    OutputLog("загружен");

    vector<int> keys;
    for (const auto& [k, _] : freq1) keys.push_back(k);
    sort(keys.begin(), keys.end());

    const size_t numLabels = 15;
    if (keys.size() >= 5)
    {
        float labelStep = plotWidth / (numLabels - 1);
        float labelX = 70.f;

        for (size_t i = 0; i < numLabels; ++i)
        {
            size_t idx = static_cast<size_t>((i * (keys.size() - 1)) / (numLabels - 1));
            int value = keys[idx];

            Text label(font);
            label.setCharacterSize(14);
            label.setFillColor(Color::Black);
            label.setString(to_string(value));
            label.setPosition(Vector2f(labelX - 10.f, window.getSize().y - marginBottom + 5.f));

            window.draw(label);
            labelX += labelStep;
        }
    }

    Text label(font);
    label.setCharacterSize(14);
    label.setFillColor(Color::Black);

    int RESULTCOUNTBOX = accumulate(Average10000.begin(), Average10000.end(), 0);

    label.setString(L"Среднее кол-во по всему диапазону: " + to_string(RESULTCOUNTBOX / static_cast<int>(Average10000.size())));
    label.setPosition(Vector2f(10, 1));
    window.draw(label);

    label.setString(L"частое колличество инструментов, необходимое для создания такого мода: " + to_string(modeValue1));
    label.setPosition(Vector2f(10, 15));
    window.draw(label);

    // ----------- Новый блок: диапазон часто встречающихся значений -----------
    float rangeThreshold = 0.6f; // Изменяемый параметр — насколько близко к максимуму

    vector<int> frequentValues;
    for (const auto& [val, count] : freq1)
    {
        if ((float)count >= (float)maxFreq1 * rangeThreshold)
        {
            frequentValues.push_back(val);
        }
    }

    if (!frequentValues.empty())
    {
        int minFreqVal = *min_element(frequentValues.begin(), frequentValues.end());
        int maxFreqVal = *max_element(frequentValues.begin(), frequentValues.end());

        label.setFillColor(Color::Black);
        label.setString(L"underprice меньше: " + to_wstring(minFreqVal));
        label.setPosition(Vector2f(10, 30));
        window.draw(label);
        label.setString(L"overprice больше: " + to_wstring(maxFreqVal));
        label.setPosition(Vector2f(10, 45));
        window.draw(label);
    }

    // ----------- Ошибки -----------
    wstring error;
    switch (Average10000[Average10000.size() - 1])
    {
    case -3333: error = L"нельзя прийти к такому моду"; break;
    case -2222: error = L"попытка уменьшить мод"; break;
    case -1111: error = L"нет варианта"; break;
    default: error = L""; break;
    }
    label.setString(error);
    label.setPosition(Vector2f(10, 60));
    window.draw(label);

    label.setFillColor(Color::Red);
    label.setString(L"Чаще");
    label.setPosition(Vector2f(10, 125));
    window.draw(label);

    label.setString(L"реже");
    label.setPosition(Vector2f(10, window.getSize().y - 70));
    window.draw(label);

    label.setString(L"кол-во");
    label.setPosition(Vector2f(10, window.getSize().y - marginBottom + 5.f));
    window.draw(label);
  
    label.setString(L"инструменты");
    label.setPosition(Vector2f(10, 75));
    window.draw(label);

    label.setFillColor(Color::Blue);
    label.setString(L"Сгенерированный шанс -> диапазон 00.00 - 100.00 | общее колличество: " + to_string(Average10000_RANDOM_GEN.size() ) );
    label.setPosition(Vector2f(10, 95));
    window.draw(label);

    return { modeValue1 };
}


