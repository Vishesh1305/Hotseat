#include "Leaderboard.h"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>

void Leaderboard::AddScoreEntry(const ScoreEntry& InEntryToAdd)
{
	m_scoresBoard.push_back(InEntryToAdd);
}

std::vector<ScoreEntry> Leaderboard::GetTheTop() const
{
	SortEntriesByPrize();

	size_t topCount = std::min<size_t>(10, m_scoresBoard.size());
	return std::vector<ScoreEntry>(m_scoresBoard.begin(), m_scoresBoard.begin() + topCount);
}

const void Leaderboard::DisplayTheLeaderboard() const
{
	std::vector<ScoreEntry> TopVec = GetTheTop();

	for (int i = 0; i < TopVec.size(); i++)
	{
		std::cout << "Player Name: " << TopVec.at(i)._playerName << " | "
			<< "Questions Answered: " << TopVec.at(i)._questionsAnswered << " | "
			<< "Prize: " << TopVec.at(i)._finalPrize << " | "
			<< "Date: " << TopVec.at(i)._date << ".\n";
	}
}

void Leaderboard::SaveScoresToFile(const std::string& InFilePath)
{
	std::ofstream OutFile(InFilePath);
	if (!OutFile.is_open())
	{
		throw std::ios_base::failure("Failed to create/open file: " + InFilePath);
	}

	std::vector<ScoreEntry>::iterator it = m_scoresBoard.begin();
	for (it; it != m_scoresBoard.end();)
	{
		OutFile << it->_playerName << "," << it->_finalPrize << "," << it->_questionsAnswered << "," << it->_date << "\n";
		++it;
	}
	OutFile.close();
}

void Leaderboard::LoadScores(const std::string& InFilePath)
{
	std::ifstream InFile(InFilePath);
	if (!InFile.is_open())
	{
		throw std::ios_base::failure("Failed to Open file: " + InFilePath);
	}
	std::string line;
	while (getline(InFile, line))
	{
		std::stringstream ss(line);
		std::string part;

		std::vector<std::string> tokens;

		while (std::getline(ss, part, ','))
		{
			tokens.push_back(part);
		}
		if (tokens.size() != 4)
		{
			continue;
		}

		ScoreEntry entry;
		entry._playerName = tokens[0];
		entry._finalPrize = std::stoi(tokens[1]);
		entry._questionsAnswered = std::stoi(tokens[2]);
		entry._date = tokens[3];

		m_scoresBoard.push_back(entry);
	}
}

void Leaderboard::SortEntriesByPrize() const
{
	std::sort(m_scoresBoard.begin(), m_scoresBoard.end(), [](const ScoreEntry& a, const ScoreEntry& b)
		{
			return a._finalPrize > b._finalPrize;
		});

}
