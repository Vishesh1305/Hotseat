#include "GameSession.h"
#include "QuestionBank.h"
#include "LifelineEngine.h"
#include <stdexcept>


GameSession::GameSession(QuestionBank* InQuestionBank, Leaderboard* InLeaderboard, PlayerManager* InPlayerManager, std::string InPlayerName, GameMode InGameMode) 
	: 
	m_gameState({}),
	m_questionBank(InQuestionBank),     
	m_prizeLadder({}),
	m_leaderboard(InLeaderboard),       
	m_playerManager(InPlayerManager),   
	m_currentQuestion(nullptr),         
	m_gameMode(InGameMode),             
	m_playerName(std::move(InPlayerName)),
	m_categoryResults()
{

}

void GameSession::StartGame()
{
	m_gameState.ResetGameState();
	m_currentQuestion = m_questionBank->GetRandomQuestion(1);
	m_gameState._currentQuestionText = m_currentQuestion->QuestionText;
	m_gameState._gameStatus = PLAYING;
}

bool GameSession::SubmitAnswer(char InAnswer)
{
	if (Question::IsValidChoice(InAnswer))
	{
		if (m_currentQuestion->IsCorrect(InAnswer))
		{
			m_gameState._currentPrizeAmount = m_prizeLadder.MoveUpLadder(m_gameState._questionNumber); //MoveUpLadder does literally move up the ladder by pushing it into the stack and also returns the updated prize
			
			auto it = m_categoryResults.find(m_currentQuestion->Category);
			if (it == m_categoryResults.end())
			{
				m_categoryResults.insert({m_currentQuestion->Category, {1, 1}});
			}
			else
			{
				it->second.first += 1;   
				it->second.second += 1;  
			}
			m_gameState._questionNumber += 1;

			if (m_gameState._questionNumber == 15)
			{
				m_gameState._gameStatus = WON;
				return true;
			}
			m_currentQuestion = m_questionBank->GetRandomQuestion(m_gameState._questionNumber);

			return true;
		}
		else
		{
			m_gameState._currentPrizeAmount = m_prizeLadder.OnWrongAnswer();

			auto it = m_categoryResults.find(m_currentQuestion->Category);
			if (it == m_categoryResults.end())
			{
				m_categoryResults.insert({ m_currentQuestion->Category, {0, 1} });
			}
			else
			{
				it->second.second += 1;
			}
			m_gameState._gameStatus = LOST;
			return false;
		}
	}
	throw std::invalid_argument("Invalid answer choice");
}

FiftyFiftyResult GameSession::ActivateFiftyFifty()
{
	if (m_gameState._fiftyFiftyUsed)
	{
		throw std::runtime_error("'50/50' lifeline already used");
	}
	m_gameState._fiftyFiftyUsed = true;
	return LifelineEngine::FiftyFifty(m_currentQuestion);
}

AskTheAudienceResult GameSession::ActivateAskTheAudience()
{
	if (m_gameState._askTheAudienceUsed)
	{
		throw std::runtime_error("'Ask The Audience' lifeline already used");
	}
	m_gameState._askTheAudienceUsed = true;
	return LifelineEngine::AskTheAudience(m_currentQuestion);
}

PhoneAFriendResult GameSession::ActivatePhoneAFriend()
{
	if (m_gameState._phoneAFriendUsed)
	{
		throw std::runtime_error("'Phone A Friend' lifeline already used");
	}
	m_gameState._phoneAFriendUsed = true;
	return LifelineEngine::PhoneAFriend(m_currentQuestion);
}

void GameSession::WalkAway()
{
	m_gameState._gameStatus = WITHDRAWN;
}

bool GameSession::IsGameOver() const
{
	if (m_gameState._gameStatus != PLAYING)
	{
		return true;
	}
	return false;
}

