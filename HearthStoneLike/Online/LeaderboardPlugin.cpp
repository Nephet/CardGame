#include "stdafx.h"
#include "LeaderboardPlugin.h"

namespace Stormancer
{
	void Stormancer::LeaderboardPlugin::sceneCreated(Stormancer::Scene * scene)
	{
		if (scene)
		{
			auto name = scene->getHostMetadata("stormancer.plugins.leaderboard");

			if (name.length() > 0)
			{
				auto service = std::make_shared<LeaderboardService>(scene);
				scene->dependencyResolver()->registerDependency<LeaderboardService>(service);
			}

		}
	}

	Stormancer::LeaderboardService::LeaderboardService(Scene * scene)
		: _scene(scene),
		_rpcService(scene->dependencyResolver()->resolve<Stormancer::IRpcService>())
	{
	}

	pplx::task<LeaderboardResult> Stormancer::LeaderboardService::Query(LeaderboardQuery query)
	{
		return _rpcService->rpc<LeaderboardQuery, LeaderboardResult>("leaderboard.query", query);
	}

	pplx::task<LeaderboardResult> Stormancer::LeaderboardService::Query(const std::string cursor)
	{
		return _rpcService->rpc<std::string, LeaderboardResult>("leaderboard.cursor", cursor);
	}
}