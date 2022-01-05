#include <QDebug>
#include <QHostAddress>
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

bool Client::isValid(){

  return this->m_client.isValid();
}

void Client::onConnected(){

  qDebug() << "connect success!";
  if( m_handler != nullptr ){

    TextMessage tm("CONN", m_client.peerAddress().toString() + ":" + QString::number(m_client.peerPort()));
    m_handler->handle(m_client, tm);
  }
}

void Client::onBytesWritten(qint64 bytes){

  (void)bytes;
}

void Client::onReadyRead(){

  char buf[128] = { 0 };

  int len = -1;
  while( (len = m_client.read(buf, sizeof(buf))) > 0){

    QSharedPointer<TextMessage> ptm = nullptr;
    m_assembler.prepare(buf, len);

    while( (ptm = m_assembler.assemble()) != nullptr ){

      if( m_handler != nullptr ){

        m_handler->handle(m_client, *ptm);
      }
    }
  }

}

void Client::onDisconnected(){

  qDebug() << "void Client::onDisconnected()";
  m_assembler.reset();

  if( m_handler != nullptr ){

    TextMessage tm("DSCN", "");
    m_handler->handle(m_client, tm);
  }

}
