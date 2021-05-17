#ifndef __SKILLS__HPP__
#define __SKILLS__HPP__

#include <vector>

#include "items.hpp"

namespace Skill
{
    enum class Type
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

    class Base
    {
    public:
        const char *Name = NULL;
        const char *Description = NULL;
        Skill::Type Type;
        Item::Type Requirement = Item::Type::NONE;

        Base(const char *name, const char *description, Skill::Type type, Item::Type item)
        {
            Name = name;
            Type = type;
            Description = description;
            Requirement = item;
        }

        Base(const char *name, const char *description, Skill::Type type)
        {
            Name = name;
            Type = type;
            Description = description;
            Requirement = Item::Type::NONE;
        }
    };

    const auto AGILITY = Base("AGILITY", "The ability to perform acrobatic feats, run, climb, balance and leap. A character with this skill is nimble and dexterous.", Type::AGILITY);
    const auto CHARMS = Base("CHARMS", "The expert use of magical wards to protect you from danger. Also includes that most elusive of qualities: luck. You must possess a MAGIC AMULET to use this skill.", Type::CHARMS, Item::Type::MAGIC_AMULET);
    const auto CUNNING = Base("CUNNING", "The ability to think on your feet and devise clever schemes for getting out of trouble. Useful in countless situations.", Type::CUNNING);
    const auto ETIQUETTE = Base("ETIQUETTE", "Understanding of the courtly manners which are essential to proper conduct in the upper echelons of the nobility.", Type::ETIQUETTE);
    const auto FOLKLORE = Base("FOLKLORE", "Knowledge of myth and legend, and how best to deal with supernatural menaces such as garlic against vampires, silver bullets against a werewolf, and so on.", Type::FOLKLORE);
    const auto ROGUERY = Base("ROGUERY", "The traditional repertoire of a thief's tricks: picking pockets, opening locks, and skulking unseen in the shadows.", Type::ROGUERY);
    const auto SEAFARING = Base("SEAFARING", "Knowing all about life at sea, including the ability to handle anything from a rowing boat right up to a large sailing ship.", Type::SEAFARING);
    const auto SPELLS = Base("SPELLS", "A range of magical effects encompassing illusions, elemental effects, commands, and summonings. You must possess a MAGIC WAND to use this skill.", Type::SPELLS, Item::Type::MAGIC_WAND);
    const auto SWORDPLAY = Base("SWORDPLAY", "The best fighting skill. You must possess a SWORD to use this skill.", Type::SWORDPLAY, Item::Type::SWORD);
    const auto TARGETING = Base("TARGETING", "A long-range attack skill. You must possess a BLOWGUN to use this skill.", Type::TARGETING, Item::Type::BLOWGUN);
    const auto UNARMED_COMBAT = Base("UNARMED COMBAT", "Fisticuffs, wrestling holds, jabs and kicks, and the tricks of infighting. Not as effective as SWORDPLAY, but you do not need weapons - your own body is the weapon!", Type::UNARMED_COMBAT);
    const auto WILDERNESS_LORE = Base("WILDERNESS LORE", "A talent for survival in the wild - whether it be forest, desert, swamp or mountain peak.", Type::WILDERNESS_LORE);

    std::vector<Skill::Base> ALL = {
        AGILITY, CHARMS, CUNNING, ETIQUETTE,
        FOLKLORE, ROGUERY, SEAFARING, SPELLS,
        SWORDPLAY, TARGETING, UNARMED_COMBAT, WILDERNESS_LORE};

    int FIND(std::vector<Skill::Base> &skills, Skill::Base skill)
    {
        auto found = -1;

        if (skills.size() > 0 && skill.Type != Skill::Type::NONE)
        {
            for (auto i = 0; i < skills.size(); i++)
            {
                if (skills[i].Type == skill.Type)
                {
                    found = i;

                    break;
                }
            }
        }

        return found;
    }

    bool VERIFY(std::vector<Skill::Base> &skills, Skill::Base skill)
    {
        return FIND(skills, skill) >= 0;
    }

    void ADD(std::vector<Skill::Base> &skills, Skill::Base skill)
    {
        if (!VERIFY(skills, skill))
        {
            skills.push_back(skill);
        }
    }

    void REMOVE(std::vector<Skill::Base> &skills, Skill::Base skill)
    {
        auto result = FIND(skills, skill);

        if (result >= 0)
        {
            skills.erase(skills.begin() + result);
        }
    }

} // namespace Skill

#endif
