#pragma once
#include <string>
#include <map>
#include <array>
#include "raylib.h"
#include "QuestionBank.h"
#include "PlayerManager.h"
#include "Leaderboard.h"
#include "GameSession.h"
#include "Question.h"
#include "LifelineResult.h"
enum AppState
{
	MENU,
	MODE_SELECT,
	NAME_ENTRY,
	GAMEPLAY,
	LIFELINE_MENU,
	RESULTS,
	LEADERBOARD_VIEW,
	STATS_VIEW
};
namespace KBC
{
	const Color DARK_BG = { 5, 5, 30, 255 };
	const Color PANEL_BG = { 15, 25, 80, 255 };
	const Color PANEL_BORDER = { 40, 60, 160, 255 };
	const Color TEXT_PRIMARY = { 255, 255, 255, 255 };
	const Color TEXT_SECONDARY = { 170, 190, 230, 255 };
	const Color HIGHLIGHT = { 60, 100, 220, 255 };
	const Color SELECTED = { 255, 165, 0, 255 };
	const Color CORRECT = { 0, 200, 50, 255 };
	const Color WRONG = { 200, 20, 20, 255 };
	const Color SAFE_HAVEN = { 255, 200, 50, 255 };
	const Color GOLD_TEXT = { 220, 180, 50, 255 };
	const Color DISABLED = { 60, 60, 80, 180 };
	const Color BUTTON_NORMAL = { 20, 35, 100, 255 };
	const Color BUTTON_HOVER = { 40, 60, 160, 255 };
	const Color TIMER_GREEN = { 50, 200, 50, 255 };
	const Color TIMER_YELLOW = { 220, 200, 0, 255 };
	const Color TIMER_RED = { 220, 30, 30, 255 };
}
class App
{
private:
	std::string m_questionBankFilePath;
	std::string m_highScoresFilePath;
	QuestionBank m_questionBank;
	PlayerManager m_playerManager;
	Leaderboard m_leaderboard;
	AppState m_currentState;
	GameSession* m_currentSession;
	GameMode m_selectedMode;
	std::string m_inputName;
	int m_nameCharCount;
	bool m_shouldQuit;
	Font m_font;
	int m_screenWidth;
	int m_screenHeight;
	int m_hoveredAnswer;
	int m_selectedAnswer;
	int m_correctAnswerIndex;
	std::string m_revealQuestionText;
	std::array<std::string, 4> m_revealAnswers;
	float m_revealTimer;
	bool m_showingReveal;
	bool m_lastAnswerCorrect;
	FiftyFiftyResult m_fiftyFiftyResult;
	bool m_fiftyFiftyActive;
	AskTheAudienceResult m_audienceResult;
	bool m_audienceActive;
	PhoneAFriendResult m_phoneResult;
	bool m_phoneActive;
	float m_speedTimer;
	static constexpr float SPEED_TIME_LIMIT = 30.0f;
	std::string m_statsPlayerName;
	const PlayerStats* m_statsData;
	int m_statsNameCharCount;
	bool m_statsEnteringName;
public:
	App(const std::string& InQuestionBankFP, const std::string& InHighScoresFP);
	~App();
	void Run();
private:
	void UpdateMenu();
	void DrawMenu();
	void UpdateModeSelect();
	void DrawModeSelect();
	void UpdateNameEntry();
	void DrawNameEntry();
	void UpdateGameplay();
	void DrawGameplay();
	void UpdateLifelineMenu();
	void DrawLifelineMenu();
	void UpdateResults();
	void DrawResults();
	void UpdateLeaderboard();
	void DrawLeaderboard();
	void UpdateStats();
	void DrawStats();
	bool DrawButton(const char* text, float x, float y, float w, float h, Color normal, Color hover);
	void DrawPrizeLadder(float x, float y, float w, float h);
	void DrawLifelineIcons(float x, float y);
	void DrawTimerBar(float x, float y, float w, float h);
	void ResetGameplayState();
	void DT(const char* text, int x, int y, int size, Color color);
	int MT(const char* text, int size);
};
