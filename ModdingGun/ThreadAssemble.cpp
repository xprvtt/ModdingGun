#include "ThreadAssemble.h"

mutex mtx;

int threadCount = USING_ONE_THREAD ? 1 : thread::hardware_concurrency();

vector<CountModifiers> allModifiers;
vector<unsigned int>   allRandomAttemptUsed;

//-------------------------------------------------------------------------------------------------------------------

int RunSingleAssembly(
    // -1 = debug
    int index, 
    vector<Method> methodMod
)
{
    if (methodMod.empty())
    {
        if (index == -1)
        {
            OUTPUT_LOG("RunSingleAssembly -> empty vector methodMod");
        }
        return -1;
    }


    vector<unsigned int> averageRandomTemp;
    CountModifiers currentUsedModifiers;

    double chanceUpgrade;


    // проходимся по параметрам используя метод
    for (int it = 0; it < methodMod.size(); it++)
    {
        chanceUpgrade = methodMod[it].chanceUpgrade;
        if (chanceUpgrade <= 0.f)
        {
            if (index == -1)
            {
                OUTPUT_LOG("RunSingleAssembly -> chanceUpgrade <= 0");
            }
            return -2;
        }

        double succsessRandom;

        while (true)
        {
            succsessRandom = getRandomDouble(100.0);
            averageRandomTemp.push_back(static_cast<unsigned int>(round(succsessRandom * 100))); /// 0 ~ 10'000 точность не важна

            currentUsedModifiers.m_allPrice += methodMod[it].Price.PriceTool + methodMod[it].Price.PriceKit + methodMod[it].Price.PriceSkill;

            if (index != -1)
            {
                currentUsedModifiers.m_countTool[methodMod[it].ModifiersThisChance.Tool]++;
                currentUsedModifiers.m_countKit[methodMod[it].ModifiersThisChance.Kit]++;
                currentUsedModifiers.m_countSkill[methodMod[it].ModifiersThisChance.Skill]++;
            }
            else
            {
                auto tool  = methodMod[it].ModifiersThisChance.Tool;
                auto kit   = methodMod[it].ModifiersThisChance.Kit;
                auto skill = methodMod[it].ModifiersThisChance.Skill;
                
                if (currentUsedModifiers.m_countTool.find(tool) != currentUsedModifiers.m_countTool.end())
                {
                    currentUsedModifiers.m_countTool[tool]++;
                }
                else
                {
                    if (index == -1)
                    {
                        OUTPUT_LOG("RunSingleAssembly -> Key Tool not found: " + GunStats::Modifiers::GetToolName(tool));
                    }
                    currentUsedModifiers.m_countTool[tool] = -101;
                    return -3;
                }
                
                if (currentUsedModifiers.m_countKit.find(kit) != currentUsedModifiers.m_countKit.end())
                {
                    currentUsedModifiers.m_countKit[kit]++;
                }
                else
                {
                    if (index == -1)
                    {
                        OUTPUT_LOG("RunSingleAssembly -> Key Kit not found: " + GunStats::Modifiers::GetKitName(kit));
                    }
                    currentUsedModifiers.m_countKit[kit] = -102;
                    return -4;
                }
                
                if (currentUsedModifiers.m_countSkill.find(skill) != currentUsedModifiers.m_countSkill.end())
                {
                    currentUsedModifiers.m_countSkill[skill]++;
                }
                else
                {
                    if (index == -1)
                    {
                        OUTPUT_LOG("RunSingleAssembly -> Key skill not found: " + GunStats::Modifiers::GetSkillName(skill));
                    }
                    currentUsedModifiers.m_countSkill[skill] = -103;
                    return -5;
                }

                if (tool == GunStats::Modifiers::ToolType::noTool && kit == GunStats::Modifiers::KitType::noKit)
                {
                    return -6;
                }
            }
            
            if (succsessRandom <= chanceUpgrade)
            {
                break;
            }
        }
    }

    add_results( averageRandomTemp, currentUsedModifiers);
    return 0;
}

//-------------------------------------------------------------------------------------------------------------------

void add_results(  const vector<unsigned int>& otherAverageRandomGen, CountModifiers currentModifiersCount )
{
    lock_guard<mutex> lock(mtx);

    allRandomAttemptUsed.insert(allRandomAttemptUsed.end(), otherAverageRandomGen.begin(), otherAverageRandomGen.end());
    allModifiers.push_back(currentModifiersCount);
}