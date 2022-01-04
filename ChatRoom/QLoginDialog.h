#ifndef __QLOGINDIALOG_H__
#define __QLOGINDIALOG_H__

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTimer>

class QLoginDialog : public QDialog{

  Q_OBJECT

public:
  QLoginDialog(QWidget* parent = 0);
  QString getUser();
  QString getPwd();
  ~QLoginDialog();

private slots:
  void LoginBtn_Clicked();
  void CancelBtn_Clicked();
  void Timer_Timeout();

protected:
  void paintEvent(QPaintEvent*);
  QString getCaptcha();
  Qt::GlobalColor* getColors();
  void showEvent(QShowEvent*);

private:
  QLabel UserLabel;
  QLabel PwdLabel;
  QLabel CaptLabel;
  QLineEdit UserEdit;
  QLineEdit PwdEdit;
  QLineEdit CaptEdit;
  QPushButton LoginBtn;
  QPushButton CancelBtn;
  QString m_user;
  QString m_pwd;
  QString m_captcha;
  Qt::GlobalColor* m_colors;
  QTimer m_timer;
};


#endif
