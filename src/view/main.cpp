#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  s21::MainWindow mainWindow;
  mainWindow.setWindowTitle("A1_Maze_CPP");
  mainWindow.resize(400, 300);
  mainWindow.show();
  return app.exec();
}
