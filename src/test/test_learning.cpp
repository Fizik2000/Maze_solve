#include "test_main.h"

namespace s21 {

class LearningTest : public ::testing::Test {
protected:
  std::string filename;
  std::string input;
  std::string output;
  int rows{}, cols{};

  void SetUp() override {
    control = new ControlLearning();
    filename = "test_learning.txt";
    input = "input.txt";
    output = "output.txt";

    std::ofstream file{filename};
  }

  void TearDown() override {
    delete control;

    std::remove(filename.c_str());
    std::remove(input.c_str());
    std::remove(output.c_str());
  }

  ControlLearning *control;
};
} // namespace s21

using namespace s21;

TEST_F(LearningTest, CheckInitDate) {
  // Проверка размеров
  EXPECT_EQ(control->getRows(), -1);
  EXPECT_EQ(control->getCols(), -1);

  // Получаем размеры как size_t
  size_t rows = static_cast<size_t>(control->getRows());

  const auto &verticalWalls = control->getVerticalWalls();
  const auto &horizontalWalls = control->getHorizontalWalls();

  // Проверяем, что вектора стен соответствуют размерам
  EXPECT_TRUE(verticalWalls.empty() || (verticalWalls.size() == rows + 1));
  EXPECT_TRUE(horizontalWalls.empty() || (horizontalWalls.size() == rows));

  // Проверяем, что путь пустой
  EXPECT_TRUE(control->getFoundPath().empty());
}

//--------------------LOAD------------------------

TEST_F(LearningTest, LoadFromFileTest1) {
  std::ofstream file(filename);
  file << "0 6\n";
  file << "0 1 1 1 1 1\n";
  file << "1 0 0 0 0 1\n";
  file << "1 1 1 1 1 1\n";
  file << "0 0 0 1 1 1\n";
  file << "1 0 0 0 0 0\n";
  file << "0 0 0 0 0 0\n";
  file << "0 1 1 0 0 0\n";
  file << "1 1 1 1 1 1\n";
  file.close();

  try {
    control->loadFromFile(filename);
    FAIL() << "Expected std::runtime_error due to invalid file format";
  } catch (const std::runtime_error &e) {
    EXPECT_STREQ(e.what(),
                 "Invalid file format: rows and cols must be between 2 and 50");
  } catch (...) {
    FAIL()
        << "Expected std::runtime_error, but caught a different exception type";
  }
}
/**/
TEST_F(LearningTest, LoadFromFileTest2) {
  std::ofstream file(filename);
  file << "a 6\n";
  file << "0 1 1 1 1 1\n";
  file << "1 0 0 0 0 1\n";
  file << "1 1 1 1 1 1\n";
  file << "0 0 0 1 1 1\n";
  file << "1 0 0 0 0 0\n";
  file << "0 0 0 0 0 0\n";
  file << "0 1 1 0 0 0\n";
  file << "1 1 1 1 1 1\n";
  file.close();

  try {
    control->loadFromFile(filename);
    FAIL() << "Expected std::runtime_error due to invalid file format";
  } catch (const std::runtime_error &e) {
    EXPECT_STREQ(e.what(), "Invalid file format: cannot read size");
  } catch (...) {
    FAIL()
        << "Expected std::runtime_error, but caught a different exception type";
  }
}

TEST_F(LearningTest, LoadFromFileTest3) {
  std::ofstream file(filename);
  file << "-3 6\n";
  file << "0 1 1 1 1 1\n";
  file << "1 0 0 0 0 1\n";
  file << "1 1 1 1 1 1\n";
  file << "0 0 0 1 1 1\n";
  file << "1 0 0 0 0 0\n";
  file << "0 0 0 0 0 0\n";
  file << "0 1 1 0 0 0\n";
  file << "1 1 1 1 1 1\n";
  file.close();

  try {
    control->loadFromFile(filename);
    FAIL() << "Expected std::runtime_error due to invalid file format";
  } catch (const std::runtime_error &e) {
    EXPECT_STREQ(e.what(),
                 "Invalid file format: rows and cols must be between 2 and 50");
  } catch (...) {
    FAIL()
        << "Expected std::runtime_error, but caught a different exception type";
  }
}

TEST_F(LearningTest, LoadFromFileTest4) {
  std::ofstream file(filename);
  file << "3 62\n";
  file << "0 1 1 1 1 1\n";
  file << "1 0 0 0 0 1\n";
  file << "1 1 1 1 1 1\n";
  file << "0 0 0 1 1 1\n";
  file << "1 0 0 0 0 0\n";
  file << "0 0 0 0 0 0\n";
  file << "0 1 1 0 0 0\n";
  file << "1 1 1 1 1 1\n";
  file.close();

  try {
    control->loadFromFile(filename);
    FAIL() << "Expected std::runtime_error due to invalid file format";
  } catch (const std::runtime_error &e) {
    EXPECT_STREQ(e.what(),
                 "Invalid file format: rows and cols must be between 2 and 50");
  } catch (...) {
    FAIL()
        << "Expected std::runtime_error, but caught a different exception type";
  }
}

TEST_F(LearningTest, LoadFromFileTest5) {
  std::ofstream file(filename);
  file << "10 1\n";
  file << "0 1 1 1 1 1\n";
  file << "1 0 0 0 0 1\n";
  file << "1 1 1 1 1 1\n";
  file << "0 0 0 1 1 1\n";
  file << "1 0 0 0 0 0\n";
  file << "0 0 0 0 0 0\n";
  file << "0 1 1 0 0 0\n";
  file << "1 1 1 1 1 1\n";
  file.close();

  try {
    control->loadFromFile(filename);
    FAIL() << "Expected std::runtime_error due to invalid file format";
  } catch (const std::runtime_error &e) {
    EXPECT_STREQ(e.what(),
                 "Invalid file format: rows and cols must be between 2 and 50");
  } catch (...) {
    FAIL()
        << "Expected std::runtime_error, but caught a different exception type";
  }
}

TEST_F(LearningTest, LoadFromFileTest6) {
  std::ofstream file(this->filename);
  file << "10 -10\n";
  file << "0 1 1 1 1 1\n";
  file << "1 0 0 0 0 1\n";
  file << "1 1 1 1 1 1\n";
  file << "0 0 0 1 1 1\n";
  file << "1 0 0 0 0 0\n";
  file << "0 0 0 0 0 0\n";
  file << "0 1 1 0 0 0\n";
  file << "1 1 1 1 1 1\n";
  file.close();

  try {
    control->loadFromFile(this->filename);
    FAIL() << "Expected std::runtime_error due to invalid file format";
  } catch (const std::runtime_error &e) {
    EXPECT_STREQ(e.what(),
                 "Invalid file format: rows and cols must be between 2 and 50");
  } catch (...) {
    FAIL()
        << "Expected std::runtime_error, but caught a different exception type";
  }
}

TEST_F(LearningTest, LoadFromFileTest7) {
  std::ofstream file(this->filename);
  file << "10 51\n";
  file << "0 1 1 1 1 1\n";
  file << "1 0 0 0 0 1\n";
  file << "1 1 1 1 1 1\n";
  file << "0 0 0 1 1 1\n";
  file << "1 0 0 0 0 0\n";
  file << "0 0 0 0 0 0\n";
  file << "0 1 1 0 0 0\n";
  file << "1 1 1 1 1 1\n";
  file.close();

  try {
    control->loadFromFile(this->filename);
    FAIL() << "Expected std::runtime_error due to invalid file format";
  } catch (const std::runtime_error &e) {
    EXPECT_STREQ(e.what(),
                 "Invalid file format: rows and cols must be between 2 and 50");
  } catch (...) {
    FAIL()
        << "Expected std::runtime_error, but caught a different exception type";
  }
}

TEST_F(LearningTest, LoadFromFileTest8) {
  std::ofstream file(filename);
  file << "67 51\n";
  file << "0 1 1 1 1 1\n";
  file << "1 0 0 0 0 1\n";
  file << "1 1 1 1 1 1\n";
  file << "0 0 0 1 1 1\n";
  file << "1 0 0 0 0 0\n";
  file << "0 0 0 0 0 0\n";
  file << "0 1 1 0 0 0\n";
  file << "1 1 1 1 1 1\n";
  file.close();

  try {
    control->loadFromFile(filename);
    FAIL() << "Expected std::runtime_error due to invalid file format";
  } catch (const std::runtime_error &e) {
    EXPECT_STREQ(e.what(),
                 "Invalid file format: rows and cols must be between 2 and 50");
  } catch (...) {
    FAIL()
        << "Expected std::runtime_error, but caught a different exception type";
  }
}

TEST_F(LearningTest, LoadFromFileTest9) {
  std::ofstream file(filename);
  file << "1 0\n";
  file << "0 1 1 1 1 1\n";
  file << "1 0 0 0 0 1\n";
  file << "1 1 1 1 1 1\n";
  file << "0 0 0 1 1 1\n";
  file << "1 0 0 0 0 0\n";
  file << "0 0 0 0 0 0\n";
  file << "0 1 1 0 0 0\n";
  file << "1 1 1 1 1 1\n";
  file.close();

  try {
    control->loadFromFile(filename);
    FAIL() << "Expected std::runtime_error due to invalid file format";
  } catch (const std::runtime_error &e) {
    EXPECT_STREQ(e.what(),
                 "Invalid file format: rows and cols must be between 2 and 50");
  } catch (...) {
    FAIL()
        << "Expected std::runtime_error, but caught a different exception type";
  }
}

TEST_F(LearningTest, LoadFromFileTest10) {
  std::ofstream file(filename);
  file << "51 0\n";
  file << "0 1 1 1 1 1\n";
  file << "1 0 0 0 0 1\n";
  file << "1 1 1 1 1 1\n";
  file << "0 0 0 1 1 1\n";
  file << "1 0 0 0 0 0\n";
  file << "0 0 0 0 0 0\n";
  file << "0 1 1 0 0 0\n";
  file << "1 1 1 1 1 1\n";
  file.close();

  try {
    control->loadFromFile(filename);
    FAIL() << "Expected std::runtime_error due to invalid file format";
  } catch (const std::runtime_error &e) {
    EXPECT_STREQ(e.what(),
                 "Invalid file format: rows and cols must be between 2 and 50");
  } catch (...) {
    FAIL()
        << "Expected std::runtime_error, but caught a different exception type";
  }
}

TEST_F(LearningTest, LoadFromFileTest11) {
  std::ofstream file(filename);
  file << "1 78\n";
  file << "0 1 1 1 1 1\n";
  file << "1 0 0 0 0 1\n";
  file << "1 1 1 1 1 1\n";
  file << "0 0 0 1 1 1\n";
  file << "1 0 0 0 0 0\n";
  file << "0 0 0 0 0 0\n";
  file << "0 1 1 0 0 0\n";
  file << "1 1 1 1 1 1\n";
  file.close();

  try {
    control->loadFromFile(filename);
    FAIL() << "Expected std::runtime_error due to invalid file format";
  } catch (const std::runtime_error &e) {
    EXPECT_STREQ(e.what(),
                 "Invalid file format: rows and cols must be between 2 and 50");
  } catch (...) {
    FAIL()
        << "Expected std::runtime_error, but caught a different exception type";
  }
}

TEST_F(LearningTest, LoadFromFileTest12) {
  std::ofstream file(filename);
  file << "2 2\n";
  file << "1 0\n";
  file << "0 1\n";
  file << "0 0\n";
  file << "1 1\n";
  file.close();

  try {
    control->loadFromFile(filename);
    FAIL()
        << "Expected std::runtime_error due to invalid vertical walls format";
  } catch (const std::runtime_error &e) {
    EXPECT_STREQ(e.what(), "Invalid file format: vertical walls");
  } catch (...) {
    FAIL()
        << "Expected std::runtime_error, but caught a different exception type";
  }
}

TEST_F(LearningTest, LoadFromFileTest13) {
  std::ofstream file(filename);
  file << "2 2\n";
  file << "1 1\n";
  file << "0 0\n";
  file << "0 0\n";
  file << "1 1\n";
  file.close();

  try {
    control->loadFromFile(filename);
    FAIL()
        << "Expected std::runtime_error due to invalid vertical walls format";
  } catch (const std::runtime_error &e) {
    EXPECT_STREQ(e.what(), "Invalid file format: vertical walls");
  } catch (...) {
    FAIL()
        << "Expected std::runtime_error, but caught a different exception type";
  }
}

TEST_F(LearningTest, LoadFromFileTest14) {
  std::ofstream file(filename);
  file << "2 2\n";
  file << "1 0\n";
  file << "0 0\n";
  file << "0 0\n";
  file << "1 1\n";
  file.close();

  try {
    control->loadFromFile(filename);
    FAIL()
        << "Expected std::runtime_error due to invalid vertical walls format";
  } catch (const std::runtime_error &e) {
    EXPECT_STREQ(e.what(), "Invalid file format: vertical walls");
  } catch (...) {
    FAIL()
        << "Expected std::runtime_error, but caught a different exception type";
  }
}

TEST_F(LearningTest, LoadFromFileTest15) {
  std::ofstream file(filename);
  file << "2 2\n";
  file << "0 1\n";
  file << "0 1\n";
  file << "3 0\n";
  file << "0 0\n";
  file.close();

  try {
    control->loadFromFile(filename);
    FAIL() << "Expected std::runtime_error due to invalid file format";
  } catch (const std::runtime_error &e) {
    EXPECT_STREQ(e.what(), "Invalid horizontal wall data at row 0, col 0");
  } catch (...) {
    FAIL()
        << "Expected std::runtime_error, but caught a different exception type";
  }
}

TEST_F(LearningTest, LoadFromFileTest16) {
  std::ofstream file(filename);
  file << "2 2\n";
  file << "1 1\n";
  file << "0 1\n";
  file << "0 0\n";
  file << "0 1\n";
  file.close();

  try {
    control->loadFromFile(filename);
    FAIL()
        << "Expected std::runtime_error due to invalid vertical walls format";
  } catch (const std::runtime_error &e) {
    EXPECT_STREQ(e.what(), "Invalid file format: horizontal walls");
  } catch (...) {
    FAIL()
        << "Expected std::runtime_error, but caught a different exception type";
  }
}

TEST_F(LearningTest, LoadFromFileTest17) {
  std::ofstream file(filename);
  file << "2 2\n";
  file << "1 1\n";
  file << "0 1\n";
  file << "0 0\n";
  file << "1 0\n";
  file.close();

  try {
    control->loadFromFile(filename);
    FAIL()
        << "Expected std::runtime_error due to invalid vertical walls format";
  } catch (const std::runtime_error &e) {
    EXPECT_STREQ(e.what(), "Invalid file format: horizontal walls");
  } catch (...) {
    FAIL()
        << "Expected std::runtime_error, but caught a different exception type";
  }
}

TEST_F(LearningTest, LoadFromFileTest18) {
  std::ofstream file(filename);
  file << "2 2\n";
  file << "1 1\n";
  file << "0 1\n";
  file << "0 0\n";
  file << "0 0\n";
  file.close();

  try {
    control->loadFromFile(filename);
    FAIL()
        << "Expected std::runtime_error due to invalid vertical walls format";
  } catch (const std::runtime_error &e) {
    EXPECT_STREQ(e.what(), "Invalid file format: horizontal walls");
  } catch (...) {
    FAIL()
        << "Expected std::runtime_error, but caught a different exception type";
  }
}

TEST_F(LearningTest, LoadFromFileTest19) {
  std::ofstream file(filename);
  file << "2 2\n";
  file << "0 1\n";
  file << "3 1\n";
  file << "0 0\n";
  file << "0 0\n";
  file.close();

  try {
    control->loadFromFile(filename);
    FAIL() << "Expected std::runtime_error due to invalid file format";
  } catch (const std::runtime_error &e) {
    EXPECT_STREQ(e.what(), "Invalid vertical wall data at row 1, col 0");
  } catch (...) {
    FAIL()
        << "Expected std::runtime_error, but caught a different exception type";
  }
}

TEST_F(LearningTest, LoadFromFileTest20) {
  std::ofstream file(filename);
  file << "2 2\n";
  file << "0 1\n";
  file << "0 1\n";
  file << "0 0\n";
  file << "1 1 1\n";
  file.close();

  try {
    control->loadFromFile(filename);
    FAIL() << "Expected std::runtime_error due to unexpected additional data";
  } catch (const std::runtime_error &e) {
    EXPECT_STREQ(e.what(), "Invalid file format: unexpected additional data");
  } catch (...) {
    FAIL()
        << "Expected std::runtime_error, but caught a different exception type";
  }
}

TEST_F(LearningTest, LoadFromFile2x2Test) {
  std::ofstream file(filename);
  file << "2 2\n";
  file << "1 1\n";
  file << "0 1\n";
  file << "0 0\n";
  file << "1 1\n";
  file.close();

  control->loadFromFile(filename);

  const auto &v_walls_loaded = control->getVerticalWalls();
  const auto &h_walls_loaded = control->getHorizontalWalls();

  EXPECT_EQ(control->getRows(), 2);
  EXPECT_EQ(control->getCols(), 2);

  EXPECT_EQ(v_walls_loaded[0][0], 1);
  EXPECT_EQ(v_walls_loaded[0][1], 1);
  EXPECT_EQ(v_walls_loaded[1][0], 0);
  EXPECT_EQ(v_walls_loaded[1][1], 1);
  EXPECT_EQ(h_walls_loaded[0][0], 0);
  EXPECT_EQ(h_walls_loaded[0][1], 0);
  EXPECT_EQ(h_walls_loaded[1][0], 1);
  EXPECT_EQ(h_walls_loaded[1][1], 1);
}

TEST_F(LearningTest, LoadFromFile2x5Test) {
  std::ofstream file(filename);
  file << "2 5\n";
  file << "0 1 1 1 1\n";
  file << "0 0 0 0 1\n";
  file << "0 1 0 0 0\n";
  file << "1 1 1 1 1\n";
  file.close();

  control->loadFromFile(filename);

  const auto &v_walls_loaded = control->getVerticalWalls();
  const auto &h_walls_loaded = control->getHorizontalWalls();

  EXPECT_EQ(control->getRows(), 2);
  EXPECT_EQ(control->getCols(), 5);

  EXPECT_EQ(v_walls_loaded[0][0], 0);
  EXPECT_EQ(v_walls_loaded[0][1], 1);
  EXPECT_EQ(v_walls_loaded[0][2], 1);
  EXPECT_EQ(v_walls_loaded[0][3], 1);
  EXPECT_EQ(v_walls_loaded[0][4], 1);
  EXPECT_EQ(v_walls_loaded[1][0], 0);
  EXPECT_EQ(v_walls_loaded[1][1], 0);
  EXPECT_EQ(v_walls_loaded[1][2], 0);
  EXPECT_EQ(v_walls_loaded[1][3], 0);
  EXPECT_EQ(v_walls_loaded[1][4], 1);
  EXPECT_EQ(h_walls_loaded[0][0], 0);
  EXPECT_EQ(h_walls_loaded[0][1], 1);
  EXPECT_EQ(h_walls_loaded[0][2], 0);
  EXPECT_EQ(h_walls_loaded[0][3], 0);
  EXPECT_EQ(h_walls_loaded[0][4], 0);
  EXPECT_EQ(h_walls_loaded[1][0], 1);
  EXPECT_EQ(h_walls_loaded[1][1], 1);
  EXPECT_EQ(h_walls_loaded[1][2], 1);
  EXPECT_EQ(h_walls_loaded[1][3], 1);
  EXPECT_EQ(h_walls_loaded[1][4], 1);
}

TEST_F(LearningTest, TrainTest1) {
  std::ofstream file(filename);
  file << "2 2\n";
  file << "1 1\n";
  file << "0 1\n";
  file << "0 0\n";
  file << "1 1\n";
  file.close();

  control->loadFromFile(filename);
  EXPECT_EQ(control->getNumStates(), 4);
  control->resetPath();
  control->resetQTable();
  control->setAlpha(0.2);
  control->setEpsilon(0.3);
  control->setGamma(0.9);
  control->train(10, 20, {1, 1});

  EXPECT_GE(control->getSuccessRate(), 0.0);
  EXPECT_LE(control->getSuccessRate(), 100.0);
  EXPECT_GE(control->getTotalReward(), -1000.0);
  EXPECT_GE(control->getTotalStep(), 0);
  EXPECT_LE(control->getEpsilon(), 0.1);

  control->findPathFromStart({0, 0});
  std::vector<std::pair<int, int>> path = control->getFoundPath();

  EXPECT_EQ(path[0].first, 0);
  EXPECT_EQ(path[0].second, 0);

  for (const auto &point : path) {
    EXPECT_GE(point.first, 0);
    EXPECT_LT(point.first, 2);
    EXPECT_GE(point.second, 0);
    EXPECT_LT(point.second, 2);
  }
}

TEST_F(LearningTest, TrainTest2) {
  std::ofstream file(filename);
  file << "2 2\n";
  file << "1 1\n";
  file << "0 1\n";
  file << "0 0\n";
  file << "1 1\n";
  file.close();

  try {
    control->loadFromFile(filename);
    control->train(10, 20, {1, 3});
    FAIL() << "Expected std::runtime_error due to unexpected additional data";
  } catch (const std::runtime_error &e) {
    EXPECT_STREQ(e.what(), "End point out of bounds");
  } catch (...) {
    FAIL()
        << "Expected std::runtime_error, but caught a different exception type";
  }
}

TEST_F(LearningTest, TrainTest3) {
  std::ofstream file(filename);
  file << "2 2\n";
  file << "1 1\n";
  file << "0 1\n";
  file << "0 0\n";
  file << "1 1\n";
  file.close();

  try {
    control->loadFromFile(filename);
    control->train(10, -20, {1, 1});
    FAIL() << "Expected std::runtime_error due to unexpected additional data";
  } catch (const std::runtime_error &e) {
    EXPECT_STREQ(e.what(), "Not correct, episode > 1 or max_steps > 1");
  } catch (...) {
    FAIL()
        << "Expected std::runtime_error, but caught a different exception type";
  }
}

TEST_F(LearningTest, FoundPathTest) {
  std::ofstream file(filename);
  file << "2 2\n";
  file << "1 1\n";
  file << "0 1\n";
  file << "0 0\n";
  file << "1 1\n";
  file.close();

  try {
    control->loadFromFile(filename);
    control->resetPath();
    control->resetQTable();
    control->setAlpha(0.2);
    control->setEpsilon(0.3);
    control->setGamma(0.9);
    control->train(10, 20, {1, 1});
    control->findPathFromStart({-1, 3});
    FAIL() << "Expected std::runtime_error due to unexpected additional data";
  } catch (const std::runtime_error &e) {
    EXPECT_STREQ(e.what(), "End point out of bounds");
  } catch (...) {
    FAIL()
        << "Expected std::runtime_error, but caught a different exception type";
  }
}

TEST_F(LearningTest, SaveQTableTest) {
  std::ofstream file(filename);
  file << "2 2\n";
  file << "1 1\n";
  file << "0 1\n";
  file << "0 0\n";
  file << "1 1\n";
  file.close();

  control->loadFromFile(filename);
  control->train(10, 20, {1, 1});

  std::string qtable_filename = "test_qtable.csv";
  control->saveQTable(qtable_filename);

  std::ifstream saved_file(qtable_filename);
  EXPECT_TRUE(saved_file.is_open());

  std::string line;
  std::vector<std::string> lines;

  while (std::getline(saved_file, line)) {
    lines.push_back(line);
  }
  saved_file.close();

  EXPECT_GE(lines.size(), 5);
  EXPECT_TRUE(lines[0].find("State,Pos,Up,Right,Down,Left") !=
              std::string::npos);
  EXPECT_GE(lines.size(), 5);

  // Проверка формата данных
  for (size_t i = 1; i < lines.size() - 2; ++i) {
    std::stringstream ss(lines[i]);
    std::string token;
    std::vector<std::string> tokens;

    while (std::getline(ss, token, ',')) {
      tokens.push_back(token);
    }

    EXPECT_GE(tokens.size(), 6);
    EXPECT_FALSE(tokens[0].empty());
    EXPECT_TRUE(tokens[1].find('-') != std::string::npos);

    for (int j = 2; j < 6; ++j) {
      EXPECT_FALSE(tokens[j].empty());
    }
  }

  EXPECT_TRUE(lines[lines.size() - 2].find("Start point:") !=
              std::string::npos);
  EXPECT_TRUE(lines[lines.size() - 1].find("End point:") != std::string::npos);

  std::remove(qtable_filename.c_str());
}