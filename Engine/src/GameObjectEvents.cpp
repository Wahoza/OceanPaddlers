#include "stdafx.h"

#include "GameObjectEvents.hpp"

constexpr mmt_gd::IEvent::EventType mmt_gd::GameObjectCreateEvent::Type  = 0xdec71435;
constexpr mmt_gd::IEvent::EventType mmt_gd::GameObjectDestroyEvent::Type = 0x239f7a99;
constexpr mmt_gd::IEvent::EventType mmt_gd::ScoreEvent::Type = 0x12345678;
constexpr mmt_gd::IEvent::EventType mmt_gd::EggEvent::Type             = 0x12345679;
constexpr mmt_gd::IEvent::EventType mmt_gd::HurtEvent::Type               = 0x22345679;