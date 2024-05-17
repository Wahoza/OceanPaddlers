#include "stdafx.h"
#include "RockGenerator.hpp"
#include "StaticColliderComponent.hpp"
#include <random>
namespace mmt_gd
{
void RockGenerator::init(sf::Vector2i Area, int seed)
{
    if (!m_texture.loadFromFile("../assets/SegmentedTiles/grassTiles/ObstacleTiles.png"))
    {
        sf::err() << "Could not load icon\n";
        return;
    }
    m_Sprite.setTexture(m_texture);
    for (int x = Area.x; x >= 0; x--)
    {
        std::vector<Obstacle> SubVec;
        Rocks.push_back(SubVec); 
    }
    _seed = seed;
    _Area = Area;
}

void RockGenerator::generateNextRock(float xpos, int pathy)
{
    {
    if (!Rocks.empty())
    {
        if (!Rocks.back().empty())
        {
            if (Rocks.back()[0].pos.x < xpos - _Area.x * TextureSize - TextureSize)
            {
                for (auto& item : Rocks.back())
                {
                    if (item.collider != nullptr)
                    {
                        item.collider->removeSelf();
                        if (item.collider != nullptr)
                        {
                            delete item.collider;
                        }
                    }
                }
                Rocks.pop_back();
            }
        }
        else
        {
            Rocks.pop_back();
        }
    }

    std::vector<Obstacle> Subvector;
    for (int i = wallsize; i < _Area.y - wallsize; i++)
    {
        if (pathy - TextureSize - 1 < i * TextureSize && i * TextureSize < pathy + TextureSize + 1)
        {
            continue;
        }

        std::mt19937                       gen(_seed + xpos + i);
        std::uniform_int_distribution<int> dist(0, chance.y);

        int random_number = dist(gen);

        if (random_number <= chance.x)
        {
            Obstacle t_obstacle;
            t_obstacle.collider = new StaticColliderComponent({float(xpos + 3),
                                                               float(i * TextureSize + 5),
                                                               float(TextureSize - 6),
                                                               float(TextureSize - 10)},
                                                              "Hard");
            t_obstacle.pos      = {xpos, float(i * TextureSize)};
            t_obstacle.LeftTop  = {0, 0};

            if (random_number % chance.x == 1)
            {
                t_obstacle.LeftTop = {TextureSize, 0};
            }
            Subvector.push_back(t_obstacle);
        }
    }
    Rocks.insert(Rocks.begin(), Subvector);
    }
}

void RockGenerator::generateNextCurrent(float xpos, int pathy)
{
    if (xpos > currentArea.top + currentArea.width)
    {

    std::mt19937                       genChance(_seed + xpos);
    std::uniform_int_distribution<int> distChance(0, currentChance);
    int                                random_number = distChance(genChance);

    if (random_number == 1)
    {
        std::mt19937                       genSize(_seed + xpos + pathy + 1);
        std::uniform_int_distribution<int> distSize(1,2000);
        int                                SizeVal = distSize(genSize); 
        currentArea.width = SizeVal % maxCurrentSize.x * 16;
        currentArea.height = SizeVal % maxCurrentSize.y * 16;

        float Offset = -currentArea.height / 2;

        std::mt19937                  genOffset(_seed + xpos + 1);
        std::uniform_int_distribution offsetdist(-maxOffset, maxOffset);

        currentArea.left = xpos;
        currentArea.top  = pathy + Offset + offsetdist(genOffset);
    }
    }

    std::vector<Current> SubVector;
    for (int i = wallsize; i <= _Area.y - wallsize; i++)
    {
    if (currentArea.contains({(int)xpos, i * TextureSize}))
    {
        Current t_current;
        t_current.LeftTop = {TextureSize * 2, 0};
        t_current.pos     = {xpos, (float)i * TextureSize};
        t_current.trigger = new TriggerComponent({t_current.pos.x, t_current.pos.y, (float)TextureSize / 2, (float)TextureSize},
                                                 "Player0",
                                                 "Current");
        SubVector.push_back(t_current);
    }
    }
    Currents.insert(Currents.begin(), SubVector);

    if (!Currents.empty())
    {
    if (!Currents.back().empty())
    {
        if (Currents.back()[0].pos.x < xpos - _Area.x * TextureSize)
        {
            for (auto& item : Currents.back())
            {
                if (item.trigger != nullptr)
                {
                    item.trigger->removeSelf();
                    delete item.trigger;
                }
            }
            Currents.pop_back();
        }
    }
    else
    {
        Currents.pop_back();
    }
    }
}

void RockGenerator::draw(sf::RenderWindow& window)
{
    for (auto& currentVec : Currents)
    {
        for (auto& current : currentVec)
        {
            m_Sprite.setTextureRect({current.LeftTop.x, current.LeftTop.y, TextureSize, TextureSize});
            m_Sprite.setPosition(current.pos);

            window.draw(m_Sprite);
        }
    }
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
    Rocks.clear();
    Currents.clear();
}
} // namespace mmt_gd