#pragma once
#include "stdafx.h"
#include "GameState.hpp"
#include "FinalFrontier/dll_export.hpp"
#include "IEvent.hpp"
namespace mmt_gd
{

	class FINALFRONTIER_API GameOverState final : public GameState
	{
    public:
        GameOverState(GameStateManager* gameStateManager, Game* game);
        void init() override;
        void exit() override;

        void update(float deltaTime) override;
        void draw() override;
        void updateStats(IEvent::Ptr event);
    private:
        sf::Text m_text;
        sf::Font m_font;
        sf::View m_view;

        sf::Text m_distanceText;
        sf::Text m_eggsText;
        float distance = 0;
        int   eggsCollected = 0;
        bool  won;
        bool m_isInit = false;
	};
} // namespace mmt_gd