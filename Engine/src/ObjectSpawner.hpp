#include "stdafx.h"
namespace mmt_gd
{
class ObjectSpawner
{
public:
    void init(int TileSize, sf::RenderWindow* window);
    void createCollectible(sf::Vector2f pos);
    void createFish(sf::Vector2f pos);

private:
    int _tileSize;
    int CollectibleCtr = 0;
    int FishCtr = 0;
    sf::RenderWindow* m_window;
};
}