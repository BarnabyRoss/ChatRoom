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

class MainWinUI : public QWidget{

  Q_OBJECT

public:
  MainWinUI(QWidget* parent = nullptr);
  ~MainWinUI();

private:
  QVBoxLayout m_vLayout;
  QHBoxLayout m_hLayout;
  QGridLayout m_gridLayout;

  QGroupBox m_msgGrpBx;
  QGroupBox m_inputGrpBx;

  QPlainTextEdit m_msgEditor;
  QLineEdit m_inputEdit;
  QLabel m_statusLbl;
  QPushButton m_logInOutBtn;
  QPushButton m_sendBtn;

};
#endif // __MAINWINUI_H__
