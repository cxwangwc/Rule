#ifndef CXMAINWINDOW_H
#define CXMAINWINDOW_H

#include <QMainWindow>

class CxCentral;
class CxTextEdit;
class CxHighlighter;
class QMenu;
class QLabel;

class CxMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	CxMainWindow(QWidget *parent = 0);
	~CxMainWindow();

private:
	void initWindows();

private slots:
	void konwledgeManager();
	void help();

private:
	CxCentral *m_centralWidget;
	CxTextEdit *m_textEdit;
	QLabel *m_scriptPosition;
	CxHighlighter *m_highlighter;

	QMenu *opMenu;
};

#endif