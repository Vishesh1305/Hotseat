#pragma once
#include <string>
#include <vector>
struct ScoreEntry
{
	std::string _playerName;
	int _finalPrize;
	int _questionsAnswered;
	std::string _date;
};

class Leaderboard
{
private:
	mutable std::vector<ScoreEntry> m_scoresBoard;

public:
	
	void AddScoreEntry(const ScoreEntry& InEntryToAdd);
	std::vector<ScoreEntry> GetTheTop() const; 
	const void DisplayTheLeaderboard() const;
	void SaveScoresToFile(const std::string& InFilePath);
	void LoadScores(const std::string& InFilePath);

private:
	void SortEntriesByPrize() const;
};

