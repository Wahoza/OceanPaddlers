#pragma once
#include "stdafx.h"
#include "GameState.hpp"
#include "FinalFrontier/dll_export.hpp"
#include "IEvent.hpp"
#include "InputObject.hpp"
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

        sf::Text m_name;
        sf::Text m_distanceText;
        sf::Text m_eggsText;
        float distance = 0;
        int   eggsCollected = 0;
        bool  won = false;
        bool m_isInit = false;
        float    wait          = 0;
	};
} // namespace mmt_gd