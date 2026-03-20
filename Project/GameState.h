#pragma once
#include <string>

enum GameStatus : uint8_t
{
	PLAYING = 1,
	WON = 2,
	LOST = 3,
	WITHDRAWN = 4
};

struct GameState
{


	int _questionNumber;
	int _currentPrizeAmount;
	bool _fiftyFiftyUsed;
	bool _askTheAudienceUsed;
	bool _phoneAFriendUsed;
	std::string _currentQuestionText;
	GameStatus _gameStatus;

	const std::string EnumValueToString(GameStatus status) const
	{
		switch(status)
		{
			case PLAYING:
			{
				return "Playing";
			}
			case WON: 
			{
				return "Won";
			}
			case LOST: 
			{
				return "Lost";
			}
			case WITHDRAWN: 
			{
				return "Walked Away";
			}
			default:
			{
				return " ";
			}
		}
	}

	void ResetGameState()
	{
		_questionNumber = 1;
		_currentPrizeAmount = 0;
		_fiftyFiftyUsed = false;
		_askTheAudienceUsed = false;
		_phoneAFriendUsed = false;
	}

};

