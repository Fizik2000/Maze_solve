#include "mazewidget.h"

namespace s21 {

// Конструктор виджета
MazeWidget::MazeWidget(QWidget *parent) : QWidget(parent), control() {
  setFixedSize(750, 600);
  createButtonAndBox();
  setupLayout();

  // Функции обработки нажатия кнопок
  connect(button_open_file, &QPushButton::clicked, this,
          &MazeWidget::onOpenFileClicked);
  connect(button_save_file, &QPushButton::clicked, this,
          &MazeWidget::onSaveFileClicked);
  connect(button_generate, &QPushButton::clicked, this,
          &MazeWidget::onGenerateClicked);
  connect(button_solve, &QPushButton::clicked, this,
          &MazeWidget::onSolveClicked);
  connect(spin_box_sol_start_row, &QSpinBox::valueChanged, this,
          [=](int newValue) {
            control.resetPath();
            update();
          });
  connect(spin_box_sol_start_col, &QSpinBox::valueChanged, this,
          [=](int newValue) {
            control.resetPath();
            update();
          });
  connect(spin_box_sol_end_row, &QSpinBox::valueChanged, this,
          [=](int newValue) {
            control.resetPath();
            update();
          });
  connect(spin_box_sol_end_col, &QSpinBox::valueChanged, this,
          [=](int newValue) {
            control.resetPath();
            update();
          });
}

// Создание снопок и боксов
void MazeWidget::createButtonAndBox() {
  // Создание Buttons
  button_open_file = new QPushButton("Open File", this);
  button_save_file = new QPushButton("Save File", this);
  button_generate = new QPushButton("Generate Maze", this);
  button_solve = new QPushButton("Solve Maze", this);

  // Создание Spin Boxes
  spin_box_gen_row = new QSpinBox(this);
  spin_box_gen_col = new QSpinBox(this);
  spin_box_sol_start_row = new QSpinBox(this);
  spin_box_sol_start_col = new QSpinBox(this);
  spin_box_sol_end_row = new QSpinBox(this);
  spin_box_sol_end_col = new QSpinBox(this);

  // Установка min и max Boxes
  spin_box_gen_row->setMinimum(2);
  spin_box_gen_row->setMaximum(50);
  spin_box_gen_col->setMinimum(2);
  spin_box_gen_col->setMaximum(50);

  // Можно добавить стили, но это в конце
}

// Обновление SpinBox от загруженного лабиринта
void MazeWidget::updateSolveSpinBoxes() {
  // Запрашиваем действующие размеры
  int row = control.get_rows() - 1;
  int col = control.get_cols() - 1;

  // Обновляем значения для А Boxes
  spin_box_sol_start_row->setMinimum(0);
  spin_box_sol_start_col->setMinimum(0);
  spin_box_sol_start_row->setMaximum(row);
  spin_box_sol_start_col->setMaximum(col);
  spin_box_sol_start_row->setValue(0);
  spin_box_sol_start_col->setValue(0);

  // Обновляем значения для B Boxes
  spin_box_sol_end_row->setMinimum(0);
  spin_box_sol_end_col->setMinimum(0);
  spin_box_sol_end_row->setMaximum(row);
  spin_box_sol_end_col->setMaximum(col);
  spin_box_sol_end_row->setValue(row);
  spin_box_sol_end_col->setValue(col);
}

// Подключение кнопки загрузки лабиринта
void MazeWidget::onOpenFileClicked() {
  QString file_path = QFileDialog::getOpenFileName(
      nullptr, "Open Maze File", "../datasets", "Text Files (*.txt)");

  if (!file_path.isEmpty()) {
    try {
      control.loadFromFile(file_path.toStdString());
      updateSolveSpinBoxes();
      update();
    } catch (const std::runtime_error &e) {
      setMessageBox("File upload error",
                    QString("Incorrect file format: %1").arg(e.what()));
    }
  }
}

// Подключение кнопки сохранения лабиранта
void MazeWidget::onSaveFileClicked() {
  QString file_path = QFileDialog::getSaveFileName(nullptr, "", "../datasets",
                                                   "Text Files (*.txt)");
  if (!file_path.isEmpty()) {
    if (!file_path.endsWith(".txt")) {
      file_path += ".txt";
    }
    control.saveToFile(file_path.toStdString());
  }
}

// Подключение кнопки генерации
void MazeWidget::onGenerateClicked() {
  try {
    control.generate(spin_box_gen_row->value(), spin_box_gen_col->value());
    updateSolveSpinBoxes();
    update();
  } catch (const std::runtime_error &e) {
    setMessageBox("Maze generate error", QString(e.what()));
  }
}

// Подключение кнопки поиска пути
void MazeWidget::onSolveClicked() {
  int start_row = spin_box_sol_start_row->value();
  int start_col = spin_box_sol_start_col->value();
  int end_row = spin_box_sol_end_row->value();
  int end_col = spin_box_sol_end_col->value();

  try {
    control.solveMaze({start_row, start_col}, {end_row, end_col});
    update();
  } catch (const std::runtime_error &e) {
    setMessageBox("Maze solve error", QString(e.what()));
  }
}

// Всплывающее окно с информацией
void MazeWidget::setMessageBox(const QString &title, const QString &message) {
  QMessageBox msgBox(this);
  msgBox.setIcon(QMessageBox::Critical);
  msgBox.setWindowTitle(title);
  msgBox.setText(message);
  msgBox.exec();
}

// Создание слоев
void MazeWidget::setupLayout() {
  QHBoxLayout *mainLayout = new QHBoxLayout(this);
  QVBoxLayout *controlLayout = new QVBoxLayout();

  // Настройка панели управления
  controlLayout->setContentsMargins(10, 10, 10, 10);
  controlLayout->setSpacing(15);

  controlLayout->addWidget(button_open_file);
  controlLayout->addWidget(button_save_file);

  QFrame *line1 = new QFrame();
  line1->setFrameShape(QFrame::HLine);
  line1->setFrameShadow(QFrame::Sunken);
  line1->setStyleSheet("background-color: #c0c0c0;");
  controlLayout->addWidget(line1);

  QHBoxLayout *size_maze_row_layout = new QHBoxLayout();
  size_maze_row_layout->addWidget(new QLabel("Rows:", this));
  size_maze_row_layout->addWidget(spin_box_gen_row);
  controlLayout->addLayout(size_maze_row_layout);

  QHBoxLayout *size_maze_col_layout = new QHBoxLayout();
  size_maze_col_layout->addWidget(new QLabel("Cols:", this));
  size_maze_col_layout->addWidget(spin_box_gen_col);
  controlLayout->addLayout(size_maze_col_layout);

  controlLayout->addWidget(button_generate);

  QFrame *line2 = new QFrame();
  line2->setFrameShape(QFrame::HLine);
  line2->setFrameShadow(QFrame::Sunken);
  line2->setStyleSheet("background-color: #c0c0c0;");
  controlLayout->addWidget(line2);

  controlLayout->addWidget(new QLabel("Start Point:", this), 0,
                           Qt::AlignCenter);
  QHBoxLayout *size_solve_start_layout = new QHBoxLayout();
  size_solve_start_layout->addWidget(spin_box_sol_start_row);
  size_solve_start_layout->addWidget(spin_box_sol_start_col);
  controlLayout->addLayout(size_solve_start_layout);

  controlLayout->addWidget(new QLabel("End Point:", this), 0, Qt::AlignCenter);
  QHBoxLayout *size_solve_end_layout = new QHBoxLayout();
  size_solve_end_layout->addWidget(spin_box_sol_end_row);
  size_solve_end_layout->addWidget(spin_box_sol_end_col);
  controlLayout->addLayout(size_solve_end_layout);

  controlLayout->addWidget(button_solve);
  controlLayout->addStretch();

  // Добавляем только панель управления
  mainLayout->addLayout(controlLayout);

  // Устанавливаем отступы для области отрисовки
  mainLayout->setContentsMargins(500, 0, 0, 0); // Слева 500px для лабиринта

  setLayout(mainLayout);
}

// Отрисовка поля лабиринта
void MazeWidget::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event);
  QPainter painter(this);

  // Очищаем ВСЮ область виджета
  painter.fillRect(rect(), Qt::white);

  // Получаем матрицы стен
  const auto &verticalWalls = control.getVerticalWalls();
  const auto &horizontalWalls = control.getHorizontalWalls();

  int rows = verticalWalls.size();
  int cols = (rows > 0) ? verticalWalls[0].size() : 0;

  // Область для отрисовки лабиринта - 500x500 пикселей
  int mazeSize = 500;
  int startX = 0;
  int startY = 0;

  // Очищаем ТОЛЬКО область лабиринта (важно!)
  painter.fillRect(startX, startY, mazeSize, mazeSize, Qt::white);

  // Рассчитываем размер ячейки
  float cellWidth = static_cast<float>(mazeSize) / cols;
  float cellHeight = static_cast<float>(mazeSize) / rows;

  // Рисуем внешние границы
  painter.setPen(QPen(Qt::black, 2));
  painter.drawRect(startX, startY, mazeSize, mazeSize);

  // Рисуем вертикальные стены
  for (int row = 0; row < rows; ++row) {
    for (int col = 0; col < cols; ++col) {
      float x = startX + col * cellWidth;
      float y = startY + row * cellHeight;

      // Правая стена
      if (col < cols - 1 && verticalWalls[row][col]) {
        painter.drawLine(x + cellWidth, y, x + cellWidth, y + cellHeight);
      }

      // Нижняя стена
      if (row < rows - 1 && horizontalWalls[row][col]) {
        painter.drawLine(x, y + cellHeight, x + cellWidth, y + cellHeight);
      }
    }
  }

  // Рисуем путь, если он есть
  drawPath(painter, startX, startY, cellWidth, cellHeight);
}

// Отрисовка пути если он есть
void MazeWidget::drawPath(QPainter &painter, int startX, int startY,
                          float cellWidth, float cellHeight) {
  auto path = control.get_path();

  // Если путь пустой рисуем только точки
  if (!path.empty()) {
    // Рисуем линии пути
    painter.setPen(QPen(Qt::red, 2));
    for (size_t i = 0; i < path.size() - 1; ++i) {
      int row1 = path[i].first;
      int col1 = path[i].second;
      int row2 = path[i + 1].first;
      int col2 = path[i + 1].second;
      // Центры ячеек
      float x1 = startX + (col1 + 0.5f) * cellWidth;
      float y1 = startY + (row1 + 0.5f) * cellHeight;
      float x2 = startX + (col2 + 0.5f) * cellWidth;
      float y2 = startY + (row2 + 0.5f) * cellHeight;
      painter.drawLine(x1, y1, x2, y2);
    }
    // Рисуем начальную и конечную точки
    painter.setPen(Qt::NoPen);

    // Начальная точка (зеленая)
    painter.setBrush(Qt::green);
    painter.drawEllipse(startX + (path.front().second + 0.25f) * cellWidth,
                        startY + (path.front().first + 0.25f) * cellHeight,
                        cellWidth * 0.5f, cellHeight * 0.5f);

    // Конечная точка (красная)
    painter.setBrush(Qt::red);
    painter.drawEllipse(startX + (path.back().second + 0.25f) * cellWidth,
                        startY + (path.back().first + 0.25f) * cellHeight,
                        cellWidth * 0.5f, cellHeight * 0.5f);
  } else {
    int start_row = spin_box_sol_start_row->value();
    int start_col = spin_box_sol_start_col->value();
    int end_row = spin_box_sol_end_row->value();
    int end_col = spin_box_sol_end_col->value();

    int rows = control.get_rows();
    int cols = control.get_cols();

    if (start_row >= 0 && start_row - 1 < rows && start_col >= 0 &&
        start_col - 1 < cols && end_row >= 0 && end_row - 1 < rows &&
        end_col >= 0 && end_col - 1 < cols) {
      // Рисуем начальную и конечную точки
      painter.setPen(Qt::NoPen);

      // Начальная точка (зеленая)
      painter.setBrush(Qt::green);
      painter.drawEllipse(startX + (start_col + 0.25f) * cellWidth,
                          startY + (start_row + 0.25f) * cellHeight,
                          cellWidth * 0.5f, cellHeight * 0.5f);

      // Конечная точка (красная)
      painter.setBrush(Qt::red);
      painter.drawEllipse(startX + (end_col + 0.25f) * cellWidth,
                          startY + (end_row + 0.25f) * cellHeight,
                          cellWidth * 0.5f, cellHeight * 0.5f);
    }
  }
}

} // namespace s21
