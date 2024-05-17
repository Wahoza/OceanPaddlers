#include "stdafx.h"
#include "TileNames.hpp"
namespace mmt_gd
{
    std::vector<std::string> TileNames::FolderNames = {"grassTiles", "desertTiles"};

    std::vector<std::vector<std::string>> TileNames::LevelNamesNouns =
        {{"Meadow", "Field", "Pasture", "Glade", "Valley", "Grove", "Hollow", "Clearing", "Vale", "Sward"},
         {"Wasteland", "Dunes", "Oasis", "Sands", "Desert", "Expanse", "Badlands", "Canyon", "Plateau", "Arroyo"}};

    std::vector<std::vector<std::string>> TileNames::LevelNamesAdjectives =
        {{"Brisk", "Whispering", "Enchanted", "Swaying", "Tranquil", "Wild", "Mystical", "Gentle", "Serene", "Vibrant"},
         {"Sizzling", "Scorching", "Arid", "Golden", "Dusty", "Barren", "Sunbaked", "Sandy", "Parched", "Mirage-like"}};

std::string TileNames::getFolderLocation(int id)
{
    if (id >= FolderNames.size())
    {
        return FolderNames.back();
    }

    return FolderNames[id];
}
std::string TileNames::getLevelNoun(int id, int location)
{
    if (id < 0 || id >= LevelNamesNouns.size())
    {
        return ""; // Return an empty string if id is out of bounds
    }

    // Retrieve the vector of nouns corresponding to the id
    const std::vector<std::string>& tempString = LevelNamesNouns[id];

    // Check if location is within bounds of the vector
    if (location < 0 || location >= tempString.size())
    {
        return ""; // Return an empty string if location is out of bounds
    }

    // Return the noun at the specified location
    return tempString[location];
}
std::string TileNames::getLevelAdjective(int id, int location)
{
    if (id < 0 || id >= LevelNamesAdjectives.size())
    {
        return ""; // Return an empty string if id is out of bounds
    }

    // Retrieve the vector of nouns corresponding to the id
    const std::vector<std::string>& tempString = LevelNamesAdjectives[id];

    // Check if location is within bounds of the vector
    if (location < 0 || location >= tempString.size())
    {
        return ""; // Return an empty string if location is out of bounds
    }

    // Return the noun at the specified location
    return tempString[location];
}

int TileNames::getLevelCount()
{
    return FolderNames.size();
}

} // namespace mmt_gd