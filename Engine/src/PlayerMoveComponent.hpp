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

private:
    bool rigidControll;
    int m_playerIndex;
};
} // namespace mmt_gd
