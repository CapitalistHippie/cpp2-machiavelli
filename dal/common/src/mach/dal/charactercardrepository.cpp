#include "mach/dal/charactercardrepository.h"

using namespace mach;
using namespace mach::dal;

CharacterCardRepository::CharacterCardRepository()
{
    infra::CsvParser parser;
    parser.OpenFile("assets/karakterkaarten.csv");

    infra::CsvRow row;

    while (parser.ParseNextRow(row) != infra::ParsingStatus::EndOfFile)
    {
        models::CharacterCard characterCard;
        row >> characterCard.number >> characterCard.name;

        characterCards.push_back(std::move(characterCard));
    }
}

const std::vector<models::CharacterCard>& CharacterCardRepository::GetCards() const
{
    return characterCards;
}
