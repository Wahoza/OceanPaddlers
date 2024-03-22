#include "stdafx.h"
#include "FastNoiseLite.h"

#include "ObstacleGenerator.hpp"
#include "StaticColliderComponent.hpp"
#include "DebugDraw.hpp"
#include "PhysicsManager.hpp"

#include "EventBus.hpp"
#include "GameObjectEvents.hpp"
#include "IEvent.hpp"

namespace mmt_gd
{
void ObstacleGenerator::init(sf::IntRect _TextureRect, sf::Vector2i Area, int seed)
{
    noise.SetSeed(seed);
    noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);

    if (!m_texture.loadFromFile("../assets/SegmentedTiles/tiles.png"))
    {
        sf::err() << "Could not load icon\n";
        return;
    }
    m_printer.setTexture(m_texture);
    m_printer.setTextureRect(_TextureRect);
    TileLength  = _TextureRect.width;
    waitingDist = Area.x * TileLength;
    
        for (int x = Area.x; x >= 0; x--)
        {
            std::vector<NoisePositionValue> SubVector;
            std::vector<ObstacleTiles> TileVec;
        for (int y = 0; y <= Area.y; y++)
        {
            NoisePositionValue NPV;

            NPV.noise = -1;
            NPV.pos        = {float(x * 16), float(y * 16)};

            SubVector.push_back(NPV);

            ObstacleTiles tile;
            tile.endTile = true;
            tile.pos     = {float(x * 16), float(y * 16)};

            TileVec.push_back(tile);
            /*std::cout << "noise: " + std::to_string(NPV.noise) + " pos: " + std::to_string(NPV.pos.x) + " " +
                             std::to_string(NPV.pos.y)
                      << std::endl;*/
        }
        NPVs.push_back(SubVector);
        ObstacleSpriteMap.push_back(TileVec);
        std::vector<StaticColliderComponent> SubColliderVector;
        Colliders.push_back(SubColliderVector);
    }
}
void ObstacleGenerator::update(float deltaTime, sf::FloatRect windowRect)
{
    if (NPVs[NPVs.size() - 1][0].pos.x < windowRect.left - TileLength)
    {
        NPVs.pop_back();

        std::vector<NoisePositionValue> SubVector = NPVs[1];
        for (auto& NPV : SubVector)
        {
            NPV.pos.x += TileLength * 2;
            if (!waiting)
            {
                NPV.noise = noise.GetNoise(NPV.pos.x / TileLength * scaleVal, NPV.pos.y / TileLength * scaleVal);
            }
            else
            {
                NPV.noise = -1;
            }
        }

        NPVs.insert(NPVs.begin(), SubVector);

        ObstacleSpriteMap.pop_back();
        std::vector<ObstacleTiles> SubTileVec = ObstacleSpriteMap[1];
        for (auto& Tile : SubTileVec)
        {
            Tile.LeftTop = {0, 0};
            Tile.endTile = false;
            Tile.pos.x += TileLength * 2;
            Tile.print = false;
            Tile.noise = noise.GetNoise(Tile.pos.x / TileLength * scaleVal, Tile.pos.y / TileLength * scaleVal);
        }
        ObstacleSpriteMap.insert(ObstacleSpriteMap.begin(), SubTileVec);

        

        //fix this
        //for (auto& SubColliderVector : Colliders)
        //{
        //    for (auto& Collider : SubColliderVector)
        //    {
        //        Collider.removeSelf();
        //    }
        //}
        if (!waiting) //!waiting
        {
            std::vector<StaticColliderComponent> SubColliderVector;

            for (auto& NPV : SubVector)
            {

                if (NPV.noise > threshold && (NPV.pos.y / 16 > 3 && NPV.pos.y / 16 <13))
                {
                    StaticColliderComponent component = *new StaticColliderComponent({NPV.pos.x + 3, NPV.pos.y + 3, 10, 10}, "hard");
                    SubColliderVector.push_back(component);

                }
            }
            //do gen for fish (at near -1)
            //do gen for currents (at -1, -0.5)
            Colliders.insert(Colliders.begin(), SubColliderVector);

            std::vector<bool> hasIsland;

            for (auto& NPV : NPVs[1])
            {
                if (NPV.noise > threshold)// && (NPV.pos.y / 16 > 3 && NPV.pos.y / 16 < 13))
                {
                    hasIsland.push_back(true);
                }
                else
                {
                    hasIsland.push_back(false);
                }
            }
            IEvent::Ptr Islands = std::make_shared<IslandTileGenEvent>(hasIsland);
            EventBus::getInstance().fireEvent(Islands);
            


            generateNextSprite();
            return;
        }

        
    }


    if (windowRect.left + windowRect.width > waitingDist)
    {
        waiting = false;
    }
}
void ObstacleGenerator::draw(sf::RenderWindow& window)
{

    if (waiting)
    {
        return;
    }
    for (auto& ObVec : ObstacleSpriteMap)
    {
        for (auto& obj : ObVec)
        {
            
            if (obj.print)
            {
                m_printer.setPosition(obj.pos);
                m_printer.setTextureRect(
                    {obj.LeftTop.x, obj.LeftTop.y, m_printer.getTextureRect().width, m_printer.getTextureRect().height});
                window.draw(m_printer);
            }
        }
    }
    int iterationCounter = 0;
    for (auto& NPV : NPVs)
    {
        iterationCounter++;
        for (auto& Tile : NPV)
        {
            /*if (iterationCounter % 2 == 1)
            {
                continue;
            }*/
            sf::Color col = {200, 20, 20, 30};

            if (Tile.noise > threshold)
            {
                col = {20, 200, 20, 30};
            }
            DebugDraw::getInstance().drawRectangle(Tile.pos, {16, 16}, col, col);
        }
    }
}
void ObstacleGenerator::generateNextSprite()
{
    //use 0,1,2 of NoiseMap for determination
    //expects a certain Pattern within the image
    auto& ToModifyColumn = ObstacleSpriteMap[1];

    std::vector<NoisePositionValue> NPV0 = NPVs[0];
    std::vector<NoisePositionValue> NPV1 = NPVs[1]; // is the column
    std::vector<NoisePositionValue> NPV2 = NPVs[2];

    //note: Check in + pattern
    //3 comes from tile barriers
    for (int iterator = 3; iterator <= ToModifyColumn.size()-3; iterator++)
    {
        if (!(NPV1[iterator].noise > threshold) || !(NPV1[iterator].pos.y / 16 > 3 && NPV1[iterator].pos.y / 16 < 13) ||
            waiting)
        {
            continue;
        }
        ObstacleSpriteMap[1][iterator].pos = NPV1[iterator].pos;
        Vector2i directionVec              = {0, 0};
        int neighbourCount                 = 0;
        bool     hasUpVec                  = false; //note: there is one use for this to find a difference between two 0,0 vecs
        
        if (NPV1[iterator + 1].noise > threshold && (NPV1[iterator + 1].pos.y/16 < 13))
        {
            directionVec += {0, -1};
            neighbourCount++;
            hasUpVec = true;
        }
        if (NPV0[iterator].noise > threshold)
        {
            directionVec += {-1, 0};
            neighbourCount++;
        }
        if (NPV1[iterator - 1].noise > threshold && (NPV1[iterator - 1].pos.y/16 > 3))
        {
            directionVec += {0, 1};
            neighbourCount++;
            hasUpVec = true;
        }
        if (NPV2[iterator].noise > threshold)
        {
            directionVec += {1, 0};
            neighbourCount++;
        }

        switch (neighbourCount)
        {
            case 0:
            {
                directionVec = Vector2i{64, 64};
                break;
            }
            case 1:
            {
                directionVec *= 32;
                directionVec += {32, 32};
                break;
            }
            case 2:
            {
                if (directionVec == Vector2i(0, 0))
                {
                    if (hasUpVec)
                    {
                        directionVec = Vector2i(0, 64);
                    }
                    else
                    {
                        directionVec = Vector2i(0, 0);
                    }
                }
                else
                {
                    directionVec *= 16;
                    directionVec += {32, 32};
                }
                break;
            }
            case 3:
            {
                directionVec *= 16;
                directionVec += {32, 32};
                break;
            }
            case 4:
            {
                directionVec = {32, 32};
                break;
            }
            default:
                directionVec = {32, 32};
        }
        ToModifyColumn[iterator].print = true;

        ToModifyColumn[iterator].LeftTop = directionVec;
    }
}
void ObstacleGenerator::shutdown()
{
    //Colliders = {};
    NPVs      = {};
    ObstacleSpriteMap = {};
}
} // namespace mmt_gd
