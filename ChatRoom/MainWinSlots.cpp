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

  this->m_handlerMap.insert("CONN", CONN_Handler);
  this->m_handlerMap.insert("DSCN", DSCN_Handler);
  this->m_handlerMap.insert("LIOK", LIOK_Handler);
  this->m_handlerMap.insert("LIER", LIER_Handler);
}

void MainWinUI::handle(QTcpSocket& tcp, TextMessage& message){

  if( m_handlerMap.contains(message.type()) ){

    MSGHandler handler = m_handlerMap.value(message.type());
    (this->*handler)(tcp, message);
  }
}

void MainWinUI::CONN_Handler(QTcpSocket&, TextMessage&){


}

void MainWinUI::DSCN_Handler(QTcpSocket&, TextMessage&){

  setCtrlEnabled(false);

  m_inputGrpBx.setTitle("用户名");
}

void MainWinUI::LIOK_Handler(QTcpSocket&, TextMessage& message){

  setCtrlEnabled(true);

  m_inputGrpBx.setTitle(message.data());
}

void MainWinUI::LIER_Handler(QTcpSocket&, TextMessage&){

  QMessageBox::critical(this, "Error", "身份验证失败!");

  m_client.close();
}


