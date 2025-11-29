#ifndef CONTROLMAZE_H
#define CONTROLMAZE_H

#include "../model/maze.h"

namespace s21 {
class ControlMaze {
public:
  explicit ControlMaze();
  ~ControlMaze();

  void generate(int rows, int cols);
  void loadFromFile(const std::string &filename);
  void saveToFile(const std::string &filename);
  void solveMaze(Maze::Point_t start, Maze::Point_t end);
  int get_rows();
  int get_cols();
  const Maze::WallMatrix_t &getVerticalWalls() const {
    return maze->get_vertical_walls();
  }
  const Maze::WallMatrix_t &getHorizontalWalls() const {
    return maze->get_horizontal_walls();
  }
  const Maze::Path_t &get_path() const { return maze->get_path(); }
  void resetPath() { maze->resetPath(); }

private:
  Maze *maze;
};
} // namespace s21

#endif // CONTROLMAZE_H
