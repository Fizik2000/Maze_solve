/*
Лабиринт 4*4 Старт 0-0 Финиш 3-3
#########
#A 	    #
### ### #
# #   # #
# # ### #
#   #   #
##### ###
#      B#
#########

Маршрут 0-0 0-1 0-2 0-3 1-3 2-3 2-2 3-2 3-3
Q(s,a) = Q(s,a) + ? * [reward + ? * max(Q(s',a')) - Q(s,a)]

1 Эпизод
(0,0)>: -0.1 + 0.9*0 = -0.1
(0,1)>: -0.1 + 0.9*0 = -0.1
(0,2)>: -0.1 + 0.9*0 = -0.1
(0,3)v: -0.1 + 0.9*0 = -0.1
(1,3)v: -0.1 + 0.9*0 = -0.1
(2,3)<: -0.1 + 0.9*0 = -0.1
(2,2)v: -0.1 + 0.9*0 = -0.1
(3,2)>: -0.1 + 0.9*100 = 89.9
(3,3): 100.0

2 Эпизод
(0,0)>: -0.1 + 0.9*(-0.1) = -0.19
(0,1)>: -0.1 + 0.9*(-0.1) = -0.19
(0,2)>: -0.1 + 0.9*(-0.1) = -0.19
(0,3)v: -0.1 + 0.9*(-0.1) = -0.19
(1,3)v: -0.1 + 0.9*(-0.1) = -0.19
(2,3)<: -0.1 + 0.9*(-0.1) = -0.19
(2,2)v: -0.1 + 0.9*89.9 = 80.81
(3,2)>: -0.1 + 0.9*100 = 89.9
(3,3): 100.0

3 Эпизод
(0,0)>: -0.1 + 0.9*(-0.19) = -0.271
(0,1)>: -0.1 + 0.9*(-0.19) = -0.271
(0,2)>: -0.1 + 0.9*(-0.19) = -0.271
(0,3)v: -0.1 + 0.9*(-0.19) = -0.271
(1,3)v: -0.1 + 0.9*(-0.19) = -0.271
(2,3)<: -0.1 + 0.9*80.81 = 72.629
(2,2)v: -0.1 + 0.9*89.9 = 80.81
(3,2)>: -0.1 + 0.9*100 = 89.9
(3,3): 100.0

и т.д. В итоге значения таблицы будут меняться от каждого эпизода:

Клетка	Действие	Итерация 1	Итерация 2	Итерация 3	Итерация
4	Итерация 5	Итерация 6	Итерация 7	Итерация 8 (0,0)
>			-0.1		-0.19		-0.271		-0.344
-0.410		-0.469		-0.522		42.477 (0,1)	>
-0.1		-0.19		-0.271		-0.344		-0.410
-0.469		47.308		47.308 (0,2)	>			-0.1
-0.19		-0.271		-0.344
-0.410		52.675		52.675		52.675 (0,3)	v
-0.1		-0.19		-0.271
-0.344		58.639		58.639		58.639		58.639 (1,3)
v			-0.1		-0.19
-0.271		65.266		65.266		65.266		65.266		65.266
(2,3)	<			-0.1
-0.19		72.629		72.629		72.629		72.629		72.629
72.629 (2,2)	v
-0.1		80.81		80.81		80.81		80.81		80.81
80.81		80.81 (3,2)
>			89.9		89.9		89.9		89.9		89.9
89.9		89.9		89.9 (3,3)	-			100.0
100.0		100.0		100.0		100.0		100.0
100.0		100.0

*/

#ifndef LEARNING_H
#define LEARNING_H

#include "../model/maze.h"
#include <algorithm>
#include <array>
#include <cmath>
#include <fstream>
#include <functional>
#include <iostream>
#include <random>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace s21 {

class Learning {
public:
  // --- Структуры данных ---
  using Point_t = std::pair<int, int>;
  using StepResult_t = std::pair<double, bool>;
  using Path_t = std::vector<Point_t>;
  using WallVector_t = std::vector<bool>;
  using WallMatrix_t = std::vector<WallVector_t>;

private:
  // --- Внутренние структуры для лабиринта ---
  int rows{}, cols{};
  WallMatrix_t vertical_walls{};
  WallMatrix_t horizontal_walls{};
  Path_t found_path{};
  Point_t start{};
  Point_t end{};
  static constexpr int kNumberDirections{4};
  static inline constexpr std::array<Direction, kNumberDirections> kDirections =
      {Direction::kUp, Direction::kRight, Direction::kDown, Direction::kLeft};
  static inline constexpr std::array<int, kNumberDirections> kRowDeltas = {
      -1, 0, 1, 0};
  static inline constexpr std::array<int, kNumberDirections> kColDeltas = {
      0, 1, 0, -1};

  // --- Внутренние структуры для Q-learning ---
  std::vector<std::vector<double>> q_table;
  double alpha{0.1};
  double gamma{0.9};
  double epsilon{0.1};
  double success_rate{0.0};
  double total_reward{0.0};
  int total_step{0};
  std::mt19937 rng;
  std::uniform_real_distribution<double> dist{0.0, 1.0};

public:
  // --- Конструктор и деструктор ---
  Learning();
  //~Learning();

  // --- Методы загрузки лабиринта ---
  void loadMazeFromFile(const std::string &filename);
  void checkFileData(std::istream &file);

  // --- Методы обучения ---
  void train(int total_episodes, int max_steps_per_episode, Point_t end_point);
  bool isInitialized() const;
  void initializeAgent(int num_states, int num_actions);
  int chooseAction(int state);
  StepResult_t step(int state, int action);
  void updateQValue(int state, int action, double reward, int next_state);

  // --- Методы поиска пути ---
  void findPathFromStart(Point_t start_point);
  int chooseGreedyAction(int state) const;
  bool isTerminalState(int state) const;

  // --- Доп методы обучения ---
  int getStateFromPos(int row, int col) const;
  Point_t getPosFromState(int state) const;
  Point_t getRandomStartPosition();
  bool isWallBetween(int row, int col, int new_row, int new_col) const;
  bool isValidMove(const Point_t &current_pos, int new_row, int new_col) const;
  void saveQTable(const std::string &filename);
  void resetQTable();
  void resetFoundPath() { found_path.clear(); }

  //	--- Геттеры и Сеттеры ---
  int getNumStates() const { return rows * cols; }
  void setEpsilon(double new_epsilon) { epsilon = new_epsilon; }
  void setAlpha(double new_alpha) { alpha = new_alpha; }
  void setGamma(double new_gamma) { gamma = new_gamma; }
  Path_t getFoundPath() const { return found_path; }
  int getRows() const { return rows; }
  int getCols() const { return cols; }
  WallMatrix_t getVerticalWalls() const { return vertical_walls; }
  WallMatrix_t getHorizontalWalls() const { return horizontal_walls; }
  double getEpsilon() const { return epsilon; }
  double getSuccessRate() const { return success_rate; }
  double getTotalReward() const { return total_reward; }
  int getTotalStep() const { return total_step; }
};
} // namespace s21

#endif // LEARNING_H
