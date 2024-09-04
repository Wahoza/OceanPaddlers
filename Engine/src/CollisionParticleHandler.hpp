#pragma once
#include "stdafx.h"
namespace mmt_gd
{
class ParticleManager
{
public:
    static ParticleManager* getInstance();
    void setGravity(sf::Vector2f _gravity);
    void                    update(float deltaTime);
    void updateParticle(float deltaTime, std::vector<std::tuple<sf::RectangleShape, sf::Vector2f, float, float, float>>& particleVec);
    void generateNewParticle(sf::Vector2f size,
                             sf::Color    col,
                             float        outline,
                             sf::Color    outlineColor,
                             float        timetolive,
                             sf::Vector2f direction,
                                                sf::Vector2f pos,
                                                float        weight);
    void                    generateBackgroundParticle(
                           sf::Vector2f size,
                           sf::Color    col,
                           float        outline,
                           sf::Color    outlineColor,
                           float        timetolive,
                           sf::Vector2f direction,
                           sf::Vector2f pos,
                           float        weight);
    void draw(sf::RenderWindow& window);
    void drawBackground(sf::RenderWindow& window);
    void shutdown();
    void init(sf::Vector2f new_gravity);
    void generateParticlesAroundPoint(
        sf::Vector2f center,
        int          amount,
        float        radius,
        float         fleeing,
        sf::Vector2f size,
        sf::Color    col,
        float        outline,
        sf::Color    outlineColor,
        float        timetolive,
        float weight);


private:
    sf::Vector2f normalize(const sf::Vector2f& vec);

    std::vector<std::tuple<sf::RectangleShape, sf::Vector2f, float, float, float>> particles; //object, direction, ttl, original ttl, weight
    std::vector<std::tuple<sf::RectangleShape, sf::Vector2f, float, float, float>> backgroundParticles; //object, direction, ttl, original ttl, weight
    sf::Vector2f gravity       = {0, 0};
    int maxParticles = 400;
    float rotationSpeed = 200;

    static ParticleManager* m_instance;
};
}