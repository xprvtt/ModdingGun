#include "Core.h"

#include "CharacteristicGun.h"

#include "GunStats.h"

#include "Thread.h"



CharacteristicGun assembly(
    int it_index,
    CharacteristicGun CurrentCharacteristicGun,
    vector<int> method,
    vector<int> CharacteristicsInitial,
    vector<int> CharacteristicsModded,
    bool Initial_OFF,
    double TOOL,
    double KIT,
    double SKILL
);


double CalculateSuccessChance(double baseChance, double toolFactor, double KitFactor, double skillFactor)
{
    return baseChance * (1 + skillFactor) * (1 + toolFactor) * (1 + KitFactor);
}


/////--------////--------////--------////--------////--------////--------////--------////--------////--------////--------////--------////--------////--------////--------////--------////--------////--------////--------////--------
CharacteristicGun RunSingleAssembly(    
    int it_index,
    CharacteristicGun CurrentCharacteristicGun,
    vector<int> method,
    vector<int> CharacteristicsInitial,
    vector<int> CharacteristicsModded,
    bool InitialMod_ON_or_OFF,
    double TOOL,
    double KIT,
    double SKILL
)
{
    return assembly(
        it_index,
        CurrentCharacteristicGun,
        method,
        CharacteristicsInitial,
        CharacteristicsModded,
        InitialMod_ON_or_OFF,
        TOOL,
        KIT,
        SKILL
    );
}



/////--------////--------////--------////--------////--------////--------////--------////--------////--------////--------////--------////--------////--------////--------////--------////--------////--------////--------////--------
bool RunSingleAssembly(
    size_t it_index,
    CharacteristicGun CurrentCharacteristicGun,
    vector<int> method,
    vector<int> CharacteristicsInitial,
    vector<int> CharacteristicsModded,
    bool InitialMod_ON_or_OFF,
    double TOOL,
    double KIT,
    double SKILL
)
{
    assembly(
        it_index,
        CurrentCharacteristicGun,
        method,
        CharacteristicsInitial,
        CharacteristicsModded,
        InitialMod_ON_or_OFF,
        TOOL,
        KIT,
        SKILL
    );
    return true;
}

/////--------////--------////--------////--------////--------////--------////--------////--------////--------////--------////--------////--------////--------////--------////--------////--------////--------////--------////--------






CharacteristicGun assembly(
    int it_index,
    CharacteristicGun CurrentCharacteristicGun,
    vector<int> method,
    vector<int> CharacteristicsInitial,
    vector<int> CharacteristicsModded,
    bool InitialMod_ON_or_OFF,
    double TOOL,
    double KIT,
    double SKILL
)
{
    vector<float> Average10000_RANDOM_GEN_TEMP;

    // колво исп инструментов
    int Result_Tools_Used = 0;

    // проходимся по параметрам х7 используя метод
    for (int iteration_option = 0; iteration_option < method.size(); iteration_option++)
    {
        // цикл апгрейда до указанной статы

        auto option = method[iteration_option];

        // получаем стату указанного мода
        double ModdedVisualProcent;

        // GunStats::ACCURACY, GunStats::RATE_OF_FIRE, GunStats::PENETRATION, GunStats::KICKBACK, GunStats::SWAY, GunStats::MALFUNCTION_DIRT, GunStats::MALFUNCTION_CONDITION 

        if      (option == GunStats::ACCURACY)     { ModdedVisualProcent = GunStats::Accuracy_VStat[CharacteristicsModded[GunStats::ACCURACY]];     }
        else if (option == GunStats::RATE_OF_FIRE) { ModdedVisualProcent = GunStats::RateFire_VStat[CharacteristicsModded[GunStats::RATE_OF_FIRE]]; }
        else                                       { ModdedVisualProcent = GunStats::Other_VStat[CharacteristicsModded[option]];                    }


        // максимум попыток чтобы не зависнуть
        int MA = 200;
        for (int maximum_attempts = 0; maximum_attempts <= MA; maximum_attempts++)
        {

            // получаем текущий визуальный процент
            double CurrentVisualProcent = CurrentCharacteristicGun.GetVisualProcentStat(option);




            // обработка ошибки
            if (maximum_attempts == MA)
            {
                if (it_index == -1)
                {
                    OutputLog("превышено колличество попыток на улучшение одной из характеристики " + to_string(option));
                }
                // выход
                Result_Tools_Used = -23;
                add_results(Average10000_RANDOM_GEN_TEMP, Result_Tools_Used);
                return CurrentCharacteristicGun;
            }





            // пока текущий процент меньше указанного
            if (CurrentVisualProcent < ModdedVisualProcent)
            {



                //получаем шанс улучшения на след позицию
                double BaseChance = CurrentCharacteristicGun.GetChanceFor_NEXT_Stat(option);
                double chance = CalculateSuccessChance(BaseChance, TOOL, KIT, SKILL);






                // обработка ошибки
                if (chance <= 0.0)
                {
                    if (it_index == -1) { OutputLog("chance " + to_string(chance)); }
                    // выход
                    Result_Tools_Used = -7;
                    add_results(Average10000_RANDOM_GEN_TEMP, Result_Tools_Used);
                    return CurrentCharacteristicGun;
                }







                // цикл пока не прокнет
                while (true)
                {
                    // засчитываем поппытку
                    Result_Tools_Used++;

                    // получаем случайную "попытку"
                    double attempt = getRandomDouble(100.0);

                    Average10000_RANDOM_GEN_TEMP.push_back(static_cast<float>(attempt * 100));

                    if (attempt <= chance)
                    {
                        CurrentCharacteristicGun.UpgradeStat(option);
                        break;
                    }
                }




            }
            else if (CurrentVisualProcent == ModdedVisualProcent)
            {
                //OutputLog("характеристика " + to_string(method[iteration_option]) + " -> совпадает, продолжаем");
                maximum_attempts = MA+10;
            }
            else
            {
                if (it_index == -1)
                {
                    OutputLog("характеристика " + to_string(option) + " оказалась больше, выход");
                    OutputLog(to_string(CurrentVisualProcent) + " " + to_string(ModdedVisualProcent));
                }
                Result_Tools_Used = -3;
                add_results(Average10000_RANDOM_GEN_TEMP, Result_Tools_Used);
                return CurrentCharacteristicGun;
            }




        }
    }
    add_results(Average10000_RANDOM_GEN_TEMP, Result_Tools_Used);
    return CurrentCharacteristicGun;
}