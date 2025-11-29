#include "learningwidget.h"
#include <QFileDialog>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QVBoxLayout>

namespace s21 {

LearningWidget::LearningWidget(QWidget *parent) : QWidget(parent), control() {
  setFixedSize(750, 600);
  checkLearning = false;
  // Можно установить стиль для основного окна, но это в конце
  createButtonAndBox();
  setupLayout();

  // Функции обработки нажатия кнопок
  connect(button_open_file, &QPushButton::clicked, this,
          &LearningWidget::onOpenFileClicked);
  connect(button_q_learning, &QPushButton::clicked, this,
          &LearningWidget::onQLearningClicked);
  connect(button_find_path, &QPushButton::clicked, this,
          &LearningWidget::onFindPathClicked);
  connect(button_save_q_table, &QPushButton::clicked, this,
          &LearningWidget::onSaveQTableClicked);
  connect(button_reset, &QPushButton::clicked, this,
          &LearningWidget::onResetClicked);
  connect(spin_box_start_row, &QSpinBox::valueChanged, this, [=](int newValue) {
    control.resetPath();
    update();
  });
  connect(spin_box_start_col, &QSpinBox::valueChanged, this, [=](int newValue) {
    control.resetPath();
    update();
  });
  connect(spin_box_end_row, &QSpinBox::valueChanged, this, [=](int newValue) {
    control.resetPath();
    control.resetQTable();
    updateStatistics();
    update();
  });
  connect(spin_box_end_col, &QSpinBox::valueChanged, this, [=](int newValue) {
    control.resetPath();
    control.resetQTable();
    updateStatistics();
    update();
  });
}

// Создание органов управления
void LearningWidget::createButtonAndBox() {
  // Создание Buttons
  button_open_file = new QPushButton("Open File", this);
  button_q_learning = new QPushButton("Q-Learning", this);
  button_find_path = new QPushButton("Find Path", this);
  button_save_q_table = new QPushButton("Save Q-Table", this);
  button_reset = new QPushButton("Reset", this);

  // Создание Spin Boxes
  spin_box_end_row = new QSpinBox(this);
  spin_box_end_col = new QSpinBox(this);
  spin_box_start_row = new QSpinBox(this);
  spin_box_start_col = new QSpinBox(this);
  spin_box_alfa = new QDoubleSpinBox(this);
  spin_box_gamma = new QDoubleSpinBox(this);
  spin_box_epsilon = new QDoubleSpinBox(this);
  spin_box_episodes = new QSpinBox(this);
  spin_box_max_steps = new QSpinBox(this);

  // enabledButtonAndBoxFlagOpenFile();

  // Установка Boxes
  spin_box_end_row->setMinimum(0);
  spin_box_end_row->setMaximum(49);
  spin_box_end_col->setMinimum(0);
  spin_box_end_col->setMaximum(49);

  spin_box_alfa->setMinimum(0.01);
  spin_box_alfa->setMaximum(1.0);
  spin_box_alfa->setValue(0.1);
  spin_box_alfa->setSingleStep(0.01);

  spin_box_gamma->setMinimum(0.01);
  spin_box_gamma->setMaximum(1.0);
  spin_box_gamma->setValue(0.9);
  spin_box_gamma->setSingleStep(0.01);

  spin_box_epsilon->setMinimum(0.01);
  spin_box_epsilon->setMaximum(1.0);
  spin_box_epsilon->setValue(0.3);
  spin_box_epsilon->setSingleStep(0.01);

  spin_box_episodes->setMinimum(100);
  spin_box_episodes->setMaximum(10000);
  spin_box_episodes->setValue(10000);
  spin_box_episodes->setSingleStep(100);

  spin_box_max_steps->setMinimum(50);
  spin_box_max_steps->setMaximum(2500);
  spin_box_max_steps->setValue(2500);
  spin_box_max_steps->setSingleStep(50);

  spin_box_start_row->setMinimum(0);
  spin_box_start_row->setMaximum(49);
  spin_box_start_col->setMinimum(0);
  spin_box_start_col->setMaximum(49);

  // Инициализация элементов статуса
  status_label = new QLabel("Status: Ready", this);
  label_success_rate = new QLabel("Success rate: N/A", this);
  label_avg_reward = new QLabel("Average reward: N/A", this);
  label_avg_steps = new QLabel("Average steps: N/A", this);
  // label_current_epsilon = new QLabel("Current epsilon: N/A", this);
}

// Установка слоев
void LearningWidget::setupLayout() {
  QHBoxLayout *mainLayout = new QHBoxLayout(this);
  QVBoxLayout *controlLayout = new QVBoxLayout();

  // Настройка панели управления
  controlLayout->setContentsMargins(10, 10, 10, 10);
  controlLayout->setSpacing(10);

  controlLayout->addWidget(button_open_file);

  QFrame *line1 = new QFrame();
  line1->setFrameShape(QFrame::HLine);
  line1->setFrameShadow(QFrame::Sunken);
  line1->setStyleSheet("background-color: #c0c0c0;");
  controlLayout->addWidget(line1);

  QHBoxLayout *set_end_point_layout = new QHBoxLayout();
  controlLayout->addWidget(new QLabel("End Point:", this), 0, Qt::AlignCenter);
  set_end_point_layout->addWidget(new QLabel("Rows:", this), 0, Qt::AlignLeft);
  set_end_point_layout->addWidget(spin_box_end_row);
  set_end_point_layout->addWidget(new QLabel("Cols:", this), 0, Qt::AlignLeft);
  set_end_point_layout->addWidget(spin_box_end_col);
  controlLayout->addLayout(set_end_point_layout);

  QFrame *line2 = new QFrame();
  line2->setFrameShape(QFrame::HLine);
  line2->setFrameShadow(QFrame::Sunken);
  line2->setStyleSheet("background-color: #c0c0c0;");
  controlLayout->addWidget(line2);

  QHBoxLayout *alfa_layout = new QHBoxLayout();
  alfa_layout->addWidget(new QLabel("Alfa:", this), 0, Qt::AlignLeft);
  alfa_layout->addWidget(spin_box_alfa, 0, Qt::AlignCenter);
  alfa_layout->addWidget(new QLabel("[0.01 - 1.0]", this), 0, Qt::AlignLeft);
  controlLayout->addLayout(alfa_layout);

  QHBoxLayout *gamma_layout = new QHBoxLayout();
  gamma_layout->addWidget(new QLabel("Gamma:", this), 0, Qt::AlignLeft);
  gamma_layout->addWidget(spin_box_gamma, 0, Qt::AlignCenter);
  gamma_layout->addWidget(new QLabel("[0.01 - 1.0]", this), 0, Qt::AlignLeft);
  controlLayout->addLayout(gamma_layout);

  QHBoxLayout *epsilon_layout = new QHBoxLayout();
  epsilon_layout->addWidget(new QLabel("Epsilon:", this), 0, Qt::AlignLeft);
  epsilon_layout->addWidget(spin_box_epsilon, 0, Qt::AlignCenter);
  epsilon_layout->addWidget(new QLabel("[0.01 - 1.0]", this), 0, Qt::AlignLeft);
  controlLayout->addLayout(epsilon_layout);

  QHBoxLayout *episodes_layout = new QHBoxLayout();
  episodes_layout->addWidget(new QLabel("Episodes:", this), 0, Qt::AlignLeft);
  episodes_layout->addWidget(spin_box_episodes, 0, Qt::AlignCenter);
  episodes_layout->addWidget(new QLabel("[100 - 10 000]", this), 0,
                             Qt::AlignLeft);
  controlLayout->addLayout(episodes_layout);

  QHBoxLayout *max_steps_layout = new QHBoxLayout();
  max_steps_layout->addWidget(new QLabel("Max steps:", this), 0, Qt::AlignLeft);
  max_steps_layout->addWidget(spin_box_max_steps, 0, Qt::AlignCenter);
  max_steps_layout->addWidget(new QLabel("[50 - 2500]", this), 0,
                              Qt::AlignLeft);
  controlLayout->addLayout(max_steps_layout);
  controlLayout->addWidget(button_q_learning);

  QHBoxLayout *status_layout = new QHBoxLayout();
  status_layout->addWidget(status_label);
  controlLayout->addLayout(status_layout);

  QFrame *line3 = new QFrame();
  line3->setFrameShape(QFrame::HLine);
  line3->setFrameShadow(QFrame::Sunken);
  line3->setStyleSheet("background-color: #c0c0c0;");
  controlLayout->addWidget(line3);

  QHBoxLayout *set_start_point_layout = new QHBoxLayout();
  set_start_point_layout->addWidget(new QLabel("Rows:", this), 0,
                                    Qt::AlignLeft);
  set_start_point_layout->addWidget(spin_box_start_row);
  set_start_point_layout->addWidget(new QLabel("Cols:", this), 0,
                                    Qt::AlignLeft);
  set_start_point_layout->addWidget(spin_box_start_col);
  controlLayout->addLayout(set_start_point_layout);
  controlLayout->addWidget(button_find_path);

  QFrame *line4 = new QFrame();
  line4->setFrameShape(QFrame::HLine);
  line4->setFrameShadow(QFrame::Sunken);
  line4->setStyleSheet("background-color: #c0c0c0;");
  controlLayout->addWidget(line4);

  controlLayout->addWidget(button_save_q_table);

  QFrame *line5 = new QFrame();
  line5->setFrameShape(QFrame::HLine);
  line5->setFrameShadow(QFrame::Sunken);
  line5->setStyleSheet("background-color: #c0c0c0;");
  controlLayout->addWidget(line5);

  controlLayout->addWidget(button_reset);

  controlLayout->addWidget(label_success_rate);
  controlLayout->addWidget(label_avg_reward);
  controlLayout->addWidget(label_avg_steps);
  // controlLayout->addWidget(label_current_epsilon);

  controlLayout->addStretch();

  // Добавляем только панель управления
  mainLayout->addLayout(controlLayout);

  // Устанавливаем отступы для области отрисовки
  mainLayout->setContentsMargins(500, 0, 0, 0); // Слева 500px для лабиринта
  setLayout(mainLayout);
}

// Открытие файла с лабиринтом
void LearningWidget::onOpenFileClicked() {
  QString file_path = QFileDialog::getOpenFileName(
      this, "Open Maze File", "../datasets", "Text Files (*.txt)");

  if (!file_path.isEmpty()) {
    try {
      control.loadFromFile(file_path.toStdString());
      update_point_spin_boxes();

      // Обновляем статус
      status_label->setText("Status: Maze loaded successfully");
      update(); // Перерисовываем виджет

    } catch (const std::exception &e) {
      status_label->setText("Status: Error loading file");
      setMessageBox("File upload error", QString("Error: %1").arg(e.what()));
    }
  }
}

// Обучение агента
void LearningWidget::onQLearningClicked() {
  try {
    status_label->setText("Status: Training in progress...");

    // Установка параметров
    control.setAlpha(spin_box_alfa->value());
    control.setGamma(spin_box_gamma->value());
    control.setEpsilon(spin_box_epsilon->value());

    // Запуск обучения
    control.train(spin_box_episodes->value(), spin_box_max_steps->value(),
                  {spin_box_end_row->value(), spin_box_end_col->value()});
    status_label->setText("Status: Training completed");

    // Для average reward и steps можно добавить соответствующие методы в
    // Learning
    updateStatistics();
    checkLearning = true;
    update(); // Перерисовываем виджет

  } catch (const std::exception &e) {
    status_label->setText("Status: Training failed");
    setMessageBox("Training error", QString("Error: %1").arg(e.what()));
  }
}

// Поиск пути
void LearningWidget::onFindPathClicked() {
  try {
    status_label->setText("Status: Finding path...");

    // Поиск пути
    control.findPathFromStart(
        {spin_box_start_row->value(), spin_box_start_col->value()});
    auto path = control.getFoundPath();
    status_label->setText("Status: Path found");

    update(); // Перерисовываем виджет чтобы показать путь

    QMessageBox::information(
        this, "Path Found",
        QString("Path found with %1 steps").arg(path.size()));

  } catch (const std::exception &e) {
    status_label->setText("Status: Path finding failed");
    setMessageBox("Path finding error", QString("Error: %1").arg(e.what()));
  }
}

// Сохранение Q-таблицы
void LearningWidget::onSaveQTableClicked() {
  QString file_path = QFileDialog::getSaveFileName(this, "Save Q-Table", "",
                                                   "Q-Table Files (*.scv)");

  if (!file_path.isEmpty()) {
    try {
      control.saveQTable(file_path.toStdString());
      status_label->setText("Status: Q-Table saved");
    } catch (const std::exception &e) {
      status_label->setText("Status: Save failed");
      setMessageBox("Save error", QString("Error: %1").arg(e.what()));
    }
  }
}

// Сброс
void LearningWidget::onResetClicked() {
  control.resetPath();
  control.resetQTable();
  checkLearning = false;
  update();
  updateStatistics();
}

// Вывод информации в отдельное окно
void LearningWidget::setMessageBox(const QString &title,
                                   const QString &message) {
  QMessageBox msgBox(this);
  msgBox.setIcon(QMessageBox::Critical);
  msgBox.setWindowTitle(title);
  msgBox.setText(message);
  msgBox.exec();
}

void LearningWidget::updateStatistics() {
  int episodes = spin_box_episodes->value();
  label_success_rate->setText(
      "Success rate: " + QString::number(control.getSuccessRate()) + "%");
  label_avg_reward->setText(
      "Average reward: " +
      QString::number(control.getTotalReward() / episodes));
  label_avg_steps->setText("Average steps: " +
                           QString::number(control.getTotalStep() / episodes));
  // label_current_epsilon->setText("Current epsilon: " +
  // QString::number(control.getEpsilon()));
}

// Обновление диапазонов spin box
void LearningWidget::update_point_spin_boxes() {
  int row = control.getRows() - 1;
  int col = control.getCols() - 1;

  spin_box_end_col->setMinimum(0);
  spin_box_end_col->setMaximum(col);
  spin_box_end_row->setMinimum(0);
  spin_box_end_row->setMaximum(row);
  spin_box_end_col->setValue(col);
  spin_box_end_row->setValue(row);

  spin_box_start_row->setMinimum(0);
  spin_box_start_row->setMaximum(row);
  spin_box_start_col->setMinimum(0);
  spin_box_start_col->setMaximum(col);
  spin_box_start_row->setValue(0);
  spin_box_start_col->setValue(0);
}

// Отрисовка поля
void LearningWidget::paintEvent(QPaintEvent *event) {
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

// Отрисовка пути
void LearningWidget::drawPath(QPainter &painter, int startX, int startY,
                              float cellWidth, float cellHeight) {
  // Получение пути
  auto path = control.getFoundPath();

  // Проверяем что путь не пустой
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
    int start_row = spin_box_start_row->value();
    int start_col = spin_box_start_col->value();
    int end_row = spin_box_end_row->value();
    int end_col = spin_box_end_col->value();

    int rows = control.getRows();
    int cols = control.getCols();

    if (start_row >= 0 && start_row - 1 < rows && start_col >= 0 &&
        start_col - 1 < cols && end_row >= 0 && end_row - 1 < rows &&
        end_col >= 0 && end_col - 1 < cols) {
      // Рисуем начальную и конечную точки
      painter.setPen(Qt::NoPen);

      if (checkLearning) {
        // Начальная точка (зеленая)
        painter.setBrush(Qt::green);
        painter.drawEllipse(startX + (start_col + 0.25f) * cellWidth,
                            startY + (start_row + 0.25f) * cellHeight,
                            cellWidth * 0.5f, cellHeight * 0.5f);
      }
      // Конечная точка (красная)
      painter.setBrush(Qt::red);
      painter.drawEllipse(startX + (end_col + 0.25f) * cellWidth,
                          startY + (end_row + 0.25f) * cellHeight,
                          cellWidth * 0.5f, cellHeight * 0.5f);
    }
  }
}

} // namespace s21
