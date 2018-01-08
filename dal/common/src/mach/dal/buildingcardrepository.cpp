#include "mach/dal/buildingcardrepository.h"

using namespace mach;
using namespace mach::dal;

models::BuildingColor BuildingColorStringToEnum(const std::string& buildingColor)
{
    if (buildingColor == "geel")
    {
        return models::BuildingColor::Yellow;
    }
    else if (buildingColor == "groen")
    {
        return models::BuildingColor::Green;
    }
    else if (buildingColor == "blauw")
    {
        return models::BuildingColor::Blue;
    }
    else if (buildingColor == "rood")
    {
        return models::BuildingColor::Red;
    }
    else if (buildingColor == "lila")
    {
        return models::BuildingColor::Lilac;
    }
}

BuildingCardRepository::BuildingCardRepository()
{
    infra::CsvParser parser;
    parser.OpenFile("assets/bouwkaarten.csv");

    infra::CsvRow row;

    while (parser.ParseNextRow(row) != infra::ParsingStatus::EndOfFile)
    {
        std::string buildingColorBuffer;

        models::BuildingCard buildingCard;
        row >> buildingCard.name >> buildingCard.cost >> buildingColorBuffer >> buildingCard.description;

        buildingCard.color = BuildingColorStringToEnum(buildingColorBuffer);

        buildingCards.push_back(std::move(buildingCard));
    }
}

const std::vector<models::BuildingCard>& BuildingCardRepository::GetCards() const
{
    return buildingCards;
}
