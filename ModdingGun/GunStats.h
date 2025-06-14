#pragma once
#include "Core.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// статы наборов
namespace GunStats 
{

    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    enum class ToolType {
        NO_TOOL,
        OLD_TOOL,
        NORMAL_TOOL,
        IMPROVED_TOOL
    };

    inline double GetToolModifier(ToolType tool) {
        switch (tool) {
        case ToolType::NO_TOOL:       return 0.0;
        case ToolType::OLD_TOOL:      return 0.2;
        case ToolType::NORMAL_TOOL:   return 0.5;
        case ToolType::IMPROVED_TOOL: return 0.8;
        }
        return 0.0;
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////




    enum class KitType {
        NO_KIT,
        REPAIR_KIT,
        DETAIL_KIT
    };

    inline double GetKitModifier(KitType kit) {
        switch (kit) {
        case KitType::NO_KIT:      return 0.0;
        case KitType::REPAIR_KIT:  return 1.0;
        case KitType::DETAIL_KIT:  return 4.0;
        }
        return 0.0;
    }

    enum class SkillType {
        PLAYER_NO_SKILL,

        PLAYER_MASTER_1,
        PLAYER_MASTER_2,
        PLAYER_MASTER_3,
        PLAYER_MASTER_4,
        PLAYER_MASTER_5,

        NPC_MASTER_2,
        NPC_MASTER_3,
        NPC_MASTER_4,
        NPC_MASTER_5
    };

    inline double GetSkillModifier(SkillType skill) {
        switch (skill) {
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
        return 0.0;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////

        // отображение процента
    static const vector<double> Accuracy_VStat = [] {
        vector<double> result;

        int size = 60;

        for (int position = 0; position < size * 2; position++)
        {
            result.push_back(getChance(position, { {0, -100}, {size, 0}, {size * 2, 100} }));
        }

        return result;

        }();

    // отображение темпа стрельбы
    static const vector<double> RateFire_VStat = [] {
        vector<double> result;

        int size = 48;

        for (int position = 0; position < size * 2; position++)
        {
            result.push_back(getChance(position, { {0, -100}, {size, 0}, {size * 2, 100} }));
        }
        return result;

        }();
    // остальные
    static const vector<double> Other_VStat = [] {
        vector<double> result;

        int size = 40;

        for (int position = 0; position < size * 2; position++)
        {
            result.push_back(getChance(position, { {0, -100}, {size, 0}, {size * 2, 100} }));
        }
        return result;

        }();

    /// <summary>
    /// кучность
    /// </summary>
    inline  constexpr int ACCURACY = 0;

    /// <summary>
    /// темп стрельбы
    /// </summary>
    inline  constexpr int RATE_OF_FIRE = 1;


    /// <summary>
    /// отдача оружия
    /// </summary>
    inline  constexpr int KICKBACK = 2;

    /// <summary>
    /// качание оружия
    /// </summary>
    inline  constexpr int SWAY = 3;

    /// <summary>
    /// пробитие
    /// </summary>
    inline  constexpr int PENETRATION = 4;

    /// <summary>
    /// отказ из-за состояния оружия
    /// </summary>
    inline  constexpr int MALFUNCTION_CONDITION = 5;

    /// <summary>
    /// отказ из-за грязи
    /// </summary>
    inline  constexpr int MALFUNCTION_DIRT = 6;

}


/////////////////////////////////////////////////////////////////////////////////////////////////////////