#include "CharacteristicGun.h"


/*
struct ParsedData7
{
    vector<pair<int, double>> pairs;
    int CurrentPositionInt = 0;
};
ParsedData7 parseChanceLine7(const string& input)
{
    ParsedData7 result;

    // Исправленный шаблон: ищем [int, double]
    regex pairPattern(R"(\[\s*(\d+)\s*,\s*([\d\.]+)\s*\])");
    smatch match;

    auto begin = input.cbegin();
    auto end = input.cend();

    while (regex_search(begin, end, match, pairPattern))
    {
        int first = stoi(match[1]);
        double second = stod(match[2]);
        result.pairs.emplace_back(first, second);
        begin = match.suffix().first;
    }

    // Парсим последнее число после внешнего массива
    size_t lastBracket = input.find_last_of(']');
    size_t trailingStart = input.find_first_of("0123456789", lastBracket);
    if (trailingStart != string::npos)
    {
        result.CurrentPositionInt = stoi(input.substr(trailingStart));
    }

    return result;
}

struct ParsedData14
{
    vector<tuple<int, int, int>> triples;
    int MaxPositionInt = 0;
};
ParsedData14 parseChanceLine14(const string& input)
{
    ParsedData14 result;

    // Регулярка для тройки чисел: [int, int, int]
    regex triplePattern(R"(\[\s*(\d+)\s*,\s*(\d+)\s*,\s*(\d+)\s*\])");
    smatch match;

    auto begin = input.cbegin();
    auto end = input.cend();

    while (regex_search(begin, end, match, triplePattern)) 
    {
        int a = stoi(match[1]);
        int b = stoi(match[2]);
        int c = stoi(match[3]);
        result.triples.emplace_back(a, b, c);
        begin = match.suffix().first;
    }

    // Ищем последнее число (после последней закрывающей скобки)
    size_t lastBracket = input.find_last_of(']');
    size_t trailingStart = input.find_first_of("0123456789", lastBracket);
    if (trailingStart != string::npos) 
    {
        result.MaxPositionInt = stoi(input.substr(trailingStart));
    }

    return result;
}

///-----///-----///-----///-----///-----///-----///-----///-----///-----///-----///-----///-----///-----///-----///-----///-----///-----

bool CharacteristicGun::Load(path PathToInfo)
{
    CharacteristicGun::clear();
    // открываем файл оружия
    ifstream file(PathToInfo);
    if (!file)
    {
        OutputLog("Файл не найден");
        return false;
    }

    string line;
    int lineCount = 0;

    // считываем 
    while (getline(file, line) && lineCount < 14)
    {
        // первые 7 строк \\ статы
        if      (lineCount < 7)
        {
            // получаем вектор разметки и нулевой стат
            ParsedData7 data = parseChanceLine7(line);

            const size_t SIZE = [lineCount] {

                if (lineCount == 0) { return  60; }
                else if (lineCount == 1) { return 48; }
                else { return 40; }

                }();

            ///////
            vector<AllStat> result;

            // разметка
            for (int position = 0; position <= SIZE; position++)
            {
                // позиция известна
                // получаем шанс по позиции
                // относительную стату находим позже
                float parametr;
                if      (lineCount == 0) { parametr = 36 - (0.6 * position); }
                else if (lineCount == 1) { parametr = 50 + (25 * position); }
                else if (lineCount == 2) { parametr = -100 + (5 * position); }
                else                     { parametr = position; }

                result.push_back(AllStat(position, parametr, (getChance(position - 1, data.pairs)), 0.0));

            }

            // находим проценты
            vector<double> Procent;
            for (int position = 0; position < SIZE * 2; position++)
            {
                Procent.push_back(getChance(position, { {0, -100}, {SIZE, 0}, {SIZE * 2, 100} }));
            }

            // размечаем отображаемые проценты
            for (int ps = data.CurrentPositionInt, it = 0; ps < result.size(); ps++, it++)
            {
                result[ps].VisualProcentStat = Procent[SIZE + it];
            }

            for (int ps = data.CurrentPositionInt - 1, it = 0; ps >= 0; ps--, it++)
            {
                if (ps < 0) { break; }

                auto stat = Procent[SIZE - it - 1];
                result[ps].VisualProcentStat = stat;
            }
            ///////
            // 0 куч
            // 1 темп            // 
            // 
            // 2 отдача
            // 3 качание            // 
            // 
            // 4 пробитие            // 
            // 
            // 5 сост
            // 6 грязь
            //
            CurrentStatPosition.push_back(data.CurrentPositionInt);

            CharacteristicGun::FulAllStat.push_back(result);
        }

        else if (lineCount < 14)
        {
            ParsedData14 Line14 = parseChanceLine14(line);

            vector<DecreaseStatST> result;

            for (int it = 0; it < Line14.triples.size(); it++)
            {
                DecreaseStatST GradeTemp = { get<0>(Line14.triples[it]), get<1>(Line14.triples[it]), get<2>(Line14.triples[it]) };
                result.push_back(GradeTemp);
            }
            FullDecreaseStat.push_back(result);
            MaxStatPosition.push_back(Line14.MaxPositionInt);
        }


        ++lineCount;
    }
    
    if (lineCount != 14)
    {
        OutputLog("class CharacteristicGun -> line != 14");
        CharacteristicGun::clear();
        return false;
    }

    // разместили
    Empty = false;

    InitialStatPosition = CurrentStatPosition;

    return true;
}

*/




wstring StringToWString(const string& str)
{
    if (str.empty()) return L"";

    int size_needed = MultiByteToWideChar(
        CP_UTF8,            // Кодировка исходной строки (UTF-8)
        0,
        str.c_str(),
        (int)str.size(),
        NULL,
        0
    );

    wstring wstrTo(size_needed, 0);

    MultiByteToWideChar(
        CP_UTF8,
        0,
        str.c_str(),
        (int)str.size(),
        &wstrTo[0],
        size_needed
    );

    return wstrTo;
}

bool CharacteristicGun::Load(path PathToInfo_JS, wstring namegun)
{




    ifstream JSON_File_IFS(PathToInfo_JS);


    if (!JSON_File_IFS)
    {
        OutputLog("Не удалось открыть файл");
        return false;
    }
    


    nlohmann::json JSON_STAT;

    JSON_File_IFS >> JSON_STAT;



    // чистим перед новой загрузкой
    CharacteristicGun::clear();








    for (const auto& CurrentSelectGun : JSON_STAT)
    {
        
        auto JSON_NameGunB = CurrentSelectGun.contains("NameGun");


        wstring JSON_NameGun = StringToWString(CurrentSelectGun["NameGun"]);

        bool HUISOS = JSON_NameGun == namegun;

        if (JSON_NameGunB && HUISOS)
        {

            OutputLog("Нашел оружие");




            /////////////////////////////////////////////////////////////////////////
            /////////////////////////////////////////////////////////////////////////

            // считываем первый массив ALLSTATGUN
            auto& ALLSTATGUN = CurrentSelectGun["ALLSTATGUN"];



            /// сичтываем 6 строк chance_points (и некоторых других данных)
            for (size_t lineCount = 0; lineCount < ALLSTATGUN.size(); lineCount++)
            {
                /// текущий индекс
                /// параметр в колличестве
                /// текуший шанс
                /// текущий процент
                vector<AllStat> result;



                /////////////////////////////////////////////////////////////////////////
                // получаем строку chance_points
                vector<pair<int, double>> pairs;


                auto& chance_points = ALLSTATGUN[to_string(lineCount)]["ChanceVector"];

                for (auto& item : chance_points)
                {                    
                    int    first  = item[0].get<int>();
                    double second = item[1].get<double>();
                    pairs.emplace_back(first, second);
                }
                /////////////////////////////////////////////////////////////////////////












                /////////////////////////////////////////////////////////////////////////
                // максимальное коллличество единиц характеристики для текущей статы
                
                auto& MaxSize = ALLSTATGUN[to_string(lineCount)]["MaxSize"];

                // максимальное коллличество единиц характеристики для текущей статы
                const int MAX_COUNT_UNITS_STAT = MaxSize.get<int>();

                /////////////////////////////////////////////////////////////////////////













                /////////////////////////////////////////////////////////////////////////
                for (int Real_Position = 0; Real_Position <= MAX_COUNT_UNITS_STAT; Real_Position++)
                {
                    // позиция известна Real_Position

                    // получаем шанс по позиции getChance()

                    // ВИЗУАЛЬНЫЙ ПРОЦЕНТ АПГРЕЙДА ПОЗЖЕ //////  СЕЙЧАС - 0.0 

                    //реальный параметр = parametr например темп огня parametr = 650
                    float parametr;

                    if      (lineCount == 0) { parametr = 36 - (0.6 * Real_Position); }
                    else if (lineCount == 1) { parametr = 50 + (25 * Real_Position); }
                    else if (lineCount == 2) { parametr = -100 + (5 * Real_Position); }
                    else                     { parametr = Real_Position; }

                    auto ALLSTAT = AllStat(Real_Position, parametr, (getChance(Real_Position - 1, pairs)), 0.0);

                    result.push_back(ALLSTAT);
                }
                /////////////////////////////////////////////////////////////////////////













                ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

                //  КАЛИБРОВКА ВИЗУАЛЬНОГО ПРОЦЕНТА ДЛЯ ТЕКУЩЕГО ОРУЖИЯ \\
                //  НАПРИМЕР ЧТОБЫ 650 ТЕМП ОГНЯ БЫЛ 0% -> ТО ЕСТЬ ДЕФОЛТ СОСТОЯНИЕ


                // размечаем проценты по позициям
                vector<double> Procent;
                for (int position = 0; position < MAX_COUNT_UNITS_STAT * 2; position++)
                {
                    Procent.push_back(getChance(position, { {0, -100}, {MAX_COUNT_UNITS_STAT, 0}, {MAX_COUNT_UNITS_STAT * 2, 100} }));
                }


                // текущая позиция относительно максимально
                int СurrentPositionRelative_MAX_COUNT_UNITS_STAT;
                /////////////////////
                // 0 куч
                // 1 темп            
                //
                // 4 пробитие            
                // 
                // 2 отдача
                // 3 качание    
                // 5 сост
                // 6 грязь
                //
                /////////////////////
                
                // получаем реальную позицию относительно максимальной
                switch (lineCount)
                {
                // кучность
                case 0: 
                    СurrentPositionRelative_MAX_COUNT_UNITS_STAT = static_cast<int>(round((0.6f - CurrentSelectGun["AccuracyDefault"].get<float>()) * 100));
                    break;

                //  темп огня 
                case 1:                    
                    //  -50 потому что минимаоьный темп = 50  \\\\\  делим на 25 т.к. одна прибавка дает +25
                    // в итоге получаем реальную позицию относительно максимальной
                    СurrentPositionRelative_MAX_COUNT_UNITS_STAT = (CurrentSelectGun["RateFireDefault"].get<int>() -50) / 25;
                    break;

                // пробитие
                case 4:
                    // +100 потому что минимальное пробитие = -100 
                    // прибавляем текущеей пробитие и делим на 5 потому что один апгрейд дает +5 единици пробития
                    // в итоге получаем реальную позицию относительно максимальной
                    СurrentPositionRelative_MAX_COUNT_UNITS_STAT = (100 + CurrentSelectGun["PenetrationDefault"].get<int>()) / 5 ;
                    break;

                // в других случаях есть CurrentPosition             
                // 2 отдача
                case 2:
                // 3 отдача
                case 3:
                // 5 отдача
                case 5:
                // 6 отдача
                case 6:
                    СurrentPositionRelative_MAX_COUNT_UNITS_STAT = ALLSTATGUN[to_string(lineCount)]["CurrentPosition"].get<int>();
                    break;

                default:
                    СurrentPositionRelative_MAX_COUNT_UNITS_STAT = 0;
                    break;
                }

                // размечаем ВИЗУАЛЬНЫЕ проценты апгрейда
                for (int PositionPlus = СurrentPositionRelative_MAX_COUNT_UNITS_STAT, it = 0; PositionPlus < result.size(); PositionPlus++, it++)
                {
                    result[PositionPlus].VisualProcentStat = Procent[MAX_COUNT_UNITS_STAT + it];
                }


                for (int PositionMinus = СurrentPositionRelative_MAX_COUNT_UNITS_STAT - 1, it = 0; PositionMinus >= 0; PositionMinus--, it++)
                {
                    if (PositionMinus < 0) { break; }

                    auto stat = Procent[MAX_COUNT_UNITS_STAT - it - 1];
                    result[PositionMinus].VisualProcentStat = stat;
                }
                ////////////////////////////////////////////////////////////////////////////////////////////////////////








                CurrentStatPosition.push_back(СurrentPositionRelative_MAX_COUNT_UNITS_STAT);
                FulAllStat.push_back(result);
                /// конец считывания строки lineCount 0й ...... 6й  из ALLSTATGUN
            }

            /////////////////////////////////////////////////////////////////////////
            /////////////////////////////////////////////////////////////////////////







            /////////////////////////////////////////////////////////////////////////
            /////////////////////////////////////////////////////////////////////////

            // считываем DecreaseStat
           

            auto& DecreaseStat = CurrentSelectGun["DecreaseStat"];

            
            for (int Line = 0; Line < DecreaseStat.size(); Line++)
            {
                vector<DecreaseStatST> result;

                auto& chance_points = DecreaseStat[to_string(Line)];

                for (auto& triple : chance_points)
                {
                    //  что уменьшать?
                    int  PositionLower     = triple[0].get<int>();
                    //  насколько уменьшать?
                    int  HowMany           = triple[1].get<int>();
                    /// после какой позиции начать уменьшать
                    int PositionStartLower = triple[2].get<int>();
                    

                    if (PositionLower == 7)
                    {
                        PositionLower = 0;
                        HowMany = 0;
                        PositionStartLower = 0;
                    }

                    result.emplace_back(PositionLower, HowMany, PositionStartLower);
                }

                FullDecreaseStat.push_back(result);


                /// конец считывания строки Line 0й ...... 6й  из DecreaseStat
            }


            auto& MaxStat = CurrentSelectGun["MaxStat"];

            MaxStatPosition = { 

                (360 - MaxStat[0].get<int>()) / 6,                     // куч 
                (MaxStat[1].get<int>() - 50)  / 25,                    // темп
                (MaxStat[3].get<int>() / 25 + CurrentStatPosition[2]), // отдача
                (MaxStat[4].get<int>() / 25 + CurrentStatPosition[3]), // качание
                (MaxStat[2].get<int>() + 100) / 5,                     // пробой
                (MaxStat[5].get<int>() / 25 + CurrentStatPosition[5]), // состояние
                (MaxStat[6].get<int>() / 25 + CurrentStatPosition[6])  // грязь

            };



            FulAllStat;
            FullDecreaseStat;
            CurrentStatPosition;
            MaxStatPosition;


            InitialStatPosition = CurrentStatPosition;

            Empty = false;
            return true;
            /////////////////////////////////////////////////////////////////////////
            /////////////////////////////////////////////////////////////////////////
        }
    }




    OutputLog("Не удалось найти оружие");
    return false;    
}


























void CharacteristicGun::clear()
{
    FulAllStat.clear();

    FullDecreaseStat.clear();

    CurrentStatPosition.clear();

    MaxStatPosition.clear();

    InitialStatPosition.clear();

    Empty = true;
}

bool CharacteristicGun::is_Empty()
{
    return Empty;
}

CharacteristicGun::CharacteristicGun()
{
    // пуст
    Empty = true;
}

bool CharacteristicGun::ReturnDefaultstat() 
{
    CurrentStatPosition = InitialStatPosition;

    return CurrentStatPosition == InitialStatPosition ? true : false;    
}

vector<int> CharacteristicGun::GetMaxStatPosition()
{
    return MaxStatPosition;
}

vector<double> CharacteristicGun::GetFullCurrentVisualStat()
{
    vector<double> result;


    for (int i = 0; i < CurrentStatPosition.size(); i++)
    {
        result.push_back(CharacteristicGun::GetVisualProcentStat(i));
    }
    return result;
}

vector<int> CharacteristicGun::GetCurrentPosition()
{
    return CharacteristicGun::CurrentStatPosition;
}

CharacteristicGun::CharacteristicGun(const CharacteristicGun& other)
{

    this->FulAllStat = other.FulAllStat;

    this->FullDecreaseStat = other.FullDecreaseStat;

    this->CurrentStatPosition = other.CurrentStatPosition;

    this->MaxStatPosition = other.MaxStatPosition;

    this->InitialStatPosition = other.InitialStatPosition;

    this->Empty = other.Empty;

}


double CharacteristicGun::GetMaxStatVisualProcent(unsigned int stat)
{
    return FulAllStat[stat][MaxStatPosition[stat]].VisualProcentStat;
}

double CharacteristicGun::GetVisualProcentStat(int stat)
{
    return FulAllStat[stat][CurrentStatPosition[stat]].VisualProcentStat;
}


double CharacteristicGun::GetChanceFor_NEXT_Stat(unsigned int stat)
{
    if (CurrentStatPosition[stat] + 1 > MaxStatPosition[stat])
    {
        OutputLog("class CharacteristicGun нельзая получить шанс, больше максимального");
        return 0.0;
    }
    if (CurrentStatPosition[stat] + 1 > FulAllStat[stat].size())
    {
        OutputLog("class CharacteristicGun нельзая получить шанс, выход за пределы");
        return -1.0;
    }

    double result;

    unsigned int v2 = CurrentStatPosition[stat] + 1;


    result = FulAllStat[stat][v2].CurrentChance;
    return result;
}

bool CharacteristicGun::UpStat(int stat_No)
{
    // если текущая позиция больше максимума не подымаем
    if (CurrentStatPosition[stat_No] + 1 > MaxStatPosition[stat_No])
    {
        return false;
    }

    // увеличиваем
    CurrentStatPosition[stat_No]++;


    // уменьшаем другие статы если есть указаны

    // проходимся по вектору указанных статов
    for (int i = 0; i < FullDecreaseStat[stat_No].size(); i++)
    {
        // если текущая стата больше началу уменьшения одного из GradeStat
        if (CurrentStatPosition[stat_No] > InitialStatPosition[stat_No] + FullDecreaseStat[stat_No][i].PositionStartLower)
        {
            // нельзя уменьшить позицию ниже нуля
            if (CurrentStatPosition[FullDecreaseStat[stat_No][i].PositionLower] > 0)
            {
                // уменьшаем позицию, на которую GradeStat указывает на единицы которые прописаны
                CurrentStatPosition[FullDecreaseStat[stat_No][i].PositionLower] -= FullDecreaseStat[stat_No][i].HowMany;
            }
        }
    }

    return true;
}

bool CharacteristicGun::UpgradeStat(int stat_no)
{
    if (stat_no < 0 || stat_no > CurrentStatPosition.size())
    {
        OutputLog("class CharacteristicGun UpgradeStat -> выход за пределы");
        return false;
    }
    return UpStat(stat_no);
}

///-----///-----///-----///-----///-----///-----///-----///-----///-----///-----///-----///-----///-----///-----///-----///-----///-----
