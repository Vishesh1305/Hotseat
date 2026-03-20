#pragma once
#include <map>
#include <string>
#include "PlayerStats.h"

class GameState;

class PlayerManager
{
private:
	bool DoesPlayerExist(const std::string& InName) const;

private:
	std::map<std::string, PlayerStats> m_players;

public:
	void RegisterPlayer(const std::string& InName);
	const PlayerStats* const GetPlayerStats(const std::string& InName) const;
	void UpdatePlayerStats(const std::string& InName, const GameState* InGameState, const std::map<std::string, std::pair<int, int>>& InCategoryResults);
	const std::map<std::string, PlayerStats>& GetPlayerMap() const;
	int StartingDifficulty(const std::string& InName) const;

};

