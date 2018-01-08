#ifndef MACHIAVELLI_MACH_DAL_BUILDINGCARDREPOSITORY_H_INCLUDED
#define MACHIAVELLI_MACH_DAL_BUILDINGCARDREPOSITORY_H_INCLUDED

#include <vector>

#include <mach/infra/csvparser.h>

#include "mach/dal/models/buildingcard.h"

namespace mach
{
namespace dal
{
class BuildingCardRepository
{
    std::vector<models::BuildingCard> buildingCards;

  public:
    BuildingCardRepository();

    const std::vector<models::BuildingCard>& GetCards() const;

}; // class BuildingCardRepository
} // namespace dal
} // namespace mach

#endif // #ifndef MACHIAVELLI_MACH_DAL_BUILDINGCARDREPOSITORY_H_INCLUDED
