#include "stdafx.h"
#include "TurnByTurnPlugin.h"

void Stormancer::TurnByTurnPlugin::sceneCreated(Stormancer::Scene* scene)
{
	
	if (scene)
	{
		auto name = scene->getHostMetadata("stormancer.turnByTurn");

		if (name.length() > 0)
		{
			auto service = std::make_shared<TurnByTurnService>(scene);
			scene->dependencyResolver()->registerDependency<TurnByTurnService>(service);
		}

		
	}

}

void Stormancer::TurnByTurnPlugin::destroy()
{
	delete this;
}

Stormancer::TurnByTurnService::TurnByTurnService(Scene* scene)
{
	_scene = scene;
	_scene->addRoute("tbt.desync", [this](Stormancer::Packetisp_ptr packet)
	{
		std::string input;
		std::string buffer;
		*packet->stream >> buffer;
		msgpack::unpacked unp;
		msgpack::unpack(unp, buffer.data(), buffer.size());
		unp.get().convert(&input);
		if (_onDesync)
		{
			_onDesync(input);
		}
	});

	_scene->dependencyResolver()->resolve<IRpcService>()->addProcedure("transaction.execute", [this](RpcRequestContext_ptr ctx)
	{
		UpdateDto input;
		std::string buffer;
		*ctx->inputStream() >> buffer;
		msgpack::unpacked unp;
		msgpack::unpack(unp, buffer.data(), buffer.size());

		unp.get().convert(&input);
		UpdateResponseDto parameter;
		try
		{
			int value = _onUpdateGame(input);
			parameter = UpdateResponseDto{ true, "", value };
		}
		catch (std::exception& ex)
		{
			parameter = UpdateResponseDto{ false, ex.what(),0 };
		}
		ctx->sendValue([parameter](bytestream* stream)
		{
			msgpack::pack(stream, parameter);
		}, PacketPriority::MEDIUM_PRIORITY);
		return pplx::task_from_result();
	}, true);
}

void Stormancer::TurnByTurnService::onDesyncErrorCallback(std::function<void(std::string)> callback)
{
	_onDesync = callback;
}

void Stormancer::TurnByTurnService::onReplayTLog(std::function<void(std::vector<TransactionLogItem>)> callback)
{
	_onReplayTLog = callback;
}

void Stormancer::TurnByTurnService::onUpdateGameCallback(std::function<int(UpdateDto)> callback)
{
	_onUpdateGame = callback;
	
}


pplx::task<void> Stormancer::TurnByTurnService::submitTransaction(std::string playerId, std::string cmd,const web::json::value& args) const
{
	auto json = *args.serialize();
	TransactionCommandDto dto{ playerId,cmd,std::string(json.begin(),json.end()) };
	return _scene->dependencyResolver()->resolve<IRpcService>()->rpcVoid("transaction.submit", dto);
}

pplx::task<void> Stormancer::TurnByTurnService::mapPlayer(std::string playerId) const
{
	return _scene->dependencyResolver()->resolve<IRpcService>()->rpcVoid("transaction.addplayer", playerId);
}
