#include <cstdlib>
#include <ctime>
#include "App.h"

int main()
{
	srand(static_cast<unsigned>(time(nullptr)));
	App app("Containers/question_bank.txt", "Containers/high_scores.txt");
	app.Run();
	return 0;
}
