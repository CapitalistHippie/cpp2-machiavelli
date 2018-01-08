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
        // First column is just a number.
        row.IgnoreNextColumn();

        std::string characterName;
        row.ParseNextColumn(characterName);

        models::CharacterCard characterCard;
        characterCard.name = characterName;

        characterCards.push_back(std::move(characterCard));
    }
}

const std::vector<models::CharacterCard>& CharacterCardRepository::GetCards() const
{
    return characterCards;
}
