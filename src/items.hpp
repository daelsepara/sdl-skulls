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
        First = SWORD,
        Last = BLOWGUN
    };
} // namespace Item
#endif
