#ifndef __CLIENT_H__
#define __CLIENT_H__

#include <QObject>
#include <QTcpSocket>
#include "TextMsgAssembler.h"
#include "TextMessage.h"
#include "TextMsgHandler.h"

class Client : public QObject{

  Q_OBJECT
public:
  explicit Client(QObject* parent = nullptr);

  bool connectTo(QString ip, qint64 port);
  qint64 send(TextMessage& message);
  qint64 available();
  void close();
  void setHandler(TextMsgHandler* handler);

protected slots:
  void onConnected();
  void onBytesWritten(qint64 bytes);
  void onReadyRead();
  void onDisconnected();

private:
  QTcpSocket m_client;
  TextMsgAssembler m_assembler;
  TextMsgHandler* m_handler;
};

#endif // __CLIENT_H__
