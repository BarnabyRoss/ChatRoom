#include "TextMsgAssembler.h"

TextMsgAssembler::TextMsgAssembler(QObject* parent) : QObject{parent}{

  clear();
}


void TextMsgAssembler::prepare(const char* data, int len){

  if( data != nullptr ){

    for(int i = 0; i < len; ++i){

      m_queue.enqueue(data[i]);
    }
  }
}

QSharedPointer<TextMessage> TextMsgAssembler::assemble(){

  QSharedPointer<TextMessage> ret(nullptr);
  bool tryMakeMsg = false;

  if( m_type == "" ){

    tryMakeMsg = makeTypeAndLength();

  }else{

    tryMakeMsg = true;
  }
  if( tryMakeMsg ){

    ret = makeMessage();
  }

  if( ret != nullptr ) clear();

  return ret;
}

QSharedPointer<TextMessage> TextMsgAssembler::assemble(const char* data, int len){

  prepare(data, len);

  return assemble();
}

void TextMsgAssembler::reset(){

  clear();
  m_queue.clear();
}

void TextMsgAssembler::clear(){

  this->m_type = "";
  this->m_length = 0;
  this->m_data.clear();
}

bool TextMsgAssembler::makeTypeAndLength(){

  bool ret = (m_queue.length() >= 8);
  if( ret ){

    m_type += QString(fetch(4));
    m_type = m_type.trimmed();

    QString len = QString(fetch(4));
    m_length = len.trimmed().toInt(&ret, 16);
  }

  if( !ret ) clear();

  return ret;
}

QSharedPointer<TextMessage> TextMsgAssembler::makeMessage(){

  QSharedPointer<TextMessage> ret(nullptr);
  if( m_type != "" ){

    int needed = m_length - m_data.length();
    int n = needed <= m_queue.length() ? needed : m_queue.length();

    m_data.append(fetch(n));
    if( m_length == m_data.length() ){

      ret = QSharedPointer<TextMessage>(new TextMessage(m_type, QString(m_data)));
    }

    return ret;
  }

  return ret;
}

QByteArray TextMsgAssembler::fetch(int n){

  QByteArray ret;
  if( n <= 0 ) return ret;

  for(int i = 0; i < n; ++i){

    ret.append(m_queue.dequeue());
  }

  return ret;
}
