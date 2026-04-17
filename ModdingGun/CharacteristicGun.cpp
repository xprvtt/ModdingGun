#include "CharacteristicGun.h"


bool CharacteristicGun::load(path pathToInfoJS, wstring namegun)
{
    // очищаем перед новой загрузкой
    CharacteristicGun::clear();

    ifstream jsonFileIFS(pathToInfoJS);
    if (!jsonFileIFS)
    {
        OUTPUT_LOG("CharacteristicGun -> Failed to open file: " + WstringToString(pathToInfoJS));
        return false;
    }

    nlohmann::json jsonStat;
    jsonFileIFS >> jsonStat;
    jsonFileIFS.close();

    for (const auto& currentSelectGun : jsonStat)
    {        
        auto jsonNameGunB = currentSelectGun.contains("NameGun");
        wstring jsonNameGun = StringToWString(currentSelectGun["NameGun"]);

        if (jsonNameGunB && jsonNameGun == namegun)
        {
            // считываем первый массив ALLSTATGUN
            auto& allStatGun = currentSelectGun["ALLSTATGUN"];

            /// сичтываем 6 строк chancePoints (и некоторых других данных)
            for (size_t lineCurrent = 0; lineCurrent < allStatGun.size(); lineCurrent++)
            {
                /// текущий индекс
                /// параметр в колличестве
                /// текуший шанс
                /// текущий процент
                vector<AllStat> result;

                // получаем строку chance_points
                vector<pair<int, double>> pairs;
                auto& chancePoints = allStatGun[to_string(lineCurrent)]["ChanceVector"];

                for (auto& item : chancePoints)
                {                    
                    int    first  = item[0].get<int>();
                    double second = item[1].get<double>();
                    pairs.emplace_back(first, second);
                }
               
                // максимальное коллличество единиц характеристики для текущей статы
                const int maxCountUnits = GunStats::getCountUnitsForCharacteristic[lineCurrent];

                for (int realPosition = 0; realPosition <= maxCountUnits; realPosition++)
                {
                    // позиция известна Real_Position
                    // получаем шанс по позиции getChance()
                    // ВИЗУАЛЬНЫЙ ПРОЦЕНТ АПГРЕЙДА ПОЗЖЕ //////  СЕЙЧАС - 0.0 
                    //реальный параметр = parametr например темп огня parametr = 650
                    float valueCharacteristic = 0;

                    if      (lineCurrent == 0) { valueCharacteristic = static_cast<double>(36 - (0.6 * realPosition)); }
                    else if (lineCurrent == 1) { valueCharacteristic = static_cast<double>(50 + (25 * realPosition)); }
                    else if (lineCurrent == 4) { valueCharacteristic = static_cast<double>(-100 + (5 * realPosition)); }

                    else    { valueCharacteristic = static_cast<double>(realPosition); }

                    auto allStatCurrent = AllStat(realPosition, valueCharacteristic, (getChance(realPosition - 1, pairs)), 0.0);
                    result.push_back(allStatCurrent);
                }

                //  КАЛИБРОВКА ВИЗУАЛЬНОГО ПРОЦЕНТА ДЛЯ ТЕКУЩЕГО ОРУЖИЯ \\
                //  НАПРИМЕР ЧТОБЫ 650 ТЕМП ОГНЯ БЫЛ 0% -> ТО ЕСТЬ ДЕФОЛТ СОСТОЯНИЕ

                // размечаем проценты по позициям
                vector<double> percent;
                for (int position = 0; position < maxCountUnits * 2; position++)
                {
                    percent.push_back(getChance(position, { {0, -100}, {maxCountUnits, 0}, {maxCountUnits * 2, 100} }));
                }

                // текущая позиция относительно максимально
                int currentPositionRelativeMax = 0;

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
                /////////////////////
                
                // получаем реальную позицию относительно максимальной
                switch (lineCurrent)
                {                
                case 0: // кучность
                    currentPositionRelativeMax = static_cast<int>(round((0.6f - currentSelectGun["AccuracyDefault"].get<float>()) * 100));
                    break;

                case 1: //  темп огня 
                    //  -50 потому что минимаоьный темп = 50  \\\\\  делим на 25 т.к. одна прибавка дает +25
                    // в итоге получаем реальную позицию относительно максимальной
                    currentPositionRelativeMax = (currentSelectGun["RateFireDefault"].get<int>() -50) / 25;
                    break;
                
                case 4: // пробитие
                    // +100 потому что минимальное пробитие = -100 
                    // прибавляем текущеей пробитие и делим на 5 потому что один апгрейд дает +5 единици пробития
                    // в итоге получаем реальную позицию относительно максимальной
                    currentPositionRelativeMax = (100 + currentSelectGun["PenetrationDefault"].get<int>()) / 5 ;
                    break;

                // в других случаях есть CurrentPosition                               
                case 2: // 2 отдача        
                case 3: // 3 кач            
                case 5: // 5 сост       
                case 6: // 6 грязь
                    currentPositionRelativeMax = allStatGun[to_string(lineCurrent)]["CurrentPosition"].get<int>();
                    break;

                default:
                    currentPositionRelativeMax = 0;
                    break;
                }

                // размечаем ВИЗУАЛЬНЫЕ проценты апгрейда
                // выше нуля
                for (int positionPlus = currentPositionRelativeMax, it = 0; positionPlus < result.size() && maxCountUnits + it < percent.size(); positionPlus++, it++)
                {

                    result[positionPlus].m_visualPercentStat = percent[maxCountUnits + it];
                }

                // ниже нуля
                for (int positionMinus = currentPositionRelativeMax - 1, it = 0; positionMinus >= 0; positionMinus--, it++)
                {
                    auto stat = percent[maxCountUnits - it - 1];
                    result[positionMinus].m_visualPercentStat = stat;
                }

                m_currentStatPosition.push_back(currentPositionRelativeMax);
                m_fullAllStat.push_back(result);
                /// конец считывания строки lineCurrent 0й ...... 6й  из ALLSTATGUN
            }

            // считываем DecreaseStat
            auto& decreaseStat = currentSelectGun["DecreaseStat"];
            
            for (int line = 0; line < decreaseStat.size(); line++)
            {
                vector<DecreaseStatST> result;
                auto& chancePoints = decreaseStat[to_string(line)];

                for (auto& triple : chancePoints)
                {
                    //  что уменьшать?
                    int positionLower      = triple[0].get<int>();

                    //  насколько уменьшать?
                    int howMany            = triple[1].get<int>();

                    /// после какой позиции начать уменьшать
                    int positionStartLower = triple[2].get<int>();                    

                    if (positionLower == 7)
                    {
                        positionLower = 0;
                        howMany = 0;
                        positionStartLower = 0;
                    }
                    result.emplace_back(positionLower, howMany, positionStartLower);
                }
                m_fullDecreaseStat.push_back(result);
                /// конец считывания строки Line 0й ...... 6й  из DecreaseStat
            }

            auto& maxStatSelectGun = currentSelectGun["MaxStat"];

            m_maxStatPosition = { 

                (360 - maxStatSelectGun[0].get<int>()) / 6,                     // куч 
                (maxStatSelectGun[1].get<int>() - 50)  / 25,                    // темп
                (maxStatSelectGun[3].get<int>() / 25 + m_currentStatPosition[2]), // отдача
                (maxStatSelectGun[4].get<int>() / 25 + m_currentStatPosition[3]), // качание
                (maxStatSelectGun[2].get<int>() + 100) / 5,                     // пробой
                (maxStatSelectGun[5].get<int>() / 25 + m_currentStatPosition[5]), // состояние
                (maxStatSelectGun[6].get<int>() / 25 + m_currentStatPosition[6])  // грязь
            };

            //m_fullAllStat;
            //m_fullDecreaseStat;
            //m_сurrentStatPosition;
            //MaxStatPosition;

            m_defaultStatPosition = m_currentStatPosition;

            m_empty = false;
            return true;
        }
    }
    OUTPUT_LOG("Gun not found: " + path(namegun).string());
    return false;    
}

//-----------------------------------------------------------------------------------------------------------------------

CharacteristicGun::CharacteristicGun(vector<int> maxStatPosition)
{
    m_countOption = static_cast<unsigned int>(maxStatPosition.size());

    m_currentStatPosition.resize(m_countOption, 0);
    m_defaultStatPosition.resize(m_countOption, 0);

    m_maxStatPosition = maxStatPosition;

    // пуст
    m_empty = true;
}

//-----------------------------------------------------------------------------------------------------------------------

CharacteristicGun::CharacteristicGun(const CharacteristicGun& other)
    : m_fullAllStat(other.m_fullAllStat),
    m_fullDecreaseStat(other.m_fullDecreaseStat),
    m_currentStatPosition(other.m_currentStatPosition),
    m_maxStatPosition(other.m_maxStatPosition),
    m_defaultStatPosition(other.m_defaultStatPosition),
    m_empty(other.m_empty),
    m_countOption(other.m_countOption),
    m_upgradeHistory(other.m_upgradeHistory)
{}

//-----------------------------------------------------------------------------------------------------------------------

bool CharacteristicGun::upStat(int statNo)
{
    // если текущая позиция больше максимума не подымаем
    if (m_currentStatPosition[statNo] + 1 > m_maxStatPosition[statNo])
    {
        return false;
    }

    // увеличиваем
    m_currentStatPosition[statNo]++;


    // уменьшаем другие статы если есть указаны

    // проходимся по вектору указанных статов
    for (int i = 0; i < m_fullDecreaseStat[statNo].size(); i++)
    {
        // если текущая стата больше началу уменьшения одного из GradeStat
        if (m_currentStatPosition[statNo] > m_defaultStatPosition[statNo] + m_fullDecreaseStat[statNo][i].m_positionStartLower)
        {
            // нельзя уменьшить позицию ниже нуля
            if (m_currentStatPosition[m_fullDecreaseStat[statNo][i].m_positionLower] > 0)
            {
                // уменьшаем позицию, на которую GradeStat указывает на единицы которые прописаны
                m_currentStatPosition[m_fullDecreaseStat[statNo][i].m_positionLower] -= m_fullDecreaseStat[statNo][i].m_howMany;
            }
        }
    }

    m_upgradeHistory.push_back(m_currentStatPosition);
    return true;
}

//-----------------------------------------------------------------------------------------------------------------------

bool CharacteristicGun::upgradeStat(int statNo)
{
    if (statNo < 0 || statNo > m_currentStatPosition.size())
    {
        OUTPUT_LOG("CharacteristicGun UpgradeStat -> going beyond");
        return false;
    }
    return upStat(statNo);
}

//-----------------------------------------------------------------------------------------------------------------------

void CharacteristicGun::clear()
{
    m_fullAllStat.clear();
    m_fullDecreaseStat.clear();
    m_currentStatPosition.clear();
    m_maxStatPosition.clear();
    m_defaultStatPosition.clear();
    m_upgradeHistory.clear();
    m_empty = true;
}

//-----------------------------------------------------------------------------------------------------------------------

bool CharacteristicGun::isEmpty() const
{
    return m_empty;
}

//-----------------------------------------------------------------------------------------------------------------------

bool CharacteristicGun::returnDefaultPosition() 
{
    m_currentStatPosition = m_defaultStatPosition;

    return m_currentStatPosition == m_defaultStatPosition ? true : false;    
}

//-----------------------------------------------------------------------------------------------------------------------

vector<int>    CharacteristicGun::getDefaultPosition() const
{
    return CharacteristicGun::m_defaultStatPosition;
}

//-----------------------------------------------------------------------------------------------------------------------

vector<int>    CharacteristicGun::getMaxPositionCharacteristic() const
{
    return m_maxStatPosition;
}

//-----------------------------------------------------------------------------------------------------------------------

vector<double> CharacteristicGun::getFullCurrentVisualStat() const
{
    vector<double> result;
    for (int it = 0; it < m_currentStatPosition.size(); it++)
    {
        result.push_back(CharacteristicGun::getVisualPercentUpgradeCharacteristic(it));
    }
    return result;
}

//-----------------------------------------------------------------------------------------------------------------------

vector<int> CharacteristicGun::getCurrentPosition() const
{
    return CharacteristicGun::m_currentStatPosition;
}

//-----------------------------------------------------------------------------------------------------------------------

double CharacteristicGun::getValueCharacteristic(size_t stat) const
{
    return m_fullAllStat[stat][m_currentStatPosition[stat]].m_valueCharacteristic;
}

//-----------------------------------------------------------------------------------------------------------------------

double CharacteristicGun::getMaxStatVisualPercent(size_t stat)  const
{
    return m_fullAllStat[stat][m_maxStatPosition[stat]].m_visualPercentStat;
}

//-----------------------------------------------------------------------------------------------------------------------

double CharacteristicGun::getVisualPercentUpgradeCharacteristic(size_t stat) const
{
    return m_fullAllStat[stat][m_currentStatPosition[stat]].m_visualPercentStat;
}

//-----------------------------------------------------------------------------------------------------------------------

double CharacteristicGun::getChanceForNextStat(size_t stat) const
{
    const auto next = m_currentStatPosition[stat] + 1;

    if (next > m_maxStatPosition[stat])
    {
        OUTPUT_LOG("CharacteristicGun -> can't get a chance greater than the maximum");
        return 0.0;
    }

    if (next > m_fullAllStat[stat].size())
    {
        OUTPUT_LOG("CharacteristicGun -> can't get a chance, go beyond");
        return -1.0;
    }

    return m_fullAllStat[stat][next].m_currentChance;
}

//-----------------------------------------------------------------------------------------------------------------------

bool CharacteristicGun::stepBack()
{
    // если вектор пуст выходим
    if (m_upgradeHistory.empty()) { return false; }

    // иначе удаляем из истории текущий
    m_upgradeHistory.pop_back();
    
    if (!m_upgradeHistory.empty()) // устанавливаем предыдущий если не пустой
    {
        m_currentStatPosition = m_upgradeHistory[m_upgradeHistory.size() - 1];
    }    
    else // иначе дефолтные характеристики
    {
        CharacteristicGun::returnDefaultPosition();
    }
    
    return true;
}

//-----------------------------------------------------------------------------------------------------------------------

std::vector<int> CharacteristicGun::getDecreaseForCurrentStat(size_t stat) const
{
    std::vector<int> result(m_countOption, 0);

    const auto current = m_currentStatPosition[stat];
    const auto base = m_defaultStatPosition[stat];

    for (const auto& dec : m_fullDecreaseStat[stat])
    {
        if (current > base + dec.m_positionStartLower)
        {
            result[dec.m_positionLower] += dec.m_howMany;
        }
    }
    return result;
}

//-----------------------------------------------------------------------------------------------------------------------
