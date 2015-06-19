#include "cxcentral.h"
#include "cxtree.h"

#include <QFile>
#include <QSplitter>
#include <QAction>
#include <QMenu>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QTextStream>

CxCentral::CxCentral(CxTextEdit &scEditor, CxListWidget &paramWidget, QWidget *parent)
	: QWidget(parent)
	, m_textEditor(scEditor)
	, m_paramWidget(paramWidget)
{
	QHBoxLayout *mainLayout = new QHBoxLayout(this);
	m_splitter = new QSplitter(Qt::Horizontal);
	m_editTree = new CxTree();
	m_splitter->addWidget(m_editTree);
	mainLayout->addWidget(m_splitter);
	connect(m_editTree, SIGNAL(treeContentChanged()), this, SLOT(cueTextChanged()));
	connect(m_editTree, SIGNAL(newWidget(QTreeWidget *)), SLOT(resultWidgetAdded(QTreeWidget *)));
	connect(m_editTree, SIGNAL(scriptFileSaved(const std::string &)), SLOT(readScriptFile(const std::string &)));
	connect(&m_textEditor, SIGNAL(runFile(const std::string &)), m_editTree, SLOT(call(const std::string &)));
	connect(m_editTree, SIGNAL(paramNameModified(const std::vector<std::string>&)),
		this, SLOT(paramNameModified(const std::vector<std::string>&)));
	connect(&m_paramWidget, SIGNAL(paramClicked(const QString &)), SLOT(listParamClicked(const QString &)));
	connect(&m_paramWidget, SIGNAL(paramEntered(const QString &)), SLOT(listParamEntered(const QString &)));
	connect(m_editTree, SIGNAL(paramNameChanged(QString, QString)), &m_paramWidget, SLOT(paramNameChanged(QString, QString)));
}

CxCentral::~CxCentral()
{

}

void CxCentral::resultWidgetAdded(QTreeWidget *newWidget)
{
	if (!newWidget) return;
	m_splitter->addWidget(newWidget);
}

void CxCentral::cueTextChanged()
{
	//m_textEditor->setText(m_editTree->cueText());
}

void CxCentral::clear()
{
	m_editTree->clearContents();
}

void CxCentral::removeResultWidget()
{
	QList<QWidget *> removeWidgets;
	for (int i = 1;i < m_splitter->count();i++)
	{
		removeWidgets.append(m_splitter->widget(i));
	}
	qDeleteAll(removeWidgets);
}

void CxCentral::testCall()
{
	m_editTree->call();
}

void CxCentral::openXmlFile(const std::string &file)
{
	m_editTree->load(file);
}

void CxCentral::readScriptFile(const std::string &fileName)
{
	QFile f(QString::fromStdString(fileName));
	if (f.open(QIODevice::ReadOnly))
	{
		m_textEditor.setText(f.readAll());
		f.close();
	}
}

CxTree *CxCentral::operatesTree()
{
	return m_editTree;
}

void CxCentral::paramNameModified(const std::vector<std::string> &params)
{
	m_paramWidget.clear();
	for (int i = 0;i < params.size();i++)
	{
		m_paramWidget.addItem(QString::fromStdString(params[i]));
	}

	m_textEditor.setParamCompleter(params);
}

void CxCentral::listParamClicked(const QString &paramName)
{
	m_editTree->editParam(paramName);
}

void CxCentral::listParamEntered(const QString &paramName)
{
	m_paramWidget.setToolTip(m_editTree->getParamText(paramName));
}