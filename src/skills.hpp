#ifndef __SKILLS__HPP__
#define __SKILLS__HPP__

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

    class Abstract
    {
    public:
        const char *Name = NULL;
        const char *Description = NULL;
        Skill::Type Type;
        Item::Type Requirement = Item::Type::NONE;

        Abstract(const char *name, const char *description, Skill::Type type, Item::Type item)
        {
            Name = name;
            Type = type;
            Description = description;
            Requirement = item;
        }

        Abstract(const char *name, const char *description, Skill::Type type)
        {
            Name = name;
            Type = type;
            Description = description;
            Requirement = Item::Type::NONE;
        }
    };

    const auto AGILITY = Abstract("Agility", "The ability to perform acrobatic feats, run, climb, balance and leap. A character with this skill is nimble and dexterous.", Type::AGILITY);
    const auto CHARMS = Abstract("Charms", "The expert use of magical wards to protect you from danger. Also includes that most elusive of qualities. luck. YOu must possess a magic amulet to use this skill.", Type::CHARMS, Item::Type::MAGIC_AMULET);
    const auto CUNNING = Abstract("Cunning", "The ability to think on your feet and devise clever schemes for getting out of trouble. Useful in countless situations.", Type::CUNNING);
    const auto ETIQUETTE = Abstract("Etiquette", "Understanding of the courtly manners which are essential to proper conduct in the upper echelons of the nobility.", Type::ETIQUETTE);
    const auto FOLKLORE = Abstract("Folklore", "Knowledge of myth and legend, and how best to deal with supernatural menaces such as garlic against vampires, silver bullets against a werewolf, and so on.", Type::FOLKLORE);
    const auto ROGUERY = Abstract("Roguery", "The traditional repertoire of a thief's tricks: picking pockets, opening locks, and skulking unseen in the shadows.", Type::ROGUERY);
    const auto SEAFARING = Abstract("Seafaring", "Knowing all about life at sea, including the ability to handle anything from a rowing boat right up to a large sailing ship.", Type::SEAFARING);
    const auto SPELLS = Abstract("Spells", "A range of magical effects encompassing illusions, elemental effects, commands, and summonings. You must possess a magic wand to use this skill.", Type::SPELLS, Item::Type::MAGIC_WAND);
    const auto SWORDPLAY = Abstract("Swordplay", "The best fighting skill. You must possess a sword to use this skill.", Type::SWORDPLAY, Item::Type::SWORD);
    const auto TARGETING = Abstract("Targeting", "A long-range attack skill. You must possess a blowgun to use this skill.", Type::TARGETING, Item::Type::BLOWGUN);
    const auto UNARMED_COMBAT = Abstract("Unarmed Combat", "Fisticuffs, wrestling holds, jabs and kicks, and the tricks of infighting. Not as effective as SWORDPLAY, but you do not need weapons - your own body is the weapon!", Type::UNARMED_COMBAT);
    const auto WILDERNESS_LORE = Abstract("Wilderness Lore", "A talent for survival in the wild - whether it be forest, desert, swamp or mountain peak.", Type::WILDERNESS_LORE);

} // namespace Skill

#endif
