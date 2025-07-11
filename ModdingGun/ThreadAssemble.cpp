#include "ThreadAssemble.h"

mutex mtx;


int threadCount = OneThread ? 1 : thread::hardware_concurrency();

vector<CountModifiers> ALL_modifiers;
vector<unsigned int>   All_Random_Attempt_Used;


int RunSingleAssembly(
    // -1 = debug
    int it_index, 
    vector<Method> method_MOD
)
{
    if (method_MOD.empty())
    {
        if (it_index == -1)
        {
            OutputLog("RunSingleAssembly -> Empty vector method_MOD");
        }
        return -1;
    }


    vector<unsigned int> Average_random_temp;
    CountModifiers CurrentUsedModifiers;

    double ChanceUpgrade;


    // проходимся по параметрам используя метод
    for (int iteration = 0; iteration < method_MOD.size(); iteration++)
    {
        ChanceUpgrade = method_MOD[iteration].ChanceUpgrade;
        if (ChanceUpgrade <= 0.0)
        {
            if (it_index == -1)
            {
                OutputLog("RunSingleAssembly -> ChanceUpgrade <= 0");
            }
            return -2;
        }

        double SuccsessRandom;


        while (true)
        {
            SuccsessRandom = GetRandomDouble(100.0);
            Average_random_temp.push_back(static_cast<unsigned int>(round(SuccsessRandom * 100))); /// 0 ~ 10'000 точность не важна


            CurrentUsedModifiers.AllPrice += method_MOD[iteration].Price.PriceTool + method_MOD[iteration].Price.PriceKit + method_MOD[iteration].Price.PriceSkill;


            if (it_index != -1)
            {
                CurrentUsedModifiers.CountTool[method_MOD[iteration].ModifiersThisChance.Tool]++;
                CurrentUsedModifiers.CountKit[method_MOD[iteration].ModifiersThisChance.Kit]++;
                CurrentUsedModifiers.CountSkill[method_MOD[iteration].ModifiersThisChance.Skill]++;
            }
            else
            {
                auto tool  = method_MOD[iteration].ModifiersThisChance.Tool;
                auto kit   = method_MOD[iteration].ModifiersThisChance.Kit;
                auto Skill = method_MOD[iteration].ModifiersThisChance.Skill;


                ///////////////////////////////////////////////////
                if (CurrentUsedModifiers.CountTool.find(tool) != CurrentUsedModifiers.CountTool.end())
                {
                    CurrentUsedModifiers.CountTool[tool]++;
                }
                else
                {
                    if (it_index == -1)
                    {
                        OutputLog("RunSingleAssembly -> Key Tool not found: " + GunStats::Modifiers::GetToolName(tool));
                    }
                    CurrentUsedModifiers.CountTool[tool] = -101;
                    return -3;
                }
                ///////////////////////////////////////////////////





                ///////////////////////////////////////////////////
                if (CurrentUsedModifiers.CountKit.find(kit) != CurrentUsedModifiers.CountKit.end())
                {
                    CurrentUsedModifiers.CountKit[kit]++;
                }
                else
                {
                    if (it_index == -1)
                    {
                        OutputLog("RunSingleAssembly -> Key Kit not found: " + GunStats::Modifiers::GetKitName(kit));
                    }
                    CurrentUsedModifiers.CountKit[kit] = -102;
                    return -4;
                }
                ///////////////////////////////////////////////////





                ///////////////////////////////////////////////////
                if (CurrentUsedModifiers.CountSkill.find(Skill) != CurrentUsedModifiers.CountSkill.end())
                {
                    CurrentUsedModifiers.CountSkill[Skill]++;
                }
                else
                {
                    if (it_index == -1)
                    {
                        OutputLog("RunSingleAssembly -> Key Skill not found: " + GunStats::Modifiers::GetSkillName(Skill));
                    }
                    CurrentUsedModifiers.CountSkill[Skill] = -103;
                    return -5;
                }
                ///////////////////////////////////////////////////


                if (tool == GunStats::Modifiers::ToolType::NO_TOOL && kit == GunStats::Modifiers::KitType::NO_KIT)
                {
                    return -6;
                }
            }






            ///////////////////////////////////////////////////
            if (SuccsessRandom <= ChanceUpgrade)
            {
                break;
            }
            ///////////////////////////////////////////////////

        }
    }


    add_results( Average_random_temp, CurrentUsedModifiers);
    return 0;
}


void add_results(  const vector<unsigned int>& other_Average_RANDOM_GEN, CountModifiers Current_modifiers_count )
{
    lock_guard<mutex> lock(mtx);

    All_Random_Attempt_Used.insert(All_Random_Attempt_Used.end(), other_Average_RANDOM_GEN.begin(), other_Average_RANDOM_GEN.end());
    ALL_modifiers.push_back(Current_modifiers_count);
}