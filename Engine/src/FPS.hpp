#pragma once
#include <SFML/System.hpp>

namespace mmt_gd
{
/**
 * \brief FPS-Counter inspired by https://gist.github.com/kenpower/7233967
 */
class Fps
{
public:
    /**
     * \brief Get the current FPS count.
     * \return FPS count.
     */
    unsigned int getFps() const
    {
        return m_fps;
    }

private:
    unsigned int m_frame = 0;
    unsigned int m_fps   = 0;
    sf::Clock    m_clock;

public:
    void update()
    {
        if (m_clock.getElapsedTime().asSeconds() >= 1.F)
        {
            m_fps   = m_frame;
            m_frame = 0;
            m_clock.restart();
        }

        ++m_frame;
    }
};
} // namespace mmt_gd
