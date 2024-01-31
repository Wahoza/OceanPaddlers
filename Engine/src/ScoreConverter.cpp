#include "stdafx.h"
#include "ScoreConverter.hpp"
#include "ScoreDisplayComponent.hpp"
#include "GameObject.hpp"
#include "IComponent.hpp"
#include "IRenderComponent.hpp"
#include "GameObjectEvents.hpp"
#include "EventBus.hpp"
#include "RenderComponentEvents.hpp"
namespace mmt_gd
{

ScoreConverter::ScoreConverter(GameObject& gameObject, sf::RenderWindow& renderWindow, std::string filePath) : IComponent(gameObject), IRenderComponent(gameObject, renderWindow)
{
    std::ifstream inputFile(filePath);
    if (inputFile.is_open())
    {
        // Read the file contents
        std::string fileContent((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
        inputFile.close();

        CreateScoreLabels(fileContent);
    }else{
        std::cerr << "Error opening file: " << filePath << std::endl;
    }
}
bool ScoreConverter::init()
{
    return true;
}
void ScoreConverter::CreateScoreLabels(std::string input)
{
    int                      counter    = 0;
    std::vector<std::string> substrings = splitString(input, 10);
    for (std::string& substring : substrings)
    {

        std::vector<std::string> datastring = splitString(input, 44);

        try
        {
            int intValue = std::stoi(datastring[1]);
            //create new objects here!!

            string inputText = datastring[0] + "\n" + "Eggs: " + datastring[2];
            Vector2f scorePosition = {-(float)intValue * 30,IComponent::m_gameObject.getPosition().y};
            std::cout << to_string((float)intValue) << std::endl;
            auto scoreObject = GameObject::create("score" + to_string(counter));

            scoreObject->addComponent<ScoreDisplayComponent>(*scoreObject, m_renderWindow, inputText, scorePosition);
            EventBus::getInstance().fireEvent(std::make_shared<GameObjectCreateEvent>(scoreObject));

            //std::cout << "Parsed integer value: " << intValue << std::endl;
        } catch (const std::invalid_argument& ia)
        {
            std::cerr << "Invalid argument: " << ia.what() << std::endl;
        } catch (const std::out_of_range& oor)
        {
            std::cerr << "Out of range error: " << oor.what() << std::endl;
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
