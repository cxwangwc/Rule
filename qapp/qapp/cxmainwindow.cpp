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
	setWindowTitle(QStringLiteral("规则管理"));
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

	QMenu *fileMenu = menuBar()->addMenu(QStringLiteral("文件(&F)"));
	fileMenu->addAction(QStringLiteral("打开(&O)"), cxtree, SLOT(load()), QKeySequence("CTRL+O"));
	fileMenu->addAction(QStringLiteral("保存(&S)"), cxtree, SLOT(save()), QKeySequence("CTRL+S"));
	fileMenu->addAction(QStringLiteral("编辑(&E)"), cxtree, SLOT(open()), QKeySequence("CTRL+D"));
	fileMenu->addAction(QStringLiteral("设置脚本名称(&T)"), cxtree, SLOT(setName()), QKeySequence("CTRL+R"));
	fileMenu->addAction(QStringLiteral("知识管理(&D)"), this, SLOT(konwledgeManager()), QKeySequence("CTRL+G"));
	fileMenu->addSeparator();
	fileMenu->addAction(QStringLiteral("退出(&Q)"), this, SLOT(close()));

	QMenu *opMenu = menuBar()->addMenu(QStringLiteral("操作(&F)"));
	opMenu->addAction(QStringLiteral("执行(&C)"), m_centralWidget, SLOT(testCall()), QKeySequence("F5"));
	opMenu->addAction(QStringLiteral("关闭结果窗口(&W)"), m_centralWidget, SLOT(removeResultWidget()), QKeySequence("CTRL+W"));
	opMenu->addAction(QStringLiteral("清空当前树(&D)"), m_centralWidget, SLOT(clear()), QKeySequence("CTRL+B"));
	opMenu->addSeparator();

	QMenu *helpMenu = menuBar()->addMenu(QStringLiteral("帮助(&H)"));
	QAction *stAct = helpMenu->addAction(QStringLiteral("语法介绍"), this, SLOT(help()), QKeySequence("CTRL+M"));
	QAction *funcAct = helpMenu->addAction(QStringLiteral("常用函数介绍"), this, SLOT(help()), QKeySequence("CTRL+T"));
	QAction *ruleAct = helpMenu->addAction(QStringLiteral("规则定义介绍"), this, SLOT(help()), QKeySequence("CTRL+R"));

	stAct->setData(":/syntax.txt");
	funcAct->setData(":/readMe.txt");
	ruleAct->setData(":/contents.txt");

	QDockWidget *right = new QDockWidget(QStringLiteral("脚本内容"));
	right->setWidget(scriptWidget);
	QDockWidget *rightBottom = new QDockWidget(QStringLiteral("参数和规则"));
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