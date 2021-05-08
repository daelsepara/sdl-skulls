#ifndef __CHARACTER__HPP__
#define __CHARACTER__HPP__
#include <vector>

#include "items.hpp"
#include "skills.hpp"

class Character
{
public:
    const char *Name = NULL;

    int Life = 10;
    int Money = 10;

    std::vector<Skill> Skills = std::vector<Skill>();
    std::vector<Item> Items = std::vector<Item>();

    Character(const char *name)
    {
        Name = name;
    }

    Character()
    {
        Name = "The Hero";
    }
};
#endif
