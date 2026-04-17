#include "ThreadAssemble.h"

//-----------------------------------------------------------------------------------------------------------------------

mutex mtx;

int threadCount = USING_ONE_THREAD ? 1 : thread::hardware_concurrency();

vector<CountModifiers> resultMainModifiers;

vector<unsigned int>   resultRandomAttemptUsed;

//-----------------------------------------------------------------------------------------------------------------------

int RunSingleAssembly(int currentNoThread, const std::vector<Method>& methodMod)
{
    if (methodMod.empty())
    {
        if (currentNoThread == -1)
            OUTPUT_LOG("RunSingleAssembly -> Empty vector methodMod");
        return -1;
    }

    std::vector<unsigned int> averageRandomTemp;
    CountModifiers currentUsedModifiers;

    for (const auto& method : methodMod)
    {
        double chanceUpgrade = method.m_chanceUpgrade;

        if (chanceUpgrade <= 0.0)
        {
            if (currentNoThread == -1)
                OUTPUT_LOG("RunSingleAssembly -> chanceUpgrade <= 0");
            return -2;
        }

        double successRandom;

        while (true)
        {
            successRandom = getRandomDouble(100.0);
            averageRandomTemp.push_back(
                static_cast<unsigned int>(std::round(successRandom * 100))
            );

            currentUsedModifiers.m_allPrice +=
                method.m_price.m_priceTool +
                method.m_price.m_priceKit +
                method.m_price.m_priceSkill;

            auto tool = method.m_modifiersThisChance.m_tool;
            auto kit = method.m_modifiersThisChance.m_kit;
            auto skill = method.m_modifiersThisChance.m_skill;

            if (currentNoThread != -1)
            {
                currentUsedModifiers.m_countTool[tool]++;
                currentUsedModifiers.m_countKit[kit]++;
                currentUsedModifiers.m_countSkill[skill]++;
            }
            else
            {
                // проверки существования ключей убрал бы полностью (см. ниже)
                currentUsedModifiers.m_countTool[tool]++;
                currentUsedModifiers.m_countKit[kit]++;
                currentUsedModifiers.m_countSkill[skill]++;
            }

            if (tool == GunStats::Modifiers::ToolType::noTool &&
                kit == GunStats::Modifiers::KitType::noKit)
            {
                return -6;
            }

            if (successRandom <= chanceUpgrade)
                break;
        }
    }

    addResultsThread(averageRandomTemp, currentUsedModifiers);
    return 0;
}

//-----------------------------------------------------------------------------------------------------------------------

void addResultsThread(const vector<unsigned int>& otherAverageRandomGen, CountModifiers currentModifiersCount)
{
	lock_guard<mutex> lock(mtx);

	resultRandomAttemptUsed.insert(resultRandomAttemptUsed.end(), otherAverageRandomGen.begin(), otherAverageRandomGen.end());
	resultMainModifiers.push_back(currentModifiersCount);
}

//-----------------------------------------------------------------------------------------------------------------------
