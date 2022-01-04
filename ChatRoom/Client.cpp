#include <QDebug>
#include "Client.h"

Client::Client(QObject* parent) : QObject{parent}, m_handler(nullptr){

  connect(&m_client, SIGNAL(connected()), this, SLOT(onConnected()));
  connect(&m_client, SIGNAL(bytesWritten(qint64)), this, SLOT(onBytesWritten(qint64)));
  connect(&m_client, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
  connect(&m_client, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
}

bool Client::connectTo(QString ip, qint64 port){

  m_client.connectToHost(ip, port);
  return m_client.waitForConnected();
}

qint64 Client::send(TextMessage& message){

  QByteArray ba = message.serialize();
  return m_client.write(ba.data(), ba.length());
}

qint64 Client::available(){

  return m_client.bytesAvailable();
}

void Client::close(){

  m_client.close();
}

void Client::setHandler(TextMsgHandler* handler){

  this->m_handler = handler;
}

void Client::onConnected(){

  qDebug() << "void Client::onConnected()";
}

void Client::onBytesWritten(qint64 bytes){

  (void)bytes;
}

void Client::onReadyRead(){

  char buf[128] = { 0 };

  int len = -1;
  while( (len = m_client.read(buf, sizeof(buf))) > 0){

    QSharedPointer<TextMessage> ptm = m_assembler.assemble(buf, len);
    if( ptm != nullptr && m_handler != nullptr ){

      qDebug() << "Client::onReadyRead()";
      m_handler->handler(m_client, *ptm);
    }
  }

}

void Client::onDisconnected(){

  qDebug() << "void Client::onDisconnected()";
  m_assembler.reset();
}
