#include <QMessageBox>
#include <QDebug>
#include "MainWinUI.h"

void MainWinUI::onLogInOutBtnClicked(){

  if( !this->m_client.isValid() ){

    if( m_loginDlg.exec() == QDialog::Accepted ){

      QString usr = m_loginDlg.getUser().trimmed();
      QString pwd = m_loginDlg.getPwd();

      if( m_client.connectTo("127.0.0.1", 8080) ){

        TextMessage tm("LGIN", usr + '\r' + pwd);

        m_client.send(tm);

      }else{

        QMessageBox::critical(this, "Error", "无法连接到远程服务器!");
      }
    }
  }else{

    this->m_client.close();
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

  if( message.type() == "CONN" ){


  }else if( message.type() == "DSCN" ){

    setCtrlEnabled(false);

    m_inputGrpBx.setTitle("用户名");

  }else if( message.type() == "LIOK" ){

    setCtrlEnabled(true);

    m_inputGrpBx.setTitle(message.data());

  }else if( message.type() == "LIER" ){

    QMessageBox::critical(this, "Error", "身份验证失败!");

    m_client.close();
  }
}


