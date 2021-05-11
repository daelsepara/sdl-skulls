#ifndef __CHARACTER__HPP__
#define __CHARACTER__HPP__
#include <vector>

#include "items.hpp"
#include "skills.hpp"

namespace Character
{
    class Abstract
    {
    public:
        const char *Name = NULL;
        const char *Description = NULL;

        int Life = 10;
        int Money = 10;

        std::vector<Skill::Abstract> Skills = std::vector<Skill::Abstract>();
        std::vector<Item::Type> Items = std::vector<Item::Type>();

        Abstract(const char *name, const char *description, std::vector<Skill::Abstract> skills, std::vector<Item::Type> items, int money)
        {
            Name = name;
            Description = description;

            Skills = skills;
            Items = items;
            Money = money;
        }
    };

    auto WARRIOR = Abstract("The Warrior", "A proud noble of the Maya people, and strong in the arts of war, you tolerate no insolence from any man.", {Skill::AGILITY, Skill::ETIQUETTE, Skill::SWORDPLAY, Skill::UNARMED_COMBAT}, {Item::Type::SWORD}, 10);
    auto HUNTER = Abstract("The Hunter", "You can keep pace with the deer of the woods, wrestle jaguars, and your blowgun can bring down a bird in flight. Your sharp instincts make you almost a creature of the wild yourself.", {Skill::AGILITY, Skill::TARGETING, Skill::UNARMED_COMBAT, Skill::WILDERNESS_LORE}, {Item::Type::BLOWGUN}, 10);
    auto MYSTIC = Abstract("The Mystic", "You feel that other's lives are mundane. You learnt your skills from solitary, exploration and the dreams that came while you lay asleep under the stars.", {Skill::AGILITY, Skill::CHARMS, Skill::TARGETING, Skill::WILDERNESS_LORE}, {Item::Type::MAGIC_AMULET, Item::Type::BLOWGUN}, 10);
    auto WAYFARER = Abstract("The Wayfarer", "You have travelled widely and witnessed countless strange sights. Your wanderings have taught you many useful skills.", {Skill::CUNNING, Skill::FOLKLORE, Skill::SEAFARING, Skill::WILDERNESS_LORE}, {Item::Type::MAGIC_AMULET, Item::Type::BLOWGUN}, 10);
    auto MERCHANT = Abstract("The Merchant", "Daring adventure, subtle villainy, and always one eye open for a tidy profit -- these are your tenets.", {Skill::CUNNING, Skill::ROGUERY, Skill::SEAFARING, Skill::SWORDPLAY}, {Item::Type::SWORD}, 15);
    auto ACOLYTE = Abstract("The Acolyte", "You are master of many skills, but you know it is the god who shape man's destiny.", {Skill::ETIQUETTE, Skill::FOLKLORE, Skill::SPELLS, Skill::SWORDPLAY}, {Item::Type::MAGIC_WAND, Item::Type::SWORD}, 10);
}

#endif
