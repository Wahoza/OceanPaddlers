#pragma once
#include "stdafx.h"
#include "GameState.hpp"
#include "FinalFrontier/dll_export.hpp"
namespace mmt_gd
{
class FINALFRONTIER_API HighScoreState final : public GameState
{
public:
    HighScoreState(GameStateManager* gameStateManager, Game* game);
    bool compareInnerTuple(const std::tuple<std::string, int, int, int>& a,
                           const std::tuple<std::string, int, int, int>& b,
                           int                                           position);
    void init() override;
    void exit() override;

    void update(float deltaTime) override;
    void draw() override;

private:
    std::vector<std::string> splitString(const std::string& input, char delimiter);
    void SortNext(int sortPos);

    int nextSortVal           = 0;

    int maxDisplayedScores = 11;

    std::vector<std::vector<sf::Text>> scoreLines;
    sf::Font              m_font;
    sf::Color             m_color = {255, 255, 255, 255};

    std::vector<std::tuple<std::string, int, int, int>> scoreList;

    float              BoundryLeft       = 150;
    float              BoundryTop        = 200;
    std::vector<float> OnScreenPositions = {0,0,0,0,0,0};
    float                           BonusSpacing      = 30;
    std::vector<sf::RectangleShape> Lines;
    float                           Line_Text_Offset = 10;
    sf::Vector2f     ScreenSize;
    sf::Vector2f     ScreenTopLeft;

    sf::Text         HighScoreTitle;
    sf::Text         SortedByText;
    sf::Text         Instructions;
    float currentxAxis = 0;
    float lastxAxis = 0;

    int currentSort;
    bool initButtonsDown = true;
};
}