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
    map<GunStats::Modifiers::ToolType, unsigned int > CountTool;
    map<GunStats::Modifiers::KitType, unsigned int > CountKit;
    map<GunStats::Modifiers::SkillType, unsigned int > CountSkill;

    unsigned long long AllPrice;

    CountModifiers()
    {
        auto tool = GunStats::Modifiers::ToolType::toolTypeBegin;
        auto kit = GunStats::Modifiers::KitType::kitTypeBegin;
        auto skill = GunStats::Modifiers::SkillType::skillTypeBegin;

        this->AllPrice = 0;

        for (tool++; tool < GunStats::Modifiers::ToolType::ToolTypeEnd; tool++)
        {
            this->CountTool[tool] = 0;
        }
        for (kit++; kit < GunStats::Modifiers::KitType::kitTypeEnd; kit++)
        {
            this->CountKit[kit] = 0;
        }
        for (skill++; skill < GunStats::Modifiers::SkillType::skillTypeEnd; skill++)
        {
            this->CountSkill[skill] = 0;
        }
    }

    bool operator==(const CountModifiers& other) const
    {
        auto tool = GunStats::Modifiers::ToolType::toolTypeBegin;
        for (tool++; tool < GunStats::Modifiers::ToolType::ToolTypeEnd; tool++)
        {
            try
            {
                auto This = this->CountTool.at(tool);
                auto Other = other.CountTool.at(tool);

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
                auto This = this->CountKit.at(kit);
                auto Other = other.CountKit.at(kit);

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
                auto This = this->CountSkill.at(skill);
                auto Other = other.CountSkill.at(skill);

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
        return this->AllPrice == other.AllPrice;
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
                auto This = this->CountTool.at(tool);
                auto Other = other.CountTool.at(tool);

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
                auto This = this->CountKit.at(kit);
                auto Other = other.CountKit.at(kit);

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
                auto This = this->CountSkill.at(skill);
                auto Other = other.CountSkill.at(skill);

                CountSkillThis += This;
                CountSkillOther += Other;
            }
            catch (const std::out_of_range&)
            {
                OUTPUT_LOG("CountModifiers -> operator< -> Key Skill not found");
                exit(-11);
            }
        }
        return tie(CountKitThis, CountToolThis, CountSkillThis, this->AllPrice) < tie(CountKitOther, CountToolOther, CountSkillOther, other.AllPrice);
    }
};

//-----------------------------------------------------------------------------------------------------------------------
