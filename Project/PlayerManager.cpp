#include "PlayerManager.h"
#include <stdexcept>
#include "GameState.h"
#include <algorithm>
#include <format>

bool PlayerManager::DoesPlayerExist(const std::string& InName) const
{
	if (m_players.find(InName) != m_players.end())
	{
		return true;
	}
	return false;
}

void PlayerManager::RegisterPlayer(const std::string& InName)
{
	if (DoesPlayerExist(InName))
	{
		throw std::runtime_error("Player with that name already exists!");
	}
	PlayerStats stats
	{
		._playerName = InName,
		._totalGamesPlayed = 0,
		._totalWinnings = 0,
		._highestPrizeWon = 0,
		._totalQuestionsAnswered = 0,
		._fiftyFiftyUseCount = 0,
		._askTheAudienceUseCount = 0,
		._phoneAFriendUseCount = 0,

	};
	m_players.insert(std::make_pair(InName, stats));
}

const PlayerStats* const PlayerManager::GetPlayerStats(const std::string& InName) const
{
	if (!DoesPlayerExist(InName))
	{
		throw std::runtime_error("Player with that name doesn't exist!");
	}
	return &(m_players.at(InName));
}

void PlayerManager::UpdatePlayerStats(const std::string& InName, const GameState* InGameState, const std::map<std::string, std::pair<int, int>>& InCategoryResults)
{
	if (!DoesPlayerExist(InName))
	{
		throw std::runtime_error("Player with that name doesn't exist!");
	}

	PlayerStats& ref = m_players.at(InName);
	ref._totalGamesPlayed += 1;
	ref._totalWinnings += InGameState->_currentPrizeAmount;
	ref._highestPrizeWon = std::max(ref._highestPrizeWon, InGameState->_currentPrizeAmount);
	ref._totalQuestionsAnswered += InGameState->_questionNumber;
	ref._fiftyFiftyUseCount += (InGameState->_fiftyFiftyUsed) ? 1 : 0;
	ref._askTheAudienceUseCount += (InGameState->_askTheAudienceUsed) ? 1 : 0;
	ref._phoneAFriendUseCount += (InGameState->_phoneAFriendUsed) ? 1 : 0;

	for (auto& i : InCategoryResults)
	{
		std::map<std::string, std::pair<int, int>>::iterator it = ref._categoryPerformance.find(i.first);
		if (it == ref._categoryPerformance.end())
		{
			ref._categoryPerformance.insert(std::make_pair(i.first, std::make_pair(i.second.first, i.second.second)));
			continue;
		}

		it->second.first += i.second.first;
		it->second.second += i.second.second;
	}

	const int prize = InGameState->_currentPrizeAmount;
	const int questions = InGameState->_questionNumber;
	const std::string& status = InGameState->EnumValueToString(InGameState->_gameStatus);
	const std::string result = std::format("Prize: ${} | Questions: ${} | Status: {}", prize, questions, status);
	ref._sessionHistory.push_back(result);
}

const std::map<std::string, PlayerStats>& PlayerManager::GetPlayerMap() const
{
	return m_players;
}

int PlayerManager::StartingDifficulty(const std::string& InName) const
{
	if (!DoesPlayerExist(InName)) return 1;
	if (m_players.at(InName)._totalGamesPlayed == 0) return 1;

	int average = static_cast<int>(m_players.at(InName)._totalQuestionsAnswered / m_players.at(InName)._totalGamesPlayed);

	if (average <= 5)
	{
		return 1;
	}
	else if (average <= 9 && average >= 5)
	{
		return 3;
	}
	return 10;
}
