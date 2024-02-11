//Author: Kilian Kolb
//Description: Plays a sound upon recieving Event, Sounds are mapped to a string
//License: LICENSE.txt
#pragma once
#include "stdafx.h"
#include <unordered_map>
namespace mmt_gd
{
class SoundManager
{
public:
    static SoundManager& getInstance();
    
    SoundManager(const SoundManager& rhv)             = delete;
    SoundManager(SoundManager&& rhv)                  = delete;
    SoundManager&  operator=(const SoundManager& rhv) = delete;
    SoundManager&& operator=(SoundManager&& rhv)      = delete;

    SoundManager()  = default;
    ~SoundManager() = default;

    void init();
    void playSound(std::string soundName);
    void playMusic(std::string musicName);
    void stopMusic(std::string musicName);

    void addSound(std::string filePath, std::string soundName);
    void addMusic(std::string filePath, std::string musicName);

private:
    std::unordered_map<std::string, std::shared_ptr<sf::SoundBuffer>> m_buffers;
    std::unordered_map<std::string, std::shared_ptr<sf::Music>> m_music;
    sf::Sound                                                         sound;
};
}
