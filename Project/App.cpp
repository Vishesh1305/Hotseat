#include "App.h"
#include <iostream>
#include <stdexcept>
#include <limits>
#include <thread>
#include <chrono>
#include "Leaderboard.h"

App::App(std::string InQuestionBankFP, std::string InHighScoresFP) : 
	m_questionBankFilePath(InQuestionBankFP),
	m_highScoresFilePath(InHighScoresFP),
	m_questionBank(m_questionBankFilePath)
{
	try
	{
		m_leaderboard.LoadScores(m_highScoresFilePath);
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}

void App::Run()
{
	try
	{
		bool isRunning = true;
		while (isRunning)
		{
			std::cout << "<--- Welcome to 'Who Will Become a Millionaire!/ Kaun Banega Crorepati!' --->\n";

			ShowMainMenu();

			std::cout << "Your Choice: ";
			size_t choice;
			std::cin >> choice;
			if (std::cin.fail())
			{
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				std::cerr << "Invalid input! Please enter a number.\n";
				continue;
			}
			switch (choice)
			{
			case 1:
			{
				StartNewGame();
				break;
			}
			case 2:
			{
				ShowLeaderboard();
				break;
			}
			case 3:
			{
				ShowStats();
				break;
			}
			case 4:
			{
				std::cout << "See you again!\n";
				isRunning = false;
				break;
			}
			default:
				std::cerr << "Invalid Choice! Try again!\n";
				break;
			}
		}
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}

void App::ShowMainMenu()
{
	std::cout << "1: New Game.\n2: Leaderboard\n3: Stats\n 4: Quit\n\n";

}

void App::StartNewGame()
{
	try 
	{
		std::string playerName;
		std::cout << "Enter your Name: ";
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::getline(std::cin, playerName);
		try 
		{ 
			m_playerManager.RegisterPlayer(playerName); 
		}
		catch (std::exception&) { /* returning player, continue */ }


		std::cout << "Select the Game Mode: Type the number to select.\n1: Classic\n2: Speed\n3: Practice\n\nYour Choice -> ";
		int choice;
		std::cin >> choice;

		switch (choice)
		{
			case 1: 
			{
				GameSession session(&m_questionBank, &m_leaderboard, &m_playerManager, playerName, CLASSIC);
				PlayGame(session);
				break;
			}
			case 2:
			{
				GameSession session(&m_questionBank, &m_leaderboard, &m_playerManager, playerName, SPEED);
				PlayGame(session);
				break;
			}
			case 3:
			{
				GameSession session(&m_questionBank, &m_leaderboard, &m_playerManager, playerName, PRACTICE);
				PlayGame(session);
				break;
			}
			default:
			{
				std::cerr << "Invaild Choice! Try again!" << std::endl;
				break;
			}
		}
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}

void App::ShowLeaderboard()
{
	m_leaderboard.DisplayTheLeaderboard();
}

void App::ShowStats()
{
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::string playerName;
	std::getline(std::cin, playerName);

	try
	{
		const PlayerStats* stats = m_playerManager.GetPlayerStats(playerName);
		std::cout << "Player: " << stats->_playerName << "\n";
		std::cout << "Games Played: " << stats->_totalGamesPlayed << "\n";
		std::cout << "Total Winnings: $" << stats->_totalWinnings << "\n";
		std::cout << "Highest Prize: $" << stats->_highestPrizeWon << "\n";
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	
}

void App::PlayGame(GameSession& session)
{
	session.StartGame();
	std::system("cls");

	while (true)
	{
		char choice;
		std::array<char, 6> AvailableOptions {'A', 'B', 'C', 'D', 'L', 'W'};
		std::system("cls");
		std::cout << session.GetCurrentQuestion()->QuestionText << std::endl;
		for (int i = 0; i < 4; i++)
		{
			std::cout << AvailableOptions[i] << ". " << session.GetCurrentQuestion()->Answers[i] << "  ";
		}
		std::cout << std::endl;
		std::cout << "Enter 'L' for Lifelines Menu and 'W' to Walk Away\n";
		std::cout << "Your Choice-> ";
		std::cin >> choice;
		if (choice == 'l' || choice == 'L')
		{
			if (!session.GetGameState()._fiftyFiftyUsed)
			{
				std::cout << "1. 50/50\n";
			}
			if (!session.GetGameState()._askTheAudienceUsed)
			{
				std::cout << "2. Ask The Audience\n";
			}
			if (!session.GetGameState()._phoneAFriendUsed)
			{
				std::cout << "3. Phone a Friend\n";
			}
			std::cout << "4. Back\n";
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			int lifelineChoice;
			std::cin >> lifelineChoice;

			switch (lifelineChoice)
			{
				case 1:
				{
					try
					{
						FiftyFiftyResult result = session.ActivateFiftyFifty();
						std::cout << "Eliminated: " << Question::IndexToLetter(result._index1) << " and " << Question::IndexToLetter(result._index2) << "\n";
					}
					catch (std::exception& e)
					{
						std::cerr << e.what() << "\n";
					}
					break;
				}
				case 2:
				{
					try
					{
						AskTheAudienceResult result = session.ActivateAskTheAudience();
						std::cout << "A: " << result._optionAPercent << "% "
							<< "B: " << result._optionBPercent << "% "
							<< "C: " << result._optionCPercent << "% "
							<< "D: " << result._optionDPercent << "%\n";
					}
					catch (std::exception& e)
					{
						std::cerr << e.what() << "\n";
					}
					break;
				}
				case 3:
				{
					try
					{
						PhoneAFriendResult result = session.ActivatePhoneAFriend();
						std::cout << "Your friend suggests: " << result._friendAnswer
							<< " (Confidence: " << result._friendConfidence << "%)\n";
					}
					catch (std::exception& e)
					{
						std::cerr << e.what() << "\n";
					}
					break;
				}
			default:
				break;
			}
			continue;
			
		}
		else if (choice == 'w' || choice == 'W')
		{
			session.WalkAway();
			break;
		}
		else
		{
			bool correct = session.SubmitAnswer(choice);
			if (correct)
			{
				std::cout << "Correct! Prize: $" << session.GetGameState()._currentPrizeAmount << "\n";
				std::this_thread::sleep_for(std::chrono::seconds(2));
			}
			
			else
			{
				std::cout << "Wrong! You leave with: $" << session.GetGameState()._currentPrizeAmount << "\n";
				std::this_thread::sleep_for(std::chrono::seconds(2));
			}
			if (session.IsGameOver()) break;
		}
	}
	std::cout << "\n=== Game Over ===\n";
	std::cout << "Status: " << session.GetGameState().EnumValueToString(session.GetGameState()._gameStatus) << "\n";
	std::cout << "Final Prize: $" << session.GetGameState()._currentPrizeAmount << "\n";

	ScoreEntry entry;
	entry._playerName = session.GetPlayerName();
	entry._finalPrize = session.GetGameState()._currentPrizeAmount;
	entry._questionsAnswered = session.GetGameState()._questionNumber;
	entry._date = __DATE__;

	m_leaderboard.AddScoreEntry(entry);
	m_leaderboard.SaveScoresToFile(m_highScoresFilePath);
	m_playerManager.UpdatePlayerStats(session.GetPlayerName(), &session.GetGameState(), session.GetCategoryResults());
}
