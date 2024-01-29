#pragma once

#include "TransformAnimationComponent.hpp"

namespace mmt_gd
{
/**
 * \brief This animation component is used to move an object in a straight line
 */
class TransformAnimationConstantMotion final : public ITransformAnimation
{
public:
    TransformAnimationConstantMotion(const sf::Vector2f& direction, const float speed) :
    m_direction(direction),
    m_fSpeed(speed)
    {
    }

    void update(float deltaTime, sf::Transformable& transform) override;

private:
    sf::Vector2f m_direction;
    float        m_fSpeed;
};
} // namespace mmt_gd
