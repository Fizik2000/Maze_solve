#include "cavewidget.h"

namespace s21 {

CaveWidget::CaveWidget(QWidget *parent)
    : QWidget(parent), control(), isAutoMode(false) {
  setFixedSize(750, 600);
  // Можно установить стиль для основного окна, но это в конце
  createButtonAndBox();
  setupLayout();

  // Инициализация таймера
  timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &CaveWidget::onTimerTimeout);

  // Функции обработки нажатия кнопок
  connect(button_open_file, &QPushButton::clicked, this,
          &CaveWidget::onOpenFileClicked);
  connect(button_save_file, &QPushButton::clicked, this,
          &CaveWidget::onSaveFileClicked);
  connect(button_generate, &QPushButton::clicked, this,
          &CaveWidget::onGenerateClicked);
  connect(button_cellular_avtomat, &QPushButton::clicked, this,
          &CaveWidget::onCellularAvtomatClicked);
  connect(check_avtomat, &QCheckBox::toggled, this, [=](bool checked) {
    // Если чекбокс отключен (автомат выключен)
    line_edit_time->setEnabled(checked);

    if (checked) {
      button_cellular_avtomat->setText("Start Auto");
    } else {
      button_cellular_avtomat->setText("Next Step");
      stopAutoMode();
    }
  });
}

//
void CaveWidget::createButtonAndBox() {
  // Создание Buttons
  button_open_file = new QPushButton("Open File", this);
  button_save_file = new QPushButton("Save File", this);
  button_generate = new QPushButton("Generate Cave", this);
  button_cellular_avtomat = new QPushButton("Start Auto", this);

  // Создание Spin Boxes
  spin_box_gen_row = new QSpinBox(this);
  spin_box_gen_col = new QSpinBox(this);
  spin_box_gen_init = new QSpinBox(this);
  spin_box_cel_life = new QSpinBox(this);
  spin_box_cel_dead = new QSpinBox(this);
  line_edit_time = new QLineEdit("500", this);
  check_avtomat = new QCheckBox(this);

  // Установка min и max Boxes
  spin_box_gen_row->setMinimum(2);
  spin_box_gen_row->setMaximum(50);
  spin_box_gen_col->setMinimum(2);
  spin_box_gen_col->setMaximum(50);
  spin_box_gen_init->setMinimum(0);
  spin_box_gen_init->setMaximum(100);
  spin_box_cel_life->setMinimum(0);
  spin_box_cel_life->setMaximum(7);
  spin_box_cel_dead->setMinimum(0);
  spin_box_cel_dead->setMaximum(7);
  check_avtomat->setChecked(true);
  line_edit_time->setValidator(new QIntValidator(10, 5000, this));
}

//
void CaveWidget::setupLayout() {
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

  QHBoxLayout *size_cave_row_layout = new QHBoxLayout();
  size_cave_row_layout->addWidget(new QLabel("Rows:", this), 0,
                                  Qt::AlignCenter);
  size_cave_row_layout->addWidget(spin_box_gen_row);
  controlLayout->addLayout(size_cave_row_layout);

  QHBoxLayout *size_cave_col_layout = new QHBoxLayout();
  size_cave_col_layout->addWidget(new QLabel("Cols:", this), 0,
                                  Qt::AlignCenter);
  size_cave_col_layout->addWidget(spin_box_gen_col);
  controlLayout->addLayout(size_cave_col_layout);

  QHBoxLayout *cave_init_layout = new QHBoxLayout();
  cave_init_layout->addWidget(new QLabel("Chance:", this), 0, Qt::AlignCenter);
  cave_init_layout->addWidget(spin_box_gen_init);
  controlLayout->addLayout(cave_init_layout);

  controlLayout->addWidget(button_generate);

  QFrame *line2 = new QFrame();
  line2->setFrameShape(QFrame::HLine);
  line2->setFrameShadow(QFrame::Sunken);
  line2->setStyleSheet("background-color: #c0c0c0;");
  controlLayout->addWidget(line2);

  controlLayout->addWidget(new QLabel("Cellular Avtomat", this), 0,
                           Qt::AlignCenter);
  QHBoxLayout *cave_avto_layout = new QHBoxLayout();
  cave_avto_layout->addWidget(new QLabel("Avto:", this), 0, Qt::AlignCenter);
  cave_avto_layout->addWidget(check_avtomat, 0, Qt::AlignCenter);
  controlLayout->addLayout(cave_avto_layout);

  QHBoxLayout *cave_time_layout = new QHBoxLayout();
  cave_time_layout->addWidget(new QLabel("Time:", this));
  cave_time_layout->addWidget(line_edit_time);
  controlLayout->addLayout(cave_time_layout);

  QHBoxLayout *cave_life_layout = new QHBoxLayout();
  cave_life_layout->addWidget(new QLabel("Life:", this), 0, Qt::AlignCenter);
  cave_life_layout->addWidget(spin_box_cel_life);
  controlLayout->addLayout(cave_life_layout);

  QHBoxLayout *cave_dead_layout = new QHBoxLayout();
  cave_dead_layout->addWidget(new QLabel("Dead:", this), 0, Qt::AlignCenter);
  cave_dead_layout->addWidget(spin_box_cel_dead);
  controlLayout->addLayout(cave_dead_layout);

  controlLayout->addWidget(button_cellular_avtomat);
  controlLayout->addStretch();

  // Добавляем только панель управления
  mainLayout->addLayout(controlLayout);

  // Устанавливаем отступы для области отрисовки
  mainLayout->setContentsMargins(500, 0, 0, 0); // Слева 500px для лабиринта

  setLayout(mainLayout);
}

//
void CaveWidget::onOpenFileClicked() {
  QString file_path = QFileDialog::getOpenFileName(
      nullptr, "Open Maze File", "../datasets", "Text Files (*.txt)");

  if (!file_path.isEmpty()) {
    try {
      control.loadFromFile(file_path.toStdString());
      update();
    } catch (const std::runtime_error &e) {
      setMessageBox("File upload error",
                    QString("Incorrect file format: %1").arg(e.what()));
    }
  }
}

//
void CaveWidget::onSaveFileClicked() {
  QString file_path = QFileDialog::getSaveFileName(nullptr, "", "../datasets",
                                                   "Text Files (*.txt)");
  if (!file_path.isEmpty()) {
    if (!file_path.endsWith(".txt")) {
      file_path += ".txt";
    }
    control.saveToFile(file_path.toStdString());
  }
}

//
void CaveWidget::onGenerateClicked() {
  try {
    control.generate(spin_box_gen_row->value(), spin_box_gen_col->value(),
                     spin_box_gen_init->value());
    update();
  } catch (const std::runtime_error &e) {
    setMessageBox("Cave generate error", QString(e.what()));
  }
}

//
void CaveWidget::setMessageBox(const QString &title, const QString &message) {
  QMessageBox msgBox(this);
  msgBox.setIcon(QMessageBox::Critical);
  msgBox.setWindowTitle(title);
  msgBox.setText(message);
  msgBox.exec();
}

void CaveWidget::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event);
  QPainter painter(this);

  // Очищаем всю область виджета
  painter.fillRect(rect(), Qt::white);

  const auto &cave = control.get_cave();
  int rows = cave.size();
  int cols = (rows > 0) ? cave[0].size() : 0;

  // Область для отрисовки пещеры - 500x500 пикселей
  int caveSize = 500;
  int startX = 0;
  int startY = 0;

  // ВСЕГДА рисуем внешнюю рамку (даже если пещера пустая)
  painter.setPen(QPen(Qt::black, 2));
  painter.setBrush(Qt::NoBrush);
  painter.drawRect(startX, startY, caveSize, caveSize);

  // Рассчитываем размер ячейки
  float cellWidth = static_cast<float>(caveSize) / cols;
  float cellHeight = static_cast<float>(caveSize) / rows;

  // Рисуем живые клетки
  painter.setPen(Qt::NoPen);
  painter.setBrush(Qt::black);

  for (int row = 0; row < rows; ++row) {
    for (int col = 0; col < cols; ++col) {
      if (cave[row][col]) {
        float x = startX + col * cellWidth;
        float y = startY + row * cellHeight;
        painter.drawRect(x, y, cellWidth, cellHeight);
      }
    }
  }
}

void CaveWidget::onCellularAvtomatClicked() {
  if (check_avtomat->isChecked()) {
    // Автоматический режим
    if (isAutoMode) {
      // Останавливаем, если уже запущен
      stopAutoMode();
      button_cellular_avtomat->setText("Start Auto");
    } else {
      // Запускаем автоматический режим
      startAutoMode();
      if (isAutoMode) {
        button_cellular_avtomat->setText("Stop Auto");
      }
    }
  } else {
    // Ручной режим - одиночный шаг
    performSingleStep();
  }
}

void CaveWidget::performSingleStep() {
  try {
    int birthLimit = spin_box_cel_life->value();
    int deathLimit = spin_box_cel_dead->value();

    // Выполняем один шаг клеточного автомата
    bool changes = control.cellular_automaton(birthLimit, deathLimit);
    update();

    if (!changes) {
      setMessageBox("Info", "No more changes - automaton stabilized");
    }
  } catch (const std::runtime_error &e) {
    setMessageBox("Cellular automaton error", QString(e.what()));
  }
}

void CaveWidget::startAutoMode() {
  const auto &cave = control.get_cave();
  if (cave.empty()) {
    setMessageBox("Error", "No cave generated or loaded");
    return;
  }

  QString timeText = line_edit_time->text();
  bool ok;
  int interval = timeText.toInt(&ok);

  if (!ok || interval < 10 || interval > 5000) {
    setMessageBox("Invalid time",
                  "Please enter a valid time between 10 and 5000 milliseconds");
    return;
  }

  isAutoMode = true;
  timer->start(interval);
}

void CaveWidget::stopAutoMode() {
  isAutoMode = false;
  timer->stop();
}

void CaveWidget::onTimerTimeout() {
  int birthLimit = spin_box_cel_life->value();
  int deathLimit = spin_box_cel_dead->value();

  try {
    // Выполняем шаг и проверяем, были ли изменения
    bool changes = control.cellular_automaton(birthLimit, deathLimit);
    update();

    // Если изменений не было - останавливаем авторежим
    if (!changes) {
      stopAutoMode();
      button_cellular_avtomat->setText("Start Auto");
      setMessageBox("Info", "Automaton stabilized - no more changes");
    }
  } catch (const std::runtime_error &e) {
    stopAutoMode();
    button_cellular_avtomat->setText("Start Auto");
    setMessageBox("Error",
                  QString("Cellular automaton error: %1").arg(e.what()));
  }
}

} // namespace s21
