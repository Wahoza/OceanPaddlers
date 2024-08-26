//Author: Radomir Dinic, modified by Kilian Kolb
//Description: All Event types in the Game.
//License: LICENSE.txt
#include "stdafx.h"

#include "GameObjectEvents.hpp"

constexpr mmt_gd::IEvent::EventType mmt_gd::GameObjectCreateEvent::Type  = 0xdec71435;
constexpr mmt_gd::IEvent::EventType mmt_gd::GameObjectDestroyEvent::Type = 0x239f7a99;
constexpr mmt_gd::IEvent::EventType mmt_gd::ScoreEvent::Type = 0x12345678;
constexpr mmt_gd::IEvent::EventType mmt_gd::EggEvent::Type             = 0x12345679;
constexpr mmt_gd::IEvent::EventType mmt_gd::HurtEvent::Type               = 0x22345679;
constexpr mmt_gd::IEvent::EventType mmt_gd::GameOverEvent::Type              = 0x32345679;
constexpr mmt_gd::IEvent::EventType mmt_gd::FishCollisionEvent::Type           = 0x42345679;
constexpr mmt_gd::IEvent::EventType mmt_gd::InputEvent::Type         = 0x52345679;
constexpr mmt_gd::IEvent::EventType mmt_gd::VictorySoundEvent::Type                   = 0x62345679;
constexpr mmt_gd::IEvent::EventType mmt_gd::WaveSoundEvent::Type               = 0x63345679;
constexpr mmt_gd::IEvent::EventType mmt_gd::IslandTileGenEvent::Type                       = 0x73345679;
constexpr mmt_gd::IEvent::EventType mmt_gd::SwapTiles::Type                           = 0x83345679;
constexpr mmt_gd::IEvent::EventType mmt_gd::SwapTileRequest::Type                                = 0x8334567a;
constexpr mmt_gd::IEvent::EventType mmt_gd::PauseEvent::Type                                = 0x8334567b;
constexpr mmt_gd::IEvent::EventType mmt_gd::AttackEvent::Type                                = 0x9334567b;
