#ifndef __MAINWINUI_H__
#define __MAINWINUI_H__

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QMap>
#include <QHash>
#include <QString>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMenu>
#include "QLoginDialog.h"
#include "Client.h"

class MainWinUI : public QWidget, public TextMsgHandler{

  Q_OBJECT

public:
  MainWinUI(QWidget* parent = nullptr);
  ~MainWinUI();

private:
  void initMsgGrpBx();
  void initInputGrpBx();
  void initMember();
  void initListWidgetMenu();

  void connectSlots();
  void setCtrlEnabled(bool enable);
  QString getCheckedUserId();
  void handle(QTcpSocket& tcp, TextMessage& message);

  void CONN_Handler(QTcpSocket&, TextMessage&);
  void DSCN_Handler(QTcpSocket&, TextMessage&);
  void LIOK_Handler(QTcpSocket&, TextMessage&);
  void LIER_Handler(QTcpSocket&, TextMessage&);
  void MSGU_Handler(QTcpSocket&, TextMessage&);
  void USER_Handler(QTcpSocket&, TextMessage&);
  void CTRL_Handler(QTcpSocket&, TextMessage&);

protected slots:
  void onLogInOutBtnClicked();
  void onSendMsgBtnClicked();
  void listWidgetMenuClicked();
  void listWidgetContextMenu(const QPoint&);

private:
  QVBoxLayout m_vLayout;

  QGroupBox m_msgGrpBx;
  QGroupBox m_inputGrpBx;

  QPlainTextEdit m_msgEditor;
  QListWidget m_listWidget;
  QMenu m_listWidgetMenu;
  QLineEdit m_inputEdit;
  QLabel m_statusLbl;
  QPushButton m_logInOutBtn;
  QPushButton m_sendBtn;

  QLoginDialog m_loginDlg;

  Client m_client;
  QString m_level;

  typedef void(MainWinUI::*MSGHandler)(QTcpSocket&, TextMessage&);
  QHash<QString, MSGHandler> m_handlerMap;
};
#endif // __MAINWINUI_H__
