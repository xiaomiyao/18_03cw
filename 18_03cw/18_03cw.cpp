#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

void generateMaze(std::vector<std::vector<char>>& maze, int size); // Генерация стен лабиринта
void placePlayer(std::vector<std::vector<char>>& maze, int size, int& playerRow, int& playerCol); // Случайное размещение игрока внутри лабиринта
void placeTreasure(std::vector<std::vector<char>>& maze, int size, int& treasureRow, int& treasureCol); // Случайное размещение сокровища внутри лабиринта
void printMaze(const std::vector<std::vector<char>>& maze); // Вывод лабиринта на экран
bool makeMove(char move, std::vector<std::vector<char>>& maze, int size, int& playerRow, int& playerCol, int treasureRow, int treasureCol); // Движение внутри лабиринта
bool isValidMove(int row, int col, int size, const std::vector<std::vector<char>>& maze); // Проверка допустимости хода в лабиринте

int main() {
    srand(time(nullptr)); // Инициализация генератора случайных чисел

    while (true) {
        int size;
        std::cout << "Enter maze size (between 15 and 50): ";
        std::cin >> size;

        if (size < 15 || size > 50) {
            std::cout << "Invalid size! Please enter a size between 15 and 50." << std::endl; // Проверка корректности размера лабиринта
            continue;
        }

        std::vector<std::vector<char>> maze(size, std::vector<char>(size, 'x')); // Инициализация лабиринта символами 'x'
        generateMaze(maze, size); // Генерация лабиринта

        // Добавление границ лабиринта
        for (int i = 0; i < size; ++i) {
            maze[i][0] = '|';
            maze[i][size - 1] = '|';
            maze[0][i] = '-';
            maze[size - 1][i] = '-';
        }

        int playerRow, playerCol, treasureRow, treasureCol;
        placePlayer(maze, size, playerRow, playerCol); // Размещение игрока
        placeTreasure(maze, size, treasureRow, treasureCol); // Размещение сокровища

        printMaze(maze); // Вывод лабиринта на экран

        while (true) {
            char move;
            std::cout << "Enter move (w - up, s - down, a - left, d - right, backspace - take treasure): ";
            std::cin >> move; // Ввод игроком действия
            if (!makeMove(move, maze, size, playerRow, playerCol, treasureRow, treasureCol)) { // Проверка и выполнение действия игрока
                std::cout << "Invalid move! Try again." << std::endl; // В случае некорректного действия выводится сообщение об ошибке
                continue;
            }
            if (playerRow == treasureRow && playerCol == treasureCol) { // Проверка, достиг ли игрок сокровища
                std::cout << "Congratulations! You found the treasure!" << std::endl; // В случае нахождения сокровища выводится сообщение о победе
                break;
            }
            printMaze(maze); // Вывод обновленного лабиринта на экран
        }

        char playAgain;
        std::cout << "Do you want to play again? (y/n): ";
        std::cin >> playAgain;
        if (playAgain != 'y' && playAgain != 'Y') {
            break; // Выход из основного цикла, если пользователь не хочет играть снова
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

void printMaze(const std::vector<std::vector<char>>& maze) {
    for (const auto& row : maze) {
        for (char cell : row) {
            std::cout << cell << " "; // Вывод лабиринта на экран
        }
        std::cout << std::endl;
    }
}

bool makeMove(char move, std::vector<std::vector<char>>& maze, int size, int& playerRow, int& playerCol, int treasureRow, int treasureCol) {
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
        maze[playerRow][playerCol] = '?'; // Обновление новой позиции игрока
        return true; // Ход выполнен успешно
    }

    return false; // Недопустимый ход (попытка пройти через границу или стену)
}

bool isValidMove(int row, int col, int size, const std::vector<std::vector<char>>& maze) {
    return row >= 0 && row < size && col >= 0 && col < size && maze[row][col] != 'x' && maze[row][col] != '-' && maze[row][col] != '|';
}
