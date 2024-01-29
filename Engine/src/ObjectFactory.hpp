#pragma once
#include "GameObject.hpp"
#include "SpriteManager.hpp"
#include "Tileson.hpp"

namespace tson
{
class Object;
}

namespace mmt_gd
{
class ObjectFactory
{
public:
    static GameObject::Ptr processTsonObject(tson::Object&        object,
                                             const tson::Layer&   layer,
                                             const fs::path&      path,
                                             const SpriteManager& spriteManager);
};
} //namespace mmt_gd
