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
        MAGIC_POTION,
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
        First = SWORD,
        Last = GOLD_DIADEM
    };

    std::map<Item::Type, const char *> Descriptions = {
        {Item::Type::SWORD, "SWORD"},
        {Item::Type::BLOWGUN, "BLOWGUN"},
        {Item::Type::MAGIC_AMULET, "MAGIC AMULET"},
        {Item::Type::MAGIC_WAND, "MAGIC WAND"},
        {Item::Type::MAIZE_CAKES, "MAIZE CAKES"},
        {Item::Type::JADE_BEAD, "JADE BEAD"},
        {Item::Type::LETTER_OF_INTRODUCTION, "LETTER OF INTRODUCTION"},
        {Item::Type::MAN_OF_GOLD, "MAN OF GOLD"},
        {Item::Type::WATERSKIN, "WATERSKIN"},
        {Item::Type::ROPE, "ROPE"},
        {Item::Type::FIREBRAND, "FIREBRAND"},
        {Item::Type::POT_OF_DYE, "POT OF DYE"},
        {Item::Type::CHILLI_PEPPERS, "CHILLI PEPPERS"},
        {Item::Type::PAPAYA, "PAPAYA"},
        {Item::Type::SERPENT_BRACELET, "SERPENT BRACELET"},
        {Item::Type::GREEN_MIRROR, "GREEN MIRROR"},
        {Item::Type::MAGIC_POTION, "MAGIC POTION"},
        {Item::Type::JADE_SWORD, "JADE SWORD"},
        {Item::Type::OWL, "OWL"},
        {Item::Type::TERRACOTTA_EFFIGY, "TERRACOTTA EFFIGY"},
        {Item::Type::INCENSE, "INCENSE"},
        {Item::Type::LOBSTER_POT, "LOBSTER POT"},
        {Item::Type::SHAWL, "SHAWL"},
        {Item::Type::PARCEL_OF_SALT, "PARCEL OF SALT"},
        {Item::Type::SALTED_MEAT, "SALTED MEAT"},
        {Item::Type::HAUNCH_OF_VENISON, "HAUNCH OF VENISON"},
        {Item::Type::FLINT_KNIFE, "FLINT KNIFE"},
        {Item::Type::BLANKET, "BLANKET"},
        {Item::Type::GOLD_DIADEM, "GOLD DIADEM"}};

    std::vector<Item::Type> UniqueItems = {
        Item::Type::LETTER_OF_INTRODUCTION,
        Item::Type::SERPENT_BRACELET,
        Item::Type::GREEN_MIRROR,
        Item::Type::JADE_SWORD,
        Item::Type::OWL,
        Item::Type::TERRACOTTA_EFFIGY,
        Item::Type::LOBSTER_POT,
        Item::Type::SHAWL,
        Item::Type::GOLD_DIADEM};

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

    void REMOVE(std::vector<Item::Type> &items, Item::Type item)
    {
        if (items.size() > 0)
        {
            for (auto i = 0; i < items.size(); i++)
            {
                if (items[i] == item)
                {
                    items.erase(items.begin() + i);

                    break;
                }
            }
        }
    }

    void ADD(std::vector<Item::Type> &items, Item::Type item)
    {
        if (items.size() > 0)
        {
            auto found = 0;

            for (auto i = 0; i < items.size(); i++)
            {
                if (items[i] == item)
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

    bool VERIFY(std::vector<Item::Type> &items, Item::Type item)
    {
        auto has = false;

        if (items.size() > 0)
        {
            for (auto i = 0; i < items.size(); i++)
            {
                if (items[i] == item)
                {
                    has = true;

                    break;
                }
            }
        }

        return has;
    }

} // namespace Item
#endif
