#include "MainWinUI.h"

#include <QApplication>

int main(int argc, char* argv[]){

  QApplication a(argc, argv);
  MainWinUI w;

  w.show();

  return a.exec();
}
