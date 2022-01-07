#include "MainWinUI.h"

MainWinUI::MainWinUI(QWidget* parent) : QWidget(parent), m_loginDlg(this), m_level(""){

  initMsgGrpBx();
  initInputGrpBx();
  connectSlots();
  initMember();
  initListWidgetMenu();

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
  hLayout->addWidget(&m_msgEditor, 7);   //显示文本消息
  hLayout->addWidget(&m_listWidget, 3);  //显示当前在线用户

  m_msgEditor.setReadOnly(true);
  m_msgEditor.setFocusPolicy(Qt::NoFocus);  //使当前组件失去焦点
  m_listWidget.setFocusPolicy(Qt::NoFocus);
  m_listWidget.setContextMenuPolicy(Qt::CustomContextMenu); //触发信号

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

void MainWinUI::initListWidgetMenu(){

  QAction* action = nullptr;

  action = m_listWidgetMenu.addAction("禁言", this, SLOT(listWidgetMenuClicked()));
  action->setObjectName("silent");

  action = m_listWidgetMenu.addAction("解禁", this, SLOT(listWidgetMenuClicked()));
  action->setObjectName("recover");

  m_listWidgetMenu.addSeparator();

  action = m_listWidgetMenu.addAction("封号", this, SLOT(listWidgetMenuClicked()));
  action->setObjectName("kick");
}

void MainWinUI::setCtrlEnabled(bool enable){

  this->m_inputEdit.setEnabled(enable);
  this->m_statusLbl.setText(enable ? "状态 : 连接成功" : "状态 : 未登录");
  this->m_logInOutBtn.setText(enable ? "退出" : "登录");
  this->m_sendBtn.setEnabled(enable);

  if( enable ){

    this->m_inputEdit.setFocus();

  }else{

    m_msgEditor.clear();
    m_listWidget.clear();
    m_inputEdit.clear();
  }
}

void MainWinUI::connectSlots(){

  connect(&m_logInOutBtn, SIGNAL(clicked()), this, SLOT(onLogInOutBtnClicked()));
  connect(&m_sendBtn, SIGNAL(clicked()), this, SLOT(onSendMsgBtnClicked()));
  connect(&m_listWidget, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(listWidgetContextMenu(const QPoint&)));
}

MainWinUI::~MainWinUI(){

  m_client.close();
}

