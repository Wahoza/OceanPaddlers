// Game.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include "PhysicsManager.hpp"
#include "RigidBodyComponent.hpp"
#include "DebugDraw.hpp"
#include "TetherComponent.hpp"

using namespace sf;
using namespace std;

namespace mmt_gd
{
void PhysicsManager::RegisterBody(RigidBodyComponent* RigidBody){
    m_bodies.push_back(RigidBody);
    //std::cout << m_bodies.size() << std::endl;
}
void PhysicsManager::RegisterCollider(StaticColliderComponent* Collider)
{
    m_staticcolliders.push_back(Collider);
}
void PhysicsManager::RegisterTrigger(TriggerComponent* Trigger)
{
    m_triggers.push_back(Trigger);
}
void PhysicsManager::RegisterCollectible(CollectibleComponent* Collectible)
{
    m_collectibles.push_back(Collectible);
}
PhysicsManager* PhysicsManager::m_instance;
PhysicsManager* PhysicsManager::getInstance()
{

    if (m_instance == nullptr)
    {
        m_instance = new PhysicsManager();
    }
    return m_instance;
}

void PhysicsManager::HandleCollisions(float fDeltaTimeSeconds)
{
    m_manifolds.clear();

    findCollisions();
    resolveCollisions();
    //std::cout << "update" << std::endl;
    for (auto& body : m_staticcolliders)
    {
        DebugDraw::getInstance().drawRectangle({body->m_shape.left, body->m_shape.top},
                                               {body->m_shape.width, body->m_shape.height},
                                               sf::Color::Green);
        //std::cout << body->m_shape.width << std::endl;
    }
    for (auto& body : m_triggers)
    {
        DebugDraw::getInstance().drawRectangle({body->m_shape.left, body->m_shape.top},
                                               {body->m_shape.width, body->m_shape.height},
                                               sf::Color::Red);
        //std::cout << body->m_shape.width << std::endl;
    }
    for (auto& body : m_collectibles)
    {
        DebugDraw::getInstance().drawRectangle({body->m_shape.left, body->m_shape.top},
                                               {body->m_shape.width, body->m_shape.height},
                                               sf::Color::Red);
        //std::cout << body->m_shape.width << std::endl;
    }
    for (auto& body : m_bodies)
    {
        DebugDraw::getInstance().drawRectangle({body->m_shape.left, body->m_shape.top},
                                               {body->m_shape.width, body->m_shape.height}, sf::Color::Yellow);

        Vector2f forces{};
        // forces are constantly applied to body
        for (const auto& f : body->m_forces)
            forces += f;

        // impulses are applied only once
        for (const auto& i : body->m_impulses)
            forces += i;
        body->m_impulses.clear();

        //
        // symplectic Euler
        body->m_acceleration = forces * body->m_invMass;
        body->m_velocity += body->m_acceleration * fDeltaTimeSeconds;
        //body.velocity = body.velocity * 0.99f; //< simple "friction"
        body->getGameObject().move(body->m_velocity * fDeltaTimeSeconds);
    }
}

void PhysicsManager::findCollisions()
{
    for (auto& itA = m_bodies.begin(); itA != m_bodies.end(); ++itA)
    {
        auto& body1 = *itA;
        for (auto itB = itA; itB != m_bodies.end(); ++itB)
        {
            if (itB == itA)
                continue;

            auto& body2 = *itB;
            if (body1->GetTether() != nullptr && body1->GetTether() == body2->GetTether())
                continue;
            // if both object don't have a mass or body is the same skip
            if (body1->m_mass == 0 && body2->m_mass == 0)
                continue;

            Transform body1Transform;
            body1Transform.translate(body1->getGameObject().getPosition());
            Transform body2Transform;
            body2Transform.translate(body2->getGameObject().getPosition());

            Vector2f normal;
            float    penetration;
            if (aabbVsAabb(body1Transform.transformRect(body1->m_shape),
                           body2Transform.transformRect(body2->m_shape),
                           normal,
                           penetration))
            {
                Manifold manifold;
                manifold.m_body1       = body1;
                manifold.m_body2       = body2;
                manifold.m_normal      = normal;
                manifold.m_penetration = penetration;

                m_manifolds.push_back(manifold);
            }
        }
        doStaticCollisions(body1);
        doTriggerCollisions(body1);
        doCollectibleCollisions(body1);
    }
}

void PhysicsManager::doStaticCollisions(RigidBodyComponent* body1)
{
    for (auto itB = m_staticcolliders.begin(); itB != m_staticcolliders.end(); ++itB)
    {
        auto& body2 = *itB;

        Transform body1Transform;
        body1Transform.translate(body1->getGameObject().getPosition());
        Transform body2Transform;
        body2Transform.translate(body2->getPosition());

        Vector2f normal;
        float    penetration;

        if (aabbVsAabb(body1Transform.transformRect(body1->m_shape), body2Transform.transformRect(body2->m_shape), normal, penetration))
        {
            
            if (body1->GetTether() != nullptr &&
                body2->m_coll == StaticColliderComponent::Hard) //.getId().find(itB->m_identifier) == std::string::npos))
            {
                float hurtTime = body1->GetTether()->hurtTime;
                if (hurtTime > 0 && hurtTime < 4.3) continue;
            }

            if (body2->m_coll == StaticColliderComponent::Event)
            {
                std::cout << "Event" << std::endl;
                continue;
            }

            sf::Vector2f rv             = body1->m_velocity;
            float        velAlongNormal = normal.x * rv.x + normal.y * rv.y;

            if (velAlongNormal > 0)
                return;
            float e = 1.0f;
            float j = (1 + e) * velAlongNormal;
            j /= body1->m_invMass + 0;

            sf::Vector2f impulse    = j * normal;

            if (body2->m_coll == StaticColliderComponent::Hard || body2->m_coll == StaticColliderComponent::Wall)
            {

                if (abs(normal.x) > abs(normal.y))
                {
                    body1->m_velocity -= sf::Vector2f{body1->m_velocity.x, 0.f};
                }
                else
                {
                    body1->m_velocity -= sf::Vector2f{0.f, body1->m_velocity.y};
                }
            }

            if (body2->m_coll == StaticColliderComponent::Bouncy)
            {

                body1->m_velocity -= (body1->m_invMass * impulse);

            }
            
            CheckforTether(body1);
        }
    }
}
void PhysicsManager::doTriggerCollisions(RigidBodyComponent* body1)
{
    for (auto itB = m_triggers.begin(); itB != m_triggers.end(); ++itB)
    {
        auto& body2 = *itB;
        //
        //some conditionals
        

        if (body1->getGameObject().getId().find(body2->m_identifier) == std::string::npos)
        {
            continue;
        }

        Transform body1Transform;
        body1Transform.translate(body1->getGameObject().getPosition());
        Transform body2Transform;
        body2Transform.translate(body2->getPosition());

        Vector2f normal;
        float    penetration;
        
        if (aabbVsAabb(body1Transform.transformRect(body1->m_shape), body2Transform.transformRect(body2->m_shape), normal, penetration))
        {
            
             if (body2->m_notification == "Current")
             {
                body1->addVelocity({0.01, 0});
             }
                
                
            //only trigger on inital collision
            if (body2->collided == false)
            {
                body2->Notify();
            }
            body2->collided = true;

            //std::cout << body2->collided << std::endl;


        }
        else
        {
            body2->collided = false;
        }
    }
}
void PhysicsManager::doCollectibleCollisions(RigidBodyComponent* body1)
{
    int ctr = 0;
    for (auto& itB : m_collectibles)
    {
        if (body1->getGameObject().getId().find(itB->m_identifier) == std::string::npos)
        {
            continue;
        }

        auto removeIterator = std::remove_if(m_collectibles.begin(),m_collectibles.end(),[&](const auto& body2)
        {
            Transform body1Transform;
            body1Transform.translate(body1->getGameObject().getPosition());
            Transform body2Transform;
            body2Transform.translate(body2->getPosition());
            Vector2f normal;
            float    penetration;
            if (aabbVsAabb(body1Transform.transformRect(body1->m_shape),
                body2Transform.transformRect(body2->m_shape),normal, penetration)){
                body2->CollissionMessage();
                return true; // Mark the element for removal
            }
                return false; // Element should not be removed
        });
        m_collectibles.erase(removeIterator, m_collectibles.end());

    }
}
void PhysicsManager::resolveCollisions()
{
    for (auto& man : m_manifolds)
    {
        if (man.m_body1->m_invMass == 0 && man.m_body2->m_invMass == 0)
        {
            std::cout<< "break";
            return;
        }
        sf::Vector2f rv             = man.m_body1->m_velocity - man.m_body2->m_velocity;
        float        velAlongNormal = man.m_normal.x * rv.x + man.m_normal.y * rv.y;

        if (velAlongNormal > 0)
            return;
        float e = 1.0f;
        float j = (1 + e) * velAlongNormal;
        j /= man.m_body1->m_invMass + man.m_body2->m_invMass;

        sf::Vector2f impulse    = j * man.m_normal;
        man.m_body1->m_velocity = (man.m_body1->m_velocity - man.m_body1->m_invMass * impulse);
        man.m_body2->m_velocity = (man.m_body2->m_velocity + man.m_body2->m_invMass * impulse);

        CheckforTether(man.m_body1);
        CheckforTether(man.m_body2);
    }
}

bool PhysicsManager::CheckforTether(RigidBodyComponent* obj)
{
    //returns true if HasTether && is player, else false
    if (obj->GetTether() !=nullptr)
    {
        TetherComponent* tether = obj->GetTether();
        if (tether->CollisionPhysics(&obj->getGameObject()))
        {
            return true;
        }
    }
    return false;
}
bool PhysicsManager::aabbVsAabb(const FloatRect& a, const FloatRect& b, Vector2f& normal, float& penetration)
{
    auto getCenter = [](const sf::FloatRect& rect) -> Vector2f
    { return Vector2f(rect.left, rect.top) + 1.f * Vector2f(rect.width, rect.height); };

    Vector2f n         = getCenter(b) - getCenter(a); // Vector from A to B
    float    a_extent  = a.width;              // Calculate half extents along x axis
    float    b_extent  = b.width;
    float    x_overlap = a_extent + b_extent - abs(n.x); // Calculate overlap on x axis
                                                         // SAT test on x axis
    if (x_overlap > 0)
    {
        float a_extent  = a.height; // Calculate half extents along y axis
        float b_extent  = b.height;
        float y_overlap = a_extent + b_extent - abs(n.y); // Calculate overlap on y axis

        // SAT test on y axis
        if (y_overlap > 0)
        {
            // Find out which axis is axis of least penetration
            if (x_overlap < y_overlap)
            {
                // Point towards B knowing that n points from A to B
                if (n.x < 0)
                    normal = Vector2f(1.0f, 0.0f);
                else
                    normal = Vector2f(-1.0f, 0.0f);
                penetration = x_overlap;

                return true;
            }
            else
            {
                // Point towards B knowing that n points from A to B
                if (n.y < 0)
                    normal = Vector2f(0, 1);
                else
                    normal = Vector2f(0, -1);
                penetration = y_overlap;

                return true;
            }
        }
    }
    return false;
}
void PhysicsManager::shutdown()
{
    m_bodies.clear();
    for (auto& collider : m_staticcolliders)
    {
        delete collider;
    }
    for (auto& collider : m_triggers)
    {
        delete collider;
    }
    m_staticcolliders.clear();
    m_triggers.clear();
    delete m_instance;
}
} // namespace mmt_gd
