#ifndef TREE_H
#define TREE_H

#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QDialog>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLineEdit>
#include <QTextEdit>

#include "cxscriptgenerator.h"

class QMenu;
class QAction;

// #ifndef NEED_LIMITERS
// #define NEED_LIMITERS
// #endif 

class CxTree : public QTreeWidget
{
	Q_OBJECT

public:
	CxTree(QWidget *parent = 0, bool = false);
	~CxTree(void);

	void addObject(const std::vector<CxExternalObject *> &externalObjects);
	std::vector<CxExternalObject *> toObject();

	void addInputParam(const std::vector<CxInputParam *> &);
	std::vector<CxInputParam *> toInputParam();

	void addCalcualteRule(CxCalculateRule *);
	CxCalculateRule *toCalculateRule();

	void addLimits(CxCalculateRule *);
	CxCalculateRule *toLimits();

	void addExtendsModule(const std::vector<std::pair<string, string> > &);
	std::vector<std::pair<string, string> > toExtendsModule();

	void addOutputParam(const std::vector<CxOutputParam> &outParams);
	std::vector<CxOutputParam> toOutputParams();

	QTreeWidgetItem *objectBottomItem(QTreeWidgetItem *current);

	// topLevelItem
	QTreeWidgetItem *topItem(QTreeWidgetItem *cItem = 0);

	// 提示字符
	QString cueText();
	void load(const std::string &);

	void expandResultItem();
	void clearContents();

	void editParam(const QString &paramName);
	QString getParamText(const QString &paramName);

signals:
	void newWidget(QTreeWidget *);
	void treeContentChanged();
	void scriptFileSaved(const std::string &);
	void paramNameModified(const std::vector<std::string> &names);
	void paramNameChanged(QString previous, QString current);

private slots:
	void addInputParam();
	void addOutputParam();
	void addObject();
	void addObjectAttribute();
	void addRule();
	void addModule();
	void editParam();
	void addLimits();
	
	void treeItemDoubleClicked(QTreeWidgetItem * item, int column);
	void treeItemChanged(QTreeWidgetItem *, QTreeWidgetItem *);
	void treeItemContentChanged(QTreeWidgetItem * item, int column);

public slots:
	void removeItem();
	void tst();
	void load();
	void save();
	void call();
	void call(const std::string &);
	void open();
	void setName();

protected:

	void editParam(QTreeWidgetItem *item);

	void addObject(CxExternalObject *obj, QTreeWidgetItem *parentItem);
	CxExternalObject *toObject(QTreeWidgetItem *);

	void addInputParam(CxInputParam *p);
	void addOutputParam(const CxOutputParam &op);
	void addObjectAttribute(const CxOutputParam &op);

	void addRule(const std::vector<std::string> &, QTreeWidgetItem *parentItem);
	void addCalculate(const CxCalculateRulePrivate &, QTreeWidgetItem *parent);

	void resetTree();
	void removeItem(QTreeWidgetItem *);

	void setScriptParams(); //读取界面参数，设置到m_scriptGenerator中

	void contextMenuEvent(QContextMenuEvent *);
	bool isTopLevelItem(QTreeWidgetItem *cItem);

	void expandItem(QTreeWidgetItem *);

	CxInputParam * toInputParam(QTreeWidgetItem *item);
	void updateParam(QTreeWidgetItem *pItem, CxInputParam *pParam);

	CxOutputParam toOutputParam(QTreeWidgetItem *item);
	void updateParam(QTreeWidgetItem *pItem, const CxOutputParam &);

private:

	QTreeWidgetItem *m_inputParamsItem;

	QTreeWidgetItem *m_limitsParamItem;

	QTreeWidgetItem *m_extendModuleItem;

	QTreeWidgetItem *m_outObjectItem;
	QTreeWidgetItem *m_outParamItem;
	QTreeWidgetItem *m_outputItem;

	QTreeWidgetItem *m_calRuleItem;

	QMenu *m_contextMenu;

	CxScriptGenerator *m_scriptGenerator;

	QAction *actInputParam;
	QAction *actOutputParam;
	QAction *actAddObject;
	QAction *actAddAttribute;
	QAction *actAddRule;
	QAction *actAddModule;
	QAction *actAddLimits;
	QAction *actRemoveItem;
	QAction *actClose;
	QAction *actActSave;
	QAction *actActLoad;
	QAction *actActCall;
	QAction *actActEdit;
	QAction *actSetName;

	bool m_loading;
	bool m_bResultTree;
};

#endif