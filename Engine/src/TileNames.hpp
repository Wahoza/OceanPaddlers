//this class exists for reading in the storage locations, it should never be modified by another class
#pragma once
#include "stdafx.h"
namespace mmt_gd
{
class TileNames
{
public:
    static std::string getFolderLocation(int id);
    static std::string getLevelNoun(int id, int location);
    static std::string getLevelAdjective(int id, int location);
    static int         getLevelCount();

private:
    static std::vector<std::string> FolderNames;

    static std::vector<std::vector<std::string>> LevelNamesNouns;

    static std::vector<std::vector<std::string>> LevelNamesAdjectives;
};
};
