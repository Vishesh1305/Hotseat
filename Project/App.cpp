#include "App.h"
#include <cstdlib>
#include <ctime>
#include <string>
#include <algorithm>
#include <cstring>
#include <cmath>

void App::DT(const char* text, int x, int y, int size, Color color)
{
	DrawTextEx(m_font, text, { (float)x, (float)y }, (float)size, 1, color);
}

int App::MT(const char* text, int size)
{
	Vector2 v = MeasureTextEx(m_font, text, (float)size, 1);
	return (int)v.x;
}

App::App(const std::string& InQuestionBankFP, const std::string& InHighScoresFP)
	: m_questionBankFilePath(InQuestionBankFP),
	m_highScoresFilePath(InHighScoresFP),
	m_questionBank(m_questionBankFilePath),
	m_currentState(MENU),
	m_currentSession(nullptr),
	m_selectedMode(CLASSIC),
	m_nameCharCount(0),
	m_shouldQuit(false),
	m_screenWidth(1280),
	m_screenHeight(720),
	m_hoveredAnswer(-1),
	m_selectedAnswer(-1),
	m_correctAnswerIndex(-1),
	m_revealTimer(0.0f),
	m_showingReveal(false),
	m_lastAnswerCorrect(false),
	m_fiftyFiftyActive(false),
	m_audienceActive(false),
	m_phoneActive(false),
	m_speedTimer(SPEED_TIME_LIMIT),
	m_statsData(nullptr),
	m_statsNameCharCount(0),
	m_statsEnteringName(true)
{
	m_font = { 0 };
	m_inputName = "";
	m_statsPlayerName = "";
	m_fiftyFiftyResult = { 0, 0 };
	m_audienceResult = { 0, 0, 0, 0 };
	m_phoneResult = { ' ', 0 };
	try
	{
		m_leaderboard.LoadScores(m_highScoresFilePath);
	}
	catch (std::exception&) {}
}

App::~App()
{
	if (m_currentSession)
	{
		delete m_currentSession;
		m_currentSession = nullptr;
	}
}

void App::Run()
{
	InitWindow(m_screenWidth, m_screenHeight, "HotSeat");
	SetTargetFPS(60);
	m_font = LoadFontEx("Assets/Poppins-Regular.ttf", 64, nullptr, 0);
	while (!WindowShouldClose() && !m_shouldQuit)
	{
		switch (m_currentState)
		{
		case MENU:            UpdateMenu(); break;
		case MODE_SELECT:     UpdateModeSelect(); break;
		case NAME_ENTRY:      UpdateNameEntry(); break;
		case GAMEPLAY:        UpdateGameplay(); break;
		case LIFELINE_MENU:   UpdateLifelineMenu(); break;
		case RESULTS:         UpdateResults(); break;
		case LEADERBOARD_VIEW:UpdateLeaderboard(); break;
		case STATS_VIEW:      UpdateStats(); break;
		}
		BeginDrawing();
		ClearBackground(KBC::DARK_BG);
		switch (m_currentState)
		{
		case MENU:            DrawMenu(); break;
		case MODE_SELECT:     DrawModeSelect(); break;
		case NAME_ENTRY:      DrawNameEntry(); break;
		case GAMEPLAY:        DrawGameplay(); break;
		case LIFELINE_MENU:   DrawLifelineMenu(); break;
		case RESULTS:         DrawResults(); break;
		case LEADERBOARD_VIEW:DrawLeaderboard(); break;
		case STATS_VIEW:      DrawStats(); break;
		}
		EndDrawing();
	}
	UnloadFont(m_font);
	CloseWindow();
}

void App::UpdateMenu() {}

void App::DrawMenu()
{
	const char* title = "HOTSEAT";
	int titleSize = 44;
	DT(title, (m_screenWidth - MT(title, titleSize)) / 2, 80, titleSize, KBC::GOLD_TEXT);
	const char* subtitle = "Kaun Banega Crorepati";
	int subSize = 26;
	DT(subtitle, (m_screenWidth - MT(subtitle, subSize)) / 2, 130, subSize, KBC::TEXT_SECONDARY);
	DrawRectangle(m_screenWidth / 2 - 150, 170, 300, 2, KBC::PANEL_BORDER);
	float btnW = 300, btnH = 55;
	float btnX = (m_screenWidth - btnW) / 2;
	float startY = 220;
	float gap = 70;
	if (DrawButton("NEW GAME", btnX, startY, btnW, btnH, KBC::BUTTON_NORMAL, KBC::BUTTON_HOVER))
		m_currentState = MODE_SELECT;
	if (DrawButton("LEADERBOARD", btnX, startY + gap, btnW, btnH, KBC::BUTTON_NORMAL, KBC::BUTTON_HOVER))
		m_currentState = LEADERBOARD_VIEW;
	if (DrawButton("STATISTICS", btnX, startY + gap * 2, btnW, btnH, KBC::BUTTON_NORMAL, KBC::BUTTON_HOVER))
	{
		m_statsEnteringName = true;
		m_statsPlayerName = "";
		m_statsNameCharCount = 0;
		m_statsData = nullptr;
		m_currentState = STATS_VIEW;
	}
	if (DrawButton("QUIT", btnX, startY + gap * 3, btnW, btnH, { 100, 20, 20, 255 }, { 160, 30, 30, 255 }))
		m_shouldQuit = true;
	const char* footer = "Built with C++ & Raylib";
	DT(footer, (m_screenWidth - MT(footer, 16)) / 2, m_screenHeight - 40, 16, KBC::DISABLED);
}

void App::UpdateModeSelect()
{
	if (IsKeyPressed(KEY_ESCAPE))
		m_currentState = MENU;
}

void App::DrawModeSelect()
{
	const char* title = "SELECT GAME MODE";
	DT(title, (m_screenWidth - MT(title, 36)) / 2, 80, 36, KBC::GOLD_TEXT);
	float btnW = 350, btnH = 80;
	float btnX = (m_screenWidth - btnW) / 2;
	float startY = 200;
	float gap = 110;
	if (DrawButton("CLASSIC MODE", btnX, startY, btnW, btnH, KBC::BUTTON_NORMAL, KBC::BUTTON_HOVER))
	{
		m_selectedMode = CLASSIC;
		m_inputName = "";
		m_nameCharCount = 0;
		m_currentState = NAME_ENTRY;
	}
	DT("15 questions, standard prize ladder", (int)(btnX + 20), (int)(startY + btnH + 5), 16, KBC::TEXT_SECONDARY);
	if (DrawButton("SPEED MODE", btnX, startY + gap, btnW, btnH, KBC::BUTTON_NORMAL, KBC::BUTTON_HOVER))
	{
		m_selectedMode = SPEED;
		m_inputName = "";
		m_nameCharCount = 0;
		m_currentState = NAME_ENTRY;
	}
	DT("30 seconds per question", (int)(btnX + 20), (int)(startY + gap + btnH + 5), 16, KBC::TEXT_SECONDARY);
	if (DrawButton("PRACTICE MODE", btnX, startY + gap * 2, btnW, btnH, KBC::BUTTON_NORMAL, KBC::BUTTON_HOVER))
	{
		m_selectedMode = PRACTICE;
		m_inputName = "";
		m_nameCharCount = 0;
		m_currentState = NAME_ENTRY;
	}
	DT("Unlimited questions, no prize tracking", (int)(btnX + 20), (int)(startY + gap * 2 + btnH + 5), 16, KBC::TEXT_SECONDARY);
	if (DrawButton("BACK", 20, m_screenHeight - 60, 100, 40, { 80, 20, 20, 255 }, { 120, 30, 30, 255 }))
		m_currentState = MENU;
}

void App::UpdateNameEntry()
{
	if (IsKeyPressed(KEY_ESCAPE))
		m_currentState = MODE_SELECT;
	int key = GetCharPressed();
	while (key > 0)
	{
		if ((key >= 32) && (key <= 125) && (m_nameCharCount < 20))
		{
			m_inputName += (char)key;
			m_nameCharCount++;
		}
		key = GetCharPressed();
	}
	if (IsKeyPressed(KEY_BACKSPACE) && m_nameCharCount > 0)
	{
		m_inputName.pop_back();
		m_nameCharCount--;
	}
	if (IsKeyPressed(KEY_ENTER) && m_nameCharCount > 0)
	{
		try { m_playerManager.RegisterPlayer(m_inputName); }
		catch (std::exception&) {}
		if (m_currentSession)
		{
			delete m_currentSession;
			m_currentSession = nullptr;
		}
		m_currentSession = new GameSession(&m_questionBank, &m_leaderboard, &m_playerManager, m_inputName, m_selectedMode);
		ResetGameplayState();
		m_currentSession->StartGame();
		m_speedTimer = SPEED_TIME_LIMIT;
		m_currentState = GAMEPLAY;
	}
}

void App::DrawNameEntry()
{
	const char* title = "ENTER YOUR NAME";
	DT(title, (m_screenWidth - MT(title, 36)) / 2, 150, 36, KBC::GOLD_TEXT);
	float boxW = 400, boxH = 50;
	float boxX = (m_screenWidth - boxW) / 2;
	float boxY = 280;
	DrawRectangleRounded({ boxX, boxY, boxW, boxH }, 0.2f, 8, KBC::PANEL_BG);
	DrawRectangleRoundedLines({ boxX, boxY, boxW, boxH }, 0.2f, 8, KBC::PANEL_BORDER);
	if (m_nameCharCount > 0)
		DT(m_inputName.c_str(), (int)(boxX + 15), (int)(boxY + 13), 24, KBC::TEXT_PRIMARY);
	else
		DT("Type your name...", (int)(boxX + 15), (int)(boxY + 13), 24, KBC::DISABLED);
	if (((int)(GetTime() * 2) % 2) == 0)
	{
		int textW = MT(m_inputName.c_str(), 24);
		DrawRectangle((int)(boxX + 15 + textW + 2), (int)(boxY + 12), 2, 26, KBC::TEXT_PRIMARY);
	}
	Color btnColor = (m_nameCharCount > 0) ? KBC::BUTTON_NORMAL : KBC::DISABLED;
	Color btnHover = (m_nameCharCount > 0) ? KBC::BUTTON_HOVER : KBC::DISABLED;
	if (DrawButton("START GAME", (m_screenWidth - 200) / 2.0f, 380, 200, 50, btnColor, btnHover) && m_nameCharCount > 0)
	{
		try { m_playerManager.RegisterPlayer(m_inputName); }
		catch (std::exception&) {}
		if (m_currentSession) { delete m_currentSession; m_currentSession = nullptr; }
		m_currentSession = new GameSession(&m_questionBank, &m_leaderboard, &m_playerManager, m_inputName, m_selectedMode);
		ResetGameplayState();
		m_currentSession->StartGame();
		m_speedTimer = SPEED_TIME_LIMIT;
		m_currentState = GAMEPLAY;
	}
	if (DrawButton("BACK", 20, m_screenHeight - 60, 100, 40, { 80, 20, 20, 255 }, { 120, 30, 30, 255 }))
		m_currentState = MODE_SELECT;
}

void App::ResetGameplayState()
{
	m_hoveredAnswer = -1;
	m_selectedAnswer = -1;
	m_correctAnswerIndex = -1;
	m_revealTimer = 0.0f;
	m_showingReveal = false;
	m_lastAnswerCorrect = false;
	m_fiftyFiftyActive = false;
	m_audienceActive = false;
	m_phoneActive = false;
}

void App::UpdateGameplay()
{
	if (!m_currentSession) return;
	if (m_selectedMode == SPEED && !m_showingReveal)
	{
		m_speedTimer -= GetFrameTime();
		if (m_speedTimer <= 0.0f)
		{
			m_speedTimer = 0.0f;
			m_showingReveal = true;
			m_lastAnswerCorrect = false;
			m_selectedAnswer = -1;
			m_correctAnswerIndex = Question::LetterToIndex(m_currentSession->GetCurrentQuestion()->CorrectAnswer);
			m_revealQuestionText = m_currentSession->GetCurrentQuestion()->QuestionText;
			m_revealAnswers = m_currentSession->GetCurrentQuestion()->Answers;
			m_revealTimer = 2.0f;
			m_currentSession->WalkAway();
		}
	}
	if (m_showingReveal)
	{
		m_revealTimer -= GetFrameTime();
		if (m_revealTimer <= 0.0f)
		{
			m_showingReveal = false;
			if (m_currentSession->IsGameOver())
			{
				ScoreEntry entry;
				entry._playerName = m_currentSession->GetPlayerName();
				entry._finalPrize = m_currentSession->GetGameState()._currentPrizeAmount;
				entry._questionsAnswered = m_currentSession->GetGameState()._questionNumber;
				entry._date = __DATE__;
				m_leaderboard.AddScoreEntry(entry);
				try { m_leaderboard.SaveScoresToFile(m_highScoresFilePath); }
				catch (...) {}
				try { m_playerManager.UpdatePlayerStats(m_currentSession->GetPlayerName(), &m_currentSession->GetGameState(), m_currentSession->GetCategoryResults()); }
				catch (...) {}
				m_currentState = RESULTS;
				return;
			}
			m_selectedAnswer = -1;
			m_hoveredAnswer = -1;
			m_correctAnswerIndex = -1;
			m_fiftyFiftyActive = false;
			m_audienceActive = false;
			m_phoneActive = false;
			m_speedTimer = SPEED_TIME_LIMIT;
		}
		return;
	}
	if (IsKeyPressed(KEY_W))
	{
		m_currentSession->WalkAway();
		ScoreEntry entry;
		entry._playerName = m_currentSession->GetPlayerName();
		entry._finalPrize = m_currentSession->GetGameState()._currentPrizeAmount;
		entry._questionsAnswered = m_currentSession->GetGameState()._questionNumber;
		entry._date = __DATE__;
		m_leaderboard.AddScoreEntry(entry);
		try { m_leaderboard.SaveScoresToFile(m_highScoresFilePath); }
		catch (...) {}
		try { m_playerManager.UpdatePlayerStats(m_currentSession->GetPlayerName(), &m_currentSession->GetGameState(), m_currentSession->GetCategoryResults()); }
		catch (...) {}
		m_currentState = RESULTS;
		return;
	}
	if (IsKeyPressed(KEY_L))
	{
		m_currentState = LIFELINE_MENU;
		return;
	}
}

void App::DrawGameplay()
{
	if (!m_currentSession) return;
	const Question* q = m_currentSession->GetCurrentQuestion();
	const GameState& gs = m_currentSession->GetGameState();
	const std::string& displayQuestionText = m_showingReveal ? m_revealQuestionText : q->QuestionText;
	const std::array<std::string, 4>& displayAnswers = m_showingReveal ? m_revealAnswers : q->Answers;
	DrawPrizeLadder(m_screenWidth - 260.0f, 20.0f, 240.0f, m_screenHeight - 40.0f);
	float mainW = m_screenWidth - 290.0f;
	DT(TextFormat("Player: %s", m_currentSession->GetPlayerName().c_str()), 20, 15, 18, KBC::TEXT_SECONDARY);
	DT(TextFormat("Question %d / 15", gs._questionNumber), 20, 40, 20, KBC::GOLD_TEXT);
	DT(TextFormat("Prize: $%d", gs._currentPrizeAmount), 20, 65, 18, KBC::GOLD_TEXT);
	DrawLifelineIcons(mainW / 2 - 100, 20);
	if (m_selectedMode == SPEED)
		DrawTimerBar(20, 95, mainW - 40, 16);
	float qY = (m_selectedMode == SPEED) ? 130.0f : 110.0f;
	float qW = mainW - 60;
	float qH = 80;
	float qX = 20;
	DrawRectangleRounded({ qX, qY, qW, qH }, 0.15f, 8, KBC::PANEL_BG);
	DrawRectangleRoundedLines({ qX, qY, qW, qH }, 0.15f, 8, KBC::PANEL_BORDER);
	const char* qText = displayQuestionText.c_str();
	int fontSize = 24;
	int textW = MT(qText, fontSize);
	if (textW > (int)qW - 40)
		fontSize = 20;
	DT(qText, (int)(qX + 20), (int)(qY + (qH - fontSize) / 2), fontSize, KBC::TEXT_PRIMARY);
	float ansY = qY + qH + 30;
	float ansW = (qW - 20) / 2;
	float ansH = 65;
	float ansGap = 15;
	const char* letters[] = { "A", "B", "C", "D" };
	Vector2 mouse = GetMousePosition();
	m_hoveredAnswer = -1;
	for (int i = 0; i < 4; i++)
	{
		float ax = qX + (i % 2) * (ansW + 20);
		float ay = ansY + (i / 2) * (ansH + ansGap);
		bool eliminated = false;
		if (m_fiftyFiftyActive)
		{
			if (i == m_fiftyFiftyResult._index1 || i == m_fiftyFiftyResult._index2)
				eliminated = true;
		}
		Color panelColor = KBC::PANEL_BG;
		Color borderColor = KBC::PANEL_BORDER;
		if (eliminated)
		{
			panelColor = { 20, 20, 40, 100 };
			borderColor = { 40, 40, 60, 100 };
		}
		else if (m_showingReveal)
		{
			int correctIdx = m_correctAnswerIndex;
			if (i == correctIdx)
			{
				float pulse = (sinf((float)GetTime() * 12.0f) + 1.0f) / 2.0f;
				panelColor = ColorAlpha(KBC::CORRECT, 0.5f + pulse * 0.5f);
				borderColor = KBC::CORRECT;
			}
			else if (i == m_selectedAnswer && !m_lastAnswerCorrect)
			{
				panelColor = KBC::WRONG;
				borderColor = KBC::WRONG;
			}
		}
		else if (i == m_selectedAnswer)
		{
			panelColor = KBC::SELECTED;
			borderColor = KBC::SELECTED;
		}
		else
		{
			Rectangle rect = { ax, ay, ansW, ansH };
			if (CheckCollisionPointRec(mouse, rect) && !eliminated)
			{
				m_hoveredAnswer = i;
				panelColor = KBC::HIGHLIGHT;
				borderColor = { 80, 120, 255, 255 };
			}
		}
		DrawRectangleRounded({ ax, ay, ansW, ansH }, 0.2f, 8, panelColor);
		DrawRectangleRoundedLines({ ax, ay, ansW, ansH }, 0.2f, 8, borderColor);
		DrawRectangleRounded({ ax + 8, ay + 8, 35, ansH - 16 }, 0.3f, 6, { 0, 0, 0, 80 });
		DT(letters[i], (int)(ax + 18), (int)(ay + (ansH - 26) / 2), 26, KBC::GOLD_TEXT);
		Color textColor = eliminated ? KBC::DISABLED : KBC::TEXT_PRIMARY;
		DT(displayAnswers[i].c_str(), (int)(ax + 55), (int)(ay + (ansH - 22) / 2), 22, textColor);
		if (!m_showingReveal && !eliminated && CheckCollisionPointRec(mouse, { ax, ay, ansW, ansH }))
		{
			if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
			{
				m_selectedAnswer = i;
				m_correctAnswerIndex = Question::LetterToIndex(q->CorrectAnswer);
				m_revealQuestionText = q->QuestionText;
				m_revealAnswers = q->Answers;
				char answerChar = Question::IndexToLetter(i);
				try
				{
					m_lastAnswerCorrect = m_currentSession->SubmitAnswer(answerChar);
				}
				catch (std::exception&)
				{
					m_lastAnswerCorrect = false;
				}
				m_showingReveal = true;
				m_revealTimer = 2.5f;
			}
		}
	}
	if (m_audienceActive && !m_showingReveal)
	{
		float overlayX = qX + 100;
		float overlayY = ansY - 10;
		DrawRectangleRounded({ overlayX, overlayY, 300, 180 }, 0.1f, 8, { 10, 10, 50, 240 });
		DrawRectangleRoundedLines({ overlayX, overlayY, 300, 180 }, 0.1f, 8, KBC::GOLD_TEXT);
		DT("AUDIENCE POLL", (int)(overlayX + 80), (int)(overlayY + 10), 20, KBC::GOLD_TEXT);
		int percents[] = { m_audienceResult._optionAPercent, m_audienceResult._optionBPercent, m_audienceResult._optionCPercent, m_audienceResult._optionDPercent };
		for (int i = 0; i < 4; i++)
		{
			float barX = overlayX + 30 + i * 70;
			float barMaxH = 100;
			float barH = barMaxH * (percents[i] / 100.0f);
			float barY = overlayY + 140 - barH;
			DrawRectangle((int)barX, (int)barY, 40, (int)barH, KBC::HIGHLIGHT);
			DrawRectangleLines((int)barX, (int)barY, 40, (int)barH, KBC::PANEL_BORDER);
			DT(letters[i], (int)(barX + 14), (int)(overlayY + 148), 18, KBC::TEXT_PRIMARY);
			DT(TextFormat("%d%%", percents[i]), (int)(barX + 6), (int)(barY - 20), 16, KBC::GOLD_TEXT);
		}
	}
	if (m_phoneActive && !m_showingReveal)
	{
		float overlayX = qX + 100;
		float overlayY = ansY - 10;
		DrawRectangleRounded({ overlayX, overlayY, 350, 100 }, 0.1f, 8, { 10, 10, 50, 240 });
		DrawRectangleRoundedLines({ overlayX, overlayY, 350, 100 }, 0.1f, 8, KBC::GOLD_TEXT);
		DT("PHONE A FRIEND", (int)(overlayX + 90), (int)(overlayY + 10), 20, KBC::GOLD_TEXT);
		DT(TextFormat("\"I think it's %c... I'm %d%% sure.\"", m_phoneResult._friendAnswer, m_phoneResult._friendConfidence),
			(int)(overlayX + 20), (int)(overlayY + 50), 20, KBC::TEXT_PRIMARY);
	}
	float hudY = m_screenHeight - 45.0f;
	DT("[L] Lifelines  |  [W] Walk Away", 20, (int)hudY, 18, KBC::TEXT_SECONDARY);
	const char* modeText = (m_selectedMode == CLASSIC) ? "Classic Mode" : (m_selectedMode == SPEED) ? "Speed Mode" : "Practice Mode";
	DT(modeText, (int)(mainW - MT(modeText, 18) - 20), (int)hudY, 18, KBC::DISABLED);
}

void App::UpdateLifelineMenu()
{
	if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_L))
	{
		m_currentState = GAMEPLAY;
		return;
	}
}

void App::DrawLifelineMenu()
{
	DrawGameplay();
	DrawRectangle(0, 0, m_screenWidth, m_screenHeight, { 0, 0, 0, 160 });
	const GameState& gs = m_currentSession->GetGameState();
	float panelW = 400, panelH = 300;
	float panelX = (m_screenWidth - panelW) / 2;
	float panelY = (m_screenHeight - panelH) / 2;
	DrawRectangleRounded({ panelX, panelY, panelW, panelH }, 0.1f, 8, KBC::PANEL_BG);
	DrawRectangleRoundedLines({ panelX, panelY, panelW, panelH }, 0.1f, 8, KBC::GOLD_TEXT);
	DT("LIFELINES", (int)(panelX + (panelW - MT("LIFELINES", 28)) / 2), (int)(panelY + 15), 28, KBC::GOLD_TEXT);
	float btnX = panelX + 50;
	float btnY = panelY + 60;
	float btnW = 300, btnH = 50, gap = 60;
	if (!gs._fiftyFiftyUsed)
	{
		if (DrawButton("50 : 50", btnX, btnY, btnW, btnH, KBC::BUTTON_NORMAL, KBC::BUTTON_HOVER))
		{
			try
			{
				m_fiftyFiftyResult = m_currentSession->ActivateFiftyFifty();
				m_fiftyFiftyActive = true;
			}
			catch (std::exception&) {}
			m_currentState = GAMEPLAY;
		}
	}
	else
	{
		DrawButton("50 : 50 (Used)", btnX, btnY, btnW, btnH, KBC::DISABLED, KBC::DISABLED);
	}
	if (!gs._askTheAudienceUsed)
	{
		if (DrawButton("ASK THE AUDIENCE", btnX, btnY + gap, btnW, btnH, KBC::BUTTON_NORMAL, KBC::BUTTON_HOVER))
		{
			try
			{
				m_audienceResult = m_currentSession->ActivateAskTheAudience();
				m_audienceActive = true;
			}
			catch (std::exception&) {}
			m_currentState = GAMEPLAY;
		}
	}
	else
	{
		DrawButton("ASK THE AUDIENCE (Used)", btnX, btnY + gap, btnW, btnH, KBC::DISABLED, KBC::DISABLED);
	}
	if (!gs._phoneAFriendUsed)
	{
		if (DrawButton("PHONE A FRIEND", btnX, btnY + gap * 2, btnW, btnH, KBC::BUTTON_NORMAL, KBC::BUTTON_HOVER))
		{
			try
			{
				m_phoneResult = m_currentSession->ActivatePhoneAFriend();
				m_phoneActive = true;
			}
			catch (std::exception&) {}
			m_currentState = GAMEPLAY;
		}
	}
	else
	{
		DrawButton("PHONE A FRIEND (Used)", btnX, btnY + gap * 2, btnW, btnH, KBC::DISABLED, KBC::DISABLED);
	}
	if (DrawButton("BACK", btnX, btnY + gap * 3, btnW, btnH, { 80, 20, 20, 255 }, { 120, 30, 30, 255 }))
		m_currentState = GAMEPLAY;
}

void App::UpdateResults() {}

void App::DrawResults()
{
	if (!m_currentSession) return;
	const GameState& gs = m_currentSession->GetGameState();
	const char* title = "GAME OVER";
	DT(title, (m_screenWidth - MT(title, 48)) / 2, 100, 48, KBC::GOLD_TEXT);
	std::string statusStr = gs.EnumValueToString(gs._gameStatus);
	Color statusColor = (gs._gameStatus == WON) ? KBC::CORRECT : (gs._gameStatus == LOST) ? KBC::WRONG : KBC::SELECTED;
	DT(statusStr.c_str(), (m_screenWidth - MT(statusStr.c_str(), 36)) / 2, 180, 36, statusColor);
	const char* prizeText = TextFormat("Final Prize: $%d", gs._currentPrizeAmount);
	DT(prizeText, (m_screenWidth - MT(prizeText, 32)) / 2, 240, 32, KBC::GOLD_TEXT);
	const char* qText = TextFormat("Questions Answered: %d", gs._questionNumber);
	DT(qText, (m_screenWidth - MT(qText, 24)) / 2, 300, 24, KBC::TEXT_SECONDARY);
	const char* pText = TextFormat("Player: %s", m_currentSession->GetPlayerName().c_str());
	DT(pText, (m_screenWidth - MT(pText, 24)) / 2, 340, 24, KBC::TEXT_SECONDARY);
	DrawRectangle(m_screenWidth / 2 - 150, 390, 300, 2, KBC::PANEL_BORDER);
	float btnW = 250, btnH = 55;
	float btnX = (m_screenWidth - btnW) / 2;
	if (DrawButton("PLAY AGAIN", btnX, 420, btnW, btnH, KBC::BUTTON_NORMAL, KBC::BUTTON_HOVER))
	{
		m_inputName = "";
		m_nameCharCount = 0;
		m_currentState = MODE_SELECT;
	}
	if (DrawButton("MAIN MENU", btnX, 490, btnW, btnH, KBC::BUTTON_NORMAL, KBC::BUTTON_HOVER))
	{
		m_currentState = MENU;
	}
}

void App::UpdateLeaderboard()
{
	if (IsKeyPressed(KEY_ESCAPE))
		m_currentState = MENU;
}

void App::DrawLeaderboard()
{
	const char* title = "LEADERBOARD - TOP 10";
	DT(title, (m_screenWidth - MT(title, 36)) / 2, 40, 36, KBC::GOLD_TEXT);
	std::vector<ScoreEntry> top = m_leaderboard.GetTheTop();
	float startX = 150;
	float y = 110;
	DT("RANK", (int)startX, (int)y, 20, KBC::TEXT_SECONDARY);
	DT("PLAYER", (int)(startX + 80), (int)y, 20, KBC::TEXT_SECONDARY);
	DT("PRIZE", (int)(startX + 400), (int)y, 20, KBC::TEXT_SECONDARY);
	DT("QUESTIONS", (int)(startX + 600), (int)y, 20, KBC::TEXT_SECONDARY);
	DT("DATE", (int)(startX + 780), (int)y, 20, KBC::TEXT_SECONDARY);
	DrawRectangle((int)startX, (int)(y + 28), 800, 2, KBC::PANEL_BORDER);
	y += 40;
	for (int i = 0; i < (int)top.size(); i++)
	{
		Color rowColor = (i == 0) ? KBC::GOLD_TEXT : (i < 3) ? KBC::TEXT_PRIMARY : KBC::TEXT_SECONDARY;
		DT(TextFormat("#%d", i + 1), (int)startX, (int)y, 20, rowColor);
		DT(top[i]._playerName.c_str(), (int)(startX + 80), (int)y, 20, rowColor);
		DT(TextFormat("$%d", top[i]._finalPrize), (int)(startX + 400), (int)y, 20, KBC::GOLD_TEXT);
		DT(TextFormat("%d", top[i]._questionsAnswered), (int)(startX + 630), (int)y, 20, rowColor);
		DT(top[i]._date.c_str(), (int)(startX + 780), (int)y, 20, rowColor);
		y += 35;
	}
	if (top.empty())
	{
		DT("No scores yet. Play a game!", (m_screenWidth - MT("No scores yet. Play a game!", 22)) / 2, 300, 22, KBC::TEXT_SECONDARY);
	}
	if (DrawButton("BACK", 20, m_screenHeight - 60, 100, 40, { 80, 20, 20, 255 }, { 120, 30, 30, 255 }))
		m_currentState = MENU;
}

void App::UpdateStats()
{
	if (IsKeyPressed(KEY_ESCAPE))
		m_currentState = MENU;
	if (m_statsEnteringName)
	{
		int key = GetCharPressed();
		while (key > 0)
		{
			if ((key >= 32) && (key <= 125) && (m_statsNameCharCount < 20))
			{
				m_statsPlayerName += (char)key;
				m_statsNameCharCount++;
			}
			key = GetCharPressed();
		}
		if (IsKeyPressed(KEY_BACKSPACE) && m_statsNameCharCount > 0)
		{
			m_statsPlayerName.pop_back();
			m_statsNameCharCount--;
		}
		if (IsKeyPressed(KEY_ENTER) && m_statsNameCharCount > 0)
		{
			try
			{
				m_statsData = m_playerManager.GetPlayerStats(m_statsPlayerName);
				m_statsEnteringName = false;
			}
			catch (std::exception&)
			{
				m_statsData = nullptr;
			}
		}
	}
}

void App::DrawStats()
{
	const char* title = "PLAYER STATISTICS";
	DT(title, (m_screenWidth - MT(title, 36)) / 2, 40, 36, KBC::GOLD_TEXT);
	if (m_statsEnteringName)
	{
		DT("Enter player name:", (m_screenWidth - MT("Enter player name:", 22)) / 2, 200, 22, KBC::TEXT_SECONDARY);
		float boxW = 300, boxH = 45;
		float boxX = (m_screenWidth - boxW) / 2;
		float boxY = 240;
		DrawRectangleRounded({ boxX, boxY, boxW, boxH }, 0.2f, 8, KBC::PANEL_BG);
		DrawRectangleRoundedLines({ boxX, boxY, boxW, boxH }, 0.2f, 8, KBC::PANEL_BORDER);
		if (m_statsNameCharCount > 0)
			DT(m_statsPlayerName.c_str(), (int)(boxX + 10), (int)(boxY + 12), 22, KBC::TEXT_PRIMARY);
		else
			DT("Type name...", (int)(boxX + 10), (int)(boxY + 12), 22, KBC::DISABLED);
	}
	else if (m_statsData)
	{
		float x = 300;
		float y = 120;
		float gap = 38;
		DT(TextFormat("Player: %s", m_statsData->_playerName.c_str()), (int)x, (int)y, 24, KBC::TEXT_PRIMARY);
		y += gap;
		DT(TextFormat("Games Played: %d", m_statsData->_totalGamesPlayed), (int)x, (int)y, 22, KBC::TEXT_SECONDARY);
		y += gap;
		DT(TextFormat("Total Winnings: $%d", m_statsData->_totalWinnings), (int)x, (int)y, 22, KBC::GOLD_TEXT);
		y += gap;
		DT(TextFormat("Highest Prize: $%d", m_statsData->_highestPrizeWon), (int)x, (int)y, 22, KBC::GOLD_TEXT);
		y += gap;
		DT(TextFormat("Questions Answered: %d", m_statsData->_totalQuestionsAnswered), (int)x, (int)y, 22, KBC::TEXT_SECONDARY);
		y += gap;
		DT(TextFormat("50/50 Used: %d times", m_statsData->_fiftyFiftyUseCount), (int)x, (int)y, 22, KBC::TEXT_SECONDARY);
		y += gap;
		DT(TextFormat("Audience Used: %d times", m_statsData->_askTheAudienceUseCount), (int)x, (int)y, 22, KBC::TEXT_SECONDARY);
		y += gap;
		DT(TextFormat("Phone Used: %d times", m_statsData->_phoneAFriendUseCount), (int)x, (int)y, 22, KBC::TEXT_SECONDARY);
		y += gap + 10;
		DT("Category Performance:", (int)x, (int)y, 22, KBC::GOLD_TEXT);
		y += 30;
		for (const auto& cat : m_statsData->_categoryPerformance)
		{
			int correct = cat.second.first;
			int total = cat.second.second;
			int pct = (total > 0) ? (correct * 100 / total) : 0;
			DT(TextFormat("  %s: %d/%d (%d%%)", cat.first.c_str(), correct, total, pct), (int)x, (int)y, 18, KBC::TEXT_SECONDARY);
			y += 25;
		}
	}
	if (DrawButton("BACK", 20, m_screenHeight - 60, 100, 40, { 80, 20, 20, 255 }, { 120, 30, 30, 255 }))
		m_currentState = MENU;
}

bool App::DrawButton(const char* text, float x, float y, float w, float h, Color normal, Color hover)
{
	Vector2 mouse = GetMousePosition();
	Rectangle rect = { x, y, w, h };
	bool hovered = CheckCollisionPointRec(mouse, rect);
	bool clicked = hovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
	Color col = hovered ? hover : normal;
	DrawRectangleRounded(rect, 0.25f, 8, col);
	DrawRectangleRoundedLines(rect, 0.25f, 8, hovered ? KBC::GOLD_TEXT : KBC::PANEL_BORDER);
	int textW = MT(text, 22);
	DT(text, (int)(x + (w - textW) / 2), (int)(y + (h - 22) / 2), 22, KBC::TEXT_PRIMARY);
	return clicked;
}

void App::DrawPrizeLadder(float x, float y, float w, float h)
{
	DrawRectangleRounded({ x, y, w, h }, 0.05f, 8, { 10, 15, 50, 255 });
	DrawRectangleRoundedLines({ x, y, w, h }, 0.05f, 8, KBC::PANEL_BORDER);
	DT("PRIZE LADDER", (int)(x + (w - MT("PRIZE LADDER", 18)) / 2), (int)(y + 10), 18, KBC::GOLD_TEXT);
	if (!m_currentSession) return;
	const GameState& gs = m_currentSession->GetGameState();
	float rowH = (h - 50) / 15.0f;
	float rowY = y + 40;
	int prizes[] = { 100, 200, 300, 500, 1000, 2000, 4000, 8000, 16000, 32000, 64000, 125000, 250000, 500000, 1000000 };
	for (int i = 14; i >= 0; i--)
	{
		float ry = rowY + (14 - i) * rowH;
		int qNum = i + 1;
		bool isCurrent = (qNum == gs._questionNumber);
		bool isSafeHaven = (qNum == 5 || qNum == 10);
		bool isPassed = (qNum < gs._questionNumber);
		Color textCol = KBC::TEXT_SECONDARY;
		if (isCurrent)
		{
			DrawRectangle((int)(x + 5), (int)(ry), (int)(w - 10), (int)(rowH - 2), { 255, 255, 255, 30 });
			textCol = WHITE;
		}
		if (isSafeHaven)
			textCol = KBC::SAFE_HAVEN;
		if (isPassed)
			textCol = KBC::DISABLED;
		DT(TextFormat("%2d.", qNum), (int)(x + 10), (int)(ry + 2), 16, textCol);
		DT(TextFormat("$%d", prizes[i]), (int)(x + 45), (int)(ry + 2), 16, isSafeHaven ? KBC::SAFE_HAVEN : (isCurrent ? KBC::GOLD_TEXT : textCol));
	}
}

void App::DrawLifelineIcons(float x, float y)
{
	if (!m_currentSession) return;
	const GameState& gs = m_currentSession->GetGameState();
	float iconSize = 45;
	float gap = 55;
	Color c1 = gs._fiftyFiftyUsed ? KBC::DISABLED : KBC::HIGHLIGHT;
	DrawRectangleRounded({ x, y, iconSize, iconSize }, 0.3f, 6, c1);

	DT("50", (int)(x + 4), (int)(y + 5), 14, KBC::TEXT_PRIMARY);
	DT("50", (int)(x + 4), (int)(y + 22), 14, KBC::TEXT_PRIMARY);

	Color c2 = gs._askTheAudienceUsed ? KBC::DISABLED : KBC::HIGHLIGHT;

	DrawRectangleRounded({ x + gap, y, iconSize, iconSize }, 0.3f, 6, c2);

	DT("AUD", (int)(x + gap + 6), (int)(y + 13), 16, KBC::TEXT_PRIMARY);

	Color c3 = gs._phoneAFriendUsed ? KBC::DISABLED : KBC::HIGHLIGHT;

	DrawRectangleRounded({ x + gap * 2, y, iconSize, iconSize }, 0.3f, 6, c3);
	DT("PHN", (int)(x + gap * 2 + 5), (int)(y + 13), 16, KBC::TEXT_PRIMARY);
}

void App::DrawTimerBar(float x, float y, float w, float h)
{
	float fraction = m_speedTimer / SPEED_TIME_LIMIT;
	fraction = (fraction < 0) ? 0 : (fraction > 1) ? 1 : fraction;
	Color barColor;
	if (fraction > 0.5f)
		barColor = KBC::TIMER_GREEN;
	else if (fraction > 0.25f)
		barColor = KBC::TIMER_YELLOW;
	else
		barColor = KBC::TIMER_RED;
	DrawRectangleRounded({ x, y, w, h }, 0.5f, 4, { 30, 30, 50, 255 });
	DrawRectangleRounded({ x, y, w * fraction, h }, 0.5f, 4, barColor);
	DT(TextFormat("%.0fs", m_speedTimer), (int)(x + w + 10), (int)(y - 2), 18, barColor);
}
