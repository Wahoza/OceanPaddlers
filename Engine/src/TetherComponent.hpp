//Author: Kilian Kolb
//Description: Tether that takes Rigidbody component values and creates two indipendent Rigidbody Gameobjects. These behave as if they were attached to eachother on one point.
//License: LICENSE.txt
#pragma once
#include "stdafx.h"

#include "IComponent.hpp"
#include "RigidBodyComponent.hpp"

using namespace sf;
using namespace std;

namespace mmt_gd
{
class TetherComponent : public IComponent
{
    //Note: This brakes up one Rect into two seperate ones; Since this is very specific for only player use, it will also automatically assume 2 controlls to itself if controll = true.
    //it will split up the collider into two gameObjects; Logic will be implemented into physicsmanager for testing if both colliding objects are Element of tether.
    //Also: moveVecs should be reoriented according to orientation
public:
    TetherComponent(GameObject& gameObject, FloatRect shape, Vector2f offset, string forces, float mass, float loss, bool controll, float _movePercentage);
    ~TetherComponent();
    void update(const float deltaTime) override;
    void calculateRBForce(float deltaTime);
    void CalculateEffect(float deltaTime);
    void RepositionBodies();
    bool init() override;
    Vector2f normalize(Vector2f vec);
    using ptr = std::shared_ptr<TetherComponent>;
    GameObject* getOtherObj(GameObject collider);
    GameObject& createNewTetheredObject(string id);
    GameObject& TetherItem1;
    GameObject& TetherItem2;

    vector<Vector2f> forces;
    Vector2f         orientation = {1, 0};
    vector<GameObject*> getTethers();

    bool  CollisionPhysics(GameObject* collider, Vector2f collisionVec);
    void     ApplyForce(Vector2f _force);
    void     setVelocity(Vector2f _velocity);

    Vector2f     getVelocity();
    Vector2f getPosition();

    std::vector<sf::Vector2f> StringToVecList(string input)
    {

        std::istringstream      iss(input);
        std::vector<sf::Vector2f> vecs;

        float x, y;
        while (iss >> x >> y)
        {
            vecs.emplace_back(x, y);
        }

        return vecs;
    }

private:
    //orientation should be normalized, then multiplied with length
    //position is in the center -> actual vec of orientation = pos + orientation - 1/2 orientation
    //also note: GameObject Position is defined by topleft, therefore at init change Pos of game Object, so no weird effects at init
    float movePercentage;
    float       length;
    float            velocityIncrease = 0.05 / 16;
    vector<Vector2f> constlocalForces;
    Vector2f         m_velocity      = {0, 0};
    Vector2f         Loss            = {0.4, 0.8};
    float            trueMaxVelocity = 40;
    float            initMaxVelocity    = 20;
    float            currentMaxVelocity = 0;
    void  AddLocalForce(Vector2f _force);
};
}
