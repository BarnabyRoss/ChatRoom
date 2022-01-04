#ifndef __SERVER_H__
#define __SERVER_H__

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>
#include <QMap>
#include "TextMsgAssembler.h"
#include "TextMessage.h"
#include "TextMsgHandler.h"

class Server : public QObject{

  Q_OBJECT
public:
  explicit Server(QObject* parent = nullptr);

  bool start(const QHostAddress ip, qint64 port);
  void stop();

  void setHandler(TextMsgHandler* handler);

  ~Server();

protected slots:
  void onNewConnection();
  void onConnected();
  void onBytesWritten(qint64 bytes);
  void onReadyRead();
  void onDisconnected();

private:
  QTcpServer m_server;
  TextMsgAssembler m_assembler;
  TextMsgHandler* m_handler;
  QMap<QTcpSocket*, TextMsgAssembler*> m_map;
};

#endif // __SERVER_H__
