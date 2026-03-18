#pragma once
#include <stack>
#include <string>
#include <array>

class PrizeLadder
{
private:
	static constexpr int _FIRST_SAFE_HAVEN = 4; // index
	static constexpr int _SECOND_SAFE_HAVEN = 9;

	std::stack<int> m_PlayerProgressionLadder;
	std::array<int, 15> m_PrizeAmounts{100, 200, 300, 500, 1000, 2000, 4000, 8000, 16000, 32000, 64000, 125000, 250000, 500000, 1000000};

public:
	PrizeLadder();

	int GetPrizeAmount(int questionIndex) const;

	bool IsLadderEmpty() const;
	bool IsAtSafeHaven() const;
	bool IsTheQuestionSafeHaven(int questionIndex) const;
	int MoveUpLadder(int questionIndex);
	int OnWrongAnswer();
	void DisplayThePrizeLadder() const;

	void ResetLadder();

};

