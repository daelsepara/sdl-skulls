#ifndef __CODEWORDS__HPP__
#define __CODEWORDS__HPP__

#include <map>

namespace Codeword
{
    enum class Type
    {
        ANGEL,
        BLESSING,
        CALABASH,
        CENOTE,
        EB,
        IGNIS,
        OLMEK,
        PAKAL,
        POKTAPOK,
        PSYCHODUCT,
        SAKBE,
        SALVATION,
        SHADE,
        VENUS,
        ZAZ,
        ZOTZ
    };

    std::map<Codeword::Type, const char *> Descriptions = {
        {Codeword::Type::ANGEL, "Angel"},
        {Codeword::Type::BLESSING, "Blessing"},
        {Codeword::Type::CALABASH, "Calabash"},
        {Codeword::Type::CENOTE, "Cenote"},
        {Codeword::Type::EB, "Eb"},
        {Codeword::Type::IGNIS, "Ignis"},
        {Codeword::Type::OLMEK, "Olmek"},
        {Codeword::Type::PAKAL, "Pakal"},
        {Codeword::Type::POKTAPOK, "Poktapok"},
        {Codeword::Type::PSYCHODUCT, "Psychoduct"},
        {Codeword::Type::SAKBE, "Sakbe"},
        {Codeword::Type::SALVATION, "Salvation"},
        {Codeword::Type::SHADE, "Shade"},
        {Codeword::Type::VENUS, "Venus"},
        {Codeword::Type::ZAZ, "Zaz"},
        {Codeword::Type::ZOTZ, "Zotz"}};

} // namespace Codeword
#endif
