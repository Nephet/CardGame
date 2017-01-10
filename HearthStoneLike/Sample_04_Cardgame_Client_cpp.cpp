// Sample_04_Cardgame_Client_cpp.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "stormancer.h"
#include <iostream>
#include "Online/AuthenticationPlugin.h"
#include "Online/MatchmakingPlugin.h"
#include "Online/TurnByTurnPlugin.h"
#include "Online/GameSession.h"
#include <ctime>

#include "GameManager.h"
#include "Card.h"

// send transaction on the server
int ApplyTransaction(Stormancer::UpdateDto t, int& gameState, GameManager* gameManager)
{
	if (t.cmd == "start")
	{
		gameState = t.json_args()[L"seed"].as_integer();
		gameManager->Init(gameState);
	}
	else if (t.cmd == "add")
	{
		gameState += t.json_args()[L"value"].as_integer();
	}
	else if (t.cmd == "play")
	{
		int choice = t.json_args()[L"value"].as_integer();
		auto card = gameManager->GetPlayer1()->ChooseCard(choice);
	}
	else if (t.cmd == "pick")
	{
		gameManager->GetPlayer1()->PickCard();
	}
	else if (t.cmd == "ChooseP1")
	{
		gameManager->GetPlayer1()->InitPlayer("taken");
	}
	else if (t.cmd == "ChooseP2")
	{
		gameManager->GetPlayer2()->InitPlayer("taken");
	}
	return gameState;
}


int main(int argc, char *argv[])
{
	GameManager* gameManager;
	gameManager = new GameManager();

	std::string login = "a";
	if (argc >= 2)
	{
		login = std::string(argv[1]);
	}
	//Create network client
	auto config = Stormancer::Configuration::create("http://api.stormancer.com", "ad912dff-cec9-0fb6-78f8-652d4c093508", "sample-04-cardgame");
	config->addPlugin(new Stormancer::AuthenticationPlugin()); //Adds Authentication
	config->addPlugin(new Stormancer::MatchmakingPlugin()); //Adds matchmaking & gamesession APIs
	config->addPlugin(new Stormancer::TurnByTurnPlugin());

	auto client = Stormancer::Client::createClient(config);

	//Authenticate using steam (with stub enabled)
	auto auth = client->dependencyResolver()->resolve<Stormancer::IAuthenticationService>();
	std::cout << "Authenticating as '" << login << "'...";
	auto scene = auth->steamLogin(login).then([auth](pplx::task<Stormancer::ScenePtr> t)
	{
		try 
		{
			t.get();
			std::cout << "DONE" << std::endl;
		}
		catch (std::exception& ex) {
			std::cout << "Login failed : " << ex.what();
			throw;
		}
		return auth->getPrivateScene("matchmaking-fast");
	})
		.then([](pplx::task<Stormancer::ScenePtr> t)
	{
		auto matchmakingScene = t.get();
		//Connect to the matchmaking scene
		return matchmakingScene.lock()->connect().then([matchmakingScene](pplx::task<void> connectTask) 
		{
			try {
				connectTask.get();
				return matchmakingScene;
			}
			catch(std::exception& ex)
			{
				throw;
			}
		});
	}).get();


	auto matchmaking = scene.lock()->dependencyResolver()->resolve<Stormancer::MatchmakingService>();

	///Configure a task_completion_event which will fire when a game is found.
	auto tce = pplx::task_completion_event<Stormancer::MatchmakingResponse>{};
	matchmaking->onMatchFound([tce](Stormancer::MatchmakingResponse response)
	{

		tce.set(response);

	});

	matchmaking->findMatch("matchmaking-sample");

	std::cout << "waiting for game...";
	Stormancer::MatchmakingResponse mmResponse = pplx::create_task(tce).get();
	std::cout << "FOUND" << std::endl;

	Stormancer::ScenePtr game_scene = auth->getPrivateScene(mmResponse.gameId).get();
	int gameState = 0;
	auto transactionBroker = game_scene.lock()->dependencyResolver()->resolve<Stormancer::TurnByTurnService>();
	bool running = true;
	transactionBroker->onDesyncErrorCallback([&running](std::string error)
	{
		std::cout << "A desynchronization error occured. Details : " << error << std::endl;
		running = false;
	});
	transactionBroker->onUpdateGameCallback([&gameState, &gameManager](Stormancer::UpdateDto update)
	{

		auto newHash = ApplyTransaction(update, gameState, gameManager);
		std::cout << "game state updated : " << gameState << std::endl;
		return newHash; //Returns the new hash to the server for validation
	});
	transactionBroker->onReplayTLog([&gameState,&running, &gameManager](std::vector<Stormancer::TransactionLogItem> transactions)
	{
		std::cout << "Replay existing transaction log...";
		for (auto t : transactions)
		{
			auto newHash = ApplyTransaction(t.transactionCommand, gameState, gameManager);
			if (t.hashAvailable && t.resultHash != newHash)
			{
				std::cout << "Desynchronization while playing Transaction log. Expected "<<t.resultHash << " obtained "<< newHash << std::endl;
				std::string v;
				std::cin >> v;
				running = false;
				break;

			}
		}

	});


	std::cout << "Connecting to game...";
	auto gameSession = game_scene.lock()->dependencyResolver()->resolve<Stormancer::GameSessionService>();

	gameSession->connect().get();//Connect to the game session

	gameSession->ready();//Inform the server we are ready to play
	gameSession->waitServerReady().get();//
	std::cout << "CONNECTED" << std::endl;

	/*int nb = gameManager->SelectPlayer(auth->userId());

	try
	{
		if (nb == 1)
		{
			auto t = transactionBroker->submitTransaction(auth->userId(), "ChooseP1", 0);
			t.get();
		}
		else
		{
			auto t = transactionBroker->submitTransaction(auth->userId(), "ChooseP2", 0);
			t.get();
		}
	}
	catch (std::exception& ex)
	{
		std::cout << ex.what();
	}*/

	int n;
	while (running)
	{
		// **** original code ****
		/*std::cout << "Enter number to add to game state." << std::endl;
		std::cin >> n;
		auto json = web::json::value();
		json[L"value"] = n;
		try
		{
			auto t = transactionBroker->submitTransaction(auth->userId(), "add", json);
			t.get();
		}
		catch(std::exception& ex)
		{
			std::cout << ex.what();
		}*/
		// **** original code ****

		std::cout << "Current Hand :" << std::endl;
		gameManager->GetPlayer1()->PrintHand();

		std::cout << "Enter number to play a card." << std::endl;
		std::cin >> n;

		auto json = web::json::value();
		json[L"value"] = n;

		try
		{
			auto t = transactionBroker->submitTransaction(auth->userId(), "play", json);
			t.get();
		}
		catch (std::exception& ex)
		{
			std::cout << ex.what();
		}
		
		std::cout << "Player 1 Hand :" << std::endl;
		gameManager->GetPlayer1()->PrintHand();
		
		std::cout << "Player 2 Hand :" << std::endl;
		gameManager->GetPlayer2()->PrintHand();
	}

	std::cout << "disconnecting...";
	client->disconnect().get();
	std::cout << "DONE" << std::endl;
	return 0;
}


