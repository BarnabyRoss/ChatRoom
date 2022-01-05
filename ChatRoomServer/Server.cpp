#include <QTcpSocket>
#include <QHostAddress>
#include <QObjectList>
#include <QList>
#include <QDebug>
#include "Server.h"

Server::Server(QObject* parent) : QObject{parent}, m_handler(nullptr){

  connect(&m_server, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
}

bool Server::start(const QHostAddress ip, qint64 port){

  bool ret = true;
  if( !m_server.isListening() ){

    ret = m_server.listen(ip, port);
  }

  return ret;
}

void Server::stop(){

  if( m_server.isListening() ) m_server.close();
}

void Server::setHandler(TextMsgHandler* handler){

  this->m_handler = handler;
}

void Server::onNewConnection(){

  QTcpSocket* tcp = m_server.nextPendingConnection();
  if( tcp != nullptr ){

    connect(tcp, SIGNAL(connected()), this, SLOT(onConnected()));
    connect(tcp, SIGNAL(bytesWritten(qint64)), this, SLOT(onBytesWritten(qint64)));
    connect(tcp, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(tcp, SIGNAL(disconnected()), this, SLOT(onDisconnected()));

    m_map.insert(tcp, new TextMsgAssembler());
  }

  if( m_handler != nullptr ){

    TextMessage tm("CONN", tcp->peerAddress().toString() + ":" + QString::number(tcp->peerPort()));
    this->m_handler->handle(*tcp, tm);
  }
}

void Server::onConnected(){

  qDebug() << "void Server::onConnected()";
}

void Server::onBytesWritten(qint64 bytes){

  (void)bytes;
}

void Server::onReadyRead(){

  char buf[128] = { 0 };
  QTcpSocket* tcp = dynamic_cast<QTcpSocket*>(sender());
  if( tcp != nullptr ){

    TextMsgAssembler* assembler = m_map.value(tcp);
    int len = -1;
    while( (len = tcp->read(buf, sizeof(buf))) > 0 ){

      if( assembler != nullptr ){

        QSharedPointer<TextMessage> ptm = nullptr;
        assembler->prepare(buf, len);
        while( (ptm = assembler->assemble()) != nullptr ){

          if( m_handler != nullptr ) m_handler->handle(*tcp, *ptm);
        }
      }


//      QSharedPointer<TextMessage> ptm = assembler != nullptr ? m_assembler.assemble(buf, sizeof(buf)) : nullptr;
//      if( ptm != nullptr && m_handler != nullptr ){

//        m_handler->handle(*tcp, *ptm);

//        TextMessage message("frsr", ptm->data());
//        QByteArray ba = message.serialize();
//        tcp->write(ba.data(), ba.length());
//      }
    }

  }
}

void Server::onDisconnected(){

  qDebug() << "void Server::onDisconnected()";

  QTcpSocket* tcp = dynamic_cast<QTcpSocket*>(sender());
  if( tcp != nullptr ){

    delete m_map.take(tcp);
  }

  if( this->m_handler != nullptr ){

    TextMessage tm("DISN", "");
    this->m_handler->handle(*tcp, tm);
  }
}

Server::~Server(){

  const QObjectList list = m_server.children();
  for(int i = 0; i < list.length(); ++i){
    QTcpSocket* tcp = dynamic_cast<QTcpSocket*>(list[i]);
    if( tcp != nullptr ) tcp->close();
  }

  const QList<TextMsgAssembler*> al = m_map.values();
  for(int i = 0; i < al.length(); ++i){
    delete al.at(i);
  }
}
