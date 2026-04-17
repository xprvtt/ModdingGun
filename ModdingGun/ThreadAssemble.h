#pragma once

#include "Core.h"
#include "GunStats.h"

//-----------------------------------------------------------------------------------------------------------------------

extern mutex mtx;

extern int threadCount;

struct CountModifiers;

/// <summary>
/// вектор всех сгенерированных попыток
/// </summary>
extern vector<unsigned int> resultRandomAttemptUsed;

/// <summary>
/// <para> размер вектора = кол-ву сборок </para> 
/// <para> содержит CountModifiers -> все использованные модификаторы для получения указанного мода </para> 
/// <para> (то есть модификаторы потребующиеся для прохода по такому методу)</para> 
/// </summary>
extern vector<CountModifiers> resultMainModifiers;

//-----------------------------------------------------------------------------------------------------------------------

/// <summary>
/// проводим одну сборку мода, по методу
/// </summary>
/// <param name="currentNoThread">номер сборки</param>
/// <param name="method_MOD">метод </param>
/// <returns>success или номер ошибки</returns>
int RunSingleAssembly(int currentNoThread, const vector<Method>& method_MOD);

/// <summary>
/// 
/// </summary>
/// <param name="other_Average_RANDOM_GEN"></param>
/// <param name="Current_modifiers_count"></param>
void addResultsThread(const vector<unsigned int>& otherAverageRandomGen, CountModifiers currentModifiersCount);

//-----------------------------------------------------------------------------------------------------------------------

/// <summary>
/// содержит колличество всех модификаторов
/// </summary>
struct CountModifiers
{
    map<GunStats::Modifiers::ToolType, unsigned int > m_countTool;
    map<GunStats::Modifiers::KitType, unsigned int > m_countKit;
    map<GunStats::Modifiers::SkillType, unsigned int > m_countSkill;

    unsigned long long m_allPrice;

    CountModifiers()
    {
        auto tool = GunStats::Modifiers::ToolType::toolTypeBegin;
        auto kit = GunStats::Modifiers::KitType::kitTypeBegin;
        auto skill = GunStats::Modifiers::SkillType::skillTypeBegin;

        this->m_allPrice = 0;

        for (tool++; tool < GunStats::Modifiers::ToolType::ToolTypeEnd; tool++)
        {
            this->m_countTool[tool] = 0;
        }
        for (kit++; kit < GunStats::Modifiers::KitType::kitTypeEnd; kit++)
        {
            this->m_countKit[kit] = 0;
        }
        for (skill++; skill < GunStats::Modifiers::SkillType::skillTypeEnd; skill++)
        {
            this->m_countSkill[skill] = 0;
        }
    }

    bool operator==(const CountModifiers& other) const
    {
        auto tool = GunStats::Modifiers::ToolType::toolTypeBegin;
        for (tool++; tool < GunStats::Modifiers::ToolType::ToolTypeEnd; tool++)
        {
            try
            {
                auto This = this->m_countTool.at(tool);
                auto Other = other.m_countTool.at(tool);

                if (This != Other)
                {
                    return false;
                }
            }
            catch (const std::out_of_range&)
            {
                OUTPUT_LOG("CountModifiers -> operator== -> Key Tool not found");
                exit(-10);
            }
        }

        auto kit = GunStats::Modifiers::KitType::kitTypeBegin;
        for (kit++; kit < GunStats::Modifiers::KitType::kitTypeEnd; kit++)
        {
            try
            {
                auto This = this->m_countKit.at(kit);
                auto Other = other.m_countKit.at(kit);

                if (This != Other)
                {
                    return false;
                }
            }
            catch (const std::out_of_range&)
            {
                OUTPUT_LOG("CountModifiers -> operator< -> Key Kit not found");
                exit(-10);
            }
        }

        auto skill = GunStats::Modifiers::SkillType::skillTypeBegin;
        for (skill++; skill < GunStats::Modifiers::SkillType::skillTypeEnd; skill++)
        {
            try
            {
                auto This = this->m_countSkill.at(skill);
                auto Other = other.m_countSkill.at(skill);

                if (This != Other)
                {
                    return false;
                }
            }
            catch (const std::out_of_range&)
            {
                OUTPUT_LOG("CountModifiers -> operator< -> Key Skill not found");
                exit(-10);
            }
        }
        return this->m_allPrice == other.m_allPrice;
    };

    bool operator<(const CountModifiers& other) const
    {
        unsigned CountToolThis = 0;
        unsigned CountToolOther = 0;

        auto tool = GunStats::Modifiers::ToolType::toolTypeBegin;
        for (tool++; tool < GunStats::Modifiers::ToolType::ToolTypeEnd; tool++)
        {
            try
            {
                auto This = this->m_countTool.at(tool);
                auto Other = other.m_countTool.at(tool);

                CountToolThis += This;
                CountToolOther += Other;

            }
            catch (const std::out_of_range&)
            {
                OUTPUT_LOG("CountModifiers -> operator< -> Key Tool not found");
                exit(-11);
            }
        }

        unsigned CountKitThis = 0;
        unsigned CountKitOther = 0;

        auto kit = GunStats::Modifiers::KitType::kitTypeBegin;
        for (kit++; kit < GunStats::Modifiers::KitType::kitTypeEnd; kit++)
        {
            try
            {
                auto This = this->m_countKit.at(kit);
                auto Other = other.m_countKit.at(kit);

                CountKitThis += This;
                CountKitOther += Other;

            }
            catch (const std::out_of_range&)
            {
                OUTPUT_LOG("CountModifiers -> operator< -> Key Kit not found");
                exit(-11);
            }
        }
        
        unsigned CountSkillThis = 0;
        unsigned CountSkillOther = 0;

        auto skill = GunStats::Modifiers::SkillType::skillTypeBegin;
        for (skill++; skill < GunStats::Modifiers::SkillType::skillTypeEnd; skill++)
        {
            try
            {
                auto This = this->m_countSkill.at(skill);
                auto Other = other.m_countSkill.at(skill);

                CountSkillThis += This;
                CountSkillOther += Other;
            }
            catch (const std::out_of_range&)
            {
                OUTPUT_LOG("CountModifiers -> operator< -> Key Skill not found");
                exit(-11);
            }
        }
        return tie(CountKitThis, CountToolThis, CountSkillThis, this->m_allPrice) < tie(CountKitOther, CountToolOther, CountSkillOther, other.m_allPrice);
    }
};

//-----------------------------------------------------------------------------------------------------------------------
