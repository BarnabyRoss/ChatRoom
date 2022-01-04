#include <QCoreApplication>
#include "Server.h"
#include "ServerHandler.h"

int main(int argc, char* argv[]){

  QCoreApplication a(argc, argv);

  ServerHandler handler;
  Server server;

  server.start(QHostAddress("127.0.0.1"), 8080);
  server.setHandler(&handler);

  return a.exec();
}
