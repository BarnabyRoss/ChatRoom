#ifndef __SERVERHANDLER_H__
#define __SERVERHANDLER_H__

#include <QObject>

class ServerHandler : public QObject{

  Q_OBJECT
public:
  explicit ServerHandler(QObject* parent = nullptr);

signals:

};

#endif // __SERVERHANDLER_H__
