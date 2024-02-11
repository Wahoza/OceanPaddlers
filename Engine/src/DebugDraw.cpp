//Author: Radomir Dinic
//Description: Debug Draw is used for debug drawing rects, text, etc. and can be enabled/disabled with f1 while running the program.
//License: LICENSE.txt
#include "stdafx.h"

#include "DebugDraw.hpp"

#include "InputManager.hpp"
#include "VectorAlgebra2D.h"

namespace mmt_gd
{
DebugDraw::DebugDraw() : m_enabled(false)
{
    if (!m_font.loadFromFile("../assets/consolab.ttf"))
    {
        sf::err() << "Could not load font\n";
    }
}

;

void DebugDraw::drawLine(const sf::Vector2f& p1, const sf::Vector2f& p2, const sf::Color& color, const float thickness)
{
    if (!m_enabled)
    {
        return;
    }

    auto       dir    = p2 - p1;
    const auto length = MathUtil::length(dir);

    if (length >= 1.0F)
    {
        dir = dir / length * thickness * 0.5F;
        const sf::Vector2f right(dir.y, -dir.x);

        const std::shared_ptr<sf::ConvexShape> shape(new sf::ConvexShape());
        shape->setPointCount(4);
        shape->setPoint(0, p1 - right);
        shape->setPoint(1, p1 + right);
        shape->setPoint(2, p2 + right);
        shape->setPoint(3, p2 - right);
        shape->setFillColor(color);

        m_shapes.push_back(shape);
    }
}

void DebugDraw::drawArrow(const sf::Vector2f& origin, const sf::Vector2f& direction, const sf::Color& color, const float thickness)
{
    if (!m_enabled)
    {
        return;
    }

    if (const auto length = MathUtil::length(direction); length >= 1.0F)
    {
        const auto         unitDirection = direction / length;
        const sf::Vector2f right(unitDirection.y, -unitDirection.x);

        constexpr auto headSize = 5.0F;
        const auto     head     = length > headSize * 2 ? length - headSize : length * 0.5F;

        const auto p1 = origin;
        const auto p2 = origin + unitDirection * head;
        const auto p3 = origin + direction;

        const std::shared_ptr<sf::ConvexShape> shape(new sf::ConvexShape());
        shape->setPointCount(4);

        shape->setPoint(0, p1 - right * thickness * 0.5F);
        shape->setPoint(1, p1 + right * thickness * 0.5F);
        shape->setPoint(2, p2 + right * thickness * 0.5F);
        shape->setPoint(3, p2 - right * thickness * 0.5F);
        shape->setFillColor(color);

        m_shapes.push_back(shape);

        const std::shared_ptr<sf::ConvexShape> headShape(new sf::ConvexShape());
        headShape->setPointCount(3);

        headShape->setPoint(0, p2 - right * headSize);
        headShape->setPoint(1, p2 + right * headSize);
        headShape->setPoint(2, p3);
        headShape->setFillColor(color);

        m_shapes.push_back(headShape);
    }
}

void DebugDraw::drawCircle(const sf::Vector2f& center, const float radius, const sf::Color& outlineColor, const sf::Color& fillColor)
{
    if (!m_enabled)
    {
        return;
    }

    const auto shape = std::make_shared<sf::CircleShape>(radius);
    shape->setOutlineThickness(1.0F);
    shape->setOutlineColor(outlineColor);
    shape->setFillColor(fillColor);
    shape->setPosition(center);
    shape->setOrigin(sf::Vector2f(radius, radius));

    m_shapes.push_back(shape);
}

void DebugDraw::drawRectangle(const sf::Vector2f& position,
                              const sf::Vector2f& size,
                              const sf::Color&    outlineColor,
                              const sf::Color&    fillColor)
{
    if (!m_enabled)
    {
        return;
    }

    const std::shared_ptr<sf::RectangleShape> shape(new sf::RectangleShape(size));
    shape->setOutlineThickness(1.0F);
    shape->setOutlineColor(outlineColor);
    shape->setFillColor(fillColor);
    shape->setPosition(position);
    

    m_shapes.push_back(shape);
}


void DebugDraw::drawText(const std::string& str, const sf::Vector2f& center, const sf::Color& color)
{
    const std::shared_ptr<sf::Text> text(new sf::Text());
    text->setPosition(center);
    text->setString(str);
    text->setFillColor(color);
    text->setFont(m_font);
    text->setCharacterSize(14);
    const auto bounds = text->getLocalBounds();
    text->setOrigin(bounds.width * 0.5F, bounds.height * 0.5F + bounds.top);

    m_shapes.push_back(text);
}

void DebugDraw::update(float deltaTime)
{
    if (InputManager::getInstance().isKeyReleased("debugdraw"))
    {
        setEnabled(!is_enabled());
    }
}

void DebugDraw::draw(sf::RenderWindow& renderWindow)
{
    if (m_enabled)
    {
        for (const auto& shape : m_shapes)
        {
            renderWindow.draw(*shape);
        }
    }

    m_shapes.clear();
}
} // namespace mmt_gd
