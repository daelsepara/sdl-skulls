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
        First = SWORD,
        Last = SERPENT_BRACELET
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
        {Item::Type::SERPENT_BRACELET, "SERPENT BRACELET"}};

    std::vector<Item::Type> UniqueItems = {
        Item::Type::LETTER_OF_INTRODUCTION,
        Item::Type::SERPENT_BRACELET};

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

} // namespace Item
#endif
