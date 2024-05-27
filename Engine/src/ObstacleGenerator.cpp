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
void ObstacleGenerator::init(sf::IntRect _TextureRect, sf::Vector2i Area, int seed, RenderWindow& window)
{
    _windowRect = {0, 0, (float)Area.x * 16, (float)Area.y * 16};
    PathGen.init(seed, {0,9}, Area);
    ObjSpawner.init(TileLength, &window, seed);
    RockGen.init(Area, seed);
    _seed = seed;
    noise.SetSeed(seed);
    noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);

    LoadTextures("grassTiles/", _TextureRect);

    TileLength  = _TextureRect.width;
    waitingDist = 20 * TileLength;
    waiting     = true;
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

    WallColliderBottom = new StaticColliderComponent(FloatRect{0, 0, (float)Area.x * TileLength, 3.5f * TileLength}, "Wall");
    WallColliderTop    = new StaticColliderComponent(FloatRect{0.f, (float)(Area.y - (3.5)) * TileLength, (float)Area.x * TileLength, 3.5f * TileLength}, "Wall");
}
void ObstacleGenerator::update(float deltaTime, sf::FloatRect windowRect)
{
    _windowRect                      = windowRect;
    WallColliderTop->m_shape.left = windowRect.left;
    WallColliderBottom->m_shape.left = windowRect.left;

    PathGen.GetPath(NPVs[0][1].pos.x / TileLength);

    if (NPVs[NPVs.size() - 1][0].pos.x < windowRect.left - TileLength)
    {
        GenerateDirtArea(NPVs[0][0].pos.x);
        NPVs.pop_back();
        
        std::vector<NoisePositionValue> SubVector = NPVs[1];
        std::vector<float>              PathVals  = PathGen.GetPath(SubVector[0].pos.x / TileLength);

        for (int i = 0; i < SubVector.size(); i++)
        {
            SubVector[i].pos.x += TileLength * 2;
            if (!waiting)
            {
                SubVector[i].noise = noise.GetNoise(SubVector[i].pos.x / TileLength * scaleVal,
                                                    SubVector[i].pos.y / TileLength * scaleVal);
            }
            else
            {
                SubVector[i].noise = -2;
            }
            if (!DirtArea.contains(SubVector[i].pos) && DirtAreaBarrier.contains(SubVector[i].pos))
            {
                SubVector[i].noise = -1;
            }
            else if (DirtArea.contains(SubVector[i].pos))
            {
                SubVector[i].noise += 0.3;
            }

            if (disabledArea->contains(SubVector[i].pos))
            {
                SubVector[i].noise = -100.0;
                continue;
            }
            
            if (i < PathVals.size())
            {
                if (PathVals[i] < 0)
                {
                    SubVector[i].noise = PathVals[i];

                    if (PathVals[i] <= -2)
                    {
                        pathy = SubVector[i].pos.y;
                        
                        ObjSpawner.createPotentialCollectible(SubVector[i].pos);
                    }
                }
            }
            if (i > 0)
            {
                if (SubVector[i - 1].noise > threshold)
                {
                    SubVector[i].noise += 0.1;
                }

                if (NPVs[1][i].noise > threshold)
                {
                    SubVector[i].noise += 0.1;
                }
            }
            if (SubVector[i].noise < threshold && (i > 3 && i < 13))
            {
                static std::mt19937                    genCollectible(_seed + windowRect.left + SubVector[i].pos.y);
                static std::uniform_int_distribution<> collectibleSpread(0, 150);

                if (collectibleSpread(genCollectible) == 1)
                {
                    ObjSpawner.createFish(SubVector[i].pos);
                }
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

                if (DockTileVector.empty())
                {
                    break;
                }
            }
        }

        if (true) //!waiting
        {
            std::vector<StaticColliderComponent> SubColliderVector;

            for (auto& NPV : SubVector)
            {

                if (NPV.noise > threshold && (NPV.pos.y / 16 > 2 && NPV.pos.y / 16 <14))
                {
                    StaticColliderComponent component = *new StaticColliderComponent({NPV.pos.x + 1, NPV.pos.y + 1, 14, 14}, "hard");
                    SubColliderVector.push_back(component);
                }
            }

            Colliders.insert(Colliders.begin(), SubColliderVector);

            std::vector<int> hasIsland;

            for (auto& NPV : NPVs[1])
            {
                if (NPV.noise > threshold)// && (NPV.pos.y / 16 > 3 && NPV.pos.y / 16 < 13))
                {
                    int value = 0;
                    if (DirtArea.contains(NPV.pos)) {
                        value = -1;
                        if (NPV.pos.y / 16 > 3 && NPV.pos.y / 16 < 13)
                        {
                            value = 1;
                        }
                    }
                    hasIsland.push_back(value);
                }
                else
                {
                    hasIsland.push_back(-1);
                }
            }
            IEvent::Ptr Islands = std::make_shared<IslandTileGenEvent>(hasIsland);
            EventBus::getInstance().fireEvent(Islands);
            
            if (!disabledArea->contains(NPVs[0][0].pos))
                {
                    RockGen.generateNextRock(NPVs[0][0].pos.x, pathy);
                    RockGen.generateNextCurrent(NPVs[0][0].pos.x, pathy);
                }
            generateNextSprite();
            return;
        }

        
    }

    if (windowRect.left > waitingDist)
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
                if (obj.isDirt)
                {
                    m_dirtprinter.setPosition(obj.pos);
                    m_dirtprinter.setTextureRect(
                        {obj.LeftTop.x, obj.LeftTop.y, m_dirtprinter.getTextureRect().width, m_dirtprinter.getTextureRect().height});
                    window.draw(m_dirtprinter);
                }
                else
                {
                    if (obj.pos.y / 16 <= 3)
                    {
                        m_printer.setPosition({obj.pos.x, obj.pos.y + 2});
                        m_printer.setTextureRect({obj.LeftTop.x,
                                                  obj.LeftTop.y + 2, TextureSize, TextureSize -2});
                        window.draw(m_printer);
                    }
                    else if (obj.pos.y / 16 < 13)
                    {
                        m_printer.setPosition(obj.pos);
                        m_printer.setTextureRect({obj.LeftTop.x, obj.LeftTop.y, TextureSize, TextureSize});
                        window.draw(m_printer); 
                    }
                    else
                    {
                        m_printer.setPosition(obj.pos);
                        m_printer.setTextureRect({obj.LeftTop.x, obj.LeftTop.y, TextureSize, TextureSize -2});
                        window.draw(m_printer); 
                    }
                }
            }

        }
    }
    
  if(DebugDraw::getInstance().is_enabled()){

        for (auto& NPV : NPVs)
        {
            for (auto& Tile : NPV)
            {
                sf::Color col = {200, 20, 20, 30};

                if (Tile.noise > threshold)
                {
                    col = {20, 200, 20, 30};
                }

                if (Tile.noise == -3)
                {
                    col = {100, 100, 100, 30};
                }
                DebugDraw::getInstance().drawRectangle(Tile.pos, {16, 16}, col, col);

            }
        }
        
        sf::Color col = {200, 200, 200, 30};
        DebugDraw::getInstance().drawRectangle({DirtArea.left, DirtArea.top}, {DirtArea.width, DirtArea.height}, col, col);
        col = {100, 255, 100, 30};
        DebugDraw::getInstance()
            .drawRectangle({disabledArea->left, disabledArea->top}, {disabledArea->width, disabledArea->height}, col, col);
    }
}
void ObstacleGenerator::drawWater(sf::RenderWindow& window)
{
    for (auto& ObVec : ObstacleSpriteMap)
    {
        for (auto& obj : ObVec)
        {

            if (obj.print)
            {
                if (obj.pos.y / 16 <= 3)
                {
                    m_waterPrinter.setPosition(obj.pos);
                    m_waterPrinter.setTextureRect({20, 0, 20, 20});
                    window.draw(m_waterPrinter);
                }
                else if (obj.pos.y/ 16 > 3 && obj.pos.y/ 16 <13)
                {
                    m_waterPrinter.setPosition(obj.pos);
                    m_waterPrinter.setTextureRect({0, 0, 20, 20});
                    window.draw(m_waterPrinter);
                }
                else
                {
                    m_waterPrinter.setPosition(obj.pos);
                    m_waterPrinter.setTextureRect({40, 0, 20, 20});
                    window.draw(m_waterPrinter);
                }
            }
        }
    }

    RockGen.draw(window);
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
    for (int iterator = 2; iterator <= ToModifyColumn.size()-2; iterator++)
    {
        if (!(NPV1[iterator].noise > threshold) || !(NPV1[iterator].pos.y / 16 > 2 && NPV1[iterator].pos.y / 16 < 14) ||
            waiting)
        {
            continue;
        }

        ObstacleSpriteMap[1][iterator].pos = NPV1[iterator].pos;
        Vector2i directionVec              = {0, 0};
        std::vector<Vector2i> normalVecs     = {};
        int neighbourCount                 = 0;
        bool     hasUpVec                  = false; //note: there is one use for this to find a difference between two 0,0 vecs
        
        if (!DirtArea.contains(ToModifyColumn[iterator].pos))
        {
            if ((NPV1[iterator + 1].noise > threshold && (NPV1[iterator + 1].pos.y / 16 < 14)) ||
                (NPV1[iterator + 1].pos.y / 16 >= 14))
            {
                directionVec += {0, -1};
                neighbourCount++;
                hasUpVec = true;
            }
            if ((NPV1[iterator - 1].noise > threshold && (NPV1[iterator - 1].pos.y / 16 > 2)) ||
                (NPV1[iterator - 1].pos.y / 16 <= 2))
            {
                directionVec += {0, 1};
                neighbourCount++;
                hasUpVec = true;
            }
        }
        else
        {
            if (!(ToModifyColumn[iterator].pos.y / 16 > 3 && ToModifyColumn[iterator].pos.y / 16 < 13))
            {
                continue;
            }
            if ((NPV1[iterator + 1].noise > threshold && (NPV1[iterator + 1].pos.y / 16 < 13)))
            {
                directionVec += {0, -1};
                neighbourCount++;
                hasUpVec = true;
            }
            if ((NPV1[iterator - 1].noise > threshold && (NPV1[iterator - 1].pos.y / 16 > 3)))
            {
                directionVec += {0, 1};
                neighbourCount++;
                hasUpVec = true;
            }
        }
        if (NPV0[iterator].noise > threshold)
        {
            directionVec += {-1, 0};
            neighbourCount++;
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
        if (iterator > 4 && iterator < 11)
        {
            generateNextDock(neighbourCount, normalVecs, {32, 32}, NPV1[iterator].pos);
        }

        ToModifyColumn[iterator].print = true;

        ToModifyColumn[iterator].LeftTop = directionVec;
        auto& currentPos                    = ToModifyColumn[iterator].pos;


        if (DirtArea.contains(ToModifyColumn[iterator].pos))
        {
            ToModifyColumn[iterator].isDirt = true;
        }
        else
        {
            ToModifyColumn[iterator].isDirt = false;
        }
    }
}
void ObstacleGenerator::generateNextDock(int NCount, std::vector<Vector2i> normalVec, Vector2i offset, Vector2f pos)
{
    int t_seed = _seed + pos.x + pos.y;

    static std::mt19937 gen(t_seed);
    static std::uniform_int_distribution<> spawnChance(1, 15);
    if (spawnChance(gen) != 1)
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
    StaticColliderComponent* component = new StaticColliderComponent({Tile.pos.x + 4, Tile.pos.y + 4, 8, 8}, "hard");
    DockColliders.insert(DockColliders.begin(), component);
    DockTileVector.insert(DockTileVector.begin(), Tile);
}
void ObstacleGenerator::GenerateDirtArea(float xpos)
{
    if (DirtAreaBarrier.left + DirtAreaBarrier.width +16 < xpos)
    {
        //generate new DirtArea aka place fór islands with dirt
        int t_seed = _seed + xpos;

        static std::mt19937                    gen(t_seed);
        static std::uniform_int_distribution<> spawnChance(0, dirtSpawnChance);
        if (spawnChance(gen) == 1)
        {
            DirtAreaBarrier.left = xpos + 16;

            static std::uniform_int_distribution<> areaTop(3, 9);
            DirtAreaBarrier.top = areaTop(gen) * 16;

            static std::uniform_int_distribution<> areaWidth(min_dirtArea.width, max_dirtArea.width);
            DirtAreaBarrier.width = areaWidth(gen) * 16;

            
            static std::uniform_int_distribution<> areaHeight(min_dirtArea.height, max_dirtArea.height);
            DirtAreaBarrier.height = areaHeight(gen) * 16;

            DirtArea = DirtAreaBarrier;
            DirtArea.left += 16;
            DirtArea.top += 16;
            DirtArea.width -= 16;
            DirtArea.height -= 16;

            //Adjustment to include the next tile above and below
            DirtAreaBarrier.height += 1;
            DirtAreaBarrier.width += 1;
        }
    }
}
void ObstacleGenerator::DisableGenArea(sf::FloatRect* Rect)
{
    disabledArea = Rect;
}
void ObstacleGenerator::LoadTextures(string Folder, IntRect _TextureRect)
{
    string TotalFilePath = "../assets/SegmentedTiles/" + Folder;
    if (!m_texture.loadFromFile(TotalFilePath + "tiles.png"))
    {
        sf::err() << "Could not load texture\n";
        return;
    }
    m_printer.setTexture(m_texture);
    m_printer.setTextureRect(_TextureRect);
    TextureSize = _TextureRect.width;

    if (!m_dirttexture.loadFromFile(TotalFilePath + "tilesDirt.png"))
    {
        sf::err() << "Could not load texture\n";
        return;
    }
    m_dirtprinter.setTexture(m_dirttexture);
    m_dirtprinter.setTextureRect(_TextureRect);

    if (!m_dockTexture.loadFromFile(TotalFilePath + "dockTiles2.png"))
    {
        sf::err() << "Could not load texture\n";
        return;
    }
    m_dockPrinter.setTexture(m_dockTexture);
    m_dockPrinter.setTextureRect(_TextureRect);

    if (!m_waterTexture.loadFromFile(TotalFilePath + "waterTile.png"))
    {
        sf::err() << "Could not load icon\n";
        return;
    }
    m_waterPrinter.setTexture(m_waterTexture);
    m_waterPrinter.setOrigin(2, 0);

    RockGen.loadTextures(Folder);
}
void ObstacleGenerator::shutdown()
{
    NPVs      = {};
    ObstacleSpriteMap = {};
    DockTileVector    = {};
    DockColliders     = {};
    PathGen.shutdown();
    RockGen.shutdown();
}
} // namespace mmt_gd
