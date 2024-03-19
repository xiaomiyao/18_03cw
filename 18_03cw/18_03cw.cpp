
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <stdexcept>

using namespace std;
struct Enemy {
    int row;
    int col;
    bool isAlive = true;

    Enemy(int r, int c) : row(r), col(c) {}
};
std::vector<Enemy> enemies; // Holds all enemies
bool canKillEnemies = false; // Player's ability to kill enemies

void placeEnemies(std::vector<std::vector<char>>& maze, int size, std::vector<Enemy>& enemies, int numberOfEnemies) {
    while (enemies.size() < numberOfEnemies) {
        int row = rand() % (size - 2) + 1;
        int col = rand() % (size - 2) + 1;
        if (maze[row][col] == ' ') {
            enemies.emplace_back(row, col);
            maze[row][col] = 'E'; // Represent an enemy with 'E'
        }
    }
}
void moveEnemies(std::vector<std::vector<char>>& maze, std::vector<Enemy>& enemies, int playerRow, int playerCol) {
    for (auto& enemy : enemies) {
        if (!enemy.isAlive) continue;

        int oldRow = enemy.row, oldCol = enemy.col;

        // Move enemy towards the player
        if (enemy.row < playerRow) enemy.row++;
        else if (enemy.row > playerRow) enemy.row--;

        if (enemy.col < playerCol) enemy.col++;
        else if (enemy.col > playerCol) enemy.col--;

        // Check if moved into a wall or boundary, revert if so
        if (maze[enemy.row][enemy.col] == 'x' || maze[enemy.row][enemy.col] == '|' || maze[enemy.row][enemy.col] == '-') {
            enemy.row = oldRow;
            enemy.col = oldCol;
        }
        else {
            // Update the maze positions
            maze[oldRow][oldCol] = ' ';
            if (maze[enemy.row][enemy.col] != '?') { // Check if not colliding with the player
                maze[enemy.row][enemy.col] = 'E';
            }
        }
    }
}
bool checkCollisions(std::vector<std::vector<char>>& maze, std::vector<Enemy>& enemies, int playerRow, int playerCol, bool& gameRunning, bool canKillEnemies) {
    for (auto it = enemies.begin(); it != enemies.end();) {
        if (it->row == playerRow && it->col == playerCol) {
            if (canKillEnemies) {
                // Player kills the enemy
                std::cout << "You have defeated an enemy!" << std::endl;
                maze[it->row][it->col] = ' '; // Remove the enemy from the maze
                it = enemies.erase(it); // Remove the enemy from the list
            }
            else {
                // Player encounters an enemy and hasn't collected the treasure
                std::cout << "You've been caught by an enemy! Game Over." << std::endl;
                gameRunning = false; // End the game
                return true; // Collision detected
            }
        }
        else {
            ++it;
        }
    }
    return false; // No collision
}


void placeTreasure(std::vector<std::vector<char>>& maze, int size, int& treasureRow, int& treasureCol) {
    do {
        treasureRow = rand() % (size - 2) + 1; // Ensure it's within the maze bounds
        treasureCol = rand() % (size - 2) + 1;
    } while (maze[treasureRow][treasureCol] != ' '); // Ensure the cell is empty

    maze[treasureRow][treasureCol] = 'T'; // 'T' represents a treasure
}
bool collectTreasure(int& playerRow, int& playerCol, int& treasureRow, int& treasureCol, std::vector<std::vector<char>>& maze, int size, bool& canKillEnemies, bool& finalTreasureSpawned) {
    if (playerRow == treasureRow && playerCol == treasureCol) {
        canKillEnemies = true; // The player can now kill enemies
        treasuresCollected++;
        std::cout << "You've found a treasure! " << treasuresCollected << " out of " << totalTreasures << std::endl;

        if (treasuresCollected == totalTreasures && !finalTreasureSpawned) {
            placeFinalTreasure(maze, size, finalTreasureRow, finalTreasureCol);
            finalTreasureSpawned = true;
            std::cout << "The final treasure has appeared!" << std::endl;
        }

        // Remove the collected treasure from the maze
        maze[treasureRow][treasureCol] = ' ';
        // Logic to spawn a new treasure or handle game state changes
        return true;
    }
    return false;
}


int treasuresCollected = 0;
const int totalTreasures = 5; // Example total number of treasures before the final one appears
bool finalTreasureSpawned = false;
int finalTreasureRow, finalTreasureCol;

void placeFinalTreasure(std::vector<std::vector<char>>& maze, int size, int& finalTreasureRow, int& finalTreasureCol) {
    do {
        finalTreasureRow = rand() % (size - 2) + 1;
        finalTreasureCol = rand() % (size - 2) + 1;
    } while (maze[finalTreasureRow][finalTreasureCol] != ' ');

    maze[finalTreasureRow][finalTreasureCol] = 'F'; // 'F' for Final Treasure
}
bool checkWinCondition(int playerRow, int playerCol, int finalTreasureRow, int finalTreasureCol, bool finalTreasureSpawned) {
    if (finalTreasureSpawned && playerRow == finalTreasureRow && playerCol == finalTreasureCol) {
        std::cout << "Congratulations! You've collected the final treasure and won the game!" << std::endl;
        return true; // The player wins the game
    }
    return false;
}
