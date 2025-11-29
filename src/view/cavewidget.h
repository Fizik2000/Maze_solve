#ifndef CAVEWIDGET_H
#define CAVEWIDGET_H

#include "../control/control_cave.h"
#include "../model/cave.h"
#include <QCheckBox>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPainter>
#include <QPushButton>
#include <QSpinBox>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>

namespace s21 {

class CaveWidget : public QWidget {
  Q_OBJECT

protected:
  void paintEvent(QPaintEvent *event) override;

public:
  CaveWidget(QWidget *parent = nullptr);

private:
  void performSingleStep();
  void createButtonAndBox();
  void setupLayout();
  void setMessageBox(const QString &title, const QString &message);
  void startAutoMode();
  void stopAutoMode();

private slots:
  void onOpenFileClicked();
  void onSaveFileClicked();
  void onGenerateClicked();
  void onCellularAvtomatClicked();
  void onTimerTimeout();

private:
  ControlCave control;
  QTimer *timer;
  bool isAutoMode;

  //  Buttons
  QPushButton *button_open_file;
  QPushButton *button_save_file;
  QPushButton *button_generate;
  QPushButton *button_cellular_avtomat;

  // Boxes
  QSpinBox *spin_box_gen_row;
  QSpinBox *spin_box_gen_col;
  QSpinBox *spin_box_gen_init;
  QSpinBox *spin_box_cel_life;
  QSpinBox *spin_box_cel_dead;
  QLineEdit *line_edit_time;
  QCheckBox *check_avtomat;
};
} // namespace s21

#endif // CAVEWIDGET_H
