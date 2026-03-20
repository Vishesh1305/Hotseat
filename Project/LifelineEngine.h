#pragma once
#include "LifelineResult.h"

struct Question;

class LifelineEngine
{
public:
	static FiftyFiftyResult FiftyFifty(const Question* ques);
	static AskTheAudienceResult AskTheAudience(const Question* ques);
	static PhoneAFriendResult PhoneAFriend(const Question* ques);
};

