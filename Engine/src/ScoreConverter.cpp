//Author: Kilian Kolb
//Description: Converts stored scores from given filepath to scores shown ingame. formating required is (name,distance,eggs/n)
//License: LICENSE.txt
#include "stdafx.h"
#include "ScoreConverter.hpp"
#include "ScoreDisplayComponent.hpp"
#include "GameObject.hpp"
#include "IComponent.hpp"
#include "IRenderComponent.hpp"
#include "GameObjectEvents.hpp"
#include "EventBus.hpp"
#include "RenderComponentEvents.hpp"

#include <algorithm>
#include <random>

namespace mmt_gd
{

ScoreConverter::ScoreConverter(GameObject& gameObject, sf::RenderWindow& renderWindow, std::string filePath, int displayedAmount) : IRenderComponent(gameObject, renderWindow)
{
    std::ifstream inputFile(filePath);
    if (inputFile.is_open())
    {
        // Read the file contents
        std::string fileContent((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
        inputFile.close();

        CreateScoreLabels(fileContent, displayedAmount);
    }
}

bool ScoreConverter::init()
{
    return true;
}

void ScoreConverter::CreateScoreLabels(std::string input, int displayedAmount)
{
    int                      counter    = 0;
    std::vector<std::string> substrings = splitString(input, 10);



    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    std::shuffle(substrings.begin(), substrings.end(), std::default_random_engine(std::rand()));


    for (size_t i = 0; i < 5 && i < substrings.size(); ++i)
    {
        std::vector<std::string> datastring = splitString(substrings[i], 44);

        try
        {
            if (datastring.size() <3)
            {
                continue;
            }
            int intValue = std::stoi(datastring[1]);
            //create new objects here!!

            string inputText = datastring[0] + "\n" + "Eggs: " + datastring[2];
            Vector2f scorePosition = {((float)intValue * 30), m_gameObject.getPosition().y};
           
            auto scoreObject = GameObject::create("score" + to_string(counter));

            scoreObject->setPosition(scorePosition);

            scoreObject->addComponent<ScoreDisplayComponent>(*scoreObject, m_renderWindow, inputText);




            EventBus::getInstance().fireEvent(std::make_shared<GameObjectCreateEvent>(scoreObject));
            

            //std::cout << "Parsed integer value: " << intValue << std::endl;
        } catch (const std::invalid_argument& ia)
        {
        } catch (const std::out_of_range& oor)
        {
        }
        counter++;
    }
}
std::vector<std::string> ScoreConverter::splitString(const std::string& input, char delimiter)
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

void ScoreConverter::draw()
{
}

void ScoreConverter::update(float deltaTime)
{
}
ScoreConverter::~ScoreConverter()
{
}
} // namespace mmt_gd
