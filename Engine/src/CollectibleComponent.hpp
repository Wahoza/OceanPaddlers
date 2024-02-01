#pragma once
#include "stdafx.h"

#include "IComponent.hpp"

using namespace std;
using namespace sf;

namespace mmt_gd
{

class CollectibleComponent final : public IComponent
{
public:
    using ptr = std::shared_ptr<CollectibleComponent>;

    CollectibleComponent(GameObject& gameObject, string _type, float width, float height);
    void update(float deltaTime) override;
    void CollissionMessage();
    //Vector2f  getPosition();
    bool      init();
    FloatRect m_shape = {0, 0, 0, 0};
    string    m_identifier = "Player0";

private:
    string type = "";
};
} // namespace mmt_gd