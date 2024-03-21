#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <stdexcept>
#include <limits>

void generateMaze(std::vector<std::vector<char>>& maze, int size); // Генерация стен лабиринта
void placePlayer(std::vector<std::vector<char>>& maze, int size, int& playerRow, int& playerCol); // Случайное размещение игрока внутри лабиринта
void placeTreasure(std::vector<std::vector<char>>& maze, int size, int& treasureRow, int& treasureCol);
void printMaze(const std::vector<std::vector<char>>& maze); // Вывод лабиринта на экран
bool makeMove(char move, std::vector<std::vector<char>>& maze, int size, int& playerRow, int& playerCol, int treasureRow, int treasureCol, bool& gameOver); // Движение внутри лабиринта
bool isValidMove(int row, int col, int size, const std::vector<std::vector<char>>& maze); // Проверка допустимости хода в лабиринте
void placeTraps(std::vector<std::vector<char>>& maze, int size);

int main() {
    srand(time(nullptr)); // Initialize random number generator.

    while (true) {
        try { // Start of try block to catch exceptions.
            int size;
            std::cout << "Enter maze size (between 15 and 50): ";
            std::cin >> size;

            // Check if input is a valid integer and within the desired range.
            if (std::cin.fail() || size < 15 || size > 50) {
                std::cin.clear(); // Clear error flag on cin.
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore the rest of the line.
                throw std::out_of_range("Invalid size! Please enter a size between 15 and 50."); // Throw an exception if input is invalid.
            }

            std::vector<std::vector<char>> maze(size, std::vector<char>(size, 'x')); // Initialize maze with 'x'.
            generateMaze(maze, size); // Generate the maze.

            int playerRow, playerCol, treasureRow, treasureCol;
            placePlayer(maze, size, playerRow, playerCol); // Place the player in the maze.
            placeTreasure(maze, size, treasureRow, treasureCol); // Place the treasure in the maze.

            placeTraps(maze, size);

            printMaze(maze); // Print the maze.
            bool gameOver = false;
            while (!gameOver) {
                char move;
                std::cout << "Enter move (w - up, s - down, a - left, d - right, backspace - take treasure): ";
                std::cin >> move; // Player inputs their move.

                if (!makeMove(move, maze, size, playerRow, playerCol, treasureRow, treasureCol, gameOver)) {
                    std::cout << "Invalid move! Try again." << std::endl; // In case of an invalid move.
                    continue;
                }

                if (playerRow == treasureRow && playerCol == treasureCol) { // Check if the player has found the treasure.
                    std::cout << "Congratulations! You found the treasure!" << std::endl; // Congratulate the player.
                    break;
                }

                printMaze(maze); // Print the updated maze.
            }
        }
        catch (const std::exception& e) { // Catch exceptions.
            std::cout << "Error: " << e.what() << std::endl; // Print the error message.
            continue; // Continue to the next iteration of the loop.
        }

        char playAgain;
        std::cout << "Do you want to play again? (y/n): ";
        std::cin >> playAgain;
        if (playAgain != 'y' && playAgain != 'Y') {
            break; // Exit the loop if the player does not want to play again.
        }
    }

    return 0;
}

void generateMaze(std::vector<std::vector<char>>& maze, int size) {
    for (int i = 1; i < size - 1; ++i) {
        for (int j = 1; j < size - 1; ++j) {
            if (rand() % 4 == 0) { // С вероятностью 1/4 ячейка становится стеной
                maze[i][j] = 'x';
            } else {
                maze[i][j] = ' ';
            }
        }
    }

    // Добавление границ лабиринта
    for (int i = 0; i < size; ++i) {
        maze[i][0] = '|';
        maze[i][size - 1] = '|';
        maze[0][i] = '-';
        maze[size - 1][i] = '-';
    }


}

void placePlayer(std::vector<std::vector<char>>& maze, int size, int& playerRow, int& playerCol) {
    playerRow = rand() % (size - 2) + 1; // Случайное размещение игрока внутри лабиринта
    playerCol = rand() % (size - 2) + 1;
    maze[playerRow][playerCol] = '?'; // Помещение символа игрока в лабиринт
}

void placeTreasure(std::vector<std::vector<char>>& maze, int size, int& treasureRow, int& treasureCol) {
    do {
        treasureRow = rand() % (size - 2) + 1; // Случайное размещение сокровища внутри лабиринта
        treasureCol = rand() % (size - 2) + 1;
    } while (maze[treasureRow][treasureCol] == 'x'); // Проверка, чтобы сокровище не попало на стену
    maze[treasureRow][treasureCol] = '$'; // Помещение символа сокровища в лабиринт
}
void placeTraps(std::vector<std::vector<char>>& maze, int size) {
    //randomize x coordinate
    int trapRow = rand() % (size - 2) + 1;
    //rand y coordinate
    int trapCol = rand() % (size - 2) + 1;
    //check if coordinate is empty
    if (maze[trapRow][trapCol]!='x' && maze[trapRow][trapCol] != '?' && maze[trapRow][trapCol] != '$') {
        maze[trapRow][trapCol] = '*';
        return;
    }
    else {
        placeTraps(maze, size);
        std::cout << "returned from recursion";
    }
    //if yes  place trap and return 
    //if not repeat
}

void printMaze(const std::vector<std::vector<char>>& maze) {
    for (const auto& row : maze) {
        for (char cell : row) {
            std::cout << cell << " "; // Вывод лабиринта на экран
        }
        std::cout << std::endl;
    }
}

bool makeMove(char move, std::vector<std::vector<char>>& maze, int size, int& playerRow, int& playerCol, int treasureRow, int treasureCol, bool& gameOver) {
    int newRow = playerRow;
    int newCol = playerCol;

    switch (move) {
        case 'w':
            newRow--; // Перемещение игрока вверх
            break;
        case 's':
            newRow++; // Перемещение игрока вниз
            break;
        case 'a':
            newCol--; // Перемещение игрока влево
            break;
        case 'd':
            newCol++; // Перемещение игрока вправо
            break;
        case '\b':
            if (playerRow == treasureRow && playerCol == treasureCol) {
                return true; // Взятие сокровища
            }
        default:
            return false; // Некорректный ход
    }

    // Проверка, является ли новая позиция игрока допустимой
    if (isValidMove(newRow, newCol, size, maze)) {
        maze[playerRow][playerCol] = ' '; // Очистка предыдущей позиции игрока
        playerRow = newRow; // Обновление координат игрока
        playerCol = newCol; 
        if (maze[playerRow][playerCol] == '*') {
            gameOver = true;
            std::cout << "You walked into a trap and died. " << std::endl;
        }


        maze[playerRow][playerCol] = '?'; // Обновление новой позиции игрока
        return true; // Ход выполнен успешно
    }

    return false; // Недопустимый ход (попытка пройти через границу или стену)
}

bool isValidMove(int row, int col, int size, const std::vector<std::vector<char>>& maze) {
    return row >= 0 && row < size && col >= 0 && col < size && maze[row][col] != 'x' && maze[row][col] != '-' && maze[row][col] != '|';
}
