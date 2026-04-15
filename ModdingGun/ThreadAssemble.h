#pragma once

#include "Core.h"
#include "GunStats.h"


/// <summary>
/// содержит колличество всех модификаторов
/// </summary>
struct CountModifiers
{
    map<GunStats::Modifiers::ToolType,  unsigned int > m_countTool;
    map<GunStats::Modifiers::KitType,   unsigned int > m_countKit;
    map<GunStats::Modifiers::SkillType, unsigned int > m_countSkill;

    unsigned long long m_allPrice;

    CountModifiers()
    {
        auto tool  = GunStats::Modifiers::ToolType::toolTypeBegin;
        auto kit   = GunStats::Modifiers::KitType::kitTypeBegin;
        auto skill = GunStats::Modifiers::SkillType::skillTypeBegin;

        m_allPrice = 0;

        for (tool++; tool < GunStats::Modifiers::ToolType::toolTypeEnd; tool++)
        {
            m_countTool[tool] = 0;
        }
        for (kit++; kit < GunStats::Modifiers::KitType::kitTypeEnd; kit++)
        {
            m_countKit[kit] = 0;
        }
        for (skill++; skill < GunStats::Modifiers::SkillType::skillTypeEnd; skill++)
        {
            m_countSkill[skill] = 0;
        }
    }


    bool operator==(const CountModifiers& other) const
    {
        auto tool = GunStats::Modifiers::ToolType::toolTypeBegin;
        for (tool++; tool < GunStats::Modifiers::ToolType::toolTypeEnd; tool++)
        {
            try
            {
                auto thisCountTool = this->m_countTool.at(tool);
                auto otherCountTool = other.m_countTool.at(tool);

                if (thisCountTool != otherCountTool)
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
                auto thisCountKit = this->m_countKit.at(kit);
                auto otherCountKit = other.m_countKit.at(kit);

                if (thisCountKit != otherCountKit)
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
                auto thisCountSkill  = this->m_countSkill.at(skill);
                auto otherCountSkill = other.m_countSkill.at(skill);

                if (thisCountSkill != otherCountSkill)
                {
                    return false;
                }                
            }
            catch(const std::out_of_range&)
            {
                OUTPUT_LOG("CountModifiers -> operator< -> Key Skill not found");
                exit(-10);
            }
        } 

        return this->m_allPrice == other.m_allPrice;
    };
            
    bool operator<(const CountModifiers& other) const 
    {
        unsigned countToolThis = 0;
        unsigned countToolOther = 0;

        auto tool  = GunStats::Modifiers::ToolType:: toolTypeBegin;
        for (tool++; tool < GunStats::Modifiers::ToolType::toolTypeEnd; tool++)
        {
            try
            {
                auto thisCT = this->m_countTool.at(tool);
                auto otherCT = other.m_countTool.at(tool);

                countToolThis += thisCT;
                countToolOther += otherCT;

            }
            catch(const std::out_of_range&)
            {
                OUTPUT_LOG("CountModifiers -> operator< -> Key Tool not found");
                exit(-11);
            }
        }

        unsigned countKitThis = 0;
        unsigned countKitOther = 0;

        auto kit = GunStats::Modifiers::KitType::kitTypeBegin;
        for (kit++; kit < GunStats::Modifiers::KitType::kitTypeEnd; kit++)
        {
            try
            {
                auto thisCK = this->m_countKit.at(kit);
                auto otherCK = other.m_countKit.at(kit);

                countKitThis += thisCK;
                countKitOther += otherCK;
                
            }
            catch (const std::out_of_range&)
            {
                OUTPUT_LOG("CountModifiers -> operator< -> Key Kit not found");
                exit(-11);
            }
        }

        unsigned countSkillThis = 0;
        unsigned countSkillOther = 0;

        auto skill = GunStats::Modifiers::SkillType::skillTypeBegin;
        for (skill++; skill < GunStats::Modifiers::SkillType::skillTypeEnd; skill++)
        {
            try
            {
                auto thisCS = this->m_countSkill.at(skill);
                auto otherCS = other.m_countSkill.at(skill);

                countSkillThis += thisCS;
                countSkillOther += otherCS;
            }
            catch (const std::out_of_range&)
            {
                OUTPUT_LOG("CountModifiers -> operator< -> Key Skill not found");
                exit(-11);
            }
        }
        return tie(countKitThis , countToolThis, countSkillThis, this->m_allPrice) < tie(countKitOther, countToolOther, countSkillOther, other.m_allPrice);
    }
};

extern mutex mtx;
extern int   threadCount;

/// <summary>
/// вектор всех сгенерированных попыток
/// </summary>
extern vector<unsigned int> allRandomAttemptUsed;

/// <summary>
/// <para> размер вектора = кол-ву сборок </para> 
/// <para> содержит CountModifiers -> все использованные модификаторы для получения указанного мода </para> 
/// <para> (то есть модификаторы потребующиеся для прохода по такому методу)</para> 
/// </summary>
extern vector<CountModifiers> allModifiers;

/// <summary>
/// проводим одну сборку мода, по методу
/// </summary>
/// <param name="it_index">номер сборки</param>
/// <param name="method_MOD">метод </param>
/// <returns></returns>
int RunSingleAssembly(int it_index, vector<Method> method_MOD);

/// <summary>
/// 
/// </summary>
/// <param name="other_Average_RANDOM_GEN"></param>
/// <param name="Current_modifiers_count"></param>
void add_results(const vector<unsigned int>& other_Average_RANDOM_GEN, CountModifiers Current_modifiers_count);
