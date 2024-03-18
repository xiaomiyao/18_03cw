// 18_03cw.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <cstdlib> // For srand() and rand()
#include <ctime>   // For time()

// Define symbols for different maze elements
const char WALL = 'X';
const char PLAYER = 'P';
const char TREASURE = 'T';
const char EMPTY_SPACE = '.';

class Game {
public:
    Game(int rows, int cols) : rows(rows), cols(cols), maze(rows, std::vector<char>(cols, EMPTY_SPACE)) {
        srand(time(0)); // Seed for random number generation
        placeWalls();
        placePlayer();
        placeTreasure();
    }

    void displayMaze() {
        for (const auto& row : maze) {
            for (char cell : row) {
                std::cout << cell << " ";
            }
            std::cout << std::endl;
        }
    }

private:
    int rows, cols;
    std::vector<std::vector<char>> maze;
    std::pair<int, int> playerPosition;

    void placeWalls() {
        for (int i = 0; i < rows; ++i) {
            maze[i][0] = WALL;
            maze[i][cols - 1] = WALL;
        }
        for (int j = 0; j < cols; ++j) {
            maze[0][j] = WALL;
            maze[rows - 1][j] = WALL;
        }
    }

    void placePlayer() {
        // Place player at a fixed position for now
        playerPosition = { 1, 1 }; // You can make it random or user-defined
        maze[playerPosition.first][playerPosition.second] = PLAYER;
    }

    void placeTreasure() {
        int treasureRow, treasureCol;
        do {
            treasureRow = rand() % rows;
            treasureCol = rand() % cols;
        } while (maze[treasureRow][treasureCol] != EMPTY_SPACE);

        maze[treasureRow][treasureCol] = TREASURE;
    }
};

int main() {
    Game game(10, 10); // Create a game with a 10x10 maze
    game.displayMaze();

    return 0;
}
