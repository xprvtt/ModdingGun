#pragma once

#include "Core.h"
#include "GunStats.h"

<<<<<<< HEAD
//-----------------------------------------------------------------------------------------------------------------------

extern mutex mtx;

extern int threadCount;

struct CountModifiers;
=======
struct CountModifiers;

extern mutex mtx;
extern int   threadCount;
>>>>>>> main

/// <summary>
/// вектор всех сгенерированных попыток
/// </summary>
<<<<<<< HEAD
extern vector<unsigned int> resultRandomAttemptUsed;
=======
extern vector<unsigned int> allRandomAttemptUsed;
>>>>>>> main

/// <summary>
/// <para> размер вектора = кол-ву сборок </para> 
/// <para> содержит CountModifiers -> все использованные модификаторы для получения указанного мода </para> 
/// <para> (то есть модификаторы потребующиеся для прохода по такому методу)</para> 
/// </summary>
<<<<<<< HEAD
extern vector<CountModifiers> resultMainModifiers;

//-----------------------------------------------------------------------------------------------------------------------
=======
extern vector<CountModifiers> allModifiers;

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
>>>>>>> main

/// <summary>
/// проводим одну сборку мода, по методу
/// </summary>
/// <param name="currentNoThread">номер сборки</param>
/// <param name="method_MOD">метод </param>
<<<<<<< HEAD
/// <returns>success или номер ошибки</returns>
int RunSingleAssembly(int currentNoThread, const vector<Method>& method_MOD);
=======
/// <returns></returns>
int RunSingleAssembly(int it_index, vector<Method> method_MOD);
>>>>>>> main

/// <summary>
/// 
/// </summary>
/// <param name="other_Average_RANDOM_GEN"></param>
/// <param name="Current_modifiers_count"></param>
<<<<<<< HEAD
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
=======
void add_results(const vector<unsigned int>& other_Average_RANDOM_GEN, CountModifiers Current_modifiers_count);
>>>>>>> main
