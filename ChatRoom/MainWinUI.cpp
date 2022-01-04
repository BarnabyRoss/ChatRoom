#include "MainWinUI.h"

MainWinUI::MainWinUI(QWidget* parent) : QWidget(parent){

  initMsgGrpBx();

  m_vLayout.addWidget(&m_msgGrpBx);

  this->setLayout(&m_vLayout);
}

void MainWinUI::initMsgGrpBx(){

  QHBoxLayout* hLayout = new QHBoxLayout();

  hLayout->setContentsMargins(2, 5, 2, 2);
  hLayout->addWidget(&m_msgEditor);

  m_msgEditor.setReadOnly(true);

  m_msgGrpBx.setLayout(hLayout);
  m_msgGrpBx.setWindowTitle("聊天消息");
}

MainWinUI::~MainWinUI(){

}

