#pragma once
#include "Core.h"

/// общие параметры 
namespace GunStats
{
	inline constexpr size_t countCharacteristic = 7;

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
	inline constexpr array<int, countCharacteristic> getCountUnitsForCharacteristic = { 60, 48, 40, 40, 40, 40, 40 };

	/// <summary>
	/// кучность
	/// </summary>
	inline  constexpr int kAccuracyIndex = 0;

	/// <summary>
	/// темп стрельбы
	/// </summary>
	inline  constexpr int kRateOfFireIndex = 1;

	/// <summary>
	/// отдача оружия
	/// </summary>
	inline  constexpr int kKickbackIndex = 2;

	/// <summary>
	/// качание оружия
	/// </summary>
	inline  constexpr int kSwayIndex = 3;

	/// <summary>
	/// пробитие
	/// </summary>
	inline  constexpr int kPenetrationIndex = 4;

	/// <summary>
	/// отказ из-за состояния оружия
	/// </summary>
	inline  constexpr int kMalfunctionConditionIndex = 5;

	/// <summary>
	/// отказ из-за грязи
	/// </summary>
	inline  constexpr int kMalfunctionDirtIndex = 6;

	/// <summary>
	/// получаем название характеристики по индексу GunStats
	/// </summary>
	inline constexpr array<string_view, countCharacteristic> characteristicNameInGunStat
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
	inline constexpr array<string_view, countCharacteristic> indexCharacteristicName
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
		GunStats::kAccuracyIndex,               // 0
		GunStats::kRateOfFireIndex,           // 1
		GunStats::kPenetrationIndex,            // 2
		GunStats::kKickbackIndex,               // 3
		GunStats::kSwayIndex,                   // 4
		GunStats::kMalfunctionConditionIndex,  // 5
		GunStats::kMalfunctionDirtIndex        // 6
	};

	/// <summary>
	/// Перевод из  index_GunStat index
	/// </summary>
	inline constexpr array<int, countCharacteristic> transIndexGunStatToIndex
	{
		0, // 0 kAccuracyIndex
		1, // 1 kRateOfFireIndex
		3, // 2 kKickbackIndex
		4, // 3 kSwayIndex
		2, // 4 kPenetrationIndex
		5, // 5 kMalfunctionConditionIndex
		6  // 6 kMalfunctionDirtIndex 
	};

	//-----------------------------------------------------------------------------------------------------------------------

	struct Modifiers
	{
		enum ToolType
		{
			toolTypeBegin,

			noTool,
			old,
			normal,
			improved,

			ToolTypeEnd
		};

		enum KitType
		{
			kitTypeBegin,

			noKit,
			repair,
			detail,

			kitTypeEnd
		};

		enum SkillType
		{
			skillTypeBegin,

			playerNoSkill,

			playerMasterLvl1,
			playerMasterLvl2,
			playerMasterLvl3,
			playerMasterLvl4,
			playerMasterLvl5,

			npcMasterLvl2,
			npcMasterLvl3,
			npcMasterLvl4,
			npcMasterLvl5,

			skillTypeEnd
		};

		static inline double getToolModifier(ToolType tool)
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

		static inline string getToolName(ToolType tool)
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

		static inline double getKitModifier(KitType kit)
		{
			switch (kit)
			{
			case KitType::noKit:   return 0.0;
			case KitType::repair:  return 1.0;
			case KitType::detail:  return 4.0;
			}
			return -1.0;
		}
		static inline string GetKitName(KitType kit)
		{
			switch (kit)
			{
			case KitType::noKit:   return "Without_set_parts";
			case KitType::repair:  return "Repair_kit";
			case KitType::detail:  return "Detail";
			}
			return "No_Name_Kit";
		}

		static inline double getSkillModifier(SkillType skill)
		{
			switch (skill)
			{
			case SkillType::playerNoSkill:  return 0.0;

			case SkillType::playerMasterLvl1:  return 0.2;
			case SkillType::playerMasterLvl2:  return 0.4;
			case SkillType::playerMasterLvl3:  return 0.6;
			case SkillType::playerMasterLvl4:  return 0.8;
			case SkillType::playerMasterLvl5:  return 1.0;

			case SkillType::npcMasterLvl2:     return 0.5;
			case SkillType::npcMasterLvl3:     return 1.0;
			case SkillType::npcMasterLvl4:     return 1.5;
			case SkillType::npcMasterLvl5:     return 2.0;
			}
			return -1.0;
		}

		static inline string getSkillName(SkillType skill)
		{
			switch (skill)
			{
			case SkillType::playerNoSkill:  return "without_skill";

			case SkillType::playerMasterLvl1:  return "Player_master_1";
			case SkillType::playerMasterLvl2:  return "Player_master_2";
			case SkillType::playerMasterLvl3:  return "Player_master_3";
			case SkillType::playerMasterLvl4:  return "Player_master_4";
			case SkillType::playerMasterLvl5:  return "Player_master_5";

			case SkillType::npcMasterLvl2:     return "NPC_master_2";
			case SkillType::npcMasterLvl3:     return "NPC_master_3";
			case SkillType::npcMasterLvl4:     return "NPC_master_4";
			case SkillType::npcMasterLvl5:     return "NPC_master_5";
			}
			return "No_Name_Skill";
		}
	};

	inline Modifiers::ToolType  operator++(Modifiers::ToolType& other, int)
	{
		if (other < Modifiers::ToolType::ToolTypeEnd)
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


}

//-----------------------------------------------------------------------------------------------------------------------

/// <summary>
/// модификаторы
/// </summary>
struct SelectModifiers
{
	/// <summary>
	/// инструмент
	/// </summary>
	GunStats::Modifiers::ToolType m_tool = GunStats::Modifiers::ToolType::toolTypeBegin;

	/// <summary>
	/// набор
	/// </summary>
	GunStats::Modifiers::KitType m_kit = GunStats::Modifiers::KitType::kitTypeBegin;;

	/// <summary>
	/// навык
	/// </summary>
	GunStats::Modifiers::SkillType m_skill = GunStats::Modifiers::SkillType::skillTypeBegin;


	bool operator==(const SelectModifiers& other)
	{
		return { this->m_tool == other.m_tool && this->m_kit == other.m_kit && this->m_skill == other.m_skill };
	};
};

/// <summary>
/// 
/// </summary>
struct PriceModifiers
{
	unsigned m_priceTool = 0;
	unsigned m_priceKit = 0;
	unsigned m_priceSkill = 0;
};

/// содержит 
/// int Parametr - параметр который был улучшен
/// double Chance - шанс улучшения этго параметра
/// Select_Modifiers ModifiersForChance - модификаторы, которыми получился шанс
struct Method
{
	/// <summary>
	/// модификаторы, которыми получился шанс
	/// </summary>
	SelectModifiers m_modifiersThisChance = {};

	/// <summary>
	/// цены на улучшение этого параметра
	/// </summary>
	PriceModifiers m_price = {};

	/// <summary>
	/// шанс улучшения для этого параметра
	/// </summary>
	double m_chanceUpgrade = 0.0;

	/// <summary>
	/// визуальный процент
	/// </summary>
	double m_visualPercent = 0.0;

	/// <summary>
	/// параметр который был улучшен (INDEXGUNSTAT)
	/// </summary>
	int m_characteristicGunstat = 0;
};

//-----------------------------------------------------------------------------------------------------------------------
