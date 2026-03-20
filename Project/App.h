#pragma once
#include "QuestionBank.h"
#include "PlayerManager.h"
#include "Leaderboard.h"
#include "GameSession.h"

class App
{
private:
	std::string m_questionBankFilePath;
	std::string m_highScoresFilePath;

	QuestionBank m_questionBank;
	PlayerManager m_playerManager;
	Leaderboard m_leaderboard;

public:
	App(std::string InQuestionBankFP, std::string InHighScoresFP);
	void Run();

private:
	App() = default;
	void ShowMainMenu();
	void StartNewGame();
	void ShowLeaderboard();
	void ShowStats();
	void PlayGame(GameSession& session);
};

