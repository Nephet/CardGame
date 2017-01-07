#pragma once
#include "stormancer.h"
#include "MatchMakingService.h"

namespace Stormancer
{
	class MatchmakingPlugin : public Stormancer::IPlugin
	{
	public:
		void sceneCreated(Stormancer::Scene* scene) override;
		void destroy();
	};
};
