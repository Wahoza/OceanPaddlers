#pragma once
#include "stdafx.h"
#include "GameObject.hpp"
#include "IComponent.hpp"
#include "IRenderComponent.hpp"
namespace mmt_gd
{

class ScoreConverter : public IRenderComponent
{
public:
    ScoreConverter(GameObject& gameObject, sf::RenderWindow& renderWindow, std::string filePath, int displayedAmount);
    bool                     init() override;
    void                     update(float deltaTime) override;
    void                     CreateScoreLabels(std::string input, int displayedAmount);
    std::vector<std::string> splitString(const std::string& input, char delimiter);
    void draw() override;
    ~ScoreConverter();

private:
};


} // namespace mmt_gd