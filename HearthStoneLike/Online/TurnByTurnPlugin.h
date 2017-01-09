#pragma once
#include "stormancer.h"

namespace Stormancer
{
	class TransactionCommandDto
	{
	public:
		std::string playerId;
		std::string cmd;
		std::string args;
		MSGPACK_DEFINE(playerId, cmd, args)
	};
	class UpdateDto
	{
	public:
		int finalStepId;
		long long createdOn;
		std::string issuerUserId;
		std::string issuerPlayerId;
		std::string cmd;
		std::string arguments;

		web::json::value json_args()
		{
			return web::json::value::parse(utility::string_t(arguments.begin(), arguments.end()));
		}
		MSGPACK_DEFINE(finalStepId, createdOn, issuerUserId, issuerPlayerId, cmd, arguments)
	};

	class TransactionLogItem
	{
	public:
		UpdateDto transactionCommand;
		int resultHash;
		bool hashAvailable;
	};


	class UpdateResponseDto
	{
	public:
		bool success;
		std::string reason;
		int hash;
		MSGPACK_DEFINE(success, reason, hash)
	};

	class TurnByTurnService
	{
	public:
		TurnByTurnService(Scene* scene);
		void onUpdateGameCallback(std::function<int(UpdateDto)> callback);
		void onDesyncErrorCallback(std::function<void(std::string)> callback);
		void onReplayTLog(std::function<void(std::vector<TransactionLogItem>)> callback);
		pplx::task<void> submitTransaction(std::string playerId, std::string cmd,const web::json::value& args) const;
		pplx::task<void> mapPlayer(std::string playerId) const;

	private:
		Scene* _scene;
		std::function<void(std::string)> _onDesync;
		std::function<int(UpdateDto)> _onUpdateGame;
		std::function<void(std::vector<TransactionLogItem>)> _onReplayTLog;
	};

	class TurnByTurnPlugin : public IPlugin
	{
	public:
		void sceneCreated(Stormancer::Scene* scene) override;
		void destroy() override;
	};
}
