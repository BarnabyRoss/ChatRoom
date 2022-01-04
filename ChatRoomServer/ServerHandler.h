#ifndef __SERVERHANDLER_H__
#define __SERVERHANDLER_H__

#include <QTcpSocket>
#include "TextMsgHandler.h"
#include "TextMessage.h"

class ServerHandler : public TextMsgHandler{

public:
  explicit ServerHandler();

  virtual void handle(QTcpSocket& tcp, TextMessage& message);

};

#endif // __SERVERHANDLER_H__
