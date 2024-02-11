//Author: Kilian Kolb
//Description: This is for Colliders that dont move and are solid
//License: LICENSE.txt
#pragma once
#include "stdafx.h"

#include "IComponent.hpp"

using namespace sf;
using namespace std;
namespace mmt_gd
{

class StaticColliderComponent
{
    // Collision geometry. In the component-based architecture, the
    // collision geometry should be in a separate component, e.g.,
    // BBoxCollisionComponent

public:
    using ptr = std::shared_ptr<StaticColliderComponent>;

    FloatRect m_shape;
    enum CollisionType
    {
        Bouncy,
        Hard,
        Event,
        Wall
    };

    //how do rigidbodies interact?
    const std::unordered_map<std::string, CollisionType> collisionTypeMap =
        {{"Bouncy", Bouncy}, {"Hard", Hard}, {"Event", Event}, {"Wall", Wall}};

    // The debug geometry to visualize the bounding geometry of the object.
    // Can be part of a BBoxCollisionComponent.

    // Parameters used for the rigid body physics
    CollisionType m_coll = Hard;

    std::list<Vector2f> m_forces;   ///< forces constantly applied to object, e.g., gravity
    std::list<Vector2f> m_impulses; ///< impulses fire only once, e.g., during collision

    bool     init() ;
    void     update(float deltaTime) ;

    Vector2f    getPosition();
    void        ChangePosition(Vector2f changePos);
    const CollisionType ConvertToCollType(string input)
    {
        if (collisionTypeMap.find(input) != collisionTypeMap.end())
        {
            return collisionTypeMap.at(input);
        }
        else
        {
            return CollisionType::Hard;
        }
    }
    StaticColliderComponent(FloatRect shape, string type);

};
} // namespace mmt_gd