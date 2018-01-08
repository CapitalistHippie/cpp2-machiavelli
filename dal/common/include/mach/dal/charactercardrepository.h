#ifndef MACHIAVELLI_MACH_DAL_CHARACTERCARDREPOSITORY_H_INCLUDED
#define MACHIAVELLI_MACH_DAL_CHARACTERCARDREPOSITORY_H_INCLUDED

#include <vector>

#include <mach/infra/csvparser.h>

#include "mach/dal/models/charactercard.h"

namespace mach
{
namespace dal
{
class CharacterCardRepository
{
    std::vector<models::CharacterCard> characterCards;

  public:
    CharacterCardRepository();

    const std::vector<models::CharacterCard>& GetCards() const;

}; // class CharacterCardRepository
} // namespace dal
} // namespace mach

#endif // #ifndef MACHIAVELLI_MACH_DAL_CHARACTERCARDREPOSITORY_H_INCLUDED
