#include "stdafx.h"
#include "TileNames.hpp"
namespace mmt_gd
{
std::vector<std::string> TileNames::FolderNames = {"grassTiles", "desertTiles", "winterTiles", "mountainTiles"};

std::vector<std::vector<std::string>> TileNames::LevelNamesNouns =
        {{"Meadow", "Field", "Pasture", "Glade", "Valley", "Grove", "Hollow", "Clearing", "Vale", "Sward"},
         {"Wasteland", "Dunes", "Oasis", "Sands", "Desert", "Expanse", "Badlands", "Canyon", "Plateau", "Arroyo"},
     {"Snowfield", "Tundra", "Valley", "Fjord", "Hollow", "Copse", "Dale", "Ridge", "Timberland", "Woods"},
     {"Peak", "Summit", "Ridge", "Crest", "Range", "Bluff", "Slope", "Cliff", "Highland", "Alps"}};

std::vector<std::vector<std::string>> TileNames::LevelNamesAdjectives =
        {{"Brisk", "Whispering", "Enchanted", "Swaying", "Tranquil", "Wild", "Mystical", "Gentle", "Serene", "Vibrant"},
         {"Sizzling", "Scorching", "Arid", "Golden", "Dusty", "Barren", "Sunbaked", "Sandy", "Parched", "Mirage-like"},
         {"Icy", "Frosty", "Snowy", "Chilly", "Arctic", "Nordic", "Frozen", "Wintry", "Frigid", "Glacial"},
         {"Rocky", "Steep", "Lofty", "Craggy", "Majestic", "Rugged", "Towering", "Alpine", "Sheer", "Elevated"}};

std::vector<std::vector<sf::Color>> TileNames::TileTypeBaseColors =
    {{{71, 163, 71, 255}, {171, 100, 69, 255}, {184, 111, 80, 255}, {62, 39, 49, 255}},        //grasslands
     {{250, 182, 29, 255}, {167, 188, 23, 255}, {184, 111, 80, 255}, {165, 124, 0, 255}},      //desert
     {{203, 212, 229, 255}, {227, 244, 255, 255}, {182, 180, 159, 255}, {205, 218, 241, 255}}, //winter
     {{122, 97, 91, 255}, {156, 104, 59, 255}, {184, 111, 80, 255}, {170, 170, 167, 255}}};    //mountain


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

sf::Color TileNames::getTileTypeColor(int id, int tileType)
{
    sf::Color base = sf::Color{0, 0, 0, 0};

    if (id >= 0 && id < getLevelCount())
    {
        if (tileType >= 0 && tileType < 4)
        {
            base = TileTypeBaseColors[id][tileType];
        }
    }

    return base;
}

int TileNames::getLevelCount()
{
    return FolderNames.size();
}

} // namespace mmt_gd