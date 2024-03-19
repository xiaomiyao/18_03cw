#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <stdexcept>

using namespace std;

std::vector<std::vector<char>> generateMaze(int size);

int main() {
    try {
        int size;
        std::cout << "Enter maze size (between 5 and 60): ";
        std::cin >> size;

        std::vector<std::vector<char>> maze = generateMaze(size); // Генерация лабиринта

        // Вывод сгенерированного лабиринта
        for (const auto& row : maze) {
            for (char cell : row) {
                std::cout << cell << " "; 
            }
            std::cout << std::endl;
        }
    } catch (const std::invalid_argument& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "An unexpected error occurred." << std::endl;
        return 1;
    }

    return 0;
}

// Функция для генерации лабиринта
std::vector<std::vector<char>> generateMaze(int size) {
    if (size < 5 || size > 60) {
        throw std::invalid_argument("Invalid maze size! Please enter a size between 5 and 60.");
    }

    std::vector<std::vector<char>> maze(size, std::vector<char>(size, '#')); // Инициализация лабиринта символами '#'

    srand(time(nullptr)); // Инициализация генератора случайных чисел
    for (int i = 1; i < size - 1; ++i) {
        for (int j = 1; j < size - 1; ++j) {
            if (rand() % 4 == 0) { // С вероятностью 1/4 ячейка становится стеной
                maze[i][j] = '#';
            } else {
                maze[i][j] = ' ';
            }
        }
    }

    // Добавление границ лабиринта
    for (int i = 0; i < size; ++i) {
        maze[i][0] = '#';
        maze[i][size - 1] = '#';
        maze[0][i] = '#';
        maze[size - 1][i] = '#';
    }

    return maze;
}
