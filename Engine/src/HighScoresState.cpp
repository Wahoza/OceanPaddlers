#include "stdafx.h"
#include "HighScoresState.hpp"
#include "Game.hpp"
#include "InputManager.hpp"
#include <algorithm>
namespace mmt_gd
{
HighScoreState::HighScoreState(GameStateManager* gameStateManager, Game* game) :
GameState(gameStateManager, game)
{
}
bool HighScoreState::compareInnerTuple(const std::tuple<std::string, int, int, int>& a,
                                       const std::tuple<std::string, int, int, int>& b,
                                       int                                           position)
{
    switch (position)
    {
        case 0:
            return false;
        case 1:
            return std::get<1>(a) > std::get<1>(b);
        case 2:
            return std::get<2>(a) > std::get<2>(b);
        case 3:
            return std::get<3>(a) > std::get<3>(b);
        default:
            // Handle error: Invalid position
            return false;
    }
}

void HighScoreState::init()
{
    PROFILE_FUNCTION();
    initButtonsDown = true;
    ScreenSize = m_game->getWindow().getView().getSize();
    ScreenTopLeft = m_game->getWindow().getView().getCenter() - ScreenSize/2.f;

    for (int i = 0; i < OnScreenPositions.size(); i++)
    {
        float RankAdjustment = 0;
        if (i > 0)
        {
            RankAdjustment = -100;
        }
        
        OnScreenPositions[i] = BoundryLeft * 2 + RankAdjustment +
                               ((ScreenSize.x - BoundryLeft) / OnScreenPositions.size() + BonusSpacing) * i +
                               ScreenTopLeft.x;
    }
    
    std::ifstream inputFile("../assets/scores.txt");
    if (inputFile.is_open())
    {
        // Read the file contents
        std::string fileContent((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
        
        std::vector<std::string> substrings = splitString(fileContent, 10);

        for (std::string _string : substrings)
        {
            std::vector<std::string> t_string = splitString(_string, 44);

            if (t_string.size() < 4)
            {
                continue;
            }

            scoreList.push_back({t_string[0], std::stoi(t_string[1]), std::stoi(t_string[2]), std::stoi(t_string[3])});
        }
        inputFile.close();
    }

    //Line Gen
    {
        float ypos = BoundryTop + ScreenTopLeft.y;

        for (int x = 0; x < OnScreenPositions.size(); x++)
        {
            sf::RectangleShape rect;
            rect.setFillColor(m_color);
            rect.setPosition({OnScreenPositions[x] - Line_Text_Offset, ypos - 20});
            rect.setSize({5, ScreenSize.y});
            Lines.push_back(rect);
        }

        float xlength = OnScreenPositions[OnScreenPositions.size() - 1] - OnScreenPositions[0];
        for (int i = 0; i<2; i++)
        {
            ypos = BoundryTop + ScreenTopLeft.y + (ScreenSize.y - BoundryTop) / maxDisplayedScores * i;

            float BonusYOffset = -20;
            if (i > 0)
            {
                BonusYOffset = -30;
            }
            sf::RectangleShape rect;
            rect.setFillColor(m_color);
            rect.setPosition({OnScreenPositions[0] - Line_Text_Offset, ypos + BonusYOffset});
            rect.setSize({xlength, 5});
            Lines.push_back(rect);
        }
    }

    if (!m_font.loadFromFile("../assets/HudAssets/PixelFont.ttf"))
    {
        sf::err() << "Could not load font\n";
    }
    m_font.setSmooth(false);

    HighScoreTitle.setFillColor(m_color);
    HighScoreTitle.setFont(m_font);
    HighScoreTitle.setPosition(ScreenTopLeft + sf::Vector2f{30, 20});
    HighScoreTitle.setString("Highscores");
    HighScoreTitle.setScale({3, 3});
    
    SortedByText.setFillColor(m_color);
    SortedByText.setFont(m_font);
    SortedByText.setPosition(ScreenTopLeft + sf::Vector2f{30, 180});
    SortedByText.setString("");
    SortedByText.setScale({0.8, 0.8});

    Instructions.setFillColor(m_color);
    Instructions.setFont(m_font);
    Instructions.setPosition(ScreenTopLeft + sf::Vector2f{30, 260});
    Instructions.setString("Exit: \nSpace/A \n\nSort: \nJoystick");
    Instructions.setScale({0.8, 0.8});

    SortNext(3);
    currentSort = 3;
}
void HighScoreState::exit()
{
    PROFILE_FUNCTION();
    Lines = {};
    scoreLines = {};
    scoreList  = {};
}
void HighScoreState::update(float deltaTime)
{
    PROFILE_FUNCTION();
    currentxAxis = sf::Joystick::getAxisPosition(0, sf::Joystick::X);
    if (InputManager::getInstance().isKeyDown("left", 0) || InputManager::getInstance().isKeyDown("left",1))
    {
        currentxAxis += -100;
    }
    if (InputManager::getInstance().isKeyDown("right", 0) || InputManager::getInstance().isKeyDown("right", 1))
    {
        currentxAxis += 100;
    }
    //todo: for holding direction: if held long enough switch to next, decrease waittime for next switch->itterate through many letters with ease
    if (abs(currentxAxis) >= 90 && abs(lastxAxis) <= 90)
    {
        //use to select next or previous char
        int direction = int(currentxAxis / abs(currentxAxis));
        currentSort += direction;
        if (currentSort >= 4)
        {
            currentSort = 1;
        }
        if (currentSort < 1)
        {
            currentSort = 3;
        }
        //std::cout << currentSort << std::endl;
        SortNext(currentSort);
    }

    lastxAxis = currentxAxis;


    if (InputManager::getInstance().isKeyPressed("Select") || sf::Joystick::isButtonPressed(0, 0) ||
        sf::Joystick::isButtonPressed(1, 0))
    {
        if (!initButtonsDown)
        {
            m_gameStateManager->setState("MenuState");
        }
    }
    else
    {
        initButtonsDown = false;
    }
}
void HighScoreState::draw()
{
    PROFILE_FUNCTION();
    for (auto& item : Lines)
    {
        m_game->getWindow().draw(item);
    }
    for(auto& item : scoreLines)
    {
        for (auto& _text : item)
        {
            m_game->getWindow().draw(_text);
        }
    }
    m_game->getWindow().draw(HighScoreTitle);
    m_game->getWindow().draw(SortedByText);
    m_game->getWindow().draw(Instructions);
}
void HighScoreState::SortNext(int sortPos)
{
    //dont sort by 0
    if (sortPos == 0)
    {
        return;
    }
    {
        std::string sortedByString = "";
        switch (sortPos)
        {
            case 0:
                sortedByString = "Name";
                break;
            case 1:
                sortedByString = "Dist";
                break;
            case 2:
                sortedByString = "Eggs";
                break;
            case 3:
                sortedByString = "Score";
                break;
        }
        sortedByString = "Sorted by \n <" + sortedByString + ">";
        SortedByText.setString(sortedByString);
    }
    
    std::sort(scoreList.begin(),
              scoreList.end(),
              [&](const std::tuple<std::string, int, int, int>& a, const std::tuple<std::string, int, int, int>& b)
              { return compareInnerTuple(a, b, sortPos); });

    scoreLines   = {};

    //naming the lines
    {
        float ypos = BoundryTop + ScreenTopLeft.y;

        std::vector<sf::Text> SubVector;
        sf::Text Rank;
        Rank.setFillColor(m_color);
        Rank.setFont(m_font);
        Rank.setPosition({OnScreenPositions[0], ypos});
        Rank.setString("Rank");
        SubVector.push_back(Rank);

        sf::Text Score;
        Score.setFillColor(m_color);
        Score.setFont(m_font);
        Score.setPosition({OnScreenPositions[1], ypos});
        Score.setString("Score");
        SubVector.push_back(Score);

        sf::Text Eggs;
        Eggs.setFillColor(m_color);
        Eggs.setFont(m_font);
        Eggs.setPosition({OnScreenPositions[2], ypos});
        Eggs.setString("Eggs");
        SubVector.push_back(Eggs);

        sf::Text Distance;
        Distance.setFillColor(m_color);
        Distance.setFont(m_font);
        Distance.setPosition({OnScreenPositions[3], ypos});
        Distance.setString("Distance");
        SubVector.push_back(Distance);

        sf::Text Name;
        Name.setFillColor(m_color);
        Name.setFont(m_font);
        Name.setPosition({OnScreenPositions[4], ypos});
        Name.setString("Name");

        SubVector.push_back(Name);
        scoreLines.push_back(SubVector);
    }

    int iterator = 0;
    for (auto& item : scoreList)
    {
        float ypos = BoundryTop + ScreenTopLeft.y + (ScreenSize.y - BoundryTop) / maxDisplayedScores * (iterator + 1);
        std::vector<sf::Text> SubVector;

        sf::Text              Rank;
        Rank.setFillColor(m_color);
        Rank.setFont(m_font);
        Rank.setPosition({OnScreenPositions[0], ypos});
        Rank.setString(std::to_string(iterator + 1));
        SubVector.push_back(Rank);

        sf::Text Score;
        Score.setFillColor(m_color);
        Score.setFont(m_font);
        Score.setPosition({OnScreenPositions[1], ypos});
        Score.setString(std::to_string(std::get<3>(item)));
        SubVector.push_back(Score);

        sf::Text Eggs;
        Eggs.setFillColor(m_color);
        Eggs.setFont(m_font);
        Eggs.setPosition({OnScreenPositions[2], ypos});
        Eggs.setString(std::to_string(std::get<2>(item)));
        SubVector.push_back(Eggs);

        sf::Text Distance;
        Distance.setFillColor(m_color);
        Distance.setFont(m_font);
        Distance.setPosition({OnScreenPositions[3], ypos});
        Distance.setString(std::to_string(std::get<1>(item)) + "m");
        SubVector.push_back(Distance);

        sf::Text Name;
        Name.setFillColor(m_color);
        Name.setFont(m_font);
        Name.setPosition({OnScreenPositions[4], ypos});
        Name.setString(std::get<0>(item));
        SubVector.push_back(Name);

        scoreLines.push_back(SubVector);

        if (iterator >= maxDisplayedScores -2)
        {
            return;
        }
        iterator++;
    }
}
std::vector<std::string> HighScoreState::splitString(const std::string& input, char delimiter)
{
    std::vector<std::string> result;
    std::istringstream       stream(input);
    std::string              token;

    while (std::getline(stream, token, delimiter))
    {
        result.push_back(token);
    }

    return result;
}
} // namespace mmt_gd