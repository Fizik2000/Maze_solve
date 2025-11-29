#ifndef MAZEWIDGET_H
#define MAZEWIDGET_H

#include "../control/control_maze.h"
#include "../model/maze.h"
#include <QCheckBox>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPainter>
#include <QPushButton>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QWidget>

namespace s21 {

class MazeWidget : public QWidget {
  Q_OBJECT

protected:
  void paintEvent(QPaintEvent *event) override;

public:
  MazeWidget(QWidget *parent = nullptr);
  void createButtonAndBox();
  void updateSolveSpinBoxes();
  void setupLayout();
  void setMessageBox(const QString &title, const QString &message);
  void drawPath(QPainter &painter, int startX, int startY, float cellWidth,
                float cellHeight);

private slots:
  void onOpenFileClicked();
  void onSaveFileClicked();
  void onGenerateClicked();
  void onSolveClicked();

private:
  ControlMaze control;
  //  Buttons
  QPushButton *button_open_file;
  QPushButton *button_save_file;
  QPushButton *button_generate;
  QPushButton *button_solve;

  // Boxes
  QSpinBox *spin_box_gen_row;
  QSpinBox *spin_box_gen_col;
  QSpinBox *spin_box_sol_start_row;
  QSpinBox *spin_box_sol_start_col;
  QSpinBox *spin_box_sol_end_row;
  QSpinBox *spin_box_sol_end_col;
};
} // namespace s21

#endif // MAZEWIDGET_H
