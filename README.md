# HotSeat

A Who Wants to Be a Millionaire (Kaun Banega Crorepati) game engine in C++. Built around STL data structures — `std::map` for difficulty-bucketed question storage, `std::stack` for prize ladder progression, `std::vector` for sorted leaderboards — with file I/O, exception handling, and a Raylib graphical frontend.

![C++](https://img.shields.io/badge/C%2B%2B-17-blue) ![Raylib](https://img.shields.io/badge/Raylib-5.0-green) ![Platform](https://img.shields.io/badge/Platform-Windows-lightgrey)

---

## What is this?

A KBC/Millionaire game in C++ with Raylib graphics. Pick a game mode, answer 15 questions that get progressively harder, use lifelines when you're stuck, and try to make it to the top prize before you get knocked out. The game tracks stats across sessions, maintains a leaderboard, and supports multiple players.

The look is modeled after Kaun Banega Crorepati — dark studio void, glowing answer panels, gold prize text, that whole hot seat feel.

## How to build and run

You'll need Visual Studio 2022 (or later) with the C++ desktop workload installed, and Raylib set up as a dependency.

1. Clone the repo
2. Open `HotSeat.sln` in Visual Studio
3. Make sure Raylib is linked (the project expects Raylib headers and libs in the standard include/lib paths — see Raylib's [Visual Studio setup guide](https://github.com/raysan5/raylib/wiki/Working-on-Windows) if you haven't done this before)
4. Build and run (F5)

The game looks for `questions_bank.txt` in the working directory at startup. If it's missing, you'll get an error screen. The file ships with the repo.

## Game modes

**Classic** — The standard 15-question format. Prize amounts go from $100 up to $1,000,000, with safe havens at question 5 ($1,000) and question 10 ($32,000). Answer wrong and you drop to the last safe haven you passed. You can walk away at any time and keep what you've earned.

**Speed** — Same structure, but each question has a 30-second timer. Run out of time and it counts as a wrong answer. Adds pressure, especially on the harder questions where you actually need to think.

**Practice** — No prize tracking, no consequences. Questions keep coming and you can use it to learn the question bank or just mess around without stakes.

## Lifelines

You get three per game, each usable once:

- **50:50** kills two wrong answers so you're left with a coin flip
- **Ask the Audience** shows a poll of what the "audience" would pick (more reliable on easy questions, less so on hard ones)
- **Phone a Friend** gives you a friend's best guess — right about 80% of the time

## Question bank format

Questions live in `questions_bank.txt`, one per line, pipe-delimited:

```
difficulty|question text|option A|option B|option C|option D|correct letter|category
```

Example:
```
1|What is the capital of France?|Berlin|Madrid|Paris|Rome|C|Geography
```

Difficulty ranges from 1 (easy) to 15 (hardest). The game ships with 50+ questions spread across categories and difficulty levels. You can add your own — just follow the format and restart the game.

## High scores and stats

High scores are saved to `high_scores.txt` automatically when a game ends. Format:

```
PlayerName,FinalPrize,QuestionsAnswered,Date
```

The game tracks per-player statistics across sessions: games played, average prize, win percentage, lifeline usage, and how you perform in different question categories. All of this persists between runs.

## Project structure

```
HotSeat/
├── main.cpp
├── App.h / .cpp                 — Window lifecycle, game loop, owns everything
├── ResourceManager.h / .cpp     — Font loading, color palette
├── ScreenManager.h / .cpp       — Screen state machine
├── Screen.h                     — Abstract base for all screens
├── screens/
│   ├── MainMenuScreen           — Title screen, navigation
│   ├── ModeSelectScreen         — Classic / Speed / Practice
│   ├── UsernameEntryScreen      — Player name input
│   ├── GameplayScreen           — The actual game
│   ├── ResultsScreen            — End-of-game summary
│   ├── LeaderboardScreen        — Top 10 display
│   └── StatsScreen              — Player and aggregate statistics
├── ui/
│   ├── UIButton                 — Clickable button component
│   ├── UITextInput              — Text entry field
│   ├── UIAnswerPanel            — Answer option display (4 per question)
│   ├── UIPrizeLadder            — Sidebar showing all 15 prize tiers
│   ├── UITimerBar               — Countdown bar for Speed mode
│   ├── UIBarChart               — Audience poll visualization
│   └── UILifelineIcon           — Lifeline status indicators
├── game/
│   ├── Question.h               — Question data struct
│   ├── QuestionBank             — Question storage, loading, random selection
│   ├── PrizeLadder              — Prize amounts, safe havens, stack tracking
│   ├── GameState.h              — Current game snapshot struct
│   ├── GameSession              — One game playthrough controller
│   ├── LifelineEngine           — Lifeline computation logic
│   ├── LifelineResult.h         — Lifeline output structs
│   ├── GameMode.h               — Mode enum
│   ├── PlayerStats.h            — Per-player stat struct
│   └── PlayerManager            — Player registry and stat tracking
├── data/
│   ├── Leaderboard              — Score storage, sorting, top-10
│   ├── FileIO                   — File reading/writing, error handling
│   └── SessionLogger            — Game session log writing
└── assets/
    └── font.ttf                 — UI font
```

Everything under `game/` and `data/` compiles without Raylib. The rendering layer (`screens/`, `ui/`, `App`, `ResourceManager`, `ScreenManager`) is the only code that touches `raylib.h`. You could strip the entire UI and run the game logic from a console if you wanted to.

## Architecture notes

I kept the backend and frontend separated so the game logic doesn't depend on Raylib at all. Everything in `game/` and `data/` is pure C++ and STL. The screens just hold references to those backend objects, call into them when the player does something, and read back state to figure out what to draw. I actually developed and tested the game logic through a console interface first, then layered the graphical UI on top.

Each screen has Enter/Exit/Update/Draw methods. ScreenManager keeps track of which one is active and handles transitions between them.

File I/O is wrapped in try-catch blocks throughout. If the question bank is missing or a line is malformed or a high score save fails, the game shows an error popup instead of crashing. The window stays open no matter what goes wrong.

## Controls

Everything is mouse-driven. Click answer panels to select, click lifeline icons to activate them, click buttons to navigate. Username entry uses keyboard input.

## Technical highlights

- **Data structures**: `std::map<int, vector<Question>>` for O(log n) difficulty-tier lookups, `std::stack` for prize ladder state tracking, `std::vector` with custom sorting for leaderboard management, `std::map<string, PlayerStats>` for player registry
- **File I/O**: Pipe-delimited question bank parser, CSV high score persistence, session logging — all wrapped in exception handling with graceful recovery
- **Architecture**: Clean backend/frontend separation. Game logic compiles independently of Raylib. Screen state machine with Enter/Exit/Update/Draw lifecycle
- **Error handling**: try-catch around all I/O paths. Malformed data, missing files, and invalid input are caught and surfaced as in-app error states, never crashes

## Built with

- C++17
- [Raylib](https://www.raylib.com/) by Ramon Santamaria
- Visual Studio 2022