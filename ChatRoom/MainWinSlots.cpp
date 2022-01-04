#include <QMessageBox>
#include <QDebug>
#include "MainWinUI.h"

void MainWinUI::onLogInOutBtnClicked(){

  if( m_loginDlg.exec() == QDialog::Accepted ){

    QString usr = m_loginDlg.getUser().trimmed();
    QString pwd = m_loginDlg.getPwd();

    if( m_client.connectTo("127.0.0.1", 8080) ){

      setCtrlEnabled(true);

    }else{

      QMessageBox::critical(this, "Error", "无法连接到远程服务器!");
    }
  }
}

void MainWinUI::initMember(){

  this->m_client.setHandler(this);
}

void MainWinUI::handle(QTcpSocket& tcp, TextMessage& message){

  qDebug() << &tcp;
  qDebug() << message.type();
  qDebug() << message.length();
  qDebug() << message.data();
}


