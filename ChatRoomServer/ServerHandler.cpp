#include <QDebug>
#include "ServerHandler.h"

ServerHandler::ServerHandler(){

}

void ServerHandler::handle(QTcpSocket& tcp, TextMessage& message){

  qDebug() << &tcp;
  qDebug() << message.type();
  qDebug() << message.length();
  qDebug() << message.data();

  if( message.type() == "CONN" ){


  }else if( message.type() == "DSNN" ){


  }
}
