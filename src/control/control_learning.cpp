#include "control_learning.h"

namespace s21 {

// Конструктор
ControlLearning::ControlLearning() { learning = new Learning(); }

// Деконструктор
ControlLearning::~ControlLearning() { delete learning; }

// Загрузка из файла
void ControlLearning::loadFromFile(const std::string &filename) {
  learning->loadMazeFromFile(filename);
}

// Запуск обучения
void ControlLearning::train(int total_episodes, int max_steps_per_episode,
                            Learning::Point_t end_point) {
  learning->train(total_episodes, max_steps_per_episode, end_point);
}

// Поиск решения
void ControlLearning::findPathFromStart(Learning::Point_t start_point) {
  learning->findPathFromStart(start_point);
}

//
void ControlLearning::saveQTable(const std::string &filename) {
  learning->saveQTable(filename);
}

// Получение найденного поти
Learning::Path_t ControlLearning::getFoundPath() const {
  return learning->getFoundPath();
}

// Установка Эпсилон
void ControlLearning::setEpsilon(double new_epsilon) {
  learning->setEpsilon(new_epsilon);
}

// Установка Альфа
void ControlLearning::setAlpha(double new_alpha) {
  learning->setAlpha(new_alpha);
}

// Установка Гамма
void ControlLearning::setGamma(double new_gamma) {
  learning->setGamma(new_gamma);
}

// Получение количества состояний
int ControlLearning::getNumStates() const { return learning->getNumStates(); }

// Получение количества строк
int ControlLearning::getRows() const { return learning->getRows(); }

// Получение количества столбцов
int ControlLearning::getCols() const { return learning->getCols(); }

// Получение матрицы вертикальных стен
Learning::WallMatrix_t ControlLearning::getVerticalWalls() const {
  return learning->getVerticalWalls();
}

// Получение матрицы горизонтальных стен
Learning::WallMatrix_t ControlLearning::getHorizontalWalls() const {
  return learning->getHorizontalWalls();
}

// Сброс таблицы
void ControlLearning::resetQTable() { learning->resetQTable(); }

// Сброс пути
void ControlLearning::resetPath() { learning->resetFoundPath(); }

double ControlLearning::getEpsilon() const { return learning->getEpsilon(); }
double ControlLearning::getSuccessRate() const {
  return learning->getSuccessRate();
}
double ControlLearning::getTotalReward() const {
  return learning->getTotalReward();
}
int ControlLearning::getTotalStep() const { return learning->getTotalStep(); }

} // namespace s21
