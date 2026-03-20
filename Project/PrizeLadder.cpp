#include "PrizeLadder.h"
#include <stdexcept>
#include <iostream>

PrizeLadder::PrizeLadder()
{
	ResetLadder();
}

int PrizeLadder::GetPrizeAmount(int questionIndex) const
{
	if (questionIndex < 1 || questionIndex > static_cast<int>(m_PrizeAmounts.size()))
	{
		throw std::out_of_range("Question index is out of range.");
	}
	return m_PrizeAmounts[questionIndex - 1];
}

bool PrizeLadder::IsLadderEmpty() const
{
	return m_PlayerProgressionLadder.empty();
}

bool PrizeLadder::IsAtSafeHaven() const
{
	if (m_PlayerProgressionLadder.empty())
	{
		return false;
	}
	else if (m_PlayerProgressionLadder.top() == _FIRST_SAFE_HAVEN || m_PlayerProgressionLadder.top() == _SECOND_SAFE_HAVEN )
	{
		return true;
	}
	return false;
}

bool PrizeLadder::IsTheQuestionSafeHaven(int questionIndex) const
{
	if (questionIndex == _FIRST_SAFE_HAVEN || questionIndex == _SECOND_SAFE_HAVEN)
	{
		return true;
	}
	else
	{
		return false;
	}
}

int PrizeLadder::MoveUpLadder(int questionIndex)
{
	if (questionIndex < 1 || questionIndex > static_cast<int>(m_PrizeAmounts.size()))
	{
		throw std::out_of_range("Question index is out of range.");
	}

	int prizeAmount = GetPrizeAmount(questionIndex);
	m_PlayerProgressionLadder.push(questionIndex);

	return prizeAmount;
}

int PrizeLadder::OnWrongAnswer()
{
	if (m_PlayerProgressionLadder.empty())
	{
		return 0;
	}
	else if (m_PlayerProgressionLadder.top() >= _SECOND_SAFE_HAVEN)
	{
		ResetLadder();
		return GetPrizeAmount(_SECOND_SAFE_HAVEN);
	}
	else if (m_PlayerProgressionLadder.top() >= _FIRST_SAFE_HAVEN)
	{
		ResetLadder();
		return GetPrizeAmount(_FIRST_SAFE_HAVEN);
	}

	ResetLadder();
	return 0;

}

void PrizeLadder::DisplayThePrizeLadder() const
{
	size_t currentPosition = m_PlayerProgressionLadder.empty() ? 0 : m_PlayerProgressionLadder.top();
	for (size_t i = 0; i < m_PrizeAmounts.size(); i++)
	{
		if (i + 1 == currentPosition)
		{
			std::cout << "-> ";
		}
		else
		{
			std::cout << "   ";
		}
		std::cout << "Question " << i + 1 << ": $" << m_PrizeAmounts[i] << std::endl;
	}
}

void PrizeLadder::ResetLadder()
{
	if (!IsLadderEmpty())
	{
		while (!m_PlayerProgressionLadder.empty())
		{
			m_PlayerProgressionLadder.pop();
		}
	}
}
