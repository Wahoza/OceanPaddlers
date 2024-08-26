//Author: Kilian Kolb
//Description: Is used for colliders that only cause an event on collision and has no solid collider.
//License: LICENSE.txt
#pragma once
#include "stdafx.h"

#include "IComponent.hpp"

using namespace sf;
using namespace std;
namespace mmt_gd
{

class TriggerComponent
{
    // Collision geometry. In the component-based architecture, the
    // collision geometry should be in a separate component, e.g.,
    // BBoxCollisionComponent

public:
    using ptr = std::shared_ptr<TriggerComponent>;

    FloatRect m_shape;

    //how do rigidbodies interact?


    // The debug geometry to visualize the bounding geometry of the object.
    // Can be part of a BBoxCollisionComponent.

    // Parameters used for the rigid body physics

    bool init();
    //later: use design pattern, for now just output
    void     Notify(GameObject& gameObject);
    string m_notification;
    string m_identifier;
    string   m_origin = "World";
    Vector2f getPosition();
    void     setEnabled(bool enabled);
    void     removeSelf();
    TriggerComponent(FloatRect shape, string identifier, string notification);
    float SquaredMaxVelocity = 1000;
    bool  enabled           = true;

};
} // namespace mmt_gd