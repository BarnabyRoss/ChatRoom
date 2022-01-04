#ifndef __TEXTMESSAGE_H__
#define __TEXTMESSAGE_H__

#include <QObject>
#include <QString>
#include <QByteArray>

class TextMessage : public QObject{

  Q_OBJECT
public:
  explicit TextMessage(QObject* parent = nullptr);
  explicit TextMessage(QString type, QString data, QObject* parent = nullptr);

  QByteArray serialize();
  bool unserialize(QByteArray str);

  QString type();
  int length();
  QString data();

private:
  QString m_type;
  QString m_data;
};

#endif // __TEXTMESSAGE_H__
