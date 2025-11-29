#ifndef CONTROLLEARNING_H
#define CONTROLLEARNING_H

#include "../model/learning.h"

namespace s21 {

class ControlLearning {
public:
  explicit ControlLearning();
  ~ControlLearning();

  void loadFromFile(const std::string &filename);
  void train(int total_episodes, int max_steps_per_episode,
             Learning::Point_t end_point);
  void findPathFromStart(Learning::Point_t start_point);
  void saveQTable(const std::string &filename);

  Learning::Path_t getFoundPath() const;
  void setEpsilon(double new_epsilon);
  void setAlpha(double new_alpha);
  void setGamma(double new_gamma);
  int getNumStates() const;
  int getRows() const;
  int getCols() const;
  Learning::WallMatrix_t getVerticalWalls() const;
  Learning::WallMatrix_t getHorizontalWalls() const;
  double getEpsilon() const;
  double getSuccessRate() const;
  double getTotalReward() const;
  int getTotalStep() const;
  void resetQTable();
  void resetPath();

private:
  Learning *learning;
};
} // namespace s21

#endif // CONTROLLEARNING_H
