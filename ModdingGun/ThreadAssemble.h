#pragma once

#include "Core.h"
#include "GunStats.h"


/// <summary>
/// содержит колличество всех модификаторов
/// </summary>
struct CountModifiers
{
    map<GunStats::Modifiers::ToolType, unsigned int > CountTool;
    map<GunStats::Modifiers::KitType, unsigned int > CountKit;
    map<GunStats::Modifiers::SkillType, unsigned int > CountSkill;



    CountModifiers()
    {
        auto tool  = GunStats::Modifiers::ToolType::ToolType_begin;
        auto kit   = GunStats::Modifiers::KitType::KitType_begin;
        auto skill = GunStats::Modifiers::SkillType::SkillType_begin;


        for (tool++; tool < GunStats::Modifiers::ToolType::ToolType_end; tool++)
        {
            this->CountTool[tool] = 0;
        }
        for (kit++; kit < GunStats::Modifiers::KitType::KitType_end; kit++)
        {
            this->CountKit[kit] = 0;
        }
        for (skill++; skill < GunStats::Modifiers::SkillType::SkillType_end; skill++)
        {
            this->CountSkill[skill] = 0;
        }
    }


    bool operator==(const CountModifiers& other) const
    {


        auto tool = GunStats::Modifiers::ToolType::ToolType_begin;
        for (tool++; tool < GunStats::Modifiers::ToolType::ToolType_end; tool++)
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
                OutputLog("CountModifiers -> operator== -> Key Tool not found");
                exit(-10);
            }
        }


        auto kit = GunStats::Modifiers::KitType::KitType_begin;
        for (kit++; kit < GunStats::Modifiers::KitType::KitType_end; kit++)
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
                OutputLog("CountModifiers -> operator< -> Key Kit not found");
                exit(-10);

            }
        }


        auto skill = GunStats::Modifiers::SkillType::SkillType_begin;
        for (skill++; skill < GunStats::Modifiers::SkillType::SkillType_end; skill++)
        {
            try
            {
                auto This  = this->CountSkill.at(skill);
                auto Other = other.CountSkill.at(skill);

                if (This != Other)
                {
                    return false;
                }                
            }
            catch(const std::out_of_range&)
            {
                OutputLog("CountModifiers -> operator< -> Key Skill not found");
                exit(-10);
            }
        }



        return true;
    };



    
    bool operator<(const CountModifiers& other) const 
    {
        // приоритет 1
        unsigned CountToolThis = 0;
        unsigned CountToolOther = 0;

        auto tool  = GunStats::Modifiers::ToolType:: ToolType_begin;
        for (tool++; tool < GunStats::Modifiers::ToolType::ToolType_end; tool++)
        {
            try
            {
                auto This = this->CountTool.at(tool);
                auto Other = other.CountTool.at(tool);

                CountToolThis += This;
                CountToolOther += Other;

            }
            catch(const std::out_of_range&)
            {
                OutputLog("CountModifiers -> operator< -> Key Tool not found");
                exit(-11);
            }
        }


        // приоритет 2
        unsigned CountKitThis = 0;
        unsigned CountKitOther = 0;

        auto kit = GunStats::Modifiers::KitType::KitType_begin;
        for (kit++; kit < GunStats::Modifiers::KitType::KitType_end; kit++)
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
                OutputLog("CountModifiers -> operator< -> Key Kit not found");
                exit(-11);
            }
        }

        /*     


        // приоритет 3 (не подситываем?)
        unsigned CountSkillThis = 0;
        unsigned CountSkillOther = 0;

        auto skill = GunStats::Modifiers::SkillType::SkillType_begin;
        for (skill++; skill < GunStats::Modifiers::SkillType::SkillType_end; skill++)
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
                OutputLog("CountModifiers -> operator< -> Key Skill not found");
                exit(-11);
            }
        }

        */


        return ( tie(CountToolThis, CountKitThis) < tie(CountToolOther, CountKitOther) );
    }


};



extern mutex mtx;
extern int   threadCount;





/// <summary>
/// вектор всех сгенерированных попыток
/// </summary>
extern vector<unsigned int> All_Random_Attempt_Used;

/// <summary>
/// <para> размер вектора = кол-ву сборок </para> 
/// <para> содержит CountModifiers -> все использованные модификаторы для получения указанного мода </para> 
/// <para> (то есть модификаторы потребующиеся для прохода по такому методу)</para> 
/// </summary>
extern vector<CountModifiers> ALL_modifiers;















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

