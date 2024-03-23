#include "stdafx.h"
#include "FastNoiseLite.h"

#include "ObstacleGenerator.hpp"
#include "StaticColliderComponent.hpp"
#include "DebugDraw.hpp"
#include "PhysicsManager.hpp"

#include "EventBus.hpp"
#include "GameObjectEvents.hpp"
#include "IEvent.hpp"

#include <random>

namespace mmt_gd
{
void ObstacleGenerator::init(sf::IntRect _TextureRect, sf::Vector2i Area, int seed)
{
    _seed = seed;
    noise.SetSeed(seed);
    noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);

    if (!m_texture.loadFromFile("../assets/SegmentedTiles/tiles.png"))
    {
        sf::err() << "Could not load icon\n";
        return;
    }
    m_printer.setTexture(m_texture);
    m_printer.setTextureRect(_TextureRect);

    if (!m_dockTexture.loadFromFile("../assets/SegmentedTiles/dockTiles2.png"))
    {
        sf::err() << "Could not load icon\n";
        return;
    }
    m_dockPrinter.setTexture(m_dockTexture);
    m_dockPrinter.setTextureRect(_TextureRect);

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
            tile.pos     = {float(x * 16), float(y * 16)};

            TileVec.push_back(tile);
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
            Tile.pos.x += TileLength * 2;
            Tile.print = false;
            Tile.noise = noise.GetNoise(Tile.pos.x / TileLength * scaleVal, Tile.pos.y / TileLength * scaleVal);
        }
        ObstacleSpriteMap.insert(ObstacleSpriteMap.begin(), SubTileVec);

        
        //Collider Removing Logic
        auto& SubColliderVec = Colliders.back();
        Colliders.pop_back();
        for (auto& Collider : SubColliderVec)
        {
            Collider.removeSelf();
            delete &Collider;
        }

        //Dock Removing Logic
        if (!DockTileVector.empty())
        {
            while ((DockTileVector.back().pos.x < windowRect.left - TileLength))
            {

                DockTileVector.pop_back();
                if (!DockColliders.empty())
                {
                    DockColliders.back()->removeSelf();
                    delete DockColliders.back();
                    DockColliders.pop_back();
                }
            }
        }

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
    for (auto& dockTile : DockTileVector)
    {
        m_dockPrinter.setTextureRect({dockTile.LeftTop.x,
                                      dockTile.LeftTop.y,
                                      m_dockPrinter.getTextureRect().width,
                                      m_dockPrinter.getTextureRect().height});
        m_dockPrinter.setPosition(dockTile.pos);
        window.draw(m_dockPrinter);

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
        std::vector<Vector2i> normalVecs     = {};
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
                normalVecs = {{0, 0}};
                directionVec = Vector2i{64, 64};
                break;
            }
            case 1:
            {
                directionVec *= 16 * 2;
                directionVec += {32, 32};
                normalVecs = {directionVec};
                break;
            }
            case 2:
            {
                if (directionVec == Vector2i(0, 0))
                {
                    if (hasUpVec)
                    {
                        normalVecs   = {{0, 1}, {0, -1}};
                        directionVec = Vector2i(0, 64);
                    }
                    else
                    {
                        normalVecs   = {{1,0}, {-1,0}};
                        directionVec = Vector2i(0, 0);
                    }
                }
                else
                {
                    normalVecs = {directionVec};
                    directionVec *= 16;
                    directionVec += {32, 32};
                }
                break;
            }
            case 3:
            {
                normalVecs = {directionVec};
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
        if (iterator > 4 && iterator < 10)
        {
            generateNextDock(neighbourCount, normalVecs, {32, 32}, NPV1[iterator].pos);
        }

        ToModifyColumn[iterator].print = true;

        ToModifyColumn[iterator].LeftTop = directionVec;
        
    }
}

void ObstacleGenerator::generateNextDock(int NCount, std::vector<Vector2i> normalVec, Vector2i offset, Vector2f pos)
{
    int t_seed = _seed + pos.x + pos.y;

    static std::mt19937 gen(t_seed);
    static std::uniform_int_distribution<> spawnChance(1, 20);
    if (spawnChance(gen) != 20)
    {
        return;
    }

    static std::uniform_int_distribution<> dist(1, 4); //4 for case of a lone tile
    int result = dist(gen);

    Vector2i resultVec = {0, 0};

    if (normalVec.size() == 0)
    {
        return;
    }
    else
    {
        if (normalVec.size() == 1)
        {
            if (normalVec[0].y == 0 && normalVec[0].x == 0)
            {
                int mappedX = (result == 1) ? 1 : (result == 2) ? -1 : 0;
                int mappedY = (result == 3) ? 1 : (result == 4) ? -1 : 0;
                resultVec   = {mappedX, mappedY};
            }
            else if (normalVec[0].y == 0 || normalVec[0].x == 0)
            {
                resultVec = normalVec[0];
            }else 
            if (result % 2 == 1)
            {
                //put on x axis
                resultVec = {normalVec[0].x, 0};
            }
            else
            {
                //put on y axis
                resultVec = {0, normalVec[0].y};
            }
        }
        else
        {
            resultVec = normalVec[result % 2]; //assumes that this is the case for the shapes || or =
        }     
    }
    DockTiles Tile;
    Tile.LeftTop = resultVec * 16 + Vector2i(16, 16);
    Tile.pos     = pos + Vector2f(resultVec.x, resultVec.y)*16.f;

    for (auto& DockTile : DockTileVector)
    {
        if (Tile.pos == DockTile.pos)
        {
            return;
        }
    }
    StaticColliderComponent* component = new StaticColliderComponent({Tile.pos.x + 3, Tile.pos.y + 3, 10, 10}, "hard");
    DockColliders.insert(DockColliders.begin(), component);
    DockTileVector.insert(DockTileVector.begin(), Tile);
}

void ObstacleGenerator::shutdown()
{
    //Colliders = {};
    NPVs      = {};
    ObstacleSpriteMap = {};
    DockTileVector    = {};
    DockColliders     = {};
}
} // namespace mmt_gd
