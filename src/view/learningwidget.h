#ifndef LEARNINGWIDGET_H
#define LEARNINGWIDGET_H

#include "../control/control_learning.h"
#include "../model/learning.h"
#include <QApplication>
#include <QDoubleSpinBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QPainter>
#include <QProgressBar>
#include <QPushButton>
#include <QSpinBox>
#include <QTextEdit>
#include <QWidget>

namespace s21 {

class LearningWidget : public QWidget {
  Q_OBJECT

public:
  explicit LearningWidget(QWidget *parent = nullptr);

protected:
  void paintEvent(QPaintEvent *event) override;

private slots:
  void onOpenFileClicked();
  void onQLearningClicked();
  void onFindPathClicked();
  void onSaveQTableClicked();
  void onResetClicked();

private:
  void createButtonAndBox();
  void setupLayout();
  void setMessageBox(const QString &title, const QString &message);
  void update_point_spin_boxes();
  void drawPath(QPainter &painter, int startX, int startY, float cellWidth,
                float cellHeight);
  void enabledButtonAndBoxFlagOpenFile();
  void updateStatistics();
  bool checkLearning;

  // Q-learning компоненты
  ControlLearning control;

  // Buttons
  QPushButton *button_open_file;
  QPushButton *button_q_learning;
  QPushButton *button_find_path;
  QPushButton *button_save_q_table;
  QPushButton *button_reset;

  // Boxes
  QSpinBox *spin_box_end_row;
  QSpinBox *spin_box_end_col;
  QSpinBox *spin_box_start_row;
  QSpinBox *spin_box_start_col;
  QDoubleSpinBox *spin_box_alfa;
  QDoubleSpinBox *spin_box_gamma;
  QDoubleSpinBox *spin_box_epsilon;
  QSpinBox *spin_box_episodes;
  QSpinBox *spin_box_max_steps;

  QLabel *status_label;       // Текущий статус
  QLabel *label_success_rate; // Процент успешных эпизодов
  QLabel *label_avg_reward;   // Средняя награда
  QLabel *label_avg_steps;    // Среднее количество шагов
  QLabel *label_current_epsilon; // Текущее значение epsilon
};
} // namespace s21

#endif // LEARNINGWIDGET_H
