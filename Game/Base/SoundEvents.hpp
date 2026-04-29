#pragma once

#include <unordered_map>

#include "Sounds/SoundSystem.h"


namespace Game::Sounds
{
	enum class SoundEvents
	{
		GameplayMusic,
		CollectedEmerald,
		GoldBagCountdown,
		GoldBagFalling,
		PlayerHit,
		PlayerDeath,
		PlayerDeathLong,
		BallTravelingSound,
		BallHitSound,
	};


	static std::unordered_map<SoundEvents, bae::SoundID> g_sSoundEvents;

	// gives warning if not used
#pragma warning (push)
#pragma warning(disable : 4505)

	static bae::SoundID GetSoundId(SoundEvents event)
	{
		auto it = g_sSoundEvents.find(event);
		if (it == g_sSoundEvents.end())
			return bae::SoundID{ .ID = -1 };

		return it->second;
	}

#pragma warning (pop)

}

