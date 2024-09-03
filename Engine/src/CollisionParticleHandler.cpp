#pragma once

#include "stdafx.h"
#include "CollisionParticleHandler.hpp"
#include <cstdlib>          // For std::rand, std::srand
#include <ctime>

namespace mmt_gd
{
ParticleManager* ParticleManager::m_instance = nullptr;
ParticleManager* ParticleManager::getInstance()
{
    if (!m_instance)
    {
        m_instance = new ParticleManager();
    }
    return m_instance;
}
void ParticleManager::setGravity(sf::Vector2f _gravity)
{
    gravity = _gravity;
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}
void ParticleManager::update(float deltaTime)
{
    for (auto& particle = particles.begin(); particle != particles.end();)
    {
        
        auto& ttl = std::get<2>(*particle);
        auto& originalttl = std::get<3>(*particle);
        ttl               = ttl - deltaTime;

        if (ttl < 0)
        {
            particle = particles.erase(particle);
            continue;
        }

        //std::cout << ttl << std::endl;

        auto& rect = std::get<0>(*particle);

        rect.rotate(rotationSpeed * deltaTime);

        auto& moveDir = std::get<1>(*particle);
        rect.move(moveDir * deltaTime);
        moveDir += gravity * deltaTime * std::get<4>(*particle);

        if (ttl < originalttl / 5)
        {
            sf::Color originalColor = rect.getFillColor();
            sf::Color originalOutline = rect.getOutlineColor();

            rect.setFillColor({originalColor.r, originalColor.b, originalColor.g, sf::Uint8((ttl / originalttl) * 255.0f)});
            rect.setOutlineColor(
                {originalOutline.r, originalOutline.b, originalOutline.g, sf::Uint8((ttl / originalttl) * 255.0f)});
        }
        ++particle;
    }
}
void ParticleManager::generateNewParticle(sf::Vector2f size, sf::Color col, float outline, sf::Color outlineColor, float timetolive, sf::Vector2f direction, sf::Vector2f pos, float weight)
{
    sf::RectangleShape particle;
    particle.setFillColor(col);
    particle.setOutlineColor(outlineColor);
    particle.setOutlineThickness(outline);

    particle.setSize(size);
    particle.setOrigin(size / 2.f);
    particle.setPosition(pos);

    float randomOffset = static_cast<float>(std::rand()) / RAND_MAX;

    particle.rotate(randomOffset * 365);

    float randomTTLOffset = float(int(randomOffset * 100) % 10) / 10;

    particles.push_back({particle, direction, timetolive, timetolive + randomTTLOffset, weight});

    if (particles.size() - 1 >= maxParticles)
    {
        if (!particles.empty())
        {
            // Erase the oldest item (the first element in the vector)
            particles.erase(particles.begin());
        }
    }
}
void ParticleManager::draw(sf::RenderWindow& window)
{
    for (auto& particle : particles)
    {
        window.draw(std::get<0>(particle));
    }
}
void ParticleManager::shutdown()
{
    particles.clear();
    delete m_instance;
    m_instance = nullptr;
}
void ParticleManager::init(sf::Vector2f new_gravity)
{
    gravity = new_gravity;
}
void ParticleManager::generateParticlesAroundPoint(
    sf::Vector2f center,
    int          amount,
    float        radius,
    float        fleeing,
    sf::Vector2f size,
    sf::Color    col,
    float        outline,
    sf::Color    outlineColor,
    float        timetolive, float weight = 1)
{
    float randomOffset = static_cast<float>(std::rand()) / RAND_MAX * 2 * 3.14;

    for (int i = 0; i < amount; ++i)
    {
        // Calculate angle for this point (in radians)
        float angle = randomOffset + 2 * 3.14 * i / amount;

        // Calculate x and y coordinates
        float x = center.x + radius * cos(angle);
        float y = center.y + radius * sin(angle);

        sf::Vector2f result = {x, y};
        generateNewParticle(size, col,outline,outlineColor, timetolive, normalize(result - center) * fleeing, result, weight);
    }
}
sf::Vector2f ParticleManager::normalize(const sf::Vector2f& vec)
{
    float magnitude = std::sqrt(vec.x * vec.x + vec.y * vec.y);

    if (magnitude != 0.0f)
    {
        return sf::Vector2f(vec.x / magnitude, vec.y / magnitude);
    }

    return vec; 
}
} // namespace mmt_gd