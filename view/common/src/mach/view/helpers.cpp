#include "mach/view/helpers.h"

std::string mach::view::BuildingColorEnumToString(dal::models::BuildingColor buildingColor)
{
    std::string s;

    switch (buildingColor)
    {
        case dal::models::BuildingColor::Yellow:
            return "geel";
            break;
        case dal::models::BuildingColor::Green:
            return "groen";
            break;
        case dal::models::BuildingColor::Blue:
            return "blauw";
            break;
        case dal::models::BuildingColor::Red:
            return "rood";
            break;
        case dal::models::BuildingColor::Lilac:
            return "lila";
            break;
    }
}
std::string mach::view::BooleanToString(bool boolean)
{
    return boolean ? "True" : "False";
}
