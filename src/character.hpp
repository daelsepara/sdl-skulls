#ifndef __CHARACTER__HPP__
#define __CHARACTER__HPP__
#include <vector>

#include "codewords.hpp"
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
        std::vector<Codeword::Type> Codewords = std::vector<Codeword::Type>();

        Abstract()
        {
        }

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
    auto SORCERER = Abstract("The Sorcerer", "Born into a high clan, you were schooled in sorcery by priests and wise men. Now you can twist reality itself to suit your wishes.", {Skill::CHARMS, Skill::ETIQUETTE, Skill::ROGUERY, Skill::SPELLS}, {Item::Type::MAGIC_AMULET, Item::Type::MAGIC_WAND}, 10);

    std::vector<Character::Abstract> Classes = {WARRIOR, HUNTER, MYSTIC, WAYFARER, MERCHANT, ACOLYTE, SORCERER};

    bool VERIFY_ITEM(Character::Abstract character, Item::Type item)
    {
        auto found = false;

        if (character.Items.size() > 0)
        {
            for (auto i = 0; i < character.Items.size(); i++)
            {
                if (character.Items[i] == item)
                {
                    found = true;

                    break;
                }
            }
        }

        return found;
    }

    bool VERIFY_SKILL(Character::Abstract character, Skill::Type skill)
    {
        auto found = false;

        if (character.Skills.size() > 0 && skill != Skill::Type::NONE)
        {
            for (auto i = 0; i < character.Skills.size(); i++)
            {
                if (character.Skills[i].Type == skill)
                {
                    if (character.Skills[i].Requirement != Item::Type::NONE)
                    {
                        found = VERIFY_ITEM(character, character.Skills[i].Requirement);
                    }
                    else
                    {
                        found = true;
                    }

                    break;
                }
            }
        }

        return found;
    }

    // verify that player has the skill and ANY of the items
    bool VERIFY_SKILL_ANY(Character::Abstract character, Skill::Type skill, std::vector<Item::Type> items)
    {
        auto found = false;

        if (character.Skills.size() > 0 && skill != Skill::Type::NONE)
        {
            for (auto i = 0; i < character.Skills.size(); i++)
            {
                if (character.Skills[i].Type == skill)
                {
                    for (auto j = 0; j < items.size(); j++)
                    {
                        found = VERIFY_ITEM(character, items[j]);

                        if (found)
                        {
                            break;
                        }
                    }
                }

                if (found)
                {
                    break;
                }
            }
        }

        return found;
    }

    // verify that player has the skill and ALL of the items
    bool VERIFY_SKILL_ALL(Character::Abstract character, Skill::Type skill, std::vector<Item::Type> items)
    {
        auto found = 0;

        if (character.Skills.size() > 0 && skill != Skill::Type::NONE)
        {
            for (auto i = 0; i < character.Skills.size(); i++)
            {
                if (character.Skills[i].Type == skill)
                {
                    auto result = std::find(character.Items.begin(), character.Items.end(), items[i]);

                    if (result != character.Items.end())
                    {
                        found++;
                    }
                }
            }
        }

        return found == items.size();
    }

    bool VERIFY_SKILL_ITEM(Character::Abstract character, Skill::Type skill, Item::Type item)
    {
        return VERIFY_SKILL_ALL(character, skill, {item});
    }

    bool VERIFY_CODEWORDS_ANY(Character::Abstract character, std::vector<Codeword::Type> codewords)
    {
        auto found = 0;

        if (character.Codewords.size() > 0 && codewords.size() > 0)
        {
            for (auto i = 0; i < codewords.size(); i++)
            {
                auto result = std::find(character.Codewords.begin(), character.Codewords.end(), codewords[i]);

                if (result != character.Codewords.end())
                {
                    found++;
                }
            }
        }

        return found > 0;
    }

    bool VERIFY_CODEWORDS_ALL(Character::Abstract character, std::vector<Codeword::Type> codewords)
    {
        auto found = 0;

        if (character.Codewords.size() > 0 && codewords.size() > 0)
        {
            for (auto i = 0; i < codewords.size(); i++)
            {
                auto result = std::find(character.Codewords.begin(), character.Codewords.end(), codewords[i]);

                if (result != character.Codewords.end())
                {
                    found++;
                }
            }
        }

        return found == codewords.size();
    }

    bool VERIFY_CODEWORD(Character::Abstract character, Codeword::Type codeword)
    {
        return VERIFY_CODEWORDS_ALL(character, {codeword});
    }

} // namespace Character

Character::Abstract Player;

#endif
