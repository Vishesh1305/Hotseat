#pragma once
#include <map>
#include <vector>
#include "Question.h"

class QuestionBank
{
private:
	std::map<int, std::vector<Question>> m_questionsByDifficulty;

public:
	QuestionBank(const std::string& FilePath);
	~QuestionBank() = default;

	const Question* const GetRandomQuestion(int InDifficulty) const;
	const int GetQuestionCount(int InDifficulty) const;
	const std::vector<Question>& GetQuestionList(int InDifficulty) const;

private:
	void PopulateMap(const std::string& InFileName);
};

