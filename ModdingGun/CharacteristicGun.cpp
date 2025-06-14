#include "CharacteristicGun.h"


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
                //получаем шанс по позиции
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
            // 1 темп
            // 
            // 
            // 2 отдача
            // 3 качание
            // 
            // 
            // 4 пробитие
            // 
            // 
            // 5 сост
            // 6 грязь
            //
            CurrentStatPosition.push_back(data.CurrentPositionInt);

            CharacteristicGun::Ful_Stat.push_back(result);
        }

        else if (lineCount < 14)
        {
            ParsedData14 Line14 = parseChanceLine14(line);

            vector<Grade> result;

            for (int it = 0; it < Line14.triples.size(); it++)
            {
                Grade GradeTemp = { get<0>(Line14.triples[it]), get<1>(Line14.triples[it]), get<2>(Line14.triples[it]) };
                result.push_back(GradeTemp);
            }
            GradeStat.push_back(result);
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

void CharacteristicGun::clear()
{
    Ful_Stat.clear();

    GradeStat.clear();

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

    this->Ful_Stat = other.Ful_Stat;

    this->GradeStat = other.GradeStat;

    this->CurrentStatPosition = other.CurrentStatPosition;

    this->MaxStatPosition = other.MaxStatPosition;

    this->InitialStatPosition = other.InitialStatPosition;

    this->Empty = other.Empty;

}




double CharacteristicGun::GetMaxStatVisualProcent(unsigned int stat)
{
    return Ful_Stat[stat][MaxStatPosition[stat]].VisualProcentStat;
}

double CharacteristicGun::GetVisualProcentStat(int stat)
{
    return Ful_Stat[stat][CurrentStatPosition[stat]].VisualProcentStat;
}


double CharacteristicGun::GetChanceFor_NEXT_Stat(unsigned int stat)
{
    if (CurrentStatPosition[stat] + 1 > MaxStatPosition[stat])
    {
        OutputLog("class CharacteristicGun нельзая получить шанс, больше максимального");
        return 0.0;
    }
    if (CurrentStatPosition[stat] + 1 > Ful_Stat[stat].size())
    {
        OutputLog("class CharacteristicGun нельзая получить шанс, выход за пределы");
        return -1.0;
    }

    double result;

    unsigned int v2 = CurrentStatPosition[stat] + 1;


    result = Ful_Stat[stat][v2].CurrentChance;
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
    for (int i = 0; i < GradeStat[stat_No].size(); i++)
    {
        // если текущая стата больше началу уменьшения одного из GradeStat
        if (CurrentStatPosition[stat_No] > InitialStatPosition[stat_No] + GradeStat[stat_No][i].PositionStartLower)
        {
            // нельзя уменьшить позицию ниже нуля
            if (CurrentStatPosition[GradeStat[stat_No][i].PositionLower] > 0)
            {
                // уменьшаем позицию, на которую GradeStat указывает на единицы которые прописаны
                CurrentStatPosition[GradeStat[stat_No][i].PositionLower] -= GradeStat[stat_No][i].HowMany;
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
