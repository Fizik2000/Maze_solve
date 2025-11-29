#include "control_maze.h"

namespace s21 {

// Конструктор
ControlMaze::ControlMaze() { maze = new Maze(); }

// Деконструктор
ControlMaze::~ControlMaze() { delete maze; }

// Генерация пещеры
void ControlMaze::generate(int rows, int cols) {
  maze->generate_maze(rows, cols);
}

// Загрузка из файла
void ControlMaze::loadFromFile(const std::string &filename) {
  maze->load_maze(filename);
}

// Сохранение в файл
void ControlMaze::saveToFile(const std::string &filename) {
  maze->save_maze(filename);
}

// Поиск пути
void ControlMaze::solveMaze(Maze::Point_t start, Maze::Point_t end) {
  maze->solve_maze(start, end);
}

// Получение количества строк
int ControlMaze::get_rows() { return maze->get_rows(); }

// Получение количества столбцов
int ControlMaze::get_cols() { return maze->get_cols(); }
} // namespace s21
