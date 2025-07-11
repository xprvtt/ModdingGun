#include "CharacteristicGun.h"


bool CharacteristicGun::load(path PathToInfo_JS, wstring namegun)
{

    // чистим перед новой загрузкой
    CharacteristicGun::clear();


    ifstream JSON_File_IFS(PathToInfo_JS);


    if (!JSON_File_IFS)
    {
        OutputLog("CharacteristicGun -> Failed to open file: " + WstringToString(PathToInfo_JS));
        return false;
    }
    


    nlohmann::json JSON_STAT;
    JSON_File_IFS >> JSON_STAT;
    JSON_File_IFS.close();



    for (const auto& CurrentSelectGun : JSON_STAT)
    {
        
        auto JSON_NameGunB = CurrentSelectGun.contains("NameGun");


        wstring JSON_NameGun = StringToWString(CurrentSelectGun["NameGun"]);


        if (JSON_NameGunB && JSON_NameGun == namegun)
        {
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
                const int MAX_COUNT_UNITS = GunStats::GET_COUNT_UNITS_FOR_CHARACTERISTIC[lineCount];

                /////////////////////////////////////////////////////////////////////////













                /////////////////////////////////////////////////////////////////////////
                for (int Real_Position = 0; Real_Position <= MAX_COUNT_UNITS; Real_Position++)
                {
                    // позиция известна Real_Position

                    // получаем шанс по позиции getChance()

                    // ВИЗУАЛЬНЫЙ ПРОЦЕНТ АПГРЕЙДА ПОЗЖЕ //////  СЕЙЧАС - 0.0 

                    //реальный параметр = parametr например темп огня parametr = 650
                    float ValueCharacteristic;

                    if      (lineCount == 0) { ValueCharacteristic = static_cast<double>(36 - (0.6 * Real_Position)); }
                    else if (lineCount == 1) { ValueCharacteristic = static_cast<double>(50 + (25 * Real_Position)); }
                    else if (lineCount == 4) { ValueCharacteristic = static_cast<double>(-100 + (5 * Real_Position)); }

                    else    { ValueCharacteristic = static_cast<double>(Real_Position); }

                    auto ALLSTAT = AllStat(Real_Position, ValueCharacteristic, (GetChance(Real_Position - 1, pairs)), 0.0);

                    result.push_back(ALLSTAT);
                }
                /////////////////////////////////////////////////////////////////////////













                ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

                //  КАЛИБРОВКА ВИЗУАЛЬНОГО ПРОЦЕНТА ДЛЯ ТЕКУЩЕГО ОРУЖИЯ \\
                //  НАПРИМЕР ЧТОБЫ 650 ТЕМП ОГНЯ БЫЛ 0% -> ТО ЕСТЬ ДЕФОЛТ СОСТОЯНИЕ


                // размечаем проценты по позициям
                vector<double> Percent;
                for (int position = 0; position < MAX_COUNT_UNITS * 2; position++)
                {
                    Percent.push_back(GetChance(position, { {0, -100}, {MAX_COUNT_UNITS, 0}, {MAX_COUNT_UNITS * 2, 100} }));
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
                // 3 кач
                case 3:
                // 5 сост
                case 5:
                // 6 грязь
                case 6:
                    СurrentPositionRelative_MAX_COUNT_UNITS_STAT = ALLSTATGUN[to_string(lineCount)]["CurrentPosition"].get<int>();
                    break;

                default:
                    СurrentPositionRelative_MAX_COUNT_UNITS_STAT = 0;
                    break;
                }


                // размечаем ВИЗУАЛЬНЫЕ проценты апгрейда
                // выше нуля
                for (int PositionPlus = СurrentPositionRelative_MAX_COUNT_UNITS_STAT, it = 0; PositionPlus < result.size() && MAX_COUNT_UNITS + it < Percent.size(); PositionPlus++, it++)
                {

                    result[PositionPlus].VisualPercentStat = Percent[MAX_COUNT_UNITS + it];
                }

                // ниже нуля
                for (int PositionMinus = СurrentPositionRelative_MAX_COUNT_UNITS_STAT - 1, it = 0; PositionMinus >= 0; PositionMinus--, it++)
                {
                    auto stat = Percent[MAX_COUNT_UNITS - it - 1];
                    result[PositionMinus].VisualPercentStat = stat;
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
                    int PositionLower      = triple[0].get<int>();
                    //  насколько уменьшать?
                    int HowMany            = triple[1].get<int>();
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


            DefaultStatPosition = CurrentStatPosition;

            Empty = false;
            return true;
            /////////////////////////////////////////////////////////////////////////
            /////////////////////////////////////////////////////////////////////////
        }
    }




    OutputLog("Gun not found: " + string(namegun.begin(), namegun.end()));
    return false;    
}







CharacteristicGun::CharacteristicGun(vector<int> MaxStatPosition)
{
    CountOption = static_cast<unsigned int>(MaxStatPosition.size());

    CurrentStatPosition.resize(CountOption, 0);
    DefaultStatPosition.resize(CountOption, 0);

    this->MaxStatPosition = MaxStatPosition;

    // пуст
    Empty = true;
}

CharacteristicGun::CharacteristicGun(const CharacteristicGun& other)
{

    this->FulAllStat = other.FulAllStat;

    this->FullDecreaseStat = other.FullDecreaseStat;

    this->CurrentStatPosition = other.CurrentStatPosition;

    this->MaxStatPosition = other.MaxStatPosition;

    this->DefaultStatPosition = other.DefaultStatPosition;

    this->Empty = other.Empty;

    this->CountOption = other.CountOption;

    this->UpgradeHistory = other.UpgradeHistory;
}





bool CharacteristicGun::upStat(int stat_No)
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
        if (CurrentStatPosition[stat_No] > DefaultStatPosition[stat_No] + FullDecreaseStat[stat_No][i].PositionStartLower)
        {
            // нельзя уменьшить позицию ниже нуля
            if (CurrentStatPosition[FullDecreaseStat[stat_No][i].PositionLower] > 0)
            {
                // уменьшаем позицию, на которую GradeStat указывает на единицы которые прописаны
                CurrentStatPosition[FullDecreaseStat[stat_No][i].PositionLower] -= FullDecreaseStat[stat_No][i].HowMany;
            }
        }
    }

    UpgradeHistory.push_back(CurrentStatPosition);
    return true;
}

bool CharacteristicGun::upgradeStat(int stat_no)
{
    if (stat_no < 0 || stat_no > CurrentStatPosition.size())
    {
        OutputLog("CharacteristicGun UpgradeStat -> going beyond");
        return false;
    }
    return upStat(stat_no);
}







void CharacteristicGun::clear()
{
    FulAllStat.clear();
    FullDecreaseStat.clear();
    CurrentStatPosition.clear();
    MaxStatPosition.clear();
    DefaultStatPosition.clear();
    UpgradeHistory.clear();
    Empty = true;
}

bool CharacteristicGun::is_Empty() const
{
    return Empty;
}

bool CharacteristicGun::returnDefaultPosition() 
{
    CurrentStatPosition = DefaultStatPosition;

    return CurrentStatPosition == DefaultStatPosition ? true : false;    
}

vector<int>    CharacteristicGun::getDefaultPosition() const
{
    return CharacteristicGun::DefaultStatPosition;
}

vector<int>    CharacteristicGun::getMaxPositionCharacteristic() const
{
    return MaxStatPosition;
}

vector<double> CharacteristicGun::getFullCurrentVisualStat() const
{
    vector<double> result;


    for (int i = 0; i < CurrentStatPosition.size(); i++)
    {
        result.push_back(CharacteristicGun::getVisualPercentUpgradeCharacteristic(i));
    }
    return result;
}

vector<int>    CharacteristicGun::getCurrentPosition() const
{
    return CharacteristicGun::CurrentStatPosition;
}

double CharacteristicGun::getValueCharacteristic(size_t stat) const
{
    return FulAllStat[stat][CurrentStatPosition[stat]].ValueCharacteristic;
}
               
double         CharacteristicGun::getMaxStatVisualPercent(size_t stat)  const
{
    return FulAllStat[stat][MaxStatPosition[stat]].VisualPercentStat;
}
               
double         CharacteristicGun::getVisualPercentUpgradeCharacteristic(size_t stat) const
{
    auto result = FulAllStat[stat][CurrentStatPosition[stat]].VisualPercentStat;
    return result;
}
               
double         CharacteristicGun::getChanceFor_NEXT_Stat(size_t stat)   const
{
    if (CurrentStatPosition[stat] + 1 > MaxStatPosition[stat])
    {
        OutputLog("CharacteristicGun -> can't get a chance greater than the maximum");
        return 0.0;
    }
    if (CurrentStatPosition[stat] + 1 > FulAllStat[stat].size())
    {
        OutputLog("CharacteristicGun -> can't get a chance, go beyond");
        return -1.0;
    }

    double result;

    unsigned int v2 = CurrentStatPosition[stat] + 1;


    result = FulAllStat[stat][v2].CurrentChance;
    return result;
}

bool           CharacteristicGun::stepBack()
{
    // если вектор пуст выходим
    if (UpgradeHistory.empty()) { return false; }

    // иначе удаляем из истории текущий
    UpgradeHistory.pop_back();

    // устанавливаем предыдущий если не пустой
    if (!UpgradeHistory.empty())
    {
        CurrentStatPosition = UpgradeHistory[UpgradeHistory.size() - 1];
    }
    // иначе дефолтные характеристики
    else
    {
        CharacteristicGun::returnDefaultPosition();
    }
    
    return true;
}

///-----///-----///-----///-----///-----///-----///-----///-----///-----///-----///-----///-----///-----///-----///-----///-----///-----


vector<int>    CharacteristicGun::getDecreaseForCurrentStat(size_t stat) const
{
    vector<int> result(CountOption, 0);

    // проходимся по вектору указанных статов
    for (int i = 0; i < FullDecreaseStat[stat].size(); i++)
    {
        // если текущая стата больше началу уменьшения одного из GradeStat
        if (CurrentStatPosition[stat] > DefaultStatPosition[stat] + FullDecreaseStat[stat][i].PositionStartLower)
        {
            // уменьшаем позицию, на которую GradeStat указывает на единицы которые прописаны
            result[FullDecreaseStat[stat][i].PositionLower] += FullDecreaseStat[stat][i].HowMany;
        }
    }


    return result;
}