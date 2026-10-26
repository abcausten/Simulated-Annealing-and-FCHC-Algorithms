#include <iostream>
#include <iomanip>
#include <random>
#include <cmath>
using namespace std;

//Random Number Generator
mt19937 gen(random_device{}());


int getAttackScore(int Q[])
{
    int conflictNumber = 0;
    int column;
    int row;
    int placeholderRow;
    // this is the number that it should be if its diagonal
    int intendedRow;
    int intendedColumn;
    // check every single direction that the queen can hit
    for (int i = 0; i < 8; i++)
    {
        // i is which column we are checking
        // this is the current row
        row = Q[i];
        // check row
        for (int j = 0; j < 8; j++)
        {
            // go through every column but at the same row and except the column
            //  that placement is in
            if (j == i)
            {
                // this is when this is the current column the queen is in
                continue;
            }
            if (row == Q[j])
            {
                conflictNumber++;
            }
        }
        // check diagonal going up left
        placeholderRow = row;
        for (int b = i - 1; b >= 0; b--)
        {
            intendedRow = placeholderRow + 1;
            placeholderRow++;
            if (placeholderRow > 7)
            {
                break;
            }
            if (Q[b] == intendedRow)
            {
                conflictNumber++;
            }
        }
        // check diagonal going up right
        placeholderRow = row;
        for (int b = i + 1; b <= 7; b++)
        {
            intendedRow = placeholderRow + 1;
            placeholderRow++;
            if (placeholderRow > 7)
            {
                break;
            }
            if (Q[b] == intendedRow)
            {
                conflictNumber++;
            }
        }
        // check diagonal going bottom right
        placeholderRow = row;
        for (int b = i + 1; b <= 7; b++)
        {
            intendedRow = placeholderRow - 1;
            placeholderRow--;
            if (placeholderRow < 0)
            {
                break;
            }
            if (Q[b] == intendedRow)
            {
                conflictNumber++;
            }
        }
        // check diagonal going bottom left
        placeholderRow = row;
        for (int b = i - 1; b >= 0; b--)
        {
            intendedRow = placeholderRow - 1;
            placeholderRow--;
            if (placeholderRow < 0)
            {
                break;
            }
            if (Q[b] == intendedRow)
            {
                conflictNumber++;
            }
        }
    }
    return conflictNumber;
}

void nextPosition(int Q[], int nextQ[])
{
    for (int i = 0; i < 8; i++)
    {
        nextQ[i] = Q[i];
    }
    int existingRow = 0;
    int min = 0;
    int max = 7;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(min, max);
    int randomValueCol = distrib(gen);
    int randomValueRow = distrib(gen);
    // choose a random column and row
    // don't choose the same row as it alreadly exists in
    existingRow = Q[randomValueCol];
    if (randomValueRow == existingRow)
    {
        while (randomValueRow == existingRow)
        {
            randomValueRow = distrib(gen);
        }
    }
    nextQ[randomValueCol] = randomValueRow;
}

bool acceptNext(double dE, double T)
{
    // dE should be negative
    double probability = exp(dE / T);
    double measure = ((double)rand() / (RAND_MAX));
    if (measure <= probability)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool solveSimulatedAnnealing(int Sol[], double T0, int maxSteps)
{
    int count = 0;
    double tempDecrease = T0 / maxSteps;
    int tempArray[8];
    int NewE = 0;
    int OldE = 0;
    int E = 0;
    for (double Temp = T0; Temp > 0; Temp = Temp - tempDecrease)
    {
        // count is to see how many times it occurs
        count++;
        // getting the next randomly selected successor
        nextPosition(Sol, tempArray);
        // comparing values
        NewE = getAttackScore(tempArray);
        // checking if its the solution
        if (NewE == 0)
        {
            for (int i = 0; i < 8; i++)
            {
                Sol[i] = tempArray[i];
            }
            return true;
        }
        OldE = getAttackScore(Sol);
        E = OldE - NewE;
        if (E > 0)
        {
            for (int i = 0; i < 8; i++)
            {
                Sol[i] = tempArray[i];
            }
        }
        else
        {
            if (acceptNext(E, Temp))
            {
                for (int i = 0; i < 8; i++)
                {
                    Sol[i] = tempArray[i];
                }
            }
        }
    }
    return false;
}

bool solveHillClimbingFC(int Sol[], int maxSteps)
{
    int tempArray[8];
    int NewE = 0;
    int OldE = 0;
    int E = 0;
    for (int i = maxSteps; i > 0; i--)
    {
        // getting the next randomly selected successor
        nextPosition(Sol, tempArray);
        // comparing values
        NewE = getAttackScore(tempArray);
        // checking if its the solution
        if (NewE == 0)
        {
            for (int i = 0; i < 8; i++)
            {
                Sol[i] = tempArray[i];
            }
            return true;
        }
        OldE = getAttackScore(Sol);
        E = OldE - NewE;
        if (E > 0)
        {
            for (int i = 0; i < 8; i++)
            {
                Sol[i] = tempArray[i];
            }
        }
    }
    return false;
}

//---------------------------------------------------------------
// Helpers for main
//---------------------------------------------------------------
void randomState(int Q[])
{
    uniform_int_distribution<> d(0, 7);
    for (int i = 0; i < 8; i++)
        Q[i] = d(gen);
}

void printBoard(int Q[])
{
    for (int r = 7; r >= 0; r--)
    {
        for (int c = 0; c < 8; c++)
            cout << (Q[c] == r ? "Q " : ". ");
        cout << endl;
    }
}

int main()
{
    //version showing the steps 
    // One random starting state, shared by both algorithms
    int start[8], Q[8];
    randomState(start);

    cout << "Starting state (score " << getAttackScore(start) << "):" << endl;
    printBoard(start);

    // Simulated annealing
    for (int i = 0; i < 8; i++) Q[i] = start[i];
    bool ok = solveSimulatedAnnealing(Q, 4, 5000);
    cout << "\nSimulated annealing: " << (ok ? "solved" : "not solved")
         << ", final score " << getAttackScore(Q) << endl;
    printBoard(Q);

    // First-choice hill climbing
    for (int i = 0; i < 8; i++) Q[i] = start[i];
    ok = solveHillClimbingFC(Q, 5000);
    cout << "\nFirst-choice hill climbing: " << (ok ? "solved" : "not solved")
         << ", final score " << getAttackScore(Q) << endl;
    printBoard(Q);

    //multiple trials to show overall probability
    const int TRIALS = 1000;
    const int stepCounts[2] = {1000, 5000};
    double rate[2][2]; // [algorithm][step count]

    cout << "Running " << TRIALS << " trials per setting..." << endl;
    for (int s = 0; s < 2; s++)
    {
        int saWins = 0, hcWins = 0;
        for (int t = 0; t < TRIALS; t++)
        {
            int init[8], a[8], b[8];
            randomState(init);                       // same start for both
            for (int i = 0; i < 8; i++) { a[i] = init[i]; b[i] = init[i]; }

            if (solveSimulatedAnnealing(a, 4, stepCounts[s])) saWins++;
            if (solveHillClimbingFC(b, stepCounts[s])) hcWins++;
        }
        rate[0][s] = 100.0 * saWins / TRIALS;
        rate[1][s] = 100.0 * hcWins / TRIALS;
    }

    cout << fixed << setprecision(1);
    cout << "\n" << setw(30) << "Success Rate" << endl;
    cout << setw(30) << "" << setw(10) << "N=1000" << setw(10) << "N=5000" << endl;
    cout << setw(30) << left << "Simulated Annealing" << right
         << setw(9) << rate[0][0] << "%" << setw(9) << rate[0][1] << "%" << endl;
    cout << setw(30) << left << "First Choice Hill Climbing" << right
         << setw(9) << rate[1][0] << "%" << setw(9) << rate[1][1] << "%" << endl;
    return 0;
}