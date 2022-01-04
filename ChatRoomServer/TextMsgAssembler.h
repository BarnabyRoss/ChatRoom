#ifndef __TEXTMSGASSEMBLER_H__
#define __TEXTMSGASSEMBLER_H__

#include <QObject>
#include <QQueue>
#include <QSharedPointer>
#include "TextMessage.h"

class TextMsgAssembler : public QObject{

  Q_OBJECT
public:
  explicit TextMsgAssembler(QObject* parent = nullptr);

  void prepare(const char* data, int len);
  QSharedPointer<TextMessage> assemble();
  QSharedPointer<TextMessage> assemble(const char* data, int len);

  void reset();

private:
  void clear();
  bool makeTypeAndLength();
  QSharedPointer<TextMessage> makeMessage();
  QByteArray fetch(int n);

private:
  QQueue<char> m_queue;
  QString m_type;
  int m_length;
  QByteArray m_data;
};

#endif // __TEXTMSGASSEMBLER_H__
