#ifndef __ITEMS__HPP__
#define __ITEMS__HPP__

#include <map>

namespace Item
{
    enum class Type
    {
        NONE = -1,
        SWORD,
        BLOWGUN,
        MAGIC_AMULET,
        MAGIC_WAND,
        MAIZE_CAKES,
        JADE_BEAD,
        LETTER_OF_INTRODUCTION,
        MAN_OF_GOLD,
        WATERSKIN,
        ROPE,
        FIREBRAND,
        POT_OF_DYE,
        CHILLI_PEPPERS,
        PAPAYA,
        SERPENT_BRACELET,
        GREEN_MIRROR,
        MAGIC_DRINK,
        JADE_SWORD,
        OWL,
        TERRACOTTA_EFFIGY,
        INCENSE,
        LOBSTER_POT,
        SHAWL,
        PARCEL_OF_SALT,
        SALTED_MEAT,
        HAUNCH_OF_VENISON,
        FLINT_KNIFE,
        BLANKET,
        GOLD_DIADEM,
        BROTHERS_SKULL,
        GOLDEN_HELMET,
        CHALICE_OF_LIFE,
        STONE,
        PADDLE,
        LUMP_OF_CHARCOAL,
        HAMMER,
        HYDRA_BLOOD_BALL,
        SPEAR,
        POLE,
        IVORY_RING,
        SHELL_NECKLACE,
        First = SWORD,
        Last = SHELL_NECKLACE
    };

    class Base
    {
    public:
        std::string Name;
        std::string Description;

        Item::Type Type = Item::Type::NONE;

        int Charge = -1;

        Base()
        {
        }

        Base(const char *name, const char *description, Item::Type type)
        {
            Name = name;
            Description = description;
            Type = type;
        }

        Base(const char *name, const char *description, Item::Type type, int charge)
        {
            Name = name;
            Description = description;
            Type = type;
            Charge = charge;
        }
    };

    auto SWORD = Item::Base("SWORD", "SWORD", Item::Type::SWORD);
    auto BLOWGUN = Item::Base("BLOWGUN", "BLOWGUN", Item::Type::BLOWGUN);
    auto MAGIC_AMULET = Item::Base("MAGIC AMULET", "MAGIC AMULET", Item::Type::MAGIC_AMULET);
    auto MAGIC_WAND = Item::Base("MAGIC WAND", "MAGIC WAND", Item::Type::MAGIC_WAND);
    auto MAIZE_CAKES = Item::Base("MAIZE CAKES", "MAIZE CAKES", Item::Type::MAIZE_CAKES);
    auto JADE_BEAD = Item::Base("JADE BEAD", "JADE BEAD", Item::Type::JADE_BEAD);
    auto LETTER_OF_INTRODUCTION = Item::Base("LETTER OF INTRODUCTION", "LETTER OF INTRODUCTION", Item::Type::LETTER_OF_INTRODUCTION);
    auto MAN_OF_GOLD = Item::Base("MAN OF GOLD", "MAN OF GOLD", Item::Type::MAN_OF_GOLD);
    auto WATERSKIN = Item::Base("WATERSKIN", "WATERSKIN", Item::Type::WATERSKIN);
    auto ROPE = Item::Base("ROPE", "ROPE", Item::Type::ROPE);
    auto FIREBRAND = Item::Base("FIREBRAND", "FIREBRAND", Item::Type::FIREBRAND);
    auto POT_OF_DYE = Item::Base("POT OF DYE", "POT OF DYE", Item::Type::POT_OF_DYE);
    auto CHILLI_PEPPERS = Item::Base("CHILLI PEPPERS", "CHILLI PEPPERS", Item::Type::CHILLI_PEPPERS);
    auto PAPAYA = Item::Base("PAPAYA", "PAPAYA", Item::Type::PAPAYA);
    auto SERPENT_BRACELET = Item::Base("SERPENT BRACELET", "SERPENT BRACELET", Item::Type::SERPENT_BRACELET);
    auto GREEN_MIRROR = Item::Base("GREEN MIRROR", "GREEN MIRROR", Item::Type::GREEN_MIRROR);
    auto MAGIC_DRINK = Item::Base("MAGIC DRINK", "MAGIC DRINK", Item::Type::MAGIC_DRINK);
    auto JADE_SWORD = Item::Base("JADE SWORD", "JADE SWORD", Item::Type::JADE_SWORD);
    auto OWL = Item::Base("OWL", "OWL", Item::Type::OWL);
    auto TERRACOTTA_EFFIGY = Item::Base("TERRACOTTA EFFIGY", "TERRACOTTA EFFIGY", Item::Type::TERRACOTTA_EFFIGY);
    auto INCENSE = Item::Base("INCENSE", "INCENSE", Item::Type::INCENSE);
    auto LOBSTER_POT = Item::Base("LOBSTER POT", "LOBSTER POT", Item::Type::LOBSTER_POT);
    auto SHAWL = Item::Base("SHAWL", "SHAWL", Item::Type::SHAWL);
    auto PARCEL_OF_SALT = Item::Base("PARCEL OF SALT", "PARCEL OF SALT", Item::Type::PARCEL_OF_SALT);
    auto SALTED_MEAT = Item::Base("SALTED MEAT", "SALTED MEAT", Item::Type::SALTED_MEAT);
    auto HAUNCH_OF_VENISON = Item::Base("HAUNCH OF VENISON", "HAUNCH OF VENISON", Item::Type::HAUNCH_OF_VENISON);
    auto FLINT_KNIFE = Item::Base("FLINT KNIFE", "FLINT KNIFE", Item::Type::FLINT_KNIFE);
    auto BLANKET = Item::Base("BLANKET", "BLANKET", Item::Type::BLANKET);
    auto GOLD_DIADEM = Item::Base("GOLD DIADEM", "GOLD DIADEM", Item::Type::GOLD_DIADEM);
    auto BROTHERS_SKULL = Item::Base("BROTHER's SKULL", "BROTHER's SKULL", Item::Type::BROTHERS_SKULL);
    auto GOLDEN_HELMET = Item::Base("GOLDEN HELMET", "GOLDEN HELMET", Item::Type::GOLDEN_HELMET);
    auto CHALICE_OF_LIFE = Item::Base("CHALICE OF LIFE", "CHALICE OF LIFE", Item::Type::CHALICE_OF_LIFE);
    auto STONE = Item::Base("STONE", "STONE", Item::Type::STONE);
    auto PADDLE = Item::Base("PADDLE", "PADDLE", Item::Type::PADDLE);
    auto LUMP_OF_CHARCOAL = Item::Base("LUMP OF CHARCOAL", "LUMP OF CHARCOAL", Item::Type::LUMP_OF_CHARCOAL);
    auto HAMMER = Item::Base("HAMMER", "HAMMER", Item::Type::HAMMER);
    auto HYDRA_BLOOD_BALL = Item::Base("HYDRA BLOOD BALL", "HYDRA BLOOD BALL", Item::Type::HYDRA_BLOOD_BALL);
    auto SPEAR = Item::Base("SPEAR", "SPEAR", Item::Type::SPEAR);
    auto POLE = Item::Base("POLE", "POLE", Item::Type::POLE);
    auto IVORY_RING = Item::Base("IVORY RING", "IVORY RING", Item::Type::IVORY_RING);
    auto SHELL_NECKLACE = Item::Base("SHELL NECKLACE", "SHELL NECKLACE", Item::Type::SHELL_NECKLACE);

    std::vector<Item::Type> UniqueItems = {
        Item::Type::LETTER_OF_INTRODUCTION,
        Item::Type::SERPENT_BRACELET,
        Item::Type::GREEN_MIRROR,
        Item::Type::JADE_SWORD,
        Item::Type::OWL,
        Item::Type::TERRACOTTA_EFFIGY,
        Item::Type::LOBSTER_POT,
        Item::Type::SHAWL,
        Item::Type::GOLD_DIADEM,
        Item::Type::BROTHERS_SKULL,
        Item::Type::GOLDEN_HELMET,
        Item::Type::CHALICE_OF_LIFE,
        Item::Type::HYDRA_BLOOD_BALL};

    bool IsUnique(Item::Type item)
    {
        auto unique = false;

        for (auto i = 0; i < Item::UniqueItems.size(); i++)
        {
            if (Item::UniqueItems[i] == item)
            {
                unique = true;

                break;
            }
        }

        return unique;
    }

    void REMOVE(std::vector<Item::Base> &items, Item::Base item)
    {
        if (items.size() > 0)
        {
            for (auto i = 0; i < items.size(); i++)
            {
                if (items[i].Type == item.Type)
                {
                    items.erase(items.begin() + i);

                    break;
                }
            }
        }
    }

    void ADD(std::vector<Item::Base> &items, Item::Base item)
    {
        if (items.size() > 0)
        {
            auto found = 0;

            for (auto i = 0; i < items.size(); i++)
            {
                if (items[i].Type == item.Type)
                {
                    found++;
                }
            }

            if (found == 0)
            {
                items.push_back(item);
            }
        }
        else
        {
            items.push_back(item);
        }
    }

    bool VERIFY(std::vector<Item::Base> &items, Item::Base item)
    {
        auto has = false;

        if (items.size() > 0)
        {
            for (auto i = 0; i < items.size(); i++)
            {
                if (items[i].Type == item.Type)
                {
                    has = true;

                    break;
                }
            }
        }

        return has;
    }

    int FIND(std::vector<int> list, int item)
    {
        auto found = -1;

        for (auto i = 0; i < list.size(); i++)
        {
            if (list[i] == item)
            {
                found = i;

                break;
            }
        }

        return found;
    }

    int FIND_TYPE(std::vector<Item::Base> list, Item::Type item)
    {
        auto found = -1;

        for (auto i = 0; i < list.size(); i++)
        {
            if (list[i].Type == item)
            {
                found = i;

                break;
            }
        }

        return found;
    }

    int COUNT_TYPES(std::vector<Item::Base> &list, Item::Type item)
    {
        auto found = 0;

        for (auto i = 0; i < list.size(); i++)
        {
            if (list[i].Type == item)
            {
                found++;
            }
        }

        return found;
    }

    int FIND_LEAST(std::vector<Item::Base> &list, Item::Type item)
    {
        auto found = -1;

        auto min = 255;

        for (auto i = 0; i < list.size(); i++)
        {
            if (list[i].Type == item)
            {
                if (list[i].Charge < min)
                {
                    found = i;

                    min = list[i].Charge;
                }
            }
        }

        return found;
    }

} // namespace Item
#endif
