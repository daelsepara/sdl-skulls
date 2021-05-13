#ifndef __CHARACTER__HPP__
#define __CHARACTER__HPP__
#include <vector>

#include "codewords.hpp"
#include "items.hpp"
#include "skills.hpp"

namespace Character
{
    const int ITEM_LIMIT = 8;

    class Base
    {
    public:
        const char *Name = NULL;
        const char *Description = NULL;

        int Life = 10;
        int Money = 10;

        std::vector<Skill::Base> Skills = std::vector<Skill::Base>();
        std::vector<Item::Type> Items = std::vector<Item::Type>();
        std::vector<Codeword::Type> Codewords = std::vector<Codeword::Type>();

        Base()
        {
        }

        Base(const char *name, const char *description, std::vector<Skill::Base> skills, std::vector<Item::Type> items, int money)
        {
            Name = name;
            Description = description;
            Skills = skills;
            Items = items;
            Money = money;
        }
    };

    auto WARRIOR = Base("The Warrior", "A proud noble of the Maya people, and strong in the arts of war, you tolerate no insolence from any man.", {Skill::AGILITY, Skill::ETIQUETTE, Skill::SWORDPLAY, Skill::UNARMED_COMBAT}, {Item::Type::SWORD}, 10);
    auto HUNTER = Base("The Hunter", "You can keep pace with the deer of the woods, wrestle jaguars, and your blowgun can bring down a bird in flight. Your sharp instincts make you almost a creature of the wild yourself.", {Skill::AGILITY, Skill::TARGETING, Skill::UNARMED_COMBAT, Skill::WILDERNESS_LORE}, {Item::Type::BLOWGUN}, 10);
    auto MYSTIC = Base("The Mystic", "You feel that other's lives are mundane. You learnt your skills from solitary, exploration and the dreams that came while you lay asleep under the stars.", {Skill::AGILITY, Skill::CHARMS, Skill::TARGETING, Skill::WILDERNESS_LORE}, {Item::Type::MAGIC_AMULET, Item::Type::BLOWGUN}, 10);
    auto WAYFARER = Base("The Wayfarer", "You have travelled widely and witnessed countless strange sights. Your wanderings have taught you many useful skills.", {Skill::CUNNING, Skill::FOLKLORE, Skill::SEAFARING, Skill::WILDERNESS_LORE}, {Item::Type::MAGIC_AMULET, Item::Type::BLOWGUN}, 10);
    auto MERCHANT = Base("The Merchant", "Daring adventure, subtle villainy, and always one eye open for a tidy profit -- these are your tenets.", {Skill::CUNNING, Skill::ROGUERY, Skill::SEAFARING, Skill::SWORDPLAY}, {Item::Type::SWORD}, 15);
    auto ACOLYTE = Base("The Acolyte", "You are master of many skills, but you know it is the god who shape man's destiny.", {Skill::ETIQUETTE, Skill::FOLKLORE, Skill::SPELLS, Skill::SWORDPLAY}, {Item::Type::MAGIC_WAND, Item::Type::SWORD}, 10);
    auto SORCERER = Base("The Sorcerer", "Born into a high clan, you were schooled in sorcery by priests and wise men. Now you can twist reality itself to suit your wishes.", {Skill::CHARMS, Skill::ETIQUETTE, Skill::ROGUERY, Skill::SPELLS}, {Item::Type::MAGIC_AMULET, Item::Type::MAGIC_WAND}, 10);

    std::vector<Character::Base> Classes = {WARRIOR, HUNTER, MYSTIC, WAYFARER, MERCHANT, ACOLYTE, SORCERER};

    bool VERIFY_ITEM(Character::Base &player, Item::Type item)
    {
        auto found = false;

        if (player.Items.size() > 0)
        {
            for (auto i = 0; i < player.Items.size(); i++)
            {
                if (player.Items[i] == item)
                {
                    found = true;

                    break;
                }
            }
        }

        return found;
    }

    int FIND_ITEM(Character::Base &player, Item::Type item)
    {
        auto found = -1;

        if (player.Items.size() > 0)
        {
            for (auto i = 0; i < player.Items.size(); i++)
            {
                if (player.Items[i] == item)
                {
                    found = i;

                    break;
                }
            }
        }

        return found;
    }

    // Checks if player has the skill and the required item
    bool VERIFY_SKILL(Character::Base &player, Skill::Type skill)
    {
        auto found = false;

        if (player.Skills.size() > 0 && skill != Skill::Type::NONE)
        {
            for (auto i = 0; i < player.Skills.size(); i++)
            {
                if (player.Skills[i].Type == skill)
                {
                    if (player.Skills[i].Requirement != Item::Type::NONE)
                    {
                        found = VERIFY_ITEM(player, player.Skills[i].Requirement);
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

    bool HAS_SKILL(Character::Base &player, Skill::Type skill)
    {
        auto found = false;

        if (player.Skills.size() > 0 && skill != Skill::Type::NONE)
        {
            for (auto i = 0; i < player.Skills.size(); i++)
            {
                if (player.Skills[i].Type == skill)
                {
                    found = true;

                    break;
                }
            }
        }

        return found;
    }

    int FIND_SKILL_ITEMS(Character::Base &player, Skill::Type skill, std::vector<Item::Type> items)
    {
        auto found = 0;

        if (player.Skills.size() > 0 && skill != Skill::Type::NONE)
        {
            for (auto i = 0; i < player.Skills.size(); i++)
            {
                if (player.Skills[i].Type == skill)
                {
                    auto result = FIND_ITEM(player, items[i]);

                    if (result >= 0)
                    {
                        found++;
                    }
                }
            }
        }

        return found;
    }

    // verify that player has the skill and ANY of the items
    bool VERIFY_SKILL_ANY(Character::Base &player, Skill::Type skill, std::vector<Item::Type> items)
    {
        return FIND_SKILL_ITEMS(player, skill, items) > 0;
    }

    // verify that player has the skill and ALL of the items
    bool VERIFY_SKILL_ALL(Character::Base &player, Skill::Type skill, std::vector<Item::Type> items)
    {
        return FIND_SKILL_ITEMS(player, skill, items) == items.size();
    }

    bool VERIFY_SKILL_ITEM(Character::Base &player, Skill::Type skill, Item::Type item)
    {
        return VERIFY_SKILL_ALL(player, skill, {item});
    }

    int FIND_CODEWORD(Character::Base &player, Codeword::Type codeword)
    {
        auto found = -1;

        if (player.Codewords.size() > 0)
        {
            for (auto i = 0; i < player.Codewords.size(); i++)
            {
                if (player.Codewords[i] == codeword)
                {
                    found = i;

                    break;
                }
            }
        }

        return found;
    }

    int FIND_CODEWORDS(Character::Base &player, std::vector<Codeword::Type> codewords)
    {
        auto found = 0;

        if (player.Codewords.size() > 0 && codewords.size() > 0)
        {
            for (auto i = 0; i < codewords.size(); i++)
            {
                auto result = FIND_CODEWORD(player, codewords[i]);

                if (result >= 0)
                {
                    found++;
                }
            }
        }

        return found;
    }

    bool VERIFY_CODEWORDS_ANY(Character::Base &player, std::vector<Codeword::Type> codewords)
    {
        return FIND_CODEWORDS(player, codewords) > 0;
    }

    bool VERIFY_CODEWORDS_ALL(Character::Base &player, std::vector<Codeword::Type> codewords)
    {
        return FIND_CODEWORDS(player, codewords) == codewords.size();
    }

    bool VERIFY_CODEWORD(Character::Base &player, Codeword::Type codeword)
    {
        return VERIFY_CODEWORDS_ALL(player, {codeword});
    }

    bool VERIFY_LIFE(Character::Base &player, int threshold = 0)
    {
        return player.Life > threshold;
    }

    bool VERIFY_POSSESSIONS(Character::Base &player)
    {
        return player.Items.size() <= Character::ITEM_LIMIT;
    }

    void GET_ITEMS(Character::Base &player, std::vector<Item::Type> items)
    {
        player.Items.insert(player.Items.end(), items.begin(), items.end());
    }

    void GET_CODEWORDS(Character::Base &player, std::vector<Codeword::Type> codewords)
    {
        for (auto i = 0; i < codewords.size(); i++)
        {
            if (!VERIFY_CODEWORD(player, codewords[i]))
            {
                player.Codewords.push_back(codewords[i]);
            }
        }
    }

    void GET_UNIQUE_ITEMS(Character::Base &player, std::vector<Item::Type> items)
    {
        for (auto i = 0; i < items.size(); i++)
        {
            if (!VERIFY_ITEM(player, items[i]))
            {
                player.Items.push_back(items[i]);
            }
        }
    }

    void LOSE_ITEM(Character::Base &player, std::vector<Item::Type> items)
    {
        if (player.Items.size() > 0 && items.size() > 0)
        {
            for (auto i = 0; i < items.size(); i++)
            {
                auto result = FIND_ITEM(player, items[i]);

                if (result >= 0)
                {
                    player.Items.erase(player.Items.begin() + result);
                }
            }
        }
    }

} // namespace Character

#endif
