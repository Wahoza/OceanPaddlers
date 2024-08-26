//Author:Radomir Dinic, modified by Kilian Kolb
//Description: Changes Position of a Game Object based on Inputs. Also supports Controller circle pad inputs.
//License: LICENSE.txt
#pragma once

#include "IComponent.hpp"

namespace mmt_gd
{

class PlayerMoveComponent : public IComponent
{
public:
    using ptr = std::shared_ptr<PlayerMoveComponent>;

    PlayerMoveComponent(GameObject& gameObject, int playerIndex, bool rigidControll);

    bool init() override;
    void update(float deltaTime) override;
    sf::Vector2i toSpriteOffset(int value, int inputIdx);
    
    private:
    void calculateSpriteOffset(sf::Vector2f input, int index);
    bool rigidControll;
    int m_playerIndex;

};
} // namespace mmt_gd
