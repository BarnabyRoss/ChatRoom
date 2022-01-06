#ifndef __SERVERHANDLER_H__
#define __SERVERHANDLER_H__

#include <QTcpSocket>
#include <QList>
#include <QMap>
#include "TextMsgHandler.h"
#include "TextMessage.h"

class ServerHandler : public TextMsgHandler{

public:
  explicit ServerHandler();

  virtual void handle(QTcpSocket& tcp, TextMessage& message);

private:
  void CONN_Handler(QTcpSocket&, TextMessage&);
  void DSCN_Handler(QTcpSocket&, TextMessage&);
  void LGIN_Handler(QTcpSocket&, TextMessage&);
  void MSGU_Handler(QTcpSocket&, TextMessage&);
  void MSGR_Handler(QTcpSocket&, TextMessage&);

  QString getOnlineUsrId();
  void sendToAllOnlineUsr(TextMessage& message);

private:
  //MSGHandler是一个指向ServerHandler成员函数的一个函数指针, 函数类型为void (QTcpSocket&, TextMessage&)
  typedef void(ServerHandler::*MSGHandler)(QTcpSocket&, TextMessage&);

  struct Node{

    QString usr;
    QString pwd;
    QTcpSocket* socket;

  public:
    Node() : usr(""), pwd(""), socket(nullptr) {}
  };
  QList<Node*> m_nodeList;
  QMap<QString, MSGHandler> m_handlerMap;

};

#endif // __SERVERHANDLER_H__
