#include "control_cave.h"

namespace s21 {

// Конструктор
ControlCave::ControlCave() { cave = new Cave(); }

// Деконструктор
ControlCave::~ControlCave() { delete cave; }

// Генерация стартовой пещеры
void ControlCave::generate(int rows, int cols, int init) {
  cave->generate_cave(rows, cols, init);
}

// Загрузка из файла
void ControlCave::loadFromFile(const std::string &filename) {
  cave->load_cave(filename);
}

// Сохранение в файл
void ControlCave::saveToFile(const std::string &filename) {
  cave->save_cave(filename);
}

// Клеточный автомат
int ControlCave::cellular_automaton(int life, int dead) {
  return cave->cellular_automaton(life, dead);
}
} // namespace s21
