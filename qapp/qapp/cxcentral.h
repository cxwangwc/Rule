#ifndef CXCENTRAL_H
#define CXCENTRAL_H

#include <QWidget>
#include <QTextEdit>

#include "cxtextedit.h"
#include "cxlistwidget.h"

class CxTree;
class QTreeWidget;
class QSplitter;

class CxCentral : public QWidget
{
	Q_OBJECT

public:
	CxCentral(CxTextEdit &scEditor, CxListWidget &paramWidget, QWidget *parent = 0);
	~CxCentral();

	CxTree *operatesTree();

public slots:
	void openXmlFile(const std::string &file);
	void resultWidgetAdded(QTreeWidget *newWidget);
	void readScriptFile(const std::string &);
	void cueTextChanged();
	void paramNameModified(const std::vector<std::string> &);

	void listParamClicked(const QString &paramName);
	void listParamEntered(const QString &paramName);
	void clear();

	void removeResultWidget();
	void testCall();

private:
	CxTree *m_editTree;
	CxTextEdit &m_textEditor;
	CxListWidget &m_paramWidget;
	QSplitter *m_splitter;
};

#endif