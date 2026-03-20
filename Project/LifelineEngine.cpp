#include "LifelineEngine.h"
#include "Question.h"
#include <random>
#include <array>
#include <algorithm>

FiftyFiftyResult LifelineEngine::FiftyFifty(const Question* ques)
{
	int correctAnswerIndex = ques->LetterToIndex(ques->CorrectAnswer);
	std::array<int, 3> incorrectIndices;
	int count = 0;
	for (int i = 0; i < 4; i++)
	{
		if (i != correctAnswerIndex)
		{
			incorrectIndices[count] = i;
			count++;
		}
	}
	std::random_device rd;
	std::mt19937 randomEngine(rd());
	std::shuffle(incorrectIndices.begin(), incorrectIndices.end(), randomEngine);

	const FiftyFiftyResult result{ incorrectIndices[0], incorrectIndices[1] };
	return result;

}

AskTheAudienceResult LifelineEngine::AskTheAudience(const Question* ques)
{
	int correctAnswerIndex = ques->LetterToIndex(ques->CorrectAnswer);
	std::array<int, 4> OptionsPercentages;

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> correctAnswerPercent(60, 80);
	OptionsPercentages[correctAnswerIndex] = correctAnswerPercent(gen);

	int remaining = 100 - OptionsPercentages[correctAnswerIndex];
	std::uniform_int_distribution<> dist(0, remaining);

	int a = dist(gen);
	int b = dist(gen);

	if (a > b) std::swap(a, b);

	int x = a;
	int y = b - a;
	int z = remaining - b;

	std::array<int, 3> splits {x, y, z};

	int count = 0;
	for (auto i = 0; i < 4; i++)
	{
		if (i != correctAnswerIndex)
		{
			OptionsPercentages[i] = splits[count];
			count++;
		}
	}

	const AskTheAudienceResult result
	{
		OptionsPercentages[0],
		OptionsPercentages[1],
		OptionsPercentages[2],
		OptionsPercentages[3],
	};

	return result;
}

PhoneAFriendResult LifelineEngine::PhoneAFriend(const Question* ques)
{
	
	int correctAnswerIndex = ques->LetterToIndex(ques->CorrectAnswer);

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> correctAnswerPercent(1, 100);
	std::uniform_int_distribution<> WrongAnswerIndexPick(0, 2);
	std::uniform_int_distribution<> FC(60, 95);

	std::array <int, 3> WrongOptions;
	int count = 0;
	for (auto i = 0; i < 4; i++)
	{
		if (i != correctAnswerIndex)
		{
			WrongOptions[count] = i;
			count++;
		}
	}

	int randomWrongAnswerIndex = WrongAnswerIndexPick(gen);

	int correctAnswerChance = correctAnswerPercent(gen);
	char givenAnswer = correctAnswerChance >= 80 ? ques->CorrectAnswer : ques->IndexToLetter(WrongOptions[randomWrongAnswerIndex]);

	int friendConfidence = FC(gen);

	const PhoneAFriendResult result
	{
		givenAnswer,
		friendConfidence
	};

	return result;

}
