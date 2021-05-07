#ifndef __SKILLS__HPP__
#define __SKILLS__HPP__
#include "items.hpp"

enum class SkillType
{
    NONE = -1,
    AGILITY,
    CHARMS,
    CUNNING,
    ETIQUETTE,
    FOLKLORE,
    ROGUERY,
    SEAFARING,
    SPELLS,
    SWORDPLAY,
    TARGETING,
    UNARMED_COMBAT,
    WILDERNESS_LORE,
    First = AGILITY,
    Last = WILDERNESS_LORE
};

class Skill
{
public:
    const char *Name = NULL;
    SkillType Type;
    Item Requirement = NULL;

    Skill(const char *name, SkillType type, Item item)
    {
        Name = name;
        Type = type;
        Requirement = item;
    }

    Skill(const char *name, SkillType type)
    {
        Name = name;
        Type = type;
        Requirement = NULL;
    }
};
#endif