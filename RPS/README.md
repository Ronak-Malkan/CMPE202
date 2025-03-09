# Rock-Paper-Scissors Game

This is a C++ implementation of the classic Rock-Paper-Scissors game where a human player competes against a computer player. The computer can use one of two strategies: random or smart (using machine learning).

## Game Rules

- Rock beats scissors
- Scissors beat paper
- Paper beats rock
- The game consists of 20 rounds
- The player with the most wins at the end is the winner

## Features

- Two computer strategies:
  - Random: Computer makes random choices
  - Smart: Computer uses machine learning to predict and counter the player's moves
- The smart strategy saves its learned patterns to a file and loads them when the game starts
- Clean object-oriented design with strategy pattern implementation

## Class Design

- `Move`: Enum representing Rock, Paper, or Scissors
- `Player`: Abstract base class for all players
- `HumanPlayer`: Implementation for human player
- `ComputerPlayer`: Implementation for computer player
- `Strategy`: Abstract base class for computer strategies
- `RandomStrategy`: Implementation of random strategy
- `SmartStrategy`: Implementation of smart strategy using machine learning
- `Game`: Main game engine that controls the flow

## Building the Project

### Prerequisites

- CMake (version 3.10 or higher)
- C++ compiler with C++14 support

### Build Instructions

1. Create a build directory:
   ```
   mkdir build
   cd build
   ```

2. Generate build files with CMake:
   ```
   cmake ..
   ```

3. Build the project:
   ```
   cmake --build .
   ```

## Running the Game

After building, run the executable:

```
./rps
```

Follow the on-screen instructions to play the game:
1. Choose the computer strategy (1 for Random, 2 for Smart)
2. For each round, enter your move (R for Rock, P for Paper, S for Scissors)
3. The game will display the result of each round and the final score after 20 rounds

## Design Principles

This implementation demonstrates several design principles:

1. **Strategy Pattern**: The computer player can use different strategies without changing its implementation
2. **Open/Closed Principle**: New strategies can be added without modifying existing code
3. **Dependency Inversion**: High-level modules depend on abstractions, not concrete implementations
4. **Single Responsibility**: Each class has a single responsibility
5. **Interface Segregation**: Players and strategies have focused interfaces

## Future Improvements

- Add more sophisticated machine learning strategies
- Support for different game modes (e.g., best of N, time-limited)
- Graphical user interface
- Network play against other human players
