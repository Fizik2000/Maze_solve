#include "cave.h"

namespace s21 {

// Загрузка лабиринта
void Cave::load_cave(const std::string &filename) {
  std::ifstream file{filename};
  check_file_date(file); // Проверка корректности данных
  file.clear();          // Очищает состояние потока
  file.seekg(0, std::ios::beg); // Возвращает указатель чтения в начало файла

  file >> rows >> cols; // Чтение размеров

  // Выделение памяти под полученные размеры
  cave.clear();
  cave.resize(rows, CaveVector_t(cols, false));

  // Чтение вертикальных стен
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      int tmp_value;
      file >> tmp_value;
      cave[i][j] = static_cast<bool>(tmp_value);
    }
  }

  file.close();
}

// Проверка данных в файле
void Cave::check_file_date(std::istream &file) {
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

  // Проверка данных
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      // Данные только 0 или 1
      if (!(file >> value) || (value != 0 && value != 1)) {
        throw std::runtime_error(
            "Invalid vertical wall file data format in a row " +
            std::to_string(i) + ", col " + std::to_string(j));
      }
    }
  }

  // Проверка лишних данных после основных
  if (file >> more_date) {
    throw std::runtime_error("Invalid file format: unexpected additional data");
  }
}

// Запись данных в файл
void Cave::save_cave(const std::string &filename) {
  std::ofstream file{filename};

  // Запись размеров
  file << rows << " " << cols << "\n";

  // Запись вертикальных стен
  for (const auto &row : cave) {
    for (size_t col = 0; col < row.size(); col++) {
      file << static_cast<int>(row[col]);
      if (col < row.size() - 1) {
        file << " ";
      }
    }
    file << "\n";
  }
  file.close();
}

// Создание стартовой матрица для обработки
void Cave::generate_cave(int new_row, int new_col, int new_init) {
  // Проверка корректности введенных строк и столбцов
  if (new_row < 2 || new_row > 50 || new_col < 2 || new_col > 50) {
    throw std::runtime_error(
        "Invalid file format: rows and cols must be between 2 and 50");
  }
  // Проверка корректности введенной начальной инициализации
  if (new_init < 0 || new_init > 100) {
    throw std::runtime_error(
        "Invalid file format: change init must be between 0 and 100");
  }

  // Запись размеров и выделение памяти
  rows = new_row;
  cols = new_col;
  chance_init = new_init;
  cave.clear();
  cave.resize(rows, CaveVector_t(cols, false));

  // Заполнение матрица bool значениями
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      cave[i][j] = random_bool_value();
    }
  }
}

// Случайное булевое значение с шансом инициализации
bool Cave::random_bool_value() {
  static std::random_device rand;
  static std::mt19937 gen(rand());
  std::uniform_int_distribution<int> dist(1, 100);

  return dist(gen) <= chance_init;
}

// Клеточный автомат
int Cave::cellular_automaton(int life, int dead) {
  // Проверка
  if (life < 0 || life > 7) {
    throw std::runtime_error(
        "Invalid file format: birth limit must be between 0 and 7");
  }
  // Проверка
  if (dead < 0 || dead > 7) {
    throw std::runtime_error(
        "Invalid file format: death limit must be between 0 and 7");
  }
  // Записываем пределы смерти и рождения
  birth_limit = life;
  death_limit = dead;

  int change_count = 0;  // счетчик изменений
  CaveMatrix_t tmp_cave; // Временная матрица
  tmp_cave.clear();
  tmp_cave.resize(rows, CaveVector_t(cols, false));

  // Клеточный автомат
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      int live_neighbors = count_neighbors(i, j);
      bool old_state = cave[i][j];
      bool new_state;

      if (old_state) {
        new_state = (live_neighbors > death_limit);
      } else {
        new_state = (live_neighbors >= birth_limit);
      }

      // Проверка, было ли изменение
      if (old_state != new_state) {
        change_count++;
      }

      tmp_cave[i][j] = new_state;
    }
  }

  // Обновляем матрицу
  cave = tmp_cave;

  // Возвращаем сколько клеток поменяли
  return change_count;
}

// Функция для подсчета соседей
int Cave::count_neighbors(int row, int col) {
  int count = 0;
  for (int i = -1; i <= 1; i++) {
    for (int j = -1; j <= 1; j++) {
      if (i == 0 && j == 0)
        continue; // Пропускаем саму клетку
      int neighbor_row = row + i;
      int neighbor_col = col + j;

      // Проверка границ
      if (neighbor_row >= 0 && neighbor_row < rows && neighbor_col >= 0 &&
          neighbor_col < cols) {
        count += cave[neighbor_row][neighbor_col];
      }
    }
  }
  return count;
}

} // namespace s21
