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
#include "Player.h"
#include "Board.h"

// send transaction on the server
int ApplyTransaction(Stormancer::UpdateDto t, int& gameState, GameManager* gameManager)
{
	if (t.cmd == "start")
	{
		gameState = t.json_args()[L"seed"].as_integer();
		gameManager->Init(gameState);
	}
	else if (t.cmd == "playCard")
	{
		int player = t.json_args()[L"player"].as_integer();
		int choice = t.json_args()[L"choice"].as_integer();
		if (player == 1)
		{
			Card* card = gameManager->GetPlayer1()->ChooseCard(choice);
			int mana = card->GetManaNeeded();
			gameManager->PutCardOnBoard(player, card);
			gameManager->GetPlayer1()->RemoveCard(choice);
			gameManager->GetPlayer1()->RemoveMana(mana);
		}
		else
		{
			Card* card = gameManager->GetPlayer2()->ChooseCard(choice);
			int mana = card->GetManaNeeded();
			gameManager->PutCardOnBoard(player, card);
			gameManager->GetPlayer2()->RemoveCard(choice);
			gameManager->GetPlayer2()->RemoveMana(mana);
		}
	}
	else if (t.cmd == "AttackCard")
	{
		int player = t.json_args()[L"player"].as_integer();
		int choice = t.json_args()[L"choice"].as_integer();
		int attackCard = t.json_args()[L"attackCard"].as_integer();
		int enemyCard = t.json_args()[L"enemyCard"].as_integer();

		if (enemyCard == 0)
		{
			if (player)
			{
				Card* card = gameManager->GetPlayer1Board()->GetCard(attackCard);
				gameManager->SendDamageToEnemy(player, card->GetAttack());
			}
			else
			{
				Card* card = gameManager->GetPlayer2Board()->GetCard(attackCard);
				gameManager->SendDamageToEnemy(player, card->GetAttack());
			}
		}
		else
		{
			if (player)
			{
				Card* card = gameManager->GetPlayer1Board()->GetCard(attackCard);
				gameManager->AttackCard(player, card, enemyCard);
			}
			else
			{
				Card* card = gameManager->GetPlayer2Board()->GetCard(attackCard);
				gameManager->AttackCard(player, card, enemyCard);
			}
		}

	}
	else if (t.cmd == "pick")
	{
		int player = t.json_args()[L"player"].as_integer();
		if(player == 1)
			gameManager->GetPlayer1()->PickCard();
		else
			gameManager->GetPlayer2()->PickCard();
	}
	else if (t.cmd == "endOfTurn")
	{
		gameManager->EndOfTurn();
	}
	else if (t.cmd == "AskForPlayer")
	{
		int pseudo = t.json_args()[L"pseudo"].as_integer();
		gameManager->AddPlayer(pseudo);
	}
	return gameState;
}


int main(int argc, char *argv[])
{
	GameManager* gameManager;
	gameManager = new GameManager();

	std::cout << "Enter your pseudo" << std::endl;

	std::string login = "a";
	std::cin >> login;

	std::hash<std::string> hash_fn;
	size_t str_hash = hash_fn(login);

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
			catch (std::exception& ex)
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

	matchmaking->findMatch("matchmaking-sample", "AntoineBenoitJulien");

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
	transactionBroker->onUpdateGameCallback([&gameState, &gameManager, &str_hash](Stormancer::UpdateDto update)
	{

		auto newHash = ApplyTransaction(update, gameState, gameManager);

		if (!gameManager->CheckEndOfGame() && !gameManager->CheckGameFinished() && gameManager->GetGameStarted())
		{
			system("cls");

			bool turn = gameManager->GetCurrentPlayer(str_hash);

			if (turn != gameManager->GetPlayerTurn())
			{
				gameManager->PrintPlayersLife(turn);
				gameManager->PrintPlayerMana(turn);
				gameManager->PrintPlayerHand(turn);
				gameManager->PrintBoards(turn);
			}
		}
		else if(gameManager->CheckEndOfGame() && gameManager->CheckGameFinished() && gameManager->GetGameStarted())
		{
			gameManager->PrintWinner(str_hash);
		}

		return newHash; //Returns the new hash to the server for validation
	});
	transactionBroker->onReplayTLog([&gameState, &running, &gameManager](std::vector<Stormancer::TransactionLogItem> transactions)
	{
		std::cout << "Replay existing transaction log...";
		for (auto t : transactions)
		{
			auto newHash = ApplyTransaction(t.transactionCommand, gameState, gameManager);
			if (t.hashAvailable && t.resultHash != newHash)
			{
				std::cout << "Desynchronization while playing Transaction log. Expected " << t.resultHash << " obtained " << newHash << std::endl;
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

	bool thePlayer = true;

	try
	{
		auto json = web::json::value();
		json[L"player"] = thePlayer;
		json[L"pseudo"] = str_hash;
		auto t = transactionBroker->submitTransaction(auth->userId(), "AskForPlayer", json);
		t.get();
		thePlayer = gameManager->GetCurrentPlayer(str_hash);
	}
	catch (std::exception& ex)
	{
		std::cout << ex.what();
	}

	system("cls");

	std::cout << "waiting for the other player..." << std::endl;

	int n;
	std::string key;
	// game loop
	while (running)
	{
		if (gameManager->CheckEndOfGame())
		{
			running = false;
			break;
		}

		if (gameManager->GetPlayerTurn() == thePlayer)
		{

			if (gameManager->CheckEndOfGame())
			{
				running = false;
				break;
			}

			system("cls");

			try
			{
				auto json = web::json::value();
				json[L"player"] = thePlayer;
				auto t = transactionBroker->submitTransaction(auth->userId(), "pick", json);
				t.get();
			}
			catch (std::exception& ex)
			{
				std::cout << ex.what();
			}

			gameManager->GivePlayerMana();

			// turn loop
			bool endOfTurn = false;
			while (!endOfTurn)
			{
				if (gameManager->CheckEndOfGame())
				{
					running = false;
					break;
				}

				system("cls");
				gameManager->PrintPlayersLife(thePlayer);
				gameManager->PrintPlayerMana(thePlayer);
				gameManager->PrintPlayerHand(thePlayer);
				gameManager->PrintBoards(thePlayer);

				bool success = false;

				std::cout << std::endl;

				std::cout << "0 - End The Turn" << std::endl;
				std::cout << "99 - Attack a Card" << std::endl;
				std::cout << "1..x - Play a Card" << std::endl;
				std::cin >> n;

				auto json = web::json::value();
				json[L"player"] = thePlayer;
				json[L"choice"] = n;

				// end of turn
				if (n == 0)
				{
					try
					{
						auto t = transactionBroker->submitTransaction(auth->userId(), "endOfTurn", json);
						t.get();
						success = true;
					}
					catch (std::exception& ex)
					{
						std::cout << ex.what();
					}
					endOfTurn = true;
				}
				// attack a card
				else if (n == 99)
				{
					if (thePlayer)
					{
						if (gameManager->GetPlayer1Board()->GetNbCardsOnBoard() == 0)
						{
							std::cout << "no cards on the board to attack... (press a key to continue)" << std::endl;
							std::cin >> key;
							success = false;
						}
						else
						{
							int cardToAttack = -1;
							int enemyCard = -1;
							std::cout << "Select a card to attack..." << std::endl;
							std::cin >> cardToAttack;
							Card* card = gameManager->GetPlayer1Board()->GetCard(cardToAttack);
							if (card != nullptr)
							{
								if (card->GetHadAttacked())
								{
									std::cout << "This card have already attacked (press a key to continue)" << std::endl;
									std::cin >> key;
									success = false;
								}
								else
								{

									json[L"attackCard"] = cardToAttack;

									std::cout << "Select an enemy card... (0 : enemy base)" << std::endl;
									std::cin >> enemyCard;

									Card* cardEnemy = nullptr;

									if (enemyCard != 0)
										cardEnemy = gameManager->GetPlayer2Board()->GetCard(enemyCard);

									if (cardEnemy != nullptr || enemyCard == 0)
									{
										json[L"enemyCard"] = enemyCard;
										card->SetHadAttacked(true);
										try
										{
											auto t = transactionBroker->submitTransaction(auth->userId(), "AttackCard", json);
											t.get();
											success = true;
										}
										catch (std::exception& ex)
										{
											std::cout << ex.what();
											success = false;
										}
									}
									else
									{
										std::cout << "No enemy card to attack... (press a key to continue)" << std::endl;
										std::cin >> key;
										success = false;
									}
								}
							}
							else
							{
								std::cout << "No card to attack... (press a key to continue)" << std::endl;
								std::cin >> key;
								success = false;
							}
						}
					}
					else
					{
						if (gameManager->GetPlayer2Board()->GetNbCardsOnBoard() == 0)
						{
							std::cout << "no cards on the board to attack... (press a key to continue)" << std::endl;
							std::cin >> key;
							success = false;
						}
						else
						{
							int cardToAttack = -1;
							int enemyCard = -1;
							std::cout << "Select a card to attack..." << std::endl;
							std::cin >> cardToAttack;
							Card* card = gameManager->GetPlayer2Board()->GetCard(cardToAttack);
							if (card != nullptr)
							{
								if (card->GetHadAttacked())
								{
									std::cout << "This card have already attacked (press a key to continue)" << std::endl;
									std::cin >> key;
									success = false;
								}
								else
								{
									card->SetHadAttacked(true);

									json[L"attackCard"] = cardToAttack;

									std::cout << "Select an enemy card... (0 : enemy base)" << std::endl;
									std::cin >> enemyCard;

									Card* cardEnemy = nullptr;

									if (enemyCard != 0)
										cardEnemy = gameManager->GetPlayer2Board()->GetCard(enemyCard);

									if (cardEnemy != nullptr || enemyCard == 0)
									{
										json[L"enemyCard"] = enemyCard;
										try
										{
											auto t = transactionBroker->submitTransaction(auth->userId(), "AttackCard", json);
											t.get();
											success = true;
										}
										catch (std::exception& ex)
										{
											std::cout << ex.what();
											success = false;
										}
									}
									else
									{
										std::cout << "No enemy card to attack... (press a key to continue)" << std::endl;
										std::cin >> key;
										success = false;
									}
								}
							}
							else
							{
								std::cout << "No card to attack... (press a key to continue)" << std::endl;
								std::cin >> key;

								success = false;
							}
						}
					}
				}
				// play a card
				else
				{
					if (gameManager->CheckEndOfGame())
					{
						running = false;
						break;
						system("cls");
					}

					bool playTheCard = gameManager->IsBoardFree(thePlayer);
					bool hasMana = true;
					if (thePlayer)
					{
						Card* card = gameManager->GetPlayer1()->ChooseCard(n);
						if (card != nullptr)
						{
							hasMana = gameManager->GetPlayer1()->GetCurrentMana() >= card->GetManaNeeded() ? true : false;
						}
						else
						{
							hasMana = false;
						}
					}
					else
					{
						Card* card = gameManager->GetPlayer2()->ChooseCard(n);
						if (card != nullptr)
						{
							hasMana = gameManager->GetPlayer2()->GetCurrentMana() >= card->GetManaNeeded() ? true : false;
						}
						else
						{
							hasMana = false;
						}
					}

					if (!hasMana)
					{
						std::cout << "not enought mana to play this card... (press a key to continue)" << std::endl;
						std::cin >> key;
					}
						

					if (playTheCard && hasMana)
					{
						try
						{
							auto t = transactionBroker->submitTransaction(auth->userId(), "playCard", json);
							t.get();
							success = true;
						}
						catch (std::exception& ex)
						{
							std::cout << ex.what();
						}
					}
				}
			}

			if (gameManager->CheckEndOfGame())
			{
				running = false;
				break;
			}

		}
	}

	std::cout << "Press a key to Quit";

	int l;
	std::cin >> l;

	std::cout << "disconnecting...";
	client->disconnect().get();
	std::cout << "DONE" << std::endl;
	return 0;
}