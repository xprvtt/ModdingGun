#pragma once
#include "Core.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// общие параметры 
namespace GunStats 
{
    struct Modifiers 
    {

        //////////////////////////////////
        enum ToolType
        {
            ToolType_begin,

            NO_TOOL,
            OLD_TOOL,
            NORMAL_TOOL,
            IMPROVED_TOOL,

            ToolType_end            
        };

        //////////////////////////////////
        enum KitType
        {
            KitType_begin,

            NO_KIT,
            REPAIR_KIT,
            DETAIL_KIT,

            KitType_end

        };
        //////////////////////////////////

        enum SkillType
        {
            SkillType_begin,

            PLAYER_NO_SKILL,

            PLAYER_MASTER_1,
            PLAYER_MASTER_2,
            PLAYER_MASTER_3,
            PLAYER_MASTER_4,
            PLAYER_MASTER_5,

            NPC_MASTER_2,
            NPC_MASTER_3,
            NPC_MASTER_4,
            NPC_MASTER_5,

            SkillType_end
        };
        //////////////////////////////////











        //////////////////////////////////

        static inline double GetToolModifier(ToolType tool)
        {
            switch (tool)
            {
            case ToolType::NO_TOOL:       return 0.0;
            case ToolType::OLD_TOOL:      return 0.2;
            case ToolType::NORMAL_TOOL:   return 0.5;
            case ToolType::IMPROVED_TOOL: return 0.8;
            }
            return -1.0;
        }

        static inline string GetToolName(ToolType tool)
        {
            switch (tool)
            {
            case ToolType::NO_TOOL:       return "without_tool";
            case ToolType::OLD_TOOL:      return "Old_tool";
            case ToolType::NORMAL_TOOL:   return "Common_tool";
            case ToolType::IMPROVED_TOOL: return "Improved_tool";
            }
            return "No_Name_Tool";
        }

        //////////////////////////////////

        static inline  double GetKitModifier(KitType kit)
        {
            switch (kit)
            {
            case KitType::NO_KIT:      return 0.0;
            case KitType::REPAIR_KIT:  return 1.0;
            case KitType::DETAIL_KIT:  return 4.0;
            }
            return -1.0;
        }
        static inline  string GetKitName(KitType kit)
        {
            switch (kit)
            {
            case KitType::NO_KIT:      return "Without_set_parts";
            case KitType::REPAIR_KIT:  return "Repair_kit";
            case KitType::DETAIL_KIT:  return "Detail";
            }
            return "No_Name_Kit";
        }
        //////////////////////////////////
        static inline  double GetSkillModifier(SkillType skill)
        {
            switch (skill) 
            {
            case SkillType::PLAYER_NO_SKILL:  return 0.0;
            
            case SkillType::PLAYER_MASTER_1:  return 0.2;
            case SkillType::PLAYER_MASTER_2:  return 0.4;
            case SkillType::PLAYER_MASTER_3:  return 0.6;
            case SkillType::PLAYER_MASTER_4:  return 0.8;
            case SkillType::PLAYER_MASTER_5:  return 1.0;
            
            case SkillType::NPC_MASTER_2:     return 0.5;
            case SkillType::NPC_MASTER_3:     return 1.0;
            case SkillType::NPC_MASTER_4:     return 1.5;
            case SkillType::NPC_MASTER_5:     return 2.0;
            }
            return -1.0;
        }
        static inline string GetSkillName(SkillType skill)
        {
            switch (skill)
            {
            case SkillType::PLAYER_NO_SKILL:  return "without_skill";

            case SkillType::PLAYER_MASTER_1:  return "Player_master_1";
            case SkillType::PLAYER_MASTER_2:  return "Player_master_2";
            case SkillType::PLAYER_MASTER_3:  return "Player_master_3";
            case SkillType::PLAYER_MASTER_4:  return "Player_master_4";
            case SkillType::PLAYER_MASTER_5:  return "Player_master_5";

            case SkillType::NPC_MASTER_2:     return "NPC_master_2";
            case SkillType::NPC_MASTER_3:     return "NPC_master_3";
            case SkillType::NPC_MASTER_4:     return "NPC_master_4";
            case SkillType::NPC_MASTER_5:     return "NPC_master_5";
            }
            return "No_Name_Skill";
        }
        //////////////////////////////////



    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    
    inline Modifiers::ToolType  operator++(Modifiers::ToolType& Other, int)
    {
        if (Other < Modifiers::ToolType::ToolType_end)
        {
            Other = static_cast<Modifiers::ToolType>(static_cast<int>(Other) + 1);
        }
        return Other;
    }
    inline Modifiers::KitType   operator++(Modifiers::KitType& Other, int)
    {
        if (Other < Modifiers::KitType::KitType_end)
        {
            Other = static_cast<Modifiers::KitType>(static_cast<int>(Other) + 1);
        }
        return Other;
    }
    inline Modifiers::SkillType operator++(Modifiers::SkillType& Other, int)
    {
        if (Other < Modifiers::SkillType::SkillType_end)
        {
            Other = static_cast<Modifiers::SkillType>(static_cast<int>(Other) + 1);
        }
        return Other;
    }



    /////////////////////////////////////////////////////////////////////////////////////////////////////////


    inline  constexpr size_t COUNT_CHARACTERISTIC = 7;

    /// <summary>
    /// <para> Колличество единиц для характеристики | обычный index</para>
    /// <para> 0 - кол-во юнитов кучности    </para>
    /// <para> 1 - кол-во юнитов темпа огня  </para>
    /// <para> 2 - кол-во юнитов пробития    </para>
    /// <para> 3 - кол-во юнитов отдачи      </para>
    /// <para> 4 - кол-во юнитов качания     </para>
    /// <para> 5 - кол-во юнитов отказ сост  </para> 
    /// <para> 6 - кол-во юнитов отказ грязи </para>
    /// </summary>
    inline constexpr array<int, COUNT_CHARACTERISTIC> GET_COUNT_UNITS_FOR_CHARACTERISTIC =
    {
        60, 
        48, 
        40, 

        40, 
        40, 
        40, 
        40
    };


    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    /// <summary>
    /// кучность
    /// </summary>
    inline  constexpr int INDEX_ACCURACY = 0;

    /// <summary>
    /// темп стрельбы
    /// </summary>
    inline  constexpr int INDEX_RATE_OF_FIRE = 1;


    /// <summary>
    /// отдача оружия
    /// </summary>
    inline  constexpr int INDEX_KICKBACK = 2;

    /// <summary>
    /// качание оружия
    /// </summary>
    inline  constexpr int INDEX_SWAY = 3;

    /// <summary>
    /// пробитие
    /// </summary>
    inline  constexpr int INDEX_PENETRATION = 4;

    /// <summary>
    /// отказ из-за состояния оружия
    /// </summary>
    inline  constexpr int INDEX_MALFUNCTION_CONDITION = 5;

    /// <summary>
    /// отказ из-за грязи
    /// </summary>
    inline  constexpr int INDEX_MALFUNCTION_DIRT = 6;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////






    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    /// <summary>
    /// получаем название характеристики по индексу GunStats
    /// </summary>
    inline constexpr array<string_view, COUNT_CHARACTERISTIC> MapCharacteristicName_GunStat
    {
        "ACCURACY",
        "RATE_OF_FIRE",
        "KICKBACK",
        "SWAY",
        "PENETRATION",
        "MALFUNCTION_CONDITION",
        "MALFUNCTION_DIRT"
    };
    
    /// <summary>
    /// получаем название характеристики по индексу 
    /// </summary>
    inline constexpr array<string_view, COUNT_CHARACTERISTIC> MapCharacteristicName_Index
    {
        "ACCURACY",
        "RATE_OF_FIRE",
        "PENETRATION",
        "KICKBACK",
        "SWAY",
        "MALFUNCTION_CONDITION",
        "MALFUNCTION_DIRT"
    };
    
    /// <summary>
    /// Перевод из index в index_GunStat
    /// </summary>
    inline constexpr array<int, COUNT_CHARACTERISTIC> Trans_INDEX_to_INDEXGUNSTAT
    {
        GunStats::INDEX_ACCURACY,               // 0
        GunStats::INDEX_RATE_OF_FIRE,           // 1
        GunStats::INDEX_PENETRATION,            // 2
        GunStats::INDEX_KICKBACK,               // 3
        GunStats::INDEX_SWAY,                   // 4
        GunStats::INDEX_MALFUNCTION_CONDITION,  // 5
        GunStats::INDEX_MALFUNCTION_DIRT        // 6
    };
    
    /// <summary>
    /// Перевод из  index_GunStat index
    /// </summary>
    inline constexpr array<int, COUNT_CHARACTERISTIC> Trans_INDEXGUNSTAT_to_INDEX
    {
        0, // 0 INDEX_ACCURACY
        1, // 1 INDEX_RATE_OF_FIRE
        3, // 2 INDEX_KICKBACK
        4, // 3 INDEX_SWAY
        2, // 4 INDEX_PENETRATION
        5, // 5 INDEX_MALFUNCTION_CONDITION
        6  // 6 INDEX_MALFUNCTION_DIRT 
    };


    /////////////////////////////////////////////////////////////////////////////////////////////////////////

}






/////////////////////////////////////////////////////////////////////////////////////////////////////////


/// <summary>
/// модификаторы
/// </summary>
struct Select_Modifiers
{
    /// <summary>
    /// инструмент
    /// </summary>
    GunStats::Modifiers::ToolType Tool;
    /// <summary>
    /// набор
    /// </summary>
    GunStats::Modifiers::KitType Kit;
    /// <summary>
    /// навык
    /// </summary>
    GunStats::Modifiers::SkillType Skill;

    bool operator==(const Select_Modifiers& Other)
    {
        return
        {
            this->Tool == Other.Tool &&
            this->Kit == Other.Kit &&
            this->Skill == Other.Skill
        };

    };
};



/// содержит 
/// int Parametr - параметр который был улучшен
/// double Chance - шанс улучшения этго параметра
/// Select_Modifiers ModifiersForChance - модификаторы, которыми получился шанс
struct Method
{
    /// <summary>
    /// параметр который был улучшен (INDEXGUNSTAT)
    /// </summary>
    int Characteristic_Gunstat;
    /// <summary>
    /// шанс улучшения для 4этого параметра
    /// </summary>
    double ChanceUpgrade;

    /// <summary>
    /// модификаторы, которыми получился шанс
    /// </summary>
    Select_Modifiers ModifiersThisChance;

    /// <summary>
    /// визуальный процент
    /// </summary>
    double VisualProcent;

};




/////////////////////////////////////////////////////////////////////////////////////////////////////////

