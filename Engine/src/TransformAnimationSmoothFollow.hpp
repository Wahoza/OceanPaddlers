//Author:Radomir Dinic, modified by Kilian Kolb
//Description: Is used for an Object to smoothly follow another along the x axis, in this use case it is the Camera that follows the player. Note: middle Y val is middle of the map.
//License: LICENSE.txt
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
    //note: this is the height of the map / 2 * tilesize + 1/2 of a tile
    float                               middleYval = 17 / 2 * 16 +8;
    std::shared_ptr<mmt_gd::GameObject> m_target;
    float                               m_smoothFactor;
    sf::Vector2f                        furthestTartgetPos = {0, 0};
};
} // namespace mmt
