#pragma once
#include <string>
#include <array>

struct Question
{
	std::string QuestionText;
	std::array<std::string, 4> Answers;

	char CorrectAnswer;
	int Difficulty;
	std::string Category;

	bool IsCorrect(char answer) const
	{
		return std::toupper(answer) == std::toupper(CorrectAnswer);
	}

	static bool IsValidChoice(char answer)
	{
		char upper = std::toupper(answer);
		return (upper == 'A' || upper == 'B' || upper == 'C' || upper == 'D');
	}

	static int LetterToIndex(char letter)
	{
		return std::toupper(letter) - 'A';
	}
	static char IndexToLetter(int index)
	{
		return 'A' + index;
	}
};
