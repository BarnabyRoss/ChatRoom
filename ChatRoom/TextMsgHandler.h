#ifndef __TEXTMSGHANDLER_H__
#define __TEXTMSGHANDLER_H__

#include <QTcpSocket>
#include "TextMessage.h"

class TextMsgHandler{

public:
  virtual void handle(QTcpSocket&, TextMessage&) = 0;
};

#endif // __TEXTMSGHANDLER_H__
