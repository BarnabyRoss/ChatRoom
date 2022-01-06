#include <QByteArray>
#include <QDebug>
#include "ServerHandler.h"

ServerHandler::ServerHandler(){

  m_handlerMap.insert("CONN", CONN_Handler);
  m_handlerMap.insert("DSCN", DSCN_Handler);
  m_handlerMap.insert("LGIN", LGIN_Handler);
  m_handlerMap.insert("MSGU", MSGU_Handler);
}

void ServerHandler::handle(QTcpSocket& tcp, TextMessage& message){

  if( m_handlerMap.contains(message.type()) ){

    MSGHandler handler = m_handlerMap.value(message.type());
    (this->*handler)(tcp, message);
  }

}

void ServerHandler::CONN_Handler(QTcpSocket&, TextMessage&){


}

void ServerHandler::DSCN_Handler(QTcpSocket& tcp, TextMessage&){

  for(int i = 0; i < m_nodeList.length(); ++i){

    if( &tcp == m_nodeList.at(i)->socket ){

      m_nodeList.at(i)->socket = nullptr;
    }
  }
}

void ServerHandler::LGIN_Handler(QTcpSocket& tcp, TextMessage& message){

  qDebug() << &tcp;
  qDebug() << message.type();
  qDebug() << message.length();
  qDebug() << message.data();

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

  //登录成功，发送在线用户id
  if( result == "LIOK" ){

    TextMessage tm("USER", getOnlineUsrId());
    const QByteArray& ba = tm.serialize();
    for(int i = 0; i < m_nodeList.length(); ++i){

      Node* node = m_nodeList.at(i);
      if( node->socket != nullptr ){

        node->socket->write(ba);
      }
    }
  }
}

void ServerHandler::MSGU_Handler(QTcpSocket&, TextMessage& message){

  const QByteArray& ba = message.serialize();
  for(int i = 0; i < m_nodeList.length(); ++i){

    Node* node = m_nodeList.at(i);
    if( node->socket != nullptr ){

      node->socket->write(ba);
    }
  }
}

QString ServerHandler::getOnlineUsrId(){

  QString ret = "";

  for(int i = 0; i < m_nodeList.length(); ++i){

    Node* node = m_nodeList.at(i);
    if( node->socket != nullptr ) ret += (node->usr + '\r');
  }

  return ret;
}

