#include "mainwindow.h"

namespace s21 {

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  // Создаем тулбар
  QToolBar *toolBar = addToolBar("Navigation");

  // Создаем действия
  app1Action = new QAction("Maze", this);
  app2Action = new QAction("Cave", this);
  app3Action = new QAction("Q-Learning", this);

  // Делаем их "выбираемыми" (checkable)
  app1Action->setCheckable(true);
  app2Action->setCheckable(true);
  app3Action->setCheckable(true);

  // Группируем действия, чтобы только одна могла быть активной
  QActionGroup *actionGroup = new QActionGroup(this);
  actionGroup->addAction(app1Action);
  actionGroup->addAction(app2Action);
  actionGroup->addAction(app3Action);
  actionGroup->setExclusive(true);

  // Добавляем в тулбар
  toolBar->addAction(app1Action);
  toolBar->addAction(app2Action);
  toolBar->addAction(app3Action);

  // Создаем виджеты для приложений (здесь просто разные цвета для примера)
  app1Widget = new s21::MazeWidget();
  app1Widget->setStyleSheet("background-color: lightblue;");
  app2Widget = new s21::CaveWidget();
  app2Widget->setStyleSheet("background-color: lightgreen;");
  app3Widget = new s21::LearningWidget();
  app3Widget->setStyleSheet("background-color: rgb(255, 239, 213);");

  // Создаем QStackedWidget и добавляем туда виджеты
  stackedWidget = new QStackedWidget();
  stackedWidget->addWidget(app1Widget);
  stackedWidget->addWidget(app2Widget);
  stackedWidget->addWidget(app3Widget);

  // Устанавливаем как центральный
  setCentralWidget(stackedWidget);

  // Связываем действия
  connect(app1Action, &QAction::triggered, this, &MainWindow::showApp1);
  connect(app2Action, &QAction::triggered, this, &MainWindow::showApp2);
  connect(app3Action, &QAction::triggered, this, &MainWindow::showApp3);

  // Изначально выбираем первое
  app1Action->setChecked(true);
  showApp1();
}

void MainWindow::showApp1() { stackedWidget->setCurrentIndex(0); }

void MainWindow::showApp2() { stackedWidget->setCurrentIndex(1); }

void MainWindow::showApp3() { stackedWidget->setCurrentIndex(2); }

} // namespace s21