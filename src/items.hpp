#ifndef __ITEMS__HPP__
#define __ITEMS__HPP__

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
        First = SWORD,
        Last = LETTER_OF_INTRODUCTION
    };

    std::map<Item::Type, const char *> Descriptions = {
        {Item::Type::SWORD, "SWORD"},
        {Item::Type::BLOWGUN, "BLOWGUN"},
        {Item::Type::MAGIC_AMULET, "MAGIC AMULET"},
        {Item::Type::MAGIC_WAND, "MAGIC WAND"},
        {Item::Type::MAIZE_CAKES, "MAIZE CAKES"},
        {Item::Type::JADE_BEAD, "JADE BEAD"},
        {Item::Type::LETTER_OF_INTRODUCTION, "LETTER OF INTRODUCTION"}};

} // namespace Item
#endif
