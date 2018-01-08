#ifndef MACHIAVELLI_MACH_DAL_MODELS_CHARACTERCARD_H_INCLUDED
#define MACHIAVELLI_MACH_DAL_MODELS_CHARACTERCARD_H_INCLUDED

#include <istream>
#include <ostream>
#include <string>

namespace mach
{
namespace dal
{
namespace models
{
struct CharacterCard
{
    int number;
    std::string name;
}; // class CharacterCard
} // namespace models
} // namespace dal
} // namespace mach

inline std::ostream& operator<<(std::ostream& os, const mach::dal::models::CharacterCard& card)
{
    return os << card.number << '|' << card.name << '|';
}
inline std::istream& operator>>(std::istream& is, mach::dal::models::CharacterCard& card)
{
    is >> card.number;
    is.ignore();
    std::getline(is, card.name, '|');
    return is;
}

#endif // #ifndef MACHIAVELLI_MACH_DAL_MODELS_CHARACTERCARD_H_INCLUDED
