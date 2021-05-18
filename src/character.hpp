#ifndef __CHARACTER__HPP__
#define __CHARACTER__HPP__

#include <vector>

#include "codewords.hpp"
#include "items.hpp"
#include "skills.hpp"

namespace Character
{
    enum class Type
    {
        CUSTOM = 0,
        WARRIOR,
        HUNTER,
        MYSTIC,
        WAYFARER,
        MERCHANT,
        ACOLYTE,
        SORCERER
    };

    class Base
    {
    public:
        const char *Name = NULL;

        const char *Description = NULL;

        int Life = 10;

        int Money = 10;

        int ITEM_LIMIT = 8;

        int MAX_LIFE_LIMIT = 10;

        int SKILLS_LIMIT = 4;

        int DONATION = 0;

        bool IsBlessed = false;

        bool IsImmortal = false;

        bool RitualBallStarted = false;

        int Ticks = 0;
        int Cross = 0;

        std::vector<Skill::Base> Skills = std::vector<Skill::Base>();
        std::vector<Item::Type> Items = std::vector<Item::Type>();
        std::vector<Codeword::Type> Codewords = std::vector<Codeword::Type>();

        std::vector<Item::Type> LostItems = std::vector<Item::Type>();
        std::vector<Skill::Base> LostSkills = std::vector<Skill::Base>();

        int LostMoney = 0;

        Character::Type Type = Character::Type::CUSTOM;

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

        Base(const char *name, const char *description, std::vector<Skill::Base> skills, std::vector<Item::Type> items, std::vector<Codeword::Type> codewords, int money)
        {
            Name = name;
            Description = description;
            Skills = skills;
            Codewords = codewords;
            Items = items;
            Money = money;
        }

        Base(const char *name, Character::Type type, const char *description, std::vector<Skill::Base> skills, std::vector<Item::Type> items, int money)
        {
            Name = name;
            Type = type;
            Description = description;
            Skills = skills;
            Items = items;
            Money = money;
        }

        Base(const char *name, Character::Type type, const char *description, std::vector<Skill::Base> skills, std::vector<Item::Type> items, std::vector<Codeword::Type> codewords, int money)
        {
            Name = name;
            Type = type;
            Description = description;
            Skills = skills;
            Codewords = codewords;
            Items = items;
            Money = money;
        }
    };

    auto WARRIOR = Base("The Warrior", Character::Type::WARRIOR, "A proud noble of the Maya people, and strong in the arts of war, you tolerate no insolence from any man.", {Skill::AGILITY, Skill::ETIQUETTE, Skill::SWORDPLAY, Skill::UNARMED_COMBAT}, {Item::Type::SWORD}, 10);
    auto HUNTER = Base("The Hunter", Character::Type::HUNTER, "You can keep pace with the deer of the woods, wrestle jaguars, and your blowgun can bring down a bird in flight. Your sharp instincts make you almost a creature of the wild yourself.", {Skill::AGILITY, Skill::TARGETING, Skill::UNARMED_COMBAT, Skill::WILDERNESS_LORE}, {Item::Type::BLOWGUN}, 10);
    auto MYSTIC = Base("The Mystic", Character::Type::MYSTIC, "You feel that other's lives are mundane. You learnt your skills from solitary, exploration and the dreams that came while you lay asleep under the stars.", {Skill::AGILITY, Skill::CHARMS, Skill::TARGETING, Skill::WILDERNESS_LORE}, {Item::Type::MAGIC_AMULET, Item::Type::BLOWGUN}, 10);
    auto WAYFARER = Base("The Wayfarer", Character::Type::WAYFARER, "You have travelled widely and witnessed countless strange sights. Your wanderings have taught you many useful skills.", {Skill::CUNNING, Skill::FOLKLORE, Skill::SEAFARING, Skill::WILDERNESS_LORE}, {}, 10);
    auto MERCHANT = Base("The Merchant", Character::Type::MERCHANT, "Daring adventure, subtle villainy, and always one eye open for a tidy profit -- these are your tenets.", {Skill::CUNNING, Skill::ROGUERY, Skill::SEAFARING, Skill::SWORDPLAY}, {Item::Type::SWORD}, 15);
    auto ACOLYTE = Base("The Acolyte", Character::Type::ACOLYTE, "You are master of many skills, but you know it is the god who shape man's destiny.", {Skill::ETIQUETTE, Skill::FOLKLORE, Skill::SPELLS, Skill::SWORDPLAY}, {Item::Type::MAGIC_WAND, Item::Type::SWORD}, 10);
    auto SORCERER = Base("The Sorcerer", Character::Type::SORCERER, "Born into a high clan, you were schooled in sorcery by priests and wise men. Now you can twist reality itself to suit your wishes.", {Skill::CHARMS, Skill::ETIQUETTE, Skill::ROGUERY, Skill::SPELLS}, {Item::Type::MAGIC_AMULET, Item::Type::MAGIC_WAND}, 10);

    std::vector<Character::Base> Classes = {WARRIOR, HUNTER, MYSTIC, WAYFARER, MERCHANT, ACOLYTE, SORCERER};

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

    int FIND_SKILL(Character::Base &player, Skill::Type skill)
    {
        auto found = -1;

        if (player.Skills.size() > 0)
        {
            for (auto i = 0; i < player.Skills.size(); i++)
            {
                if (player.Skills[i].Type == skill)
                {
                    found = i;

                    break;
                }
            }
        }

        return found;
    }

    bool VERIFY_ITEMS(Character::Base &player, std::vector<Item::Type> items)
    {
        auto found = 0;

        if (items.size() > 0)
        {
            for (auto i = 0; i < items.size(); i++)
            {
                if (FIND_ITEM(player, items[i]) >= 0)
                {
                    found++;
                }
            }
        }

        return found == items.size();
    }

    int COUNT_ITEMS(Character::Base &player, std::vector<Item::Type> items)
    {
        auto found = 0;

        for (auto i = 0; i < items.size(); i++)
        {
            if (FIND_ITEM(player, items[i]) >= 0)
            {
                found++;
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
                        found = VERIFY_ITEMS(player, {player.Skills[i].Requirement});
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

    bool VERIFY_ANY_SKILLS(Character::Base &player, std::vector<Skill::Type> skills)
    {
        auto found = false;

        if (player.Skills.size() > 0 && skills.size() > 0)
        {
            for (auto i = 0; i < skills.size(); i++)
            {
                if (Character::VERIFY_SKILL(player, skills[i]))
                {
                    found = true;

                    break;
                }
            }
        }

        return found;
    }

    bool VERIFY_ALL_SKILLS(Character::Base &player, std::vector<Skill::Type> skills)
    {
        auto found = 0;

        if (player.Skills.size() > 0 && skills.size() > 0)
        {
            for (auto i = 0; i < skills.size(); i++)
            {
                if (Character::VERIFY_SKILL(player, skills[i]))
                {
                    found++;
                }
            }
        }

        return found == skills.size();
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

        if (player.Skills.size() > 0 && skill != Skill::Type::NONE && items.size() > 0)
        {
            for (auto i = 0; i < player.Skills.size(); i++)
            {
                if (player.Skills[i].Type == skill)
                {
                    for (auto j = 0; j < items.size(); j++)
                    {
                        if (FIND_ITEM(player, items[j]) >= 0)
                        {
                            found++;
                        }
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
        return player.Items.size() <= player.ITEM_LIMIT;
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

    void REMOVE_CODEWORD(Character::Base &player, Codeword::Type codeword)
    {
        if (VERIFY_CODEWORD(player, codeword))
        {
            auto result = FIND_CODEWORD(player, codeword);

            if (result >= 0)
            {
                player.Codewords.erase(player.Codewords.begin() + result);
            }
        }
    }

    void GET_UNIQUE_ITEMS(Character::Base &player, std::vector<Item::Type> items)
    {
        for (auto i = 0; i < items.size(); i++)
        {
            if (!VERIFY_ITEMS(player, {items[i]}))
            {
                player.Items.push_back(items[i]);
            }
        }
    }

    void LOSE_ITEMS(Character::Base &player, std::vector<Item::Type> items)
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

    void LOSE_SKILLS(Character::Base &player, std::vector<Skill::Type> skills)
    {
        if (player.Skills.size() > 0 && skills.size() > 0)
        {
            for (auto i = 0; i < skills.size(); i++)
            {
                auto result = FIND_SKILL(player, skills[i]);

                if (result >= 0)
                {
                    player.LostSkills.push_back(player.Skills[result]);

                    player.Skills.erase(player.Skills.begin() + result);
                }
            }
        }
    }

    void GAIN_LIFE(Character::Base &player, int life)
    {
        if ((life < 0 && !player.IsImmortal) || life > 0)
        {
            player.Life += life;
        }

        if (player.Life < 0)
        {
            player.Life = 0;
        }
        else if (player.Life > player.MAX_LIFE_LIMIT)
        {
            player.Life = player.MAX_LIFE_LIMIT;
        }
    }

    void GAIN_MONEY(Character::Base &player, int money)
    {
        player.Money += money;

        if (player.Money < 0)
        {
            player.Money = 0;
        }
    }

    void LOSE_POSSESSIONS(Character::Base &player)
    {
        player.LostItems = player.Items;

        player.Items.clear();
    }

    void LOSE_ALL(Character::Base &player)
    {
        player.LostMoney = player.LostMoney;
        player.Money = 0;

        LOSE_POSSESSIONS(player);
    }

    void SCORE(Character::Base &player, int &scorer, int score)
    {
        scorer += score;

        if (scorer < 0)
        {
            scorer = 0;
        }
    }

} // namespace Character

#endif
