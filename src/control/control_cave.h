#ifndef CONTROLCAVE_H
#define CONTROLCAVE_H

#include "../model/cave.h"

namespace s21 {
class ControlCave {
public:
  explicit ControlCave();
  ~ControlCave();

  void generate(int rows, int cols, int init);
  void loadFromFile(const std::string &filename);
  void saveToFile(const std::string &filename);
  int cellular_automaton(int life, int dead);

  const Cave::CaveMatrix_t &get_cave() const { return cave->get_cave(); }
  void set_life(int life) { cave->set_life(life); }
  void set_dead(int dead) { cave->set_dead(dead); }
  int get_rows() { return cave->get_rows(); }
  int get_cols() { return cave->get_cols(); }

private:
  Cave *cave;
};
} // namespace s21

#endif // CONTROLCAVE_H
