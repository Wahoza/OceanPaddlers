//Author: Kilian Kolb
//Description: Plays a sound upon recieving Event, Sounds are mapped to a string
//License: LICENSE.txt
#include "stdafx.h"
#include "SoundManager.hpp"
#include "EventBus.hpp"
#include "GameObjectEvents.hpp"
namespace mmt_gd
{
SoundManager& SoundManager::getInstance()
{
    static SoundManager instance;
    return instance;
}
void SoundManager::init()
{
    //setSounds
    addSound("../assets/audio/sounds/bonk.wav", "StoneBonk");
    addSound("../assets/audio/sounds/fish.wav", "FishBonk");
    addSound("../assets/audio/sounds/egg.wav", "EggCollect");
    addSound("../assets/audio/sounds/input.wav", "Input");
    addSound("../assets/audio/sounds/victory.wav", "Victory");
    addSound("../assets/audio/sounds/wave.wav", "Wave");
    addSound("../assets/audio/sounds/attack_turtle.wav", "TurtleAttack");
    addSound("../assets/audio/sounds/fish_hit.wav", "FishHit");
    addSound("../assets/audio/sounds/health_collect.wav", "Health");


    addMusic("../assets/audio/music/Music.ogg", "Music");
    
    EventBus::getInstance().addListener(HurtEvent::Type, [this](IEvent::Ptr event) { playSound("StoneBonk"); });
    EventBus::getInstance().addListener(EggEvent::Type, [this](IEvent::Ptr event) { playSound("EggCollect"); });
    EventBus::getInstance().addListener(FishCollisionEvent::Type, [this](IEvent::Ptr event) { playSound("FishBonk"); });
    EventBus::getInstance().addListener(GameOverEvent::Type, [this](IEvent::Ptr event) { stopMusic("Music"); });
    EventBus::getInstance().addListener(InputEvent::Type, [this](IEvent::Ptr event) { playSound("Input"); });
    EventBus::getInstance().addListener(VictorySoundEvent::Type, [this](IEvent::Ptr event) { playSound("Victory"); });
    EventBus::getInstance().addListener(WaveSoundEvent::Type, [this](IEvent::Ptr event) { playSound("Wave"); });
    EventBus::getInstance().addListener(SwapTiles::Type, [this](IEvent::Ptr event) { playSound("Victory"); });
    EventBus::getInstance().addListener(AttackEvent::Type, [this](IEvent::Ptr event) { playSound("TurtleAttack"); });
    EventBus::getInstance().addListener(FishHitSuccess::Type, [this](IEvent::Ptr event) { playSound("FishHit"); });
    EventBus::getInstance().addListener(HealthCollect::Type, [this](IEvent::Ptr event) { playSound("Health"); });
}
void SoundManager::playSound(std::string soundName)
{
    //play soundbuffer based on name
    if (sound.getStatus() == sf::Sound::Status::Playing && soundName != "StoneBonk" && soundName != "EggCollect")
    {

        return;
    }
    auto it = m_buffers.find(soundName);
    if (it != m_buffers.end())
    {
        
        // Create a sound and set its buffer
        
        sound.setBuffer(*it->second.get());
        //std::cout << soundName << std::endl;
        // Play the sound
        sound.setVolume(100);
        sound.play();
    }
}
void SoundManager::playMusic(std::string musicName)
{
    //play music based on name
    auto it = m_music.find(musicName);
    if (it != m_music.end())
    {
        it->second->play();
    }
}
void SoundManager::stopMusic(std::string musicName)
{
    auto it = m_music.find(musicName);
    if (it != m_music.end())
    {
        it->second->stop();
    }
}
void SoundManager::addSound(std::string filePath, std::string soundName)
{
    auto buffer = std::make_shared<sf::SoundBuffer>();

    if (!buffer->loadFromFile(filePath))
    {
        return;
    }
    m_buffers[soundName] = buffer;
}
void SoundManager::addMusic(std::string filePath, std::string musicName)
{
    auto music = std::make_shared<sf::Music>();

     if (!music->openFromFile(filePath))
    {
        return;
    }
    music->setLoop(true);

    m_music[musicName] = music;
}
} // namespace mmt_gd