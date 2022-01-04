#ifndef __SERVERHANDLER_H__
#define __SERVERHANDLER_H__

#include <QTcpSocket>
#include <QList>
#include "TextMsgHandler.h"
#include "TextMessage.h"

class ServerHandler : public TextMsgHandler{

public:
  explicit ServerHandler();

  virtual void handle(QTcpSocket& tcp, TextMessage& message);

private:
  struct Node{

    QString usr;
    QString pwd;
    QTcpSocket* socket;
  };
  QList<Node> m_nodeList;
};

#endif // __SERVERHANDLER_H__
