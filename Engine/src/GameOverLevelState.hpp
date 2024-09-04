//Author: Kilian Kolb
//Description: Game State for the End of the Game. Contains posibility for victory screen or game over Screen. utilizes InputObject.
//License: LICENSE.txt
#pragma once
#include "stdafx.h"
#include "GameState.hpp"
#include "FinalFrontier/dll_export.hpp"
#include "IEvent.hpp"
namespace mmt_gd
{

	class FINALFRONTIER_API GameOverLevelState final : public GameState
	{
    public:
        GameOverLevelState(GameStateManager* gameStateManager, Game* game);
        void init() override;
        void exit() override;

        void update(float deltaTime) override;
        void draw() override;
        void updateStats(IEvent::Ptr event);

    private:
        sf::Text m_text;
        sf::Font m_font;
        sf::View m_view;

        sf::Text m_name;
        sf::Text m_distanceText;
        sf::Text m_eggsText;
        sf::Text m_totalScoreText;
        float distance = 0;
        int   eggsCollected = 0;
        int      totalScore    = 0;
        int      FishKills = 0;
        bool  won = false;
	};
} // namespace mmt_gd