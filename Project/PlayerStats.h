#pragma once
#include <string>
#include <map>
#include <vector>
struct PlayerStats
{
	std::string _playerName;
	int _totalGamesPlayed;
	int _totalWinnings;
	int _highestPrizeWon;
	int _totalQuestionsAnswered;
	int _fiftyFiftyUseCount;
	int _askTheAudienceUseCount;
	int _phoneAFriendUseCount;
	std::map<std::string, std::pair<int, int>> _categoryPerformance;
	std::vector<std::string> _sessionHistory;
};
