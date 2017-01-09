#pragma once
#include "stormancer.h"
#include "cpprest\json.h"
namespace Stormancer
{
	class LeaderboardPlugin :public IPlugin
	{
		void sceneCreated(Stormancer::Scene* scene);
	};

	struct ScoreFilter
	{
	public:
		static const int GREATER_THAN_OR_EQUAL = 0;
		static const int GREATER_THAN = 1;
		static const int LESSER_THAN_OR_EQUAL = 2;
		static const int LESSER_THAN = 3;
		int type;
		int value;

		MSGPACK_DEFINE(type, value);
	};

	struct fieldFilter
	{
	public:
		std::string field;
		std::string value;

		MSGPACK_DEFINE(field, value);
	};

	struct LeaderboardQuery
	{
	public:
		std::string startId;

		std::vector<ScoreFilter> scoreFilters;

		std::vector<fieldFilter> fieldFilters;

		int size;
		int skip;

		std::string leaderboard;

		MSGPACK_DEFINE(startId, scoreFilters, fieldFilters, size, skip, leaderboard);
	};

	struct ScoreRecord
	{
	public:
		std::string id;
		
		int score;
		uint64 createdOn;
		std::string document;

		web::json::object getScoreData()
		{
			return web::json::value::parse(utility::string_t(document.begin(),document.end())).as_object();
		}

		MSGPACK_DEFINE(id, score, createdOn, document);
	};

	struct LeaderboardRanking
	{
	public:
		int ranking;
		ScoreRecord document;


		MSGPACK_DEFINE(ranking, document);
	};


	struct LeaderboardResult
	{
	public:

		std::vector<LeaderboardRanking> results;
		std::string next;
		std::string previous;

		MSGPACK_DEFINE(results, next, previous);
	};
	class LeaderboardService
	{
	public:
		LeaderboardService(Scene* scene);

		//Query a leaderboard
		pplx::task<LeaderboardResult> Query(LeaderboardQuery query);

		//Query a leaderboard using a cursor obtained from a LeaderboardResult (result.next or result.previous)
		pplx::task<LeaderboardResult> Query(const std::string cursor);

	private:
		Stormancer::Scene* _scene = nullptr;
		std::shared_ptr<Stormancer::IRpcService> _rpcService;

	};
}