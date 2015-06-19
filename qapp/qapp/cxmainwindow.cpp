#include "cxmainwindow.h"
#include "cxcentral.h"
#include "cxknowledgemanager.h"
#include "cxhighlighter.h"
#include "cxtree.h"
#include "cxlistwidget.h"

#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QDockWidget>
#include <QMessageBox>
#include <QTextBrowser>
#include <QListWidget>

CxMainWindow::CxMainWindow(QWidget *parent)
	:QMainWindow(parent)
{
	initWindows();
	showMaximized();
	setWindowTitle(QStringLiteral("�������"));
}
CxMainWindow::~CxMainWindow()
{

}
void CxMainWindow::initWindows()
{

	QWidget *scriptWidget = new QWidget();
	QVBoxLayout *scriptLayout = new QVBoxLayout(scriptWidget);
	m_textEdit = new CxTextEdit();
	m_scriptPosition = new QLabel(this);
	m_scriptPosition->setAlignment(Qt::AlignRight);
	scriptLayout->addWidget(m_textEdit);
	scriptLayout->addWidget(m_scriptPosition);
	connect(m_textEdit, SIGNAL(selectedPos(const QString &)), m_scriptPosition, SLOT(setText(const QString &)));

	CxListWidget *listWidget = new CxListWidget();
	m_highlighter = new CxHighlighter(m_textEdit->document());
	m_centralWidget = new CxCentral(*m_textEdit, *listWidget);
	CxTree *cxtree = m_centralWidget->operatesTree();
	setCentralWidget(m_centralWidget);

	QMenu *fileMenu = menuBar()->addMenu(QStringLiteral("�ļ�(&F)"));
	fileMenu->addAction(QStringLiteral("��(&O)"), cxtree, SLOT(load()), QKeySequence("CTRL+O"));
	fileMenu->addAction(QStringLiteral("����(&S)"), cxtree, SLOT(save()), QKeySequence("CTRL+S"));
	fileMenu->addAction(QStringLiteral("�༭(&E)"), cxtree, SLOT(open()), QKeySequence("CTRL+D"));
	fileMenu->addAction(QStringLiteral("���ýű�����(&T)"), cxtree, SLOT(setName()), QKeySequence("CTRL+R"));
	fileMenu->addAction(QStringLiteral("֪ʶ����(&D)"), this, SLOT(konwledgeManager()), QKeySequence("CTRL+G"));
	fileMenu->addSeparator();
	fileMenu->addAction(QStringLiteral("�˳�(&Q)"), this, SLOT(close()));

	QMenu *opMenu = menuBar()->addMenu(QStringLiteral("����(&F)"));
	opMenu->addAction(QStringLiteral("ִ��(&C)"), m_centralWidget, SLOT(testCall()), QKeySequence("F5"));
	opMenu->addAction(QStringLiteral("�رս������(&W)"), m_centralWidget, SLOT(removeResultWidget()), QKeySequence("CTRL+W"));
	opMenu->addAction(QStringLiteral("��յ�ǰ��(&D)"), m_centralWidget, SLOT(clear()), QKeySequence("CTRL+B"));
	opMenu->addSeparator();

	QMenu *helpMenu = menuBar()->addMenu(QStringLiteral("����(&H)"));
	QAction *stAct = helpMenu->addAction(QStringLiteral("�﷨����"), this, SLOT(help()), QKeySequence("CTRL+M"));
	QAction *funcAct = helpMenu->addAction(QStringLiteral("���ú�������"), this, SLOT(help()), QKeySequence("CTRL+T"));
	QAction *ruleAct = helpMenu->addAction(QStringLiteral("���������"), this, SLOT(help()), QKeySequence("CTRL+R"));

	stAct->setData(":/syntax.txt");
	funcAct->setData(":/readMe.txt");
	ruleAct->setData(":/contents.txt");

	QDockWidget *right = new QDockWidget(QStringLiteral("�ű�����"));
	right->setWidget(scriptWidget);
	QDockWidget *rightBottom = new QDockWidget(QStringLiteral("�����͹���"));
	rightBottom->setWidget(listWidget);
	addDockWidget(Qt::RightDockWidgetArea, right);
	addDockWidget(Qt::RightDockWidgetArea, rightBottom);
}

void CxMainWindow::konwledgeManager()
{
	CxKnowlegdeManager *manager = new CxKnowlegdeManager();
	connect(manager, SIGNAL( fileSelected(const std::string &)), m_centralWidget, SLOT(openXmlFile(const std::string &)));
	manager->show();
}

void CxMainWindow::help()
{
	QAction *act = qobject_cast<QAction *> (sender());
	if (!act) return;
	QString fileName = act->data().toString();
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly))
		return;
	QString helpText = file.readAll();
	file.close();
	QTextBrowser *browser = new QTextBrowser();
	browser->setTabStopWidth(30);
	new CxHighlighter(browser->document());
	browser->setAttribute(Qt::WA_DeleteOnClose);
	browser->setWindowTitle(act->text());
	browser->setText(helpText);
	browser->resize(600, 400);
	browser->show();
}