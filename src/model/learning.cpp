#include "learning.h"

namespace s21 {

Learning::Learning() {
  rows = -1;
  cols = -1;
  start = {-1, -1};
  end = {-1, -1};
  found_path.clear();
}

// ---------- Методы загрузки лабиринта ----------

// Загрузка данных из файла
void Learning::loadMazeFromFile(const std::string &filename) {
  std::ifstream file{filename};
  if (!file.is_open()) {
    throw std::runtime_error("Cannot open file: " + filename);
  }
  checkFileData(file);
  file.clear(); // Очищает состояние потока
  file.seekg(0, std::ios::beg); // Возвращает указатель чтения в начало файла
  // Чтение размеров
  file >> rows >> cols;

  vertical_walls.clear();
  horizontal_walls.clear();
  resetQTable();

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
  found_path.clear();
}

// Проверка данных в файле
void Learning::checkFileData(std::istream &file) {
  int rows{}, cols{}, value{}, more_date{};

  // Чтение размеров
  if (!(file >> rows >> cols)) {
    throw std::runtime_error("Invalid file format: cannot read size");
  }

  // Проверка строк и столбцов (сделать диапазон шире)
  if (rows < 2 || rows > 50 || cols < 2 || cols > 50) {
    throw std::runtime_error(
        "Invalid file format: rows and cols must be between 2 and 50");
  }

  // Проверка вертикальных стен
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      if (!(file >> value) || (value != 0 && value != 1)) {
        throw std::runtime_error("Invalid vertical wall data at row " +
                                 std::to_string(i) + ", col " +
                                 std::to_string(j));
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
      if (!(file >> value) || (value != 0 && value != 1)) {
        throw std::runtime_error("Invalid horizontal wall data at row " +
                                 std::to_string(i) + ", col " +
                                 std::to_string(j));
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

// ---------- Основные методы для обучения ----------

// Основная функция обучения
void Learning::train(int total_episodes, int max_steps_per_episode,
                     Point_t end_point) {
  if (end_point.first < 0 || end_point.first >= rows || end_point.second < 0 ||
      end_point.second >= cols) {
    throw std::runtime_error("End point out of bounds");
  }
  end = end_point;
  if (total_episodes < 1 || max_steps_per_episode < 1) {
    throw std::runtime_error("Not correct, episode > 1 or max_steps > 1");
  }

  success_rate = 0.0;
  total_reward = 0.0;
  total_step = 0;

  // Инициализация агента если еще не инициализирован
  if (!isInitialized()) {
    initializeAgent(-1, -1);
  }
  // Статистика обучения
  int successful_episodes = 0;
  // Основной цикл обучения по эпизодам
  for (int episode = 0; episode < total_episodes; ++episode) {
    // Выбор случайной стартовой позиции
    Point_t start_pos = getRandomStartPosition();
    int state = getStateFromPos(start_pos.first, start_pos.second);

    bool done = false;
    double episode_reward = .0;
    int steps = 0;

    while (steps < max_steps_per_episode && !done) {
      int action = chooseAction(state);
      auto [reward, is_terminal] = step(state, action);

      Point_t current_pos = getPosFromState(state);
      int new_row = current_pos.first + kRowDeltas[action];
      int new_col = current_pos.second + kColDeltas[action];

      int next_state = state;
      if (isValidMove(current_pos, new_row, new_col)) {
        next_state = getStateFromPos(new_row, new_col);
      }

      // ОБНОВЛЯЕМ ПЕРЕД переходом в следующее состояние
      updateQValue(state, action, reward, next_state);

      episode_reward += reward;
      steps++;

      // ЕСЛИ достигли финиша - НЕМЕДЛЕННО завершаем
      if (is_terminal) {
        done = true;
      } else {
        state = next_state; // Переходим только если не терминальное
      }
    }

    // Обновление общей статистики
    total_step += steps;
    total_reward += episode_reward;
    if (done) {
      successful_episodes++;
    }
    // Постепенное уменьшение exploration rate
    if (epsilon > 0.01) {
      epsilon *= 0.995; // Exponential decay
    }
  }
  // Финальная статистика
  success_rate =
      static_cast<double>(successful_episodes) / total_episodes * 100;
}

// Проверка инициализации Q-таблицы
bool Learning::isInitialized() const {
  // Проверяет, что Q-таблица создана и имеет правильный размер
  return !q_table.empty() &&
         q_table.size() == static_cast<size_t>(getNumStates()) &&
         !q_table[0].empty();
}

// Инициализация таблицы и сброс параметров обучения
void Learning::initializeAgent(int num_states, int num_actions) {
  // Если размеры не указаны, используем размеры лабиринта
  if (num_states == -1) {
    num_states = getNumStates();
  }
  if (num_actions == -1) {
    num_actions = 4; // < 4 направления
  }

  // Создаем Q-таблицу: [состояния * действия]
  q_table.resize(num_states);
  for (auto &row : q_table) {
    row.resize(num_actions, 0.0); // Инициализируем нулями
  }

  // Сбрасываем параметры обучения
  epsilon = 0.1; // 10% chance случайного действия
  alpha = 0.1;   // Скорость обучения
  gamma = 0.9;   // Коэффициент дисконтирования
}

// Выбор действия агента
int Learning::chooseAction(int state) {
  // Жадная стратегия: с вероятностью выбираем случайное действие
  if (std::uniform_real_distribution<double>(0, 1)(rng) < epsilon) {
    // Случайное действие 0-3
    return std::uniform_int_distribution<int>(0, 3)(rng);
  }

  // Жадная стратегия: выбираем действие с максимальным Q-значением
  const auto &actions = q_table[state];
  double max_q = *std::max_element(actions.begin(), actions.end());

  // Если несколько действий с максимальным значением - выбираем случайное из
  // них
  std::vector<int> best_actions;
  for (size_t i = 0; i < actions.size(); ++i) {
    if (actions[i] == max_q) {
      best_actions.push_back(i);
    }
  }

  return best_actions[std::uniform_int_distribution<int>(
      0, best_actions.size() - 1)(rng)];
}

// Шаг по лабиринту
Learning::StepResult_t Learning::step(int state, int action) {
  // Преобразуем состояние в координаты (row, col)
  Point_t current_pos = getPosFromState(state);

  // Если уже находимся в конечной точке
  if (current_pos.first == end.first && current_pos.second == end.second) {
    return {0.0, true};
  }

  int row = current_pos.first;
  int col = current_pos.second;

  // Вычисляем новую позицию на основе действия
  int new_row = row + kRowDeltas[action];
  int new_col = col + kColDeltas[action];

  // Проверка и расчет награды
  StepResult_t result;

  // Столкновение со стеной или границей
  if (new_row < 0 || new_row >= rows || new_col < 0 || new_col >= cols ||
      isWallBetween(row, col, new_row, new_col)) {
    result = {-1.0, false}; // Награда: -1, false
  } else if (new_row == end.first && new_col == end.second) {
    result = {100.0, true}; // Достижение финиша Награда: +100, true
  } else {
    result = {-0.1, false}; // Обычный шаг Награда: -0.1, false
  }
  return result;
}

// Обновление данных Q-таблицы
void Learning::updateQValue(int state, int action, double reward,
                            int next_state) {
  double max_next_q = 0.0;

  Point_t next_pos = getPosFromState(next_state);
  if (next_pos.first != end.first || next_pos.second != end.second) {
    // Только если НЕ терминальное состояние, ищем максимальное Q-value
    max_next_q = *std::max_element(q_table[next_state].begin(),
                                   q_table[next_state].end());
  }

  double target = reward + gamma * max_next_q;
  q_table[state][action] += alpha * (target - q_table[state][action]);

  // Ограничение значений для стабильности
  if (q_table[state][action] > 100.0)
    q_table[state][action] = 100.0;
  if (q_table[state][action] < -100.0)
    q_table[state][action] = -100.0;
}

// 	---------- Дополнительные методы для обучения ----------

// Преобразует координаты позиции в уникальный индекс состояния
int Learning::getStateFromPos(int row, int col) const {
  // Проверка данных
  if (row < 0 || row >= rows || col < 0 || col >= cols) {
    throw std::out_of_range("Position out of maze bounds");
  }
  return row * cols + col;
}

// Преобразует уникальный индекс в координаты
Learning::Point_t Learning::getPosFromState(int state) const {
  // Проверка входных данных
  if (state < 0 || state >= getNumStates()) {
    throw std::out_of_range("State out of range");
  }
  return {state / cols, state % cols};
}

// Генерация случайной начальной точки
Learning::Point_t Learning::getRandomStartPosition() {
  // Общее число клеток кроме финиша
  int total_cells = rows * cols - 1;
  int random_index =
      std::uniform_int_distribution<int>(0, total_cells - 1)(rng);

  // Преобразуем индекс в координаты, пропуская финиш
  int count = 0;
  Point_t result = {0, 0};
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      if (i == end.first && j == end.second)
        continue;
      if (count++ == random_index) {
        result = {i, j};
      }
    }
  }

  return result;
}

// Проверка стены между клетками
bool Learning::isWallBetween(int row, int col, int new_row, int new_col) const {
  // Проверка границ
  if (new_row < 0 || new_row >= rows || new_col < 0 || new_col >= cols) {
    return true;
  }

  if (row == new_row) {
    // Горизонтальное движение
    if (col < new_col) {
      // Движение ВПРАВО - проверяем правую стену текущей клетки
      return vertical_walls[row][col];
    } else {
      // Движение ВЛЕВО - проверяем левую стену целевой клетки
      return vertical_walls[new_row][new_col];
    }
  } else if (col == new_col) {
    // Вертикальное движение
    if (row < new_row) {
      // Движение ВНИЗ - проверяем нижнюю стену текущей клетки
      return horizontal_walls[row][col];
    } else {
      // Движение ВВЕРХ - проверяем верхнюю стену целевой клетки
      return horizontal_walls[new_row][new_col];
    }
  }

  return true; // Диагональное движение не допускается
}

// Проверка допустимости хода
bool Learning::isValidMove(const Point_t &current_pos, int new_row,
                           int new_col) const {
  if (new_row < 0 || new_row >= rows || new_col < 0 || new_col >= cols) {
    return false;
  }
  return !isWallBetween(current_pos.first, current_pos.second, new_row,
                        new_col);
}

// Сохранение Q-Таблицы
void Learning::saveQTable(const std::string &filename) {
  std::string final_filename = filename;

  if (final_filename.find('.') == std::string::npos) {
    final_filename += ".csv";
  }

  std::ofstream file(final_filename);
  if (!file.is_open()) {
    throw std::runtime_error("Cannot open file for writing: " + final_filename);
  }

  // Записываем заголовки
  file << "State,Pos,Up,Right,Down,Left\n";

  // Перебираем таблицу с индексом
  for (size_t state_idx = 0; state_idx < q_table.size(); ++state_idx) {
    const auto &state_actions = q_table[state_idx];

    // В начале строки добавляем номер состояния
    file << state_idx << ",";
    file << state_idx / cols << "-" << state_idx % cols;

    // Записываем Q-значения
    for (size_t i = 0; i < state_actions.size(); ++i) {
      file << "," << state_actions[i];
    }

    file << "\n";
  }

  file << ",,,,,,Start point: " << start.first << "-" << start.second << "\n";
  file << ",,,,,,End point: " << end.first << "-" << end.second << "\n";

  file.close();
}

// Сброс Q-Таблицы
void Learning::resetQTable() {
  for (auto &row : q_table) {
    std::fill(row.begin(), row.end(), 0.0);
  }
  success_rate = 0.0;
  total_reward = 0.0;
  total_step = 0;
}

// 	---------- Методы для нахождению пути по таблице ----------

// Метод поиска пути
void Learning::findPathFromStart(Point_t start_point) {
  if (!isInitialized()) {
    throw std::runtime_error("Q-table not initialized. Train agent first!");
  }

  if (start_point.first < 0 || start_point.first >= rows ||
      start_point.second < 0 || start_point.second >= cols) {
    throw std::runtime_error("End point out of bounds");
  }
  start = start_point;

  int current_state = getStateFromPos(start.first, start.second);

  found_path.clear();
  // Добавляем начальную точку
  found_path.push_back(start);

  // Максимальное количество шагов для избежания бесконечного цикла
  const int max_steps = rows * cols * 2;
  int steps = 0;

  while (steps < max_steps) {
    // Если достигли финиша - завершаем
    if (isTerminalState(current_state)) {
      break;
    }

    // Выбираем лучшее действие (без исследования, только exploitation)
    int best_action = chooseGreedyAction(current_state);

    // Получаем новую позицию
    Point_t current_pos = getPosFromState(current_state);
    int new_row = current_pos.first + kRowDeltas[best_action];
    int new_col = current_pos.second + kColDeltas[best_action];

    // Проверяем валидность move (на всякий случай)
    if (!isValidMove(current_pos, new_row, new_col)) {
      throw std::runtime_error(
          "Invalid move in optimal path - Q-table may be corrupted");
    }

    // Переходим в новое состояние
    int next_state = getStateFromPos(new_row, new_col);
    found_path.push_back({new_row, new_col});

    current_state = next_state;
    steps++;
  }

  if (steps >= max_steps) {
    std::cout << "Warning: Path finding reached step limit" << std::endl;
  }
}

// Метод выбора наилучшего действия из Q-таблицы
int Learning::chooseGreedyAction(int state) const {
  const auto &actions = q_table[state];
  double max_q = *std::max_element(actions.begin(), actions.end());

  std::vector<int> best_actions;
  for (size_t i = 0; i < actions.size(); ++i) {
    if (actions[i] == max_q) {
      best_actions.push_back(i);
    }
  }

  // Всегда возвращаем первое лучшее действие (детерминировано)
  return best_actions[0];
}

// Проверка терминального состояния
bool Learning::isTerminalState(int state) const {
  Point_t pos = getPosFromState(state);
  return (pos.first == end.first && pos.second == end.second);
}

} // namespace s21
