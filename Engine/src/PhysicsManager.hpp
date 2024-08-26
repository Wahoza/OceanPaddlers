//Author: Kilian Kolb
//Description: Singelton, handles Physics for all Objects with colliders. Does not calculate tehter behaviour.
//License: LICENSE.txt
#pragma once
#include "stdafx.h"
#include "RigidBodyComponent.hpp"
#include "StaticColliderComponent.hpp"
#include "TriggerComponent.hpp"
#include "CollectibleComponent.hpp"
namespace mmt_gd
{
class PhysicsManager
{
public:

    static PhysicsManager* getInstance();
    void shutdown();
    void DebugDrawBoxes();
    void HandleCollisions(float fDeltaTimeSeconds);
    void RegisterBody(RigidBodyComponent* RigidBody);
    void RegisterCollider(StaticColliderComponent* Collider);
    void RegisterTrigger(TriggerComponent* Trigger);
    void RegisterCollectible(CollectibleComponent* Collectible);

    void UnregisterBody(RigidBodyComponent* RigidBody);
    void UnregisterStaticCollider(StaticColliderComponent* Collider);
    void UnregisterTrigger(TriggerComponent* Trigger);
    private:
    struct Manifold
    {
        RigidBodyComponent* m_body1{};
        RigidBodyComponent* m_body2{};

        float    m_penetration{};
        Vector2f m_normal;
    };

    vector<RigidBodyComponent*> m_bodies;
    vector<StaticColliderComponent*> m_staticcolliders;
    vector<TriggerComponent*> m_triggers;
    vector<CollectibleComponent*> m_collectibles;

    vector<Manifold> m_manifolds;
    static PhysicsManager * m_instance;
    bool CheckforTether(RigidBodyComponent* gameObject,Vector2f collisionVec, bool damage);
    void findCollisions();
    void resolveCollisions();
    void doStaticCollisions(RigidBodyComponent* body1);
    void doTriggerCollisions(RigidBodyComponent* body1);
    void doCollectibleCollisions(RigidBodyComponent* body1);
    static bool aabbVsAabb(const FloatRect& a, const FloatRect& b, Vector2f& normal, float& penetration);
    
};

}