#include "TextMessage.h"

TextMessage::TextMessage(QObject* parent) : QObject{parent}, m_type(""), m_data(""){

}

TextMessage::TextMessage(QString type, QString data, QObject* parent) : QObject{parent}{

  this->m_type = type.trimmed();
  this->m_type.resize(4, ' ');
  this->m_data = data.mid(0, 15000);
}

QByteArray TextMessage::serialize(){

  QByteArray ret;
  QByteArray dba = m_data.toUtf8();
  QString len = QString::asprintf("%X", dba.length());
  len.resize(4, ' ');

  ret.append(m_type.toStdString().c_str(), 4);
  ret.append(len.toStdString().c_str(), 4);
  ret.append(dba);

  return ret;
}

bool TextMessage::unserialize(QByteArray dba){

  bool ret = (dba.length() >= 8);
  if( ret != 0 ){

    QString type = QString(dba.mid(0, 4));
    QString len = QString(dba.mid(4, 4)).trimmed();
    int l = len.toInt(&ret, 16);

    if( ret != 0 && (l == dba.length() - 8)){

      m_type = type;
      m_data = QString(dba.mid(8));
    }
  }

  return ret;
}

QString TextMessage::type(){

  return m_type.trimmed();
}

int TextMessage::length(){

  return m_data.length();
}

QString TextMessage::data(){

  return m_data;
}
