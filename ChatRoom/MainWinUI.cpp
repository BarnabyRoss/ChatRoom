#include "MainWinUI.h"

MainWinUI::MainWinUI(QWidget* parent) : QWidget(parent){

  initMsgGrpBx();
  initInputGrpBx();

  m_vLayout.setSpacing(10);
  m_vLayout.addWidget(&m_msgGrpBx);
  m_vLayout.addWidget(&m_inputGrpBx);

  this->setLayout(&m_vLayout);
  this->setMinimumSize(600, 400);
  this->resize(600, 400);
}

void MainWinUI::initMsgGrpBx(){

  QHBoxLayout* hLayout = new QHBoxLayout();

  hLayout->setContentsMargins(2, 5, 2, 2);
  hLayout->addWidget(&m_msgEditor);

  m_msgEditor.setReadOnly(true);

  m_msgGrpBx.setLayout(hLayout);
  m_msgGrpBx.setTitle("聊天消息");
}

void MainWinUI::initInputGrpBx(){

  QGridLayout* gridLayout = new QGridLayout();

  gridLayout->setSpacing(10);

  gridLayout->addWidget(&m_inputEdit, 0, 0, 1, 5);
  gridLayout->addWidget(&m_statusLbl, 1, 0, 1, 3);
  gridLayout->addWidget(&m_logInOutBtn, 1, 3);
  gridLayout->addWidget(&m_sendBtn, 1, 4);

  this->m_inputEdit.setEnabled(false);
  this->m_inputEdit.setFixedHeight(25);

  this->m_statusLbl.setText("状态 : 未登录");

  this->m_logInOutBtn.setText("登录");
  this->m_logInOutBtn.setFixedHeight(30);
  this->m_sendBtn.setText("发送");
  this->m_sendBtn.setFixedHeight(30);
  this->m_sendBtn.setEnabled(false);

  this->m_inputGrpBx.setLayout(gridLayout);
  this->m_inputGrpBx.setTitle("用户名");
  this->m_inputGrpBx.setFixedHeight(100);

}

MainWinUI::~MainWinUI(){

}

