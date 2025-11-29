#ifndef CAVE_H
#define CAVE_H

#include <array>
#include <fstream>
#include <random>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace s21 {

class Cave {
public:
  using CaveVector_t = std::vector<bool>;
  using CaveMatrix_t = std::vector<CaveVector_t>;

private:
  int rows{}, cols{};
  CaveMatrix_t cave{};
  int birth_limit{};
  int death_limit{};
  int chance_init{};

public:
  void load_cave(const std::string &filename);
  void save_cave(const std::string &filename);
  void generate_cave(int row, int col, int init);
  int cellular_automaton(int life, int dead);
  const CaveMatrix_t &get_cave() const { return cave; }
  void set_life(int life) { birth_limit = life; }
  void set_dead(int dead) { death_limit = dead; }
  int get_rows() { return rows; }
  int get_cols() { return cols; }

private:
  void check_file_date(std::istream &file);
  bool random_bool_value();
  int count_neighbors(int row, int col);
};
} // namespace s21

#endif // CAVE_H
