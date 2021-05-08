#ifndef __ITEMS__HPP__
#define __ITEMS__HPP__

enum class ItemType
{
    ANY = -1,
    SWORD,
    BLOWGUN,
    First = SWORD,
    Last = BLOWGUN
};

class Item
{
public:

    int Quantity = 0;
    const char *Name = NULL;
    ItemType Type = ItemType::ANY;

    Item(const char *name, ItemType type, int quantity)
    {
        Name = name;
        Type = type;
        Quantity = quantity;
    }

    Item(const char *name, int quantity)
    {
        Name = name;
        Quantity = quantity;
        Type = ItemType::ANY;
    }

    Item(const char *name)
    {
        Name = name;
        Quantity = 1;
        Type = ItemType::ANY;
    }
};
#endif
