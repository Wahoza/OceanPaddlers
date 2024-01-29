#pragma once
#include "GameObject.hpp"

namespace mmt_gd
{
/**
 * \brief Simple debug draw functionality.
 * Use the relevant draw* method to render debug stuff in the current frame
 */
class DebugDraw final
{
public:
    static DebugDraw& getInstance()
    {
        static DebugDraw instance;
        return instance;
    }

    void drawLine(const sf::Vector2f& p1, const sf::Vector2f& p2, const sf::Color& color, float thickness = 1.0f);
    void drawArrow(const sf::Vector2f& origin, const sf::Vector2f& direction, const sf::Color& color, float thickness = 1.0f);
    void drawCircle(const sf::Vector2f& center,
                    float               radius,
                    const sf::Color&    outlineColor,
                    const sf::Color&    fillColor = sf::Color::Transparent);
    void drawText(const std::string& str, const sf::Vector2f& center, const sf::Color& color);
    void drawRectangle(const sf::Vector2f& position,
                       const sf::Vector2f& size,
                       const sf::Color&    outlineColor,
                       const sf::Color&    fillColor = sf::Color::Transparent);



    bool is_enabled() const
    {
        return m_enabled;
    }

    void setEnabled(bool enabled)
    {
        m_enabled = enabled;
    }

    void update(float deltaTime);

    void draw(sf::RenderWindow& renderWindow);

private:
    DebugDraw();

    DebugDraw(DebugDraw const&) : m_enabled(false)
    {
    }

    void operator=(DebugDraw const&) const
    {
    }

    bool                                       m_enabled;
    std::vector<std::shared_ptr<sf::Drawable>> m_shapes;
    sf::Font                                   m_font;
};
} // namespace mmt_gd
