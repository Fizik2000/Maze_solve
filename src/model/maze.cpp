#include "maze.h"

namespace s21 {

// Проверка данных в файле
void Maze::check_file_date(std::istream &file) {
  int rows{}, cols{}, value{}, more_date{};

  // Чтение размеров
  if (!(file >> rows >> cols)) {
    throw std::runtime_error("Invalid file format: cannot read size");
  }

  // Проверка строк и столбцов
  if (rows < 2 || rows > 50 || cols < 2 || cols > 50) {
    throw std::runtime_error(
        "Invalid file format: rows and cols must be between 2 and 50");
  }

  // Проверка вертикальных стен
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      // Данные только 0 или 1
      if (!(file >> value) || (value != 0 && value != 1)) {
        throw std::runtime_error(
            "Invalid vertical wall file data format in a row " +
            std::to_string(i) + ", col " + std::to_string(j));
      }
      // Вконце должна быть стена
      if (j == cols - 1 && value == 0) {
        throw std::runtime_error("Invalid file format: vertical walls");
      }
    }
  }

  // Проверка горизонтальных стен
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      // Данные только 0 или 1
      if (!(file >> value) || (value != 0 && value != 1)) {
        throw std::runtime_error(
            "Invalid horizontal wall file data format in a row " +
            std::to_string(i) + ", col " + std::to_string(j));
      }
      // Вконце должна быть стена
      if (i == rows - 1 && value != 1) {
        throw std::runtime_error("Invalid file format: horizontal walls");
      }
    }
  }

  // Проверка лишних данных после основных
  if (file >> more_date) {
    throw std::runtime_error("Invalid file format: unexpected additional data");
  }
}

// Загрузка данных из файла
void Maze::load_maze(const std::string &filename) {
  std::ifstream file{filename};
  check_file_date(file);
  file.clear(); // Очищает состояние потока
  file.seekg(0, std::ios::beg); // Возвращает указатель чтения в начало файла

  // Чтение размеров
  file >> rows >> cols;

  vertical_walls.clear();
  horizontal_walls.clear();

  // Выделение памяти под полученные размеры
  vertical_walls.resize(rows, WallVector_t(cols, false));
  horizontal_walls.resize(rows, WallVector_t(cols, false));

  // Чтение вертикальных стен
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      int tmp_wall_value;
      file >> tmp_wall_value;
      vertical_walls[i][j] = static_cast<bool>(tmp_wall_value);
    }
  }

  // Чтение горизонтальных стен
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      int tmp_wall_value;
      file >> tmp_wall_value;
      horizontal_walls[i][j] = static_cast<bool>(tmp_wall_value);
    }
  }
  file.close();
  clear_path();
}

// Запись данных в файл
void Maze::save_maze(const std::string &filename) {
  std::ofstream file{filename};

  // Запись размеров
  file << rows << " " << cols << "\n";

  // Запись вертикальных стен
  for (const auto &row_v : vertical_walls) {
    for (size_t col_v = 0; col_v < row_v.size(); col_v++) {
      file << static_cast<int>(row_v[col_v]);
      if (col_v < row_v.size() - 1) {
        file << " ";
      }
    }
    file << "\n";
  }
  file << "\n";

  // Запись горизонтальныз стен
  for (const auto &row_v : horizontal_walls) {
    for (size_t col_v = 0; col_v < row_v.size(); col_v++) {
      file << static_cast<int>(row_v[col_v]);
      if (col_v < row_v.size() - 1) {
        file << " ";
      }
    }
    file << "\n";
  }
  file.close();
}

// Нахождение пути между двумя точками
void Maze::solve_maze(Point_t start_point, Point_t end_point) {
  // Проверка выхода за границы для start_point
  if (start_point.first < 0 || start_point.first >= rows ||
      start_point.second < 0 || start_point.second >= cols) {
    throw std::runtime_error("Start point is outside maze boundaries");
  }
  // Проверка выхода за границы для end_point
  if (end_point.first < 0 || end_point.first >= rows || end_point.second < 0 ||
      end_point.second >= cols) {
    throw std::runtime_error("End point is outside maze boundaries");
  }

  clear_path();
  start = start_point;
  end = end_point;
  visited_point.clear();
  visited_point.resize(rows, WallVector_t(cols, false));

  // Метод DFS
  if (!depth_first_search(start.first, start.second)) {
    throw std::runtime_error("No solution found for the maze.");
  }
}

// Очистка пути
void Maze::clear_path() { found_path.clear(); }

// Генерация лабиринта
void Maze::generate_maze(int new_row, int new_col) {
  if (new_row < 2 || new_row > 50 || new_col < 2 || new_col > 50) {
    throw std::runtime_error(
        "Invalid file format: rows and cols must be between 2 and 50");
  }

  rows = new_row;
  cols = new_col;

  // Очищаем и выделяем память
  vertical_walls.clear();
  horizontal_walls.clear();
  vertical_walls.resize(rows, WallVector_t(cols, false));
  horizontal_walls.resize(rows, WallVector_t(cols, false));

  // Правильная инициализация string_values
  string_values.resize(cols, 0);
  value_set = 1;

  for (int i = 0; i < rows - 1; i++) {
    union_sets();
    set_vertical_walls(i);
    set_horizontal_walls(i);
    set_zero_for_wall(i);
  }

  // Генерация последней строки - исправленная логика
  union_sets();
  set_vertical_walls(rows - 1);

  // Объединяем все множества в последней строке
  for (int cur_col = 0; cur_col < cols - 1; cur_col++) {
    if (string_values[cur_col] != string_values[cur_col + 1]) {
      vertical_walls[rows - 1][cur_col] = false;
      merge_set(cur_col);
    }
  }

  // Все горизонтальные стены в последней строке должны быть true
  for (int cur_col = 0; cur_col < cols; cur_col++) {
    horizontal_walls[rows - 1][cur_col] = true;
  }

  string_values.clear();
  value_set = 1;
  clear_path();
}

// Присвоение множества
void Maze::union_sets() {
  for (int i = 0; i < cols; i++) {
    if (string_values[i] == 0) {
      string_values[i] = value_set;
      value_set++;
    }
  }
}

// Установка вертикальных стен
void Maze::set_vertical_walls(int current_row) {
  bool choise;
  // Цикл по строке
  for (int current_col = 0; current_col < cols - 1; current_col++) {
    choise = random_bool_value();
    // Если рандом или одинаковое множество
    if (choise ||
        string_values[current_col] == string_values[current_col + 1]) {
      // то ставим стенку вертикальную
      vertical_walls[current_row][current_col] = true;
    } else {
      // если нет, то слияние множеств
      merge_set(current_col);
    }
  }
  vertical_walls[current_row][cols - 1] = true;
}

// Случайное булевое значение (50/50)
bool Maze::random_bool_value() {
  static std::random_device rand;
  static std::mt19937 gen(rand());
  std::uniform_int_distribution<int> dist(0, 1);

  return dist(gen);
}

// Объединение множеств
void Maze::merge_set(int index) {
  int current_val = string_values[index];
  int next_val = string_values[index + 1];

  for (int current_col = 0; current_col < cols; current_col++) {
    if (string_values[current_col] == next_val) {
      string_values[current_col] = current_val;
    }
  }
}

// Установка горизонтальных стен
void Maze::set_horizontal_walls(int current_row) {
  bool choise;
  for (int current_col = 0; current_col < cols - 1; current_col++) {
    choise = random_bool_value();

    if (choise && !is_one_element_in_set(string_values[current_col])) {
      horizontal_walls[current_row][current_col] = true;
    }
  }
  check_isolation_set(current_row);
}

// Проверка сколько значений в одном множестве
bool Maze::is_one_element_in_set(int element) {
  int count{0};

  for (int current_col = 0; current_col < cols; current_col++) {
    if (string_values[current_col] == element) {
      count++;
    }
  }
  return count == 1;
}

// Проверка изоляции множества
void Maze::check_isolation_set(int row) {
  for (int current_col = 0; current_col < cols; current_col++) {
    if (count_horizontal_walls(row, string_values[current_col]) == 0) {
      horizontal_walls[row][current_col] = false;
    }
  }
}

// Подсчет горизонтальных стен
int Maze::count_horizontal_walls(int input_row, int input_element) {
  int count{0};

  for (int current_col = 0; current_col < cols; current_col++) {
    if (string_values[current_col] == input_element &&
        horizontal_walls[input_row][current_col] == false) {
      count++;
    }
  }
  return count;
}

// Установка нуля для ячейки с вертикальной стеной
void Maze::set_zero_for_wall(int input_row) {
  for (int current_col = 0; current_col < cols; current_col++) {
    if (horizontal_walls[input_row][current_col]) {
      string_values[current_col] = 0;
    }
  }
}

// Поиск пути, с помощью поиска в глубину - DFS
bool Maze::depth_first_search(int row, int col) {
  //
  if (row < 0 || row > rows || col < 0 || col > cols ||
      visited_point[row][col]) {
    return false;
  }

  // Отмечаем посещение данной точки
  visited_point[row][col] = true;

  // Проверяем достигли ли конечной точки
  if (row == end.first && col == end.second) {
    found_path.push_back({row, col});
    return true;
  }

  // Перебор всех остальных вариантов пути
  for (int i = 0; i < kNumberDirections; ++i) {
    if (is_possible_to_move(row, col, kDirections[i])) {
      if (depth_first_search(row + kRowDeltas[i], col + kColDeltas[i])) {
        found_path.push_back({row, col});
        return true;
      }
    }
  }
  return false;
}

// Проверка возможности движения по сторонам
bool Maze::is_possible_to_move(int row, int col, Direction direct) {
  bool result;
  switch (direct) {
  case Direction::kLeft:
    result = col > 0 && !vertical_walls[row][col - 1];
    break;
  case Direction::kRight:
    result = col < cols - 1 && !vertical_walls[row][col];
    break;
  case Direction::kUp:
    result = row > 0 && !horizontal_walls[row - 1][col];
    break;
  case Direction::kDown:
    result = row < rows - 1 && !horizontal_walls[row][col];
    break;
  default:
    result = false;
  }
  return result;
}

// Получение строк
int Maze::get_rows() { return rows; }

// Получение стобцов
int Maze::get_cols() { return cols; }

} // namespace s21