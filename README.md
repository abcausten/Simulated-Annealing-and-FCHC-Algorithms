# 8-Queens: Simulated Annealing and First-Choice Hill Climbing (C++)

This code solves the 8-queens search problem with two local search algorithms: **Simulated Annealing** and **First-Choice Hill Climbing**. The goal is to place 8 queens on a chessboard so that no two queens attack each other. The trials mainly show the large difference between Simulated Annealing and First-Choice Hill Climbing as FCHC is a very low accuracy search algorithm without many changes made. The original code for all 5 functions listed below was written in Fall 2025 with some extra functions to make easier to execute created in Fall 2026. 

## State Representation

A state is an array `Q` of 8 integers, one queen per column. `Q[i] = j` means the queen in column `i` sits on row `j`. Columns and rows are numbered 0 to 7.

```
Q = {0, 4, 7, 5, 2, 6, 1, 3}
```

Because every queen has its own column, queens can never attack each other vertically, so only rows and diagonals matter.

## Functions

| Function | Purpose |
|----------|---------|
| `int getAttackScore(int Q[])` | Returns the number of conflicts (0 means solved) |
| `void nextPosition(int Q[], int nextQ[])` | Copies `Q` into `nextQ`, then moves one random queen to a different row in its column |
| `bool acceptNext(double dE, double T)` | Returns `true` with probability e^(dE/T) |
| `bool solveSimulatedAnnealing(int Sol[], double T0 = 4, int maxSteps = 5000)` | Simulated annealing with a linear cooling schedule |
| `bool solveHillClimbingFC(int Sol[], int maxSteps = 5000)` | First-choice hill climbing |

### a. `getAttackScore`

For each queen, the function counts the other queens that can attack it:

- **Row:** any other column holding a queen in the same row
- **Diagonals:** it walks in all four diagonal directions (up-left, up-right, down-left, down-right) and checks whether the queen in each neighboring column is on the diagonal square

A score of **0** means no queens attack each other, so the board is a solution. Each attacking pair is counted from both queens, so the score is twice the number of attacking pairs.

### b. `nextPosition` and `acceptNext`

- `nextPosition` picks a random column and a random new row (always different from the current one) and writes the changed board into `nextQ`. This is the "neighbor" in the search.
- `acceptNext(dE, T)` takes a change in energy `dE <= 0` and a temperature `T`, and returns `true` with probability `e^(dE/T)`. A big drop in quality or a low temperature makes acceptance unlikely.

### c. `solveSimulatedAnnealing`

1. Start at temperature `T0`. Each step lowers it by `T0 / maxSteps`, so it reaches 0 after `maxSteps` steps (a linear schedule).
2. Generate a random neighbor with `nextPosition` and score it.
3. If the neighbor has score 0, copy it into `Sol` and return `true`.
4. Compute `E = oldScore - newScore`:
   - `E > 0` (better): always move to the neighbor.
   - `E <= 0` (same or worse): move with probability `e^(E/T)`, using `acceptNext`.
5. If the temperature runs out without a solution, return `false`. `Sol` holds the last state reached.

High temperatures allow many bad moves, which helps escape local minima. As the temperature falls, the search becomes pickier.

### d. `solveHillClimbingFC`

The same loop without temperature: generate a random neighbor, and move to it **only if it is strictly better** (`E > 0`). Return `true` on a score of 0, or `false` after `maxSteps` steps. It is faster per step but can get stuck in local minima, since it never accepts a worse move.

###d. `queens`

This is the function that is created after Simulated_Annealing_and_FCHC.cpp is ran. It is then called to run all the functions in a testing environement. 

## Output Values

Both solve simulated annealing and solve hill climbing will return `true` if a solution was found and `false` otherwise. In either case, `Sol` contains the final board (the solution, or the last state reached). Hill Climbing has a much lower solution rate which will be more evident in the 1000 and 5000 run output. 

This is an example of the graph for the final solution for solve SimulatedAnnealing. 

### Example Solution

This is the output of a simulated annealing solution with the graphing function. Row 7 is at the top, row 0 at the bottom; `Q` marks a queen.

```
Simulated annealing: solved, final score 0
. . Q . . . . .
. . . . . Q . .
. Q . . . . . .
. . . . . . Q .
Q . . . . . . .
. . . Q . . . .
. . . . . . . Q
. . . . Q . . .
```

There will be the format for the output of the 1000 and 5000 tests.
### Success Rate

| Algorithm | N = 1,000 | N = 5,000 |
|-----------|-----------|------------|
| Simulated Annealing |  | |
| First Choice Hill Climbing |  | |


## Usage

```
g++ -std=c++17 -O2 -o Simulated_Annealing_and_FCHC.cpp
./queens (for mac)
.\queens (for windows)

```
