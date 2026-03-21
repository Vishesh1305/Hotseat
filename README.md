# HotSeat

A Kaun Banega Crorepati / Who Wants to Be a Millionaire game in C++ with a Raylib graphical frontend.

![C++](https://img.shields.io/badge/C%2B%2B-20-blue) ![Raylib](https://img.shields.io/badge/Raylib-5.0-green) ![Platform](https://img.shields.io/badge/Platform-Windows-lightgrey)

---

## How to build

Requires Visual Studio 2022+ with C++ desktop workload and Raylib linked via vcpkg or manual setup.

1. Clone the repo
2. Open `Project.sln` in Visual Studio
3. Build and run (F5)

The game expects `Containers/question_bank.txt` and `Containers/high_scores.txt` relative to the working directory. Both ship with the repo.

## Game modes

**Classic**: 15 questions, prize ladder from $100 to $1,000,000. Safe havens at question 5 and 10. Wrong answer drops you to the last safe haven. Walk away anytime.

**Speed**: Same as Classic but each question has a 30-second countdown. Time runs out, game ends.

**Practice**: Endless questions, no prize tracking, no penalty for wrong answers.

## Lifelines

Three per game, each usable once:

- **50:50**: eliminates two wrong answers
- **Ask the Audience**: percentage poll weighted toward the correct answer
- **Phone a Friend**: friend's suggestion with ~80% accuracy

## File formats

Question bank (`question_bank.txt`), pipe-delimited:
```
difficulty|question|A|B|C|D|correct_letter|category
```

High scores (`high_scores.txt`), comma-delimited:
```
PlayerName,FinalPrize,QuestionsAnswered,Date
```

## Project structure

```
Project/
├── Assets/
│   └── Poppins-Regular.ttf
├── Containers/
│   ├── high_scores.txt
│   └── question_bank.txt
├── App.h / .cpp
├── GameSession.h / .cpp
├── GameState.h
├── Leaderboard.h / .cpp
├── LifelineEngine.h / .cpp
├── LifelineResult.h
├── PlayerManager.h / .cpp
├── PlayerStats.h
├── PrizeLadder.h / .cpp
├── Question.h
├── QuestionBank.h / .cpp
└── main.cpp
```

Backend classes (GameSession, QuestionBank, PrizeLadder, LifelineEngine, PlayerManager, Leaderboard) have zero Raylib dependency. I built and tested the full game logic through a console interface first, then wrote the graphical layer on top.

## Controls

Mouse-driven. Click answer panels, click buttons, press [L] for lifelines, [W] to walk away. Name entry is keyboard.

## Technical highlights

- `std::map<int, vector<Question>>` for difficulty-bucketed question storage
- `std::stack<int>` for prize ladder progression tracking
- `std::vector<ScoreEntry>` with custom sort for leaderboard
- `std::map<string, PlayerStats>` for player registry
- Pipe-delimited and CSV file parsing with exception handling
- `try-catch` around all I/O, missing files, malformed data, and invalid input handled gracefully
- Three game modes with distinct behavior (timer, no-penalty, standard)
- Answer reveal animation with stored question state to prevent race conditions

## Built with

- C++20, Visual Studio 2022
- [Raylib](https://www.raylib.com/)
- [Poppins](https://fonts.google.com/specimen/Poppins) font (Google Fonts)
