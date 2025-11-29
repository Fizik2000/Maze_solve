.DEFAULT_GOAL := all

.PHONY: all install clean start uninstall clang-format format dvi dist run_dvi valgrind test gcov-report

# Компиляторы
CXX = g++
# Флаги компилятора
CXXFLAGS = -Wall -Wextra -Werror -std=c++20
GTEST_FLAGS = -lgtest -lgtest_main -pthread
GCOV_FLAGS = -fprofile-arcs -ftest-coverage --coverage
GCOVR = gcovr
GCOVR_FLAGS = -r . --html --html-details -o $(BUILD_DIR)/coverage_report/index.html
# Флаги линковщика
LDFLAGS = -lncurses -lgcov
# Каталоги
BUILD_DIR = install
# Исходные файлы на C++
CPP_SOURCES = \
	control/control_maze.cpp \
	control/control_cave.cpp \
	control/control_learning.cpp \
	model/maze.cpp \
	model/cave.cpp \
	model/learning.cpp \

# Файлы модульных тестов
TEST_SOURCES =  \
	test/test_maze.cpp \
	test/test_cave.cpp \
	test/test_learning.cpp \
	test/test_main.cpp
# Файлы для dist
SRC_FILES := model control view test Makefile documantation.tex
# Объектные файлы
CPP_OBJS = $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(CPP_SOURCES))
TEST_OBJS = $(patsubst test/%.cpp,$(BUILD_DIR)/test/%.o,$(TEST_SOURCES))

# Компиляция C++ файлов
$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(GCOV_FLAGS) -c $< -o $@

# Компиляция тестовых файлов
$(BUILD_DIR)/test/%.o: test/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(GCOV_FLAGS) -c $< -o $@

# Сборка основного тестового исполняемого файла
$(BUILD_DIR)/all_tests: $(TEST_OBJS) $(CPP_OBJS)
	@mkdir -p $(BUILD_DIR)/test
	$(CXX) $(CXXFLAGS) $(GCOV_FLAGS) $^ -o $@ $(GTEST_FLAGS) $(LDFLAGS)

# Цель по умолчанию
all: clean install start

# Установка в директорию install
install:
	cmake -S ./view/ -B ./$(BUILD_DIR) && cd ./$(BUILD_DIR) && make

start:
	./$(BUILD_DIR)/maze_view

# Запуск всех тестов
test: $(BUILD_DIR)/all_tests
	./$(BUILD_DIR)/all_tests

# Отчет о покрытии для всех тестов
gcov-report: test
	@mkdir -p $(BUILD_DIR)/coverage_report
	# Собираем данные покрытия
	lcov --capture --directory . --output-file $(BUILD_DIR)/coverage.info --no-external --ignore-errors mismatch
	# Исключаем тестовые файлы
	lcov --remove $(BUILD_DIR)/coverage.info '*/test/*' --output-file $(BUILD_DIR)/coverage.info
	# Генерируем HTML-отчет
	genhtml $(BUILD_DIR)/coverage.info --output-directory $(BUILD_DIR)/coverage_report
	@echo "Отчет о покрытии сгенерирован в $(BUILD_DIR)/coverage_report/index.html"
	xdg-open $(BUILD_DIR)/coverage_report/index.html 2>/dev/null || open $(BUILD_DIR)/coverage_report/index.html 2>/dev/null || start $(BUILD_DIR)/coverage_report/index.html 2>/dev/null || echo "Открыть в браузере: $(BUILD_DIR)/coverage_report/index.html"

# Документация в формате PDF
dvi:
	xelatex documantation.tex
	xdg-open documantation.pdf

# Создание архива с исходниками
dist:
	tar cvzf Maze.tgz $(SRC_FILES)

# Очистка сборки
clean:
	rm -rf $(BUILD_DIR)
	rm -rf Maze.tgz
	rm -rf dvi
	find . -name "*.gcda" -delete
	find . -name "*.gcno" -delete
	rm -rf documantation.aux
	rm -rf documantation.log
	rm -rf documantation.pdf

# Удаление сборки
uninstall:
	rm -rf install

# Проверка форматирования кода
clang-format:
	@echo "Checking code format with clang-format..."
	@find . \( -name "*.c" -o -name "*.h" -o -name "*.cpp" \) -print0 | xargs -0 clang-format -n
	@echo "Format check completed."

# Автоматическое форматирование кода
format:
	@echo "Formatting code with clang-format..."
	@find . \( -name "*.c" -o -name "*.h" -o -name "*.cpp" \) -print0 | xargs -0 clang-format -i
	@echo "Code formatting completed."

# Запуск программы под valgrind
valgrind: install
	valgrind --leak-check=full --show-reachable=no ./$(BUILD_DIR)/maze_view

# Запуск CPP Check для проверки кода
cpp_check:
	@echo "Запуск проверки кода с помощью cppcheck..."
	cppcheck --enable=all --inconclusive --error-exitcode=1 --force --std=c++20 --suppress=missingIncludeSystem --suppress=unusedFunction $(CPP_SOURCES)
