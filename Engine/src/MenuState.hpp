#pragma once

#include "GameState.hpp"

#include <SFML/Graphics/Text.hpp>

namespace mmt_gd
{
class MenuState final : public GameState
{
public:
    using GameState::GameState;

    void init() override;

    void update(float delta) override;
    void draw() override;
    void exit() override;

private:
    sf::Text m_text;
    sf::Font m_font;
    sf::View m_view;

    bool m_isInit = false;
};
} // namespace mmt_gd
