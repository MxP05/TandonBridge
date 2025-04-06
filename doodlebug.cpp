#include <iostream>
#include <cstdlib>
#include <ctime>

const int GRID_SIZE = 20;
const int DOODLEBUGS = 5;
const int ANTS = 100;

// the types of creatures
enum CreatureType
{
    EMPTY = 0,
    ANT = 1,
    DOODLEBUG = 2
};

class World
{
private:
    //  main grid storing creature types
    int grid[GRID_SIZE][GRID_SIZE];

    //  breeding and starving
    int breedTime[GRID_SIZE][GRID_SIZE];
    int starveTime[GRID_SIZE][GRID_SIZE];

public:
    World()
    {
        // Initialize empty grid
        for (int i = 0; i < GRID_SIZE; i++)
        {
            for (int j = 0; j < GRID_SIZE; j++)
            {
                grid[i][j] = EMPTY;
                breedTime[i][j] = 0;
                starveTime[i][j] = 0;
            }
        }
    }

    void initialize()
    {
        srand(time(0)); // Seed random number generator

        // Place doodlebugs
        int doodlebugsPlaced = 0;
        while (doodlebugsPlaced < DOODLEBUGS)
        {
            int x = rand() % GRID_SIZE;
            int y = rand() % GRID_SIZE;
            if (grid[x][y] == EMPTY)
            {
                grid[x][y] = DOODLEBUG;
                doodlebugsPlaced++;
            }
        }

        // Place ants
        int antsPlaced = 0;
        while (antsPlaced < ANTS)
        {
            int x = rand() % GRID_SIZE;
            int y = rand() % GRID_SIZE;
            if (grid[x][y] == EMPTY)
            {
                grid[x][y] = ANT;
                antsPlaced++;
            }
        }
    }

    void moveCreature(int x, int y)
    {
        int moves[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}; // Up, Down, Left, Right

        // If it's a doodlebug
        if (grid[x][y] == DOODLEBUG)
        {
            // Try to eat nearby ants first
            for (int i = 0; i < 4; i++)
            {
                int newX = x + moves[i][0];
                int newY = y + moves[i][1];

                if (newX >= 0 && newX < GRID_SIZE && newY >= 0 && newY < GRID_SIZE)
                {
                    if (grid[newX][newY] == ANT)
                    {
                        grid[newX][newY] = DOODLEBUG;
                        grid[x][y] = EMPTY;
                        starveTime[newX][newY] = 0;
                        breedTime[newX][newY] = breedTime[x][y] + 1;
                        return;
                    }
                }
            }

            // If no ants nearby, try to move to empty space
            int direction = rand() % 4;
            int newX = x + moves[direction][0];
            int newY = y + moves[direction][1];

            if (newX >= 0 && newX < GRID_SIZE && newY >= 0 && newY < GRID_SIZE)
            {
                if (grid[newX][newY] == EMPTY)
                {
                    grid[newX][newY] = DOODLEBUG;
                    grid[x][y] = EMPTY;
                    starveTime[newX][newY] = starveTime[x][y] + 1;
                    breedTime[newX][newY] = breedTime[x][y] + 1;
                }
            }
        }
        // If it's an ant
        else if (grid[x][y] == ANT)
        {
            int direction = rand() % 4;
            int newX = x + moves[direction][0];
            int newY = y + moves[direction][1];

            if (newX >= 0 && newX < GRID_SIZE && newY >= 0 && newY < GRID_SIZE)
            {
                if (grid[newX][newY] == EMPTY)
                {
                    grid[newX][newY] = ANT;
                    grid[x][y] = EMPTY;
                    breedTime[newX][newY] = breedTime[x][y] + 1;
                }
            }
        }
    }

    void timeStep()
    {
        // Move doodlebugs first
        for (int i = 0; i < GRID_SIZE; i++)
        {
            for (int j = 0; j < GRID_SIZE; j++)
            {
                if (grid[i][j] == DOODLEBUG)
                {
                    moveCreature(i, j);
                }
            }
        }

        // Then move ants
        for (int i = 0; i < GRID_SIZE; i++)
        {
            for (int j = 0; j < GRID_SIZE; j++)
            {
                if (grid[i][j] == ANT)
                {
                    moveCreature(i, j);
                }
            }
        }

        // Breeding and starving
        for (int i = 0; i < GRID_SIZE; i++)
        {
            for (int j = 0; j < GRID_SIZE; j++)
            {
                // Doodlebug breeding
                if (grid[i][j] == DOODLEBUG && breedTime[i][j] >= 8)
                {
                    // Try to breed in adjacent cell
                    int direction = rand() % 4;
                    int newX = i + (direction == 0 ? -1 : direction == 1 ? 1
                                                                         : 0);
                    int newY = j + (direction == 2 ? -1 : direction == 3 ? 1
                                                                         : 0);

                    if (newX >= 0 && newX < GRID_SIZE && newY >= 0 && newY < GRID_SIZE)
                    {
                        if (grid[newX][newY] == EMPTY)
                        {
                            grid[newX][newY] = DOODLEBUG;
                            breedTime[i][j] = 0;
                        }
                    }
                }

                // Ant breeding
                if (grid[i][j] == ANT && breedTime[i][j] >= 3)
                {
                    // Try to breed in adjacent cell
                    int direction = rand() % 4;
                    int newX = i + (direction == 0 ? -1 : direction == 1 ? 1
                                                                         : 0);
                    int newY = j + (direction == 2 ? -1 : direction == 3 ? 1
                                                                         : 0);

                    if (newX >= 0 && newX < GRID_SIZE && newY >= 0 && newY < GRID_SIZE)
                    {
                        if (grid[newX][newY] == EMPTY)
                        {
                            grid[newX][newY] = ANT;
                            breedTime[i][j] = 0;
                        }
                    }
                }

                // Doodlebug starving
                if (grid[i][j] == DOODLEBUG && starveTime[i][j] >= 3)
                {
                    grid[i][j] = EMPTY;
                    starveTime[i][j] = 0;
                    breedTime[i][j] = 0;
                }
            }
        }
    }

    void display()
    {
        for (int i = 0; i < GRID_SIZE; i++)
        {
            for (int j = 0; j < GRID_SIZE; j++)
            {
                if (grid[i][j] == EMPTY)
                    std::cout << "-";
                else if (grid[i][j] == ANT)
                    std::cout << "o";
                else
                    std::cout << "X";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
};

int main()
{
    World world;
    world.initialize();

    int timeStep = 0;
    while (true)
    {
        std::cout << "World at time: " << timeStep << std::endl;
        world.display();
        std::cout << "Press Enter for next step..." << std::endl;
        std::cin.get();
        world.timeStep();
        timeStep++;
    }

    return 0;
}
