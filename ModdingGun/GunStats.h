#pragma once
#include "Core.h"

/// общие параметры 
namespace GunStats 
{
    struct Modifiers 
    {
        enum ToolType : uint8_t
        {
            toolTypeBegin = 0,

            noTool,
            old,
            normal,
            improved,

            toolTypeEnd = 100
        };

        enum KitType : uint8_t
        {
            kitTypeBegin = 0,

            noKit,
            repair,
            detail,

            kitTypeEnd = 100
        };

        enum SkillType : uint8_t
        {
            skillTypeBegin = 0,

            playerNoSkill,

            playerMaster1,
            playerMaster2,
            playerMaster3,
            playerMaster4,
            playerMaster5,

            NPCMaster2,
            NPCMaster3,
            NPCMaster4,
            NPCMaster5,

            skillTypeEnd = 100
        };

        static inline double GetToolModifier(ToolType tool)
        {
            switch (tool)
            {
            case ToolType::noTool:   return 0.0;
            case ToolType::old:      return 0.2;
            case ToolType::normal:   return 0.5;
            case ToolType::improved: return 0.8;
            }
            return -1.0;
        }

        static inline string GetToolName(ToolType tool)
        {
            switch (tool)
            {
            case ToolType::noTool:   return "without_tool";
            case ToolType::old:      return "Old_tool";
            case ToolType::normal:   return "Common_tool";
            case ToolType::improved: return "Improved_tool";
            }
            return "No_Name_Tool";
        }

        static inline  double GetKitModifier(KitType kit)
        {
            switch (kit)
            {
            case KitType::noKit:   return 0.0;
            case KitType::repair:  return 1.0;
            case KitType::detail:  return 4.0;
            }
            return -1.0;
        }

        static inline  string GetKitName(KitType kit)
        {
            switch (kit)
            {
            case KitType::noKit:   return "Without_set_parts";
            case KitType::repair:  return "Repair_kit";
            case KitType::detail:  return "Detail";
            }
            return "No_Name_Kit";
        }

        static inline  double GetSkillModifier(SkillType skill)
        {
            switch (skill) 
            {
            case SkillType::playerNoSkill:  return 0.0;
            
            case SkillType::playerMaster1:  return 0.2;
            case SkillType::playerMaster2:  return 0.4;
            case SkillType::playerMaster3:  return 0.6;
            case SkillType::playerMaster4:  return 0.8;
            case SkillType::playerMaster5:  return 1.0;
            
            case SkillType::NPCMaster2:     return 0.5;
            case SkillType::NPCMaster3:     return 1.0;
            case SkillType::NPCMaster4:     return 1.5;
            case SkillType::NPCMaster5:     return 2.0;
            }
            return -1.0;
        }

        static inline string GetSkillName(SkillType skill)
        {
            switch (skill)
            {
            case SkillType::playerNoSkill:  return "without_skill";

            case SkillType::playerMaster1:  return "Player_master_1";
            case SkillType::playerMaster2:  return "Player_master_2";
            case SkillType::playerMaster3:  return "Player_master_3";
            case SkillType::playerMaster4:  return "Player_master_4";
            case SkillType::playerMaster5:  return "Player_master_5";

            case SkillType::NPCMaster2:     return "NPC_master_2";
            case SkillType::NPCMaster3:     return "NPC_master_3";
            case SkillType::NPCMaster4:     return "NPC_master_4";
            case SkillType::NPCMaster5:     return "NPC_master_5";
            }
            return "No_Name_Skill";
        }
    };
    
    inline Modifiers::ToolType  operator++(Modifiers::ToolType& other, int)
    {
        if (other < Modifiers::ToolType::toolTypeEnd)
        {
            other = static_cast<Modifiers::ToolType>(static_cast<int>(other) + 1);
        }
        return other;
    }

    inline Modifiers::KitType   operator++(Modifiers::KitType& other, int)
    {
        if (other < Modifiers::KitType::kitTypeEnd)
        {
            other = static_cast<Modifiers::KitType>(static_cast<int>(other) + 1);
        }
        return other;
    }
    inline Modifiers::SkillType operator++(Modifiers::SkillType& other, int)
    {
        if (other < Modifiers::SkillType::skillTypeEnd)
        {
            other = static_cast<Modifiers::SkillType>(static_cast<int>(other) + 1);
        }
        return other;
    }



    //-------------------------------------------------------------------------------------------------------------------


    inline  constexpr size_t countCharacteristic = 7;

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
    inline constexpr array<int, countCharacteristic> CountUnitsForCharacteristic =
    {
        60, 
        48, 
        40, 

        40, 
        40, 
        40, 
        40
    };


    //-------------------------------------------------------------------------------------------------------------------

    /// <summary>
    /// кучность
    /// </summary>
    inline  constexpr int indexAccuracy = 0;

    /// <summary>
    /// темп стрельбы
    /// </summary>
    inline  constexpr int IndexRateOfFire = 1;

    /// <summary>
    /// отдача оружия
    /// </summary>
    inline  constexpr int indexKickback = 2;

    /// <summary>
    /// качание оружия
    /// </summary>
    inline  constexpr int indexSway = 3;

    /// <summary>
    /// пробитие
    /// </summary>
    inline  constexpr int indexPenetration = 4;

    /// <summary>
    /// отказ из-за состояния оружия
    /// </summary>
    inline  constexpr int indexMalfunctionCondition = 5;

    /// <summary>
    /// отказ из-за грязи
    /// </summary>
    inline  constexpr int indexMalfunctionDirt = 6;

    /// <summary>
    /// получаем название характеристики по индексу GunStats
    /// </summary>
    inline constexpr array<string_view, countCharacteristic> mapCharacteristicNameGunStat
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
    inline constexpr array<string_view, countCharacteristic> mapCharacteristicNameIndex
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
    inline constexpr array<int, countCharacteristic> transIndexToIndexGunStat
    {
        GunStats::indexAccuracy,               // 0
        GunStats::IndexRateOfFire,           // 1
        GunStats::indexPenetration,            // 2
        GunStats::indexKickback,               // 3
        GunStats::indexSway,                   // 4
        GunStats::indexMalfunctionCondition,  // 5
        GunStats::indexMalfunctionDirt        // 6
    };
    
    /// <summary>
    /// Перевод из  index_GunStat index
    /// </summary>
    inline constexpr array<int, countCharacteristic> transIndexGunStatToIndex
    {
        0, // 0 indexAccuracy
        1, // 1 IndexRateOfFire
        3, // 2 indexKickback
        4, // 3 indexSway
        2, // 4 indexPenetration
        5, // 5 indexMalfunctionCondition
        6  // 6 indexMalfunctionDirt 
    };


    //-------------------------------------------------------------------------------------------------------------------

}






//-------------------------------------------------------------------------------------------------------------------


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

struct PriceModifiers
{
    unsigned PriceTool;
    unsigned PriceKit;
    unsigned PriceSkill;
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
    /// шанс улучшения для этого параметра
    /// </summary>
    double chanceUpgrade;

    /// <summary>
    /// модификаторы, которыми получился шанс
    /// </summary>
    Select_Modifiers ModifiersThisChance;

    /// <summary>
    /// визуальный процент
    /// </summary>
    double VisualPercent;

    /// <summary>
    /// цены на улучшение этого параметра
    /// </summary>
    PriceModifiers Price;

};




//-------------------------------------------------------------------------------------------------------------------

