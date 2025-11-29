#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "cavewidget.h"
#include "learningwidget.h"
#include "mazewidget.h"
#include <QAction>
#include <QActionGroup>
#include <QMainWindow>
#include <QPushButton>
#include <QStackedWidget>
#include <QToolBar>
#include <QVBoxLayout>
#include <QWidget>

namespace s21 {

class MainWindow : public QMainWindow {
  Q_OBJECT
public:
  explicit MainWindow(QWidget *parent = nullptr);

private slots:
  void showApp1();
  void showApp2();
  void showApp3();

private:
  QWidget *app1Widget;
  QWidget *app2Widget;
  QWidget *app3Widget;
  QStackedWidget *stackedWidget;
  QAction *app1Action;
  QAction *app2Action;
  QAction *app3Action;
};
} // namespace s21

#endif // MAINWINDOW_H
