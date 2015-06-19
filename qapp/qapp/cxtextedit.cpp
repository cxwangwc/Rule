#include "cxtextedit.h"

#include <QCompleter>
#include <QKeyEvent>
#include <QAbstractItemView>
#include <QtDebug>
#include <QApplication>
#include <QModelIndex>
#include <QStringListModel>
#include <QScrollBar>
#include <QFile>
#include <QSplitter>
#include <QAction>
#include <QMenu>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QTextStream>
#include <QMouseEvent>
#include <QTextCursor>

bool caseInsensitiveLessThan(const QString &s1, const QString &s2)
{
	return s1.toLower() < s2.toLower();
}

CxTextEdit::CxTextEdit(QWidget *parent, const QStringList &keywordsList)
	: QTextEdit(parent)
	, m_completer(0)
{
	setTabStopWidth(30);
	functionsFromFile(":/function.txt");
	m_functionList.append(keywordsList);
	setCompleter(m_functionList);
}

CxTextEdit::~CxTextEdit()
{
}

void CxTextEdit::setParamCompleter(const std::vector<std::string> &params)
{
	QStringList keyList = m_functionList;
	for (int i = 0;i < params.size();i++)
	{
		keyList.append(QString::fromStdString(params[i]));
	}
	setCompleter(keyList);
}

void CxTextEdit::setCompleter(const QStringList &keyList)
{ 
	if(m_completer) {delete m_completer;m_completer = 0;}
	m_completer = new QCompleter(this);
	QStringList sortList = keyList;
	qSort(sortList.begin(), sortList.end(), caseInsensitiveLessThan);
	m_completer->setModel(new QStringListModel(sortList, m_completer));
	m_completer->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
	m_completer->setCaseSensitivity(Qt::CaseInsensitive);
	m_completer->setWrapAround(false);
	m_completer->setWidget(this);
	QObject::connect(m_completer, SIGNAL(activated(QString)),
		this, SLOT(insertCompletion(QString)));
}

QCompleter *CxTextEdit::completer() const
{
	return m_completer;
}

void CxTextEdit::insertCompletion(const QString& completion)
{
	if (m_completer->widget() != this)
		return;
	QTextCursor tc = textCursor();
	int extra = completion.length() - m_completer->completionPrefix().length();
	tc.movePosition(QTextCursor::Left);
	tc.movePosition(QTextCursor::EndOfWord);
	tc.insertText(completion.right(extra));
	setTextCursor(tc);
}

QString CxTextEdit::textUnderCursor() const
{
	QTextCursor tc = textCursor();
	tc.select(QTextCursor::WordUnderCursor);
	return tc.selectedText();
}

void CxTextEdit::focusInEvent(QFocusEvent *e)
{
	if (m_completer)
		m_completer->setWidget(this);
	QTextEdit::focusInEvent(e);
}

void CxTextEdit::keyPressEvent(QKeyEvent *e)
{
	if (m_completer && m_completer->popup()->isVisible())
	{
		switch (e->key()) 
		{
		case Qt::Key_Enter:
		case Qt::Key_Return:
		case Qt::Key_Escape:
		case Qt::Key_Tab:
		case Qt::Key_Backtab:
			e->ignore();
			return; 
		default:
			break;
		}
	}

	bool isShortcut = ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_E); 
	if (!m_completer || !isShortcut) 
		QTextEdit::keyPressEvent(e);

	const bool ctrlOrShift = e->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier);

	if (!m_completer || (ctrlOrShift && e->text().isEmpty()))
		return;

	static QString eow("~!@#$%^&*()+{}|:\"<>?,./;'[]\\-="); 
	bool hasModifier = (e->modifiers() != Qt::NoModifier) && !ctrlOrShift;
	QString completionPrefix = textUnderCursor();

	if (!isShortcut &&  (hasModifier|| e->text().isEmpty()|| completionPrefix.length() < 1
		|| eow.contains(e->text().right(1))))
	{
			m_completer->popup()->hide();
			return;
	}
	QString cPre = m_completer->completionPrefix();
	if (completionPrefix != cPre) {
		m_completer->setCompletionPrefix(completionPrefix);
		m_completer->popup()->setCurrentIndex(m_completer->completionModel()->index(0, 0));
	}
	QRect cr = cursorRect();
	cr.setWidth(m_completer->popup()->sizeHintForColumn(0)
		+ m_completer->popup()->verticalScrollBar()->sizeHint().width());
	m_completer->complete(cr);
}



void CxTextEdit::contextMenuEvent(QContextMenuEvent *e)
{
	QMenu *menu = createStandardContextMenu();
	menu->addSeparator();
	menu->addAction(QStringLiteral("打开文件(&O)"), this, SLOT(openFile()), QKeySequence("CTRL+U"));
	menu->addAction(QStringLiteral("执行(&S)"), this, SLOT(runSimpleString()), QKeySequence("CTRL+T"));
	menu->exec(QCursor::pos());
}

void CxTextEdit::mousePressEvent(QMouseEvent *e)
{
	QTextEdit::mousePressEvent(e);

	const QTextCursor cursor = textCursor();
	int col = cursor.columnNumber();
	int row = cursor.blockNumber()+1;

	emit selectedPos(QString("Line : %1, Column : %2").arg(row).arg(col));

}

void CxTextEdit::runSimpleString()
{
	if (toPlainText().isEmpty()) return;

	QFile f("cxtmp.py");
	if (f.open(QIODevice::WriteOnly))
	{
		/*		f.write(toPlainText().toLocal8Bit());*/
		QTextStream fs(&f);
// 		QTextCursor cursor = textCursor();
// 		cursor.movePosition(QTextCursor::Start);
// 		cursor.insertText("#-*- coding: gb2312 -*-\n\n");
		fs << "#-*- coding: gb2312 -*-\n\n";
		fs << toPlainText();
		f.close();
		emit runFile("cxtmp");
	}
}

void CxTextEdit::openFile()
{
	QString fileName = QFileDialog::getOpenFileName(this, "", "", "*.py");
	if (fileName.isEmpty()) return;
	QFile f(fileName);
	if (f.open(QIODevice::ReadOnly))
	{
		setText(f.readAll());
		f.close();
	}
}

void CxTextEdit::functionsFromFile(const QString& fileName)
{

	QFile file(fileName);
	if (!file.open(QFile::ReadOnly))
	{
		return;
	}
	m_functionList.clear();

	while (!file.atEnd())
	{
		QByteArray line = file.readLine();
		if (!line.isEmpty())
		{
			m_functionList << line.trimmed();
		}
	}

	return;
}