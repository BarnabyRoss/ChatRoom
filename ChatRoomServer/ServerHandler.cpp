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


  }else if( message.type() == "LGIN" ){

    QString data = message.data();
    int index = data.indexOf('\r');
    QString usr = data.mid(0, index);
    QString pwd = data.mid(index + 1);
    QString result = "";

    index = -1;
    for(int i = 0; i < m_nodeList.length(); ++i){

      if( usr == m_nodeList.at(i)->usr ){

        index = i;
        break;
      }
    }
    if( index == -1 ){  //用户第一次登录, 立即注册

      Node* newClient = new Node();
      if( newClient != nullptr ){

        newClient->usr = usr;
        newClient->pwd = pwd;
        newClient->socket = &tcp;

        m_nodeList.append(newClient);

        result = "LIOK";

      }else{

        result = "LIER";
      }

    }else{//用户登陆过，验证密码

      Node* node = m_nodeList.at(index);
      if( node->pwd == pwd ){

        node->socket = &tcp;

        result = "LIOK";
      }else{

        result = "LIER";
      }
    }

    tcp.write(TextMessage(result, usr).serialize());  //将登录信息发送出去
  }
}
