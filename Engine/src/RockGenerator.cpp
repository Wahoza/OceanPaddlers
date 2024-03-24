#include "stdafx.h"
#include "RockGenerator.hpp"
#include "StaticColliderComponent.hpp"
#include <random>
namespace mmt_gd
{
void RockGenerator::init(sf::Vector2i Area, int seed)
{
    if (!m_texture.loadFromFile("../assets/SegmentedTiles/ObstacleTiles.png"))
    {
        sf::err() << "Could not load icon\n";
        return;
    }
    m_Sprite.setTexture(m_texture);
    std::vector<Obstacle> SubVec;
    for (int x = Area.x; x >= 0; x--)
    {
        Rocks.push_back(SubVec); 
    }
    _seed = seed;
}

void RockGenerator::generateNextTile(float xpos)
{
    if (!Rocks.empty())
    {
        if (!Rocks.back().empty())
        {
            if (Rocks.back()[0].pos.x < xpos - _Area.x * TextureSize)
            {
                for (auto& item : Rocks.back())
                {
                    if (item.collider != nullptr)
                    {
                        item.collider->removeSelf();
                        delete item.collider;
                    }
                }
                Rocks.pop_back();

            }

        }
    }

    std::vector<Obstacle> Subvector;
    for (int i = wallsize; i < _Area.y - wallsize; i++)
    {
        std::mt19937 gen(_seed + xpos + i);  
        std::uniform_int_distribution<int> dist(0, chance);

        int                                random_number = dist(gen);

        if (random_number == 1)
        {
            Obstacle t_obstacle;
            t_obstacle.collider = new StaticColliderComponent({float(xpos + 3), float(i * TextureSize + 3), float(TextureSize - 3), float(TextureSize - 3)},
                                                              "Hard");
            t_obstacle.pos = {xpos,float(i * TextureSize)};
            t_obstacle.LeftTop  = {0, 0};

            if (random_number % 10 == 0)
            {
                t_obstacle.LeftTop = {TextureSize, 0};
            }
            Subvector.push_back(t_obstacle);
        }
    }
    Rocks.push_back(Subvector);
}

void RockGenerator::draw(sf::RenderWindow& window)
{
    for (auto& obstacleVec : Rocks)
    {
        for (auto& obstacle : obstacleVec)
        {
            m_Sprite.setTextureRect({obstacle.LeftTop.x, obstacle.LeftTop.y, TextureSize, TextureSize});
            m_Sprite.setPosition(obstacle.pos);

            window.draw(m_Sprite);
        }
    }
}
void RockGenerator::shutdown()
{
    Rocks = {};
}
} // namespace mmt_gd