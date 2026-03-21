#pragma once
#include <string>
#include <map>
#include "LifelineResult.h"
#include "PrizeLadder.h"
#include "GameState.h"

enum GameMode : uint8_t
{
	CLASSIC = 1,
	SPEED = 2,
	PRACTICE = 3
};

class QuestionBank;
class Leaderboard;
class PlayerManager;
struct Question;

class GameSession
{
private:
	GameState m_gameState;
	QuestionBank* m_questionBank;
	PrizeLadder m_prizeLadder;
	Leaderboard* m_leaderboard;
	PlayerManager* m_playerManager;
	const Question* m_currentQuestion;
	GameMode m_gameMode;

	std::string m_playerName;
	std::map<std::string, std::pair<int, int>> m_categoryResults;

public:
	GameSession(QuestionBank* InQuestionBank, Leaderboard* InLeaderboard, PlayerManager* InPlayerManager, std::string InPlayerName, GameMode InGameMode);

	void StartGame();
	bool SubmitAnswer(char InAnswer);

	FiftyFiftyResult ActivateFiftyFifty();
	AskTheAudienceResult ActivateAskTheAudience();
	PhoneAFriendResult ActivatePhoneAFriend();

	void WalkAway();
	const std::string& GetPlayerName() const { return m_playerName; }
	const GameState& GetGameState() const { return m_gameState; }
	const Question* GetCurrentQuestion() const { return m_currentQuestion; }
	const std::map<std::string, std::pair<int, int>>& GetCategoryResults() const { return m_categoryResults; }
	bool IsGameOver() const;
};
