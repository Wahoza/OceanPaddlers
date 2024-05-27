#pragma once

#include "GameState.hpp"

#include <SFML/Graphics/Text.hpp>

namespace mmt_gd
{
class CreditsState final : public GameState
{
public:
    using GameState::GameState;

    void init() override;

    void update(float delta) override;
    void draw() override;
    void exit() override;

private:
    std::vector<std::string> creditsStrings = {"Code:\n Ema Selmanovic\n Kilian Kolb",
                                               "Art:\n Ema Selmanovic\n Kilian Kolb\n PixelHole"};
    std::string              BackButton     = {"<Back>"};

    sf::Font m_font;
    sf::Texture m_backgroundTexture;
    sf::Sprite  m_backgroundSprite;
    std::vector<sf::Text> m_CreditText;

    sf::Sprite m_icon;
    sf::Texture m_iconTexture;

    float offsetPerText = 50;
    bool  initButtonsDown;
    bool  m_isInit = false;
};
} // namespace mmt_gd
