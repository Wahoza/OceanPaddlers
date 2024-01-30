#pragma once
#include "GameObject.hpp"
#include "ITransformAnimation.hpp"

namespace mmt
{
class TransformAnimationSmoothFollow final : public mmt_gd::ITransformAnimation
{
public:
    explicit TransformAnimationSmoothFollow(std::shared_ptr<mmt_gd::GameObject> target, float smoothFactor);

    void update(float deltaTime, sf::Transformable& transform) override;

private:
    //note: this is the height of the map / 2 * tilesize
    float                               middleYval = 16 / 2 * 16;
    std::shared_ptr<mmt_gd::GameObject> m_target;
    float                               m_smoothFactor;
};
} // namespace mmt
