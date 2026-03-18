#include "QuestionBank.h"
#include <stdexcept>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <sstream>

QuestionBank::QuestionBank(const std::string& FilePath)
{
	if (FilePath.empty())
	{
		throw std::invalid_argument("File path cannot be empty.");
	}
	PopulateMap(FilePath);
}

const Question* const QuestionBank::GetRandomQuestion(int InDifficulty) const
{
	const std::vector<Question>& questions = m_questionsByDifficulty.at(InDifficulty);
	int randomIndex = std::rand() % questions.size();

	return &questions[randomIndex];
}

const int QuestionBank::GetQuestionCount(int InDifficulty) const
{

	return m_questionsByDifficulty.at(InDifficulty).size();
}

const std::vector<Question>& QuestionBank::GetQuestionList(int InDifficulty) const
{
	return m_questionsByDifficulty.at(InDifficulty);
}

void QuestionBank::PopulateMap(const std::string& InFileName)
{
	std::ifstream file(InFileName);
	if (!file.is_open())
	{
		throw std::runtime_error("Could not open file: " + InFileName);
	}
	std::string line;

	while (getline(file, line))
	{
		std::stringstream ss(line);
		std::string part;
		std::vector<std::string> parts;

		while (getline(ss, part, '|'))
		{
			parts.push_back(part);
		}
		if (parts.size() == 8)
		{
			int difficulty;
			std::string ques = parts[1];
			std::string answerA = parts[2];
			std::string answerB = parts[3];
			std::string answerC = parts[4];
			std::string answerD = parts[5];
			char correctAnswer = parts[6][0];
			std::string category = parts[7];
			try 
			{
				 difficulty = std::stoi(parts[0]);
			}
			catch (const std::exception& e)
			{
				std::cerr << "Error parsing difficulty: " << e.what() << " in line: " << line << std::endl;
				continue;
			}
			Question question
			{
				.QuestionText = ques,
				.Answers = { answerA, answerB, answerC, answerD },
				.CorrectAnswer = correctAnswer,
				.Difficulty = difficulty,
				.Category = category

			};
			m_questionsByDifficulty[difficulty].push_back(question);
			
		}
	}
	file.close();
}
