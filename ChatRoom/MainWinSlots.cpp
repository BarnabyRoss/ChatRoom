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

void MainWinUI::onSendMsgBtnClicked(){

  QString input = m_inputEdit.text().trimmed();
  if( input != "" ){

    QString self = m_inputGrpBx.title();
    QString text = self + ":\n" + "    " + input + "\n";
    QString uid = getCheckedUserId();
    bool ok = true;

    if( uid == "" ){ //公聊消息发送

      TextMessage tm("MSGU", text);

      ok = m_client.send(tm);

    }else{//发送私聊消息

      QString sid = (uid.indexOf(self) >= 0) ? uid : uid + self + '\r';
      TextMessage tm("MSGR", sid + text);

      ok = m_client.send(tm);
    }

    if( ok ){

      m_inputEdit.clear();
    }
  }
//  QString text = m_inputGrpBx.title() + ":" + '\n' + "    " + m_inputEdit.text() + '\n';
//  TextMessage tm("MSGU", text);
//  if( this->m_client.send(tm) ){

//    this->m_inputEdit.clear();
//  }
}

void MainWinUI::initMember(){

#define MapToHandler(MSG) m_handlerMap.insert(#MSG, MSG##_Handler)

  this->m_client.setHandler(this);

  MapToHandler(CONN);
  MapToHandler(DSCN);
  MapToHandler(LIOK);
  MapToHandler(LIER);
  MapToHandler(MSGU);
  MapToHandler(USER);

//  this->m_handlerMap.insert("CONN", CONN_Handler);
//  this->m_handlerMap.insert("DSCN", DSCN_Handler);
//  this->m_handlerMap.insert("LIOK", LIOK_Handler);
//  this->m_handlerMap.insert("LIER", LIER_Handler);
//  this->m_handlerMap.insert("MSGU", MSGU_Handler);
//  this->m_handlerMap.insert("USER", USER_Handler);
}

QString MainWinUI::getCheckedUserId(){

  QString ret = "";
  for(int i = 0; i < m_listWidget.count(); ++i){

    QListWidgetItem* item = m_listWidget.item(i);
    if( item->checkState() == Qt::Checked ){

      ret += item->text() + '\r';
    }
  }

  return ret;
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

void MainWinUI::USER_Handler(QTcpSocket&, TextMessage& message){

  QStringList users = message.data().split('\r', QString::SkipEmptyParts);
  QStringList checked = getCheckedUserId().split('\r', QString::SkipEmptyParts);

  m_listWidget.clear();

  for(int i = 0; i < users.length(); ++i){

    QListWidgetItem* item = new QListWidgetItem();
    if( item != nullptr ){

      item->setText(users[i]);
      item->setCheckState(Qt::Unchecked);
      m_listWidget.addItem(item);
    }
  }

  for(int i = 0; i < m_listWidget.count(); ++i){

    QListWidgetItem* item = m_listWidget.item(i);
    for(int j = 0; j < checked.length(); ++j){

      if( checked.at(j) == item->text() ){

        item->setCheckState(Qt::Checked);
      }
    }
  }
}

void MainWinUI::MSGU_Handler(QTcpSocket&, TextMessage& message){

  this->m_msgEditor.appendPlainText(message.data());
}


