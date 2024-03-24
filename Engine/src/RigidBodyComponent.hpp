//Author: Kilian Kolb
//Description: RigidBody Component for Physics.
//License: LICENSE.txt
#pragma once

#include "stdafx.h"
#include "TetherComponent.hpp"
#include "IComponent.hpp"

using namespace sf;
using namespace std;
namespace mmt_gd
{

class RigidBodyComponent : public IComponent
{
    // Collision geometry. In the component-based architecture, the
    // collision geometry should be in a separate component, e.g.,
    // BBoxCollisionComponent

public:
    using ptr = std::shared_ptr<RigidBodyComponent>;

    FloatRect m_shape;


    // The debug geometry to visualize the bounding geometry of the object.
    // Can be part of a BBoxCollisionComponent.
    RectangleShape m_debugGeometry;

    // Parameters used for the rigid body physics

    float m_mass = 10.f;
    float m_invMass = 0;
    float maxSpeed  = 600.f;
    float               Loss      = 0.0f;
    bool                isActive  = true;

    std::list<Vector2f> m_forces;   ///< forces constantly applied to object, e.g., gravity
    std::list<Vector2f> m_impulses; ///< impulses fire only once, e.g., during collision

    Vector2f m_acceleration;
    Vector2f m_velocity;
    Vector2f m_gameObjectOffset;

    bool        init() override;
    void        update(float deltaTime) override;
    const float getInvMass();
    Vector2f    getVelocity();
    void        addVelocity(Vector2f velocity);
    Vector2f    getPosition();
    void        ChangePosition(Vector2f changePos);
    void        calculateOffset();
    void        setVelocity(Vector2f velocity);
    void        SetTether(TetherComponent* Tether);
    RigidBodyComponent(GameObject& gameObject, FloatRect shape, Vector2f offset, string forces, float mass, float Loss);
    TetherComponent*        GetTether();
    std::list<sf::Vector2f> StringToVecList(string input)
    {

        std::istringstream iss(input);
        std::list<sf::Vector2f> vecs;

        float x, y;
        while (iss >> x >> y)
        {
            vecs.emplace_back(x, y);
        }

        return vecs;
    }
    ~RigidBodyComponent();

private:
    TetherComponent* m_tether = nullptr;
};
} // namespace mmt_gd