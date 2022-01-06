#include <QByteArray>
#include <QDebug>
#include "ServerHandler.h"

ServerHandler::ServerHandler(){

#define MapToHandler(MSG) m_handlerMap.insert(#MSG, MSG##_Handler);

  MapToHandler(CONN);
  MapToHandler(DSCN);
  MapToHandler(LGIN);
  MapToHandler(MSGU);
  MapToHandler(MSGR);
//  m_handlerMap.insert("CONN", CONN_Handler);
//  m_handlerMap.insert("DSCN", DSCN_Handler);
//  m_handlerMap.insert("LGIN", LGIN_Handler);
//  m_handlerMap.insert("MSGU", MSGU_Handler);
//  m_handlerMap.insert("MSGR", MSGR_Handler);
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

  //退出时，发送用户id
  TextMessage tm("USER", getOnlineUsrId());
  sendToAllOnlineUsr(tm);
}

void ServerHandler::LGIN_Handler(QTcpSocket& tcp, TextMessage& message){

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
    sendToAllOnlineUsr(tm);
  }
}

//公聊消息
void ServerHandler::MSGU_Handler(QTcpSocket&, TextMessage& message){

  sendToAllOnlineUsr(message);
}

//私聊消息
void ServerHandler::MSGR_Handler(QTcpSocket&, TextMessage& message){

  QStringList tl = message.data().split('\r', QString::SkipEmptyParts);
  const QByteArray& ba = TextMessage("MSGU", tl.last()).serialize();

  tl.removeLast();

  for(int i = 0; i < tl.length(); ++i){

    for(int j = 0; j < m_nodeList.length(); ++j){

      Node* node = m_nodeList.at(j);
      if( node->socket != nullptr && node->usr == tl[i]){

        node->socket->write(ba);
        break;
      }
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

void ServerHandler::sendToAllOnlineUsr(TextMessage& message){

  const QByteArray& ba = message.serialize();
  for(int i = 0; i < m_nodeList.length(); ++i){

    Node* node = m_nodeList.at(i);
    if( node->socket != nullptr ){

      node->socket->write(ba);
    }
  }
}

