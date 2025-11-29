#ifndef MAZE_H
#define MAZE_H

#include <array>
#include <fstream>
#include <random>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include <iostream>

enum class Direction { kUp, kDown, kLeft, kRight };

namespace s21 {

class Maze {
public:
  using Point_t = std::pair<int, int>;
  using Path_t = std::vector<Point_t>;
  using WallVector_t = std::vector<bool>;
  using WallMatrix_t = std::vector<WallVector_t>;
  using Vector_t = std::vector<int>;

private:
  int rows{}, cols{};
  WallMatrix_t vertical_walls{};
  WallMatrix_t horizontal_walls{};
  Path_t found_path{};
  Vector_t string_values{};
  int value_set{1};
  Point_t start{};
  Point_t end{};
  WallMatrix_t visited_point{};
  static constexpr int kNumberDirections{4};
  static inline constexpr std::array<Direction, kNumberDirections> kDirections =
      {Direction::kLeft, Direction::kRight, Direction::kUp, Direction::kDown};
  static inline constexpr std::array<int, kNumberDirections> kRowDeltas = {
      0, 0, -1, 1};
  static inline constexpr std::array<int, kNumberDirections> kColDeltas = {
      -1, 1, 0, 0};

public:
  void load_maze(const std::string &filename);
  void save_maze(const std::string &filename);
  void generate_maze(int new_row, int new_col);
  void solve_maze(Point_t start_point, Point_t end_point);

  int get_rows();
  int get_cols();
  const WallMatrix_t &get_vertical_walls() const { return vertical_walls; }
  const WallMatrix_t &get_horizontal_walls() const { return horizontal_walls; }
  const Path_t &get_path() const { return found_path; }
  void resetPath() { found_path.clear(); }

private:
  void check_file_date(std::istream &file);
  void union_sets();
  void set_vertical_walls(int current_row);
  void set_horizontal_walls(int current_row);
  bool random_bool_value();
  void merge_set(int index);
  bool is_one_element_in_set(int element);
  void check_isolation_set(int row);
  int count_horizontal_walls(int input_row, int input_element);
  void set_zero_for_wall(int input_row);
  bool depth_first_search(int row, int col);
  bool is_possible_to_move(int row, int col, Direction direct);
  void clear_path();
};
} // namespace s21

#endif // MAZE_H
