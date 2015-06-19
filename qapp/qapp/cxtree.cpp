#include "cxtree.h"
#include "cxdialog.h"
#include "cxknowledgemanager.h"
#include "cxtranstoxml.h"
#include "cxobjectinit.h"

#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextCodec>
#include <QProcess>
#include <QFileInfo>

#ifdef	Q_OS_WIN32
#include <QProcess>
#else
#include <QDesktopServices>
#endif

#define ITEM_EDITABLE 0x0001
#define ITEM_OBJECT   0x0002
#define CAPP_PATH QString("")

#define XML_TEST

static string toGbkString(QString gbk)
{
	string inv = gbk.toStdString();
	QTextCodec *c = QTextCodec::codecForName("gb18030");
	if (c) 
	{
		inv = c->fromUnicode(gbk).data();
	}

	return inv;
}


CxTree::CxTree(QWidget *parent, bool isResult)
	:QTreeWidget(parent)
	, m_bResultTree(isResult)
	, m_loading(false)
{
	m_scriptGenerator = new CxScriptGenerator("");

	setFont(QFont("宋体", 12));
	m_contextMenu = new QMenu();
	actInputParam = m_contextMenu->addAction(QStringLiteral("添加输入参数(&A)"), this, SLOT(addInputParam()));
	actOutputParam = m_contextMenu->addAction(QStringLiteral("添加输出参数(&P)"), this, SLOT(addOutputParam()));
	actAddObject = m_contextMenu->addAction(QStringLiteral("添加输出对象(&O)"), this, SLOT(addObject()));
	actAddAttribute = m_contextMenu->addAction(QStringLiteral("添加属性(&T)"), this, SLOT(addObjectAttribute()));
	actAddRule = m_contextMenu->addAction(QStringLiteral("添加规则(&R)"), this, SLOT(addRule()));
	actAddLimits = m_contextMenu->addAction(QStringLiteral("添加限定条件(&L)"), this, SLOT(addLimits()));
	actAddModule = m_contextMenu->addAction(QStringLiteral("添加扩展模块(&M)"), this, SLOT(addModule()));
	actActEdit = m_contextMenu->addAction(QStringLiteral("编辑参数(&E)"), this, SLOT(editParam()));
	m_contextMenu->addSeparator();
	actRemoveItem = m_contextMenu->addAction(QStringLiteral("删除节点(&D)"), this, SLOT(removeItem()));
	actClose  = m_contextMenu->addAction(QStringLiteral("关闭(&C)"), this, SLOT(deleteLater()));
 	actActSave = m_contextMenu->addAction(QStringLiteral("另存为(&S)"), this, SLOT(save()));
// 	actActLoad = m_contextMenu->addAction(QStringLiteral("打开文件(&F)"), this, SLOT(load()));
 	actActCall = m_contextMenu->addAction(QStringLiteral("执行脚本(&L)"), this, SLOT(call()));
// 	actSetName = m_contextMenu->addAction(QStringLiteral("设置脚本文件名称(&N)"), this, SLOT(setName()));

	resetTree();
	setEditTriggers(DoubleClicked );
	connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), SLOT(treeItemDoubleClicked(QTreeWidgetItem *, int)));
	connect(this, SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)), SLOT(treeItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)));
	connect(this, SIGNAL(itemChanged(QTreeWidgetItem *, int)), this, SLOT(treeItemContentChanged(QTreeWidgetItem *, int)));
}

CxTree::~CxTree(void)
{

}

void CxTree::contextMenuEvent(QContextMenuEvent *)
{
	if (m_contextMenu)
	{
		QTreeWidgetItem *item = currentItem();
		actInputParam->setVisible(topItem(item) == m_inputParamsItem);
		actOutputParam->setVisible(topItem(item) == m_outputItem);
		actAddObject->setVisible(topItem(item) == m_outputItem);
		actAddAttribute->setVisible(item && item->data(0, Qt::UserRole).toInt() & ITEM_OBJECT);
		actAddRule->setVisible(topItem(item) == m_calRuleItem);
		actAddLimits->setVisible(topItem(item) == m_inputParamsItem);
		actAddModule->setVisible(topItem(item) == m_extendModuleItem);
		actRemoveItem->setVisible(!isTopLevelItem(item));
		actClose->setVisible(m_bResultTree);
		actActCall->setVisible(!m_bResultTree);
		actActEdit->setVisible(item && (item->parent() == m_calRuleItem || item->parent() == m_inputParamsItem));
		m_contextMenu->exec(QCursor::pos());
	}
}

bool CxTree::isTopLevelItem(QTreeWidgetItem *cItem)
{
	if (!cItem) return false;

	return cItem == m_outParamItem || cItem == m_outputItem || cItem == m_outObjectItem || cItem == m_inputParamsItem
		|| cItem == m_extendModuleItem || cItem == m_calRuleItem;
}

void CxTree::tst()
{
	std::vector<CxExternalObject *> obj = toObject();

	QLabel *l = new QLabel();
	QString text;
	for (int i = 0;i < obj.size();i++)
	{
		text += QString::fromStdString(obj[i]->objName) + QString::fromStdString(obj[i]->clsName) + "\n";
	}
	l->setText(text);
	l->show();
}

void CxTree::removeItem()
{
	QTreeWidgetItem *cCurrent = currentItem();
	if (!cCurrent) return;
	QTreeWidgetItem *cParent = currentItem()->parent();
	if (cParent)
	{
		cParent->removeChild(cCurrent);
		DEL_OBJ(cCurrent);
		setScriptParams();
	}
}

void CxTree::addObject()
{
	CxObjectDialog obj(m_scriptGenerator->classNameList());
	obj.setDefinedObjects(m_scriptGenerator->objectNames());
	if (obj.exec() == QDialog::Accepted)
	{
		CxExternalObject *eObj = obj.createdObj();
		QTreeWidgetItem *item = currentItem();
		QTreeWidgetItem *addItem = 0;
		if (!item) return;
		else if (item->data(0, Qt::UserRole).toInt() & ITEM_OBJECT)
			addItem = item;
		else if (item->parent() && item->parent()->data(0, Qt::UserRole).toInt() & ITEM_OBJECT)
			addItem = item->parent();
		else
			addItem = m_outObjectItem;
		addObject(eObj, new QTreeWidgetItem(addItem));
		setScriptParams();
	}
}

void CxTree::addInputParam()
{
	CxParamDialog p;
	p.setDefinedObjects(m_scriptGenerator->objectNames());
	if (p.exec() == QDialog::Accepted)
	{
		CxInputParam *newParam = p.param();
		addInputParam(newParam);
		setScriptParams();
	}
}

void CxTree::addOutputParam()
{
	CxAttributeDialog att(QStringLiteral("说明："), this, true);
	//att.setDefinedObjects(m_scriptGenerator->objectNames());
	if (att.exec() == QDialog::Accepted)
	{
		CxOutputParam p = att.param();
		addOutputParam(p);
		setScriptParams();
	}
}

void CxTree::addObjectAttribute()
{
	CxAttributeDialog att(QStringLiteral("值:"));
	if (att.exec() == QDialog::Accepted)
	{
		CxOutputParam p = att.param();
		addObjectAttribute(p);
	}
}

void CxTree::addOutputParam(const CxOutputParam &op)
{
	QTreeWidgetItem *newChild = new QTreeWidgetItem(m_outParamItem, QStringList() << QString::fromStdString(op.m_strName)
		<< QString::fromStdString(op.m_strType) << QString::fromStdString(op.m_strDescription));
	newChild->setData(0, Qt::UserRole, ITEM_EDITABLE);
	newChild->setData(1, Qt::UserRole, ITEM_EDITABLE);
	newChild->setData(2, Qt::UserRole, ITEM_EDITABLE);
}

void CxTree::addObjectAttribute(const CxOutputParam &op)
{
	QTreeWidgetItem *item = currentItem();
	QTreeWidgetItem *addItem = 0;
	if (!item) return;
	else if (item->data(0, Qt::UserRole).toInt() & ITEM_OBJECT)
		addItem = item;
	else 
		addItem = item->parent();
	if (!addItem) return;
	QTreeWidgetItem *newChild = new QTreeWidgetItem(addItem, QStringList() << QString::fromStdString(op.m_strName)
		<< QString::fromStdString(op.m_strType) << QString::fromStdString(op.m_strDescription));
	newChild->setData(0, Qt::UserRole, ITEM_EDITABLE);
	newChild->setData(1, Qt::UserRole, ITEM_EDITABLE);
	newChild->setData(2, Qt::UserRole, ITEM_EDITABLE);
}

void CxTree::addInputParam(CxInputParam *p)
{
	QTreeWidgetItem *newParam = new QTreeWidgetItem(m_inputParamsItem, QStringList() << QString::fromStdString(p->m_name));
	newParam->setData(0, Qt::UserRole, ITEM_EDITABLE);
	QTreeWidgetItem *pro = new QTreeWidgetItem(newParam, QStringList() << "nullAllowed" << QString::fromStdString(p->m_nullAllowed));

	pro = new QTreeWidgetItem(newParam, QStringList() << "type"
		<< QString::fromStdString(p->m_type));
	pro->setData(1, Qt::UserRole, ITEM_EDITABLE);
	pro = new QTreeWidgetItem(newParam, QStringList() << "description"
		<< QString::fromStdString(p->m_description));
	pro->setData(1, Qt::UserRole, ITEM_EDITABLE);
	pro = new QTreeWidgetItem(newParam, QStringList() << "min"
		<< QString::fromStdString(p->m_min));
	pro->setData(1, Qt::UserRole, ITEM_EDITABLE);
	pro = new QTreeWidgetItem(newParam, QStringList() << "max"
		<< QString::fromStdString(p->m_max));
	pro->setData(1, Qt::UserRole, ITEM_EDITABLE);
	pro = new QTreeWidgetItem(newParam, QStringList() << "value"
		<< QString::fromStdString(p->m_value));
	pro->setData(1, Qt::UserRole, ITEM_EDITABLE);

	m_inputParamsItem->setExpanded(true);
}


void CxTree::addRule()
{
	CxRuleDialog rl(this, m_scriptGenerator->objectNames());
	rl.setDefinedObjects(m_scriptGenerator->objectNames());
	if (rl.exec() == QDialog::Accepted)
	{
		addRule(rl.rule(), m_calRuleItem);
	}
}

void CxTree::addLimits()
{
	CxRuleDialog rl(this, m_scriptGenerator->objectNames());
	rl.setDefinedObjects(m_scriptGenerator->objectNames());
	if (rl.exec() == QDialog::Accepted)
	{
		addRule(rl.rule(), m_limitsParamItem);
	}
}

void CxTree::addRule(const std::vector<std::string> &ru, QTreeWidgetItem *parentItem)
{
	addCalculate(CxCalculateRulePrivate(ru[0], ru[1], ru[2]), parentItem);
}

void CxTree::addCalculate(const CxCalculateRulePrivate &rulePrivate, QTreeWidgetItem *parent)
{
	QTreeWidgetItem *newParam = new QTreeWidgetItem(parent,  QStringList() << QString::fromStdString(rulePrivate.m_strName));
	newParam->setData(0, Qt::UserRole, ITEM_EDITABLE);
	QTreeWidgetItem *pro = new QTreeWidgetItem(newParam, QStringList() << QString::fromStdString(rulePrivate.m_strDescription) 
		<< QString::fromStdString(rulePrivate.m_strStatement));
	pro->setData(0, Qt::UserRole, ITEM_EDITABLE);
	pro->setData(1, Qt::UserRole, ITEM_EDITABLE);
	newParam->setExpanded(true);
}

void CxTree::addModule()
{
	CxModuleDialog md;
	if (md.exec() == QDialog::Accepted)
	{
		QTreeWidgetItem *newParam = new QTreeWidgetItem(m_extendModuleItem,  QStringList() << QString::fromStdString(md.rule()[0])
			<< QString::fromStdString(md.rule()[1]));
		newParam->setExpanded(true);
		newParam->setData(0, Qt::UserRole, ITEM_EDITABLE);
		newParam->setData(1, Qt::UserRole, ITEM_EDITABLE);
	}
}

void CxTree::save()
{
	CxKnowlegdeManager *kmanager = new CxKnowlegdeManager(m_scriptGenerator->currentKnowledge(), this);
	if (kmanager->exec() == QDialog::Accepted)
	{
		QString fileName = QFileDialog::getSaveFileName(this, "", "", "Caxa Rule Files (*.rxl)");
		if (!fileName.isEmpty())
		{
			m_scriptGenerator->setKnowledge(kmanager->toKnowledge());
			setScriptParams();
			string name = fileName.toStdString();
			QTextCodec *c = QTextCodec::codecForName("gb18030");
			if (c) 
			{
				name = c->fromUnicode(fileName).data();
				m_scriptGenerator->saveToFile(name);
			}
		}
	}
}

void CxTree::load(const std::string &fileName)
{
	if (m_scriptGenerator->openFromFile(fileName))
	{
		resetTree();
		emit paramNameModified(m_scriptGenerator->objectNames());
	}
}

void CxTree::load()
{
	m_loading = true;
	QString fileName = QFileDialog::getOpenFileName(this, "", "", "Caxa Rule Files (*.rxl)");
	if (!fileName.isEmpty())
	{
		string name = fileName.toStdString();
		QTextCodec *c = QTextCodec::codecForName("gb18030");
		if (c) 
		{
			name = c->fromUnicode(fileName).data();
			load(name);
		}
	}
	m_loading = false;
}

void CxTree::setScriptParams()
{
	m_scriptGenerator->setInputParams(toInputParam());
	m_scriptGenerator->setExtends(toExtendsModule());
	m_scriptGenerator->setObjectList(toObject());
	m_scriptGenerator->setOutParamList(toOutputParams());
	m_scriptGenerator->setCalculateRule(toCalculateRule());

#ifdef NEED_LIMITERS
	m_scriptGenerator->setLimiter(toLimits());
#endif

	emit paramNameModified(m_scriptGenerator->objectNames());
}

void CxTree::open()
{
	QString fileName = QString::fromStdString(m_scriptGenerator->scriptFileName())+".py";
#ifdef	Q_OS_WIN32
	QProcess::startDetached(QString("notepad.exe %1").arg(fileName.toStdString().c_str()));
#else
	QDesktopServices::openUrl(QUrl::fromLocalFile(fileName));
#endif
}

void CxTree::setName()
{
	QString fileName = QFileDialog::getSaveFileName(this, "", "", "*.py");
	if (!fileName.isEmpty())
	{
		m_scriptGenerator->setScriptFileName(QFileInfo(fileName).baseName().toStdString());
	}
}

void CxTree::clearContents()
{
	foreach (QTreeWidgetItem *item, QList<QTreeWidgetItem *>() << m_inputParamsItem << m_extendModuleItem
		<< m_outObjectItem << m_outParamItem << m_calRuleItem)
	{
		QList<QTreeWidgetItem *> itemList = item->takeChildren();
		foreach (QTreeWidgetItem *it, itemList)
			DEL_OBJ(it);
	}
	setScriptParams();
}

void CxTree::expandResultItem()
{
	collapseAll();
	expandItem(m_outputItem);
}

void CxTree::expandItem(QTreeWidgetItem *eItem)
{
	if (!eItem) return;
	eItem->setExpanded(true);
	for (int i = 0;i < eItem->childCount();i++)
	{
		expandItem(eItem->child(i));
	}
}

void CxTree::call(const std::string &scName)
{
	vector<CxExternalObject *> retObject;
	vector<pair<string, string> > retParams;
	CxScriptGenerator::CallResult res = m_scriptGenerator->callScript(retObject, retParams, scName);
	if (res == CxScriptGenerator::CalculateRight)
	{
		//QString templateName = QFileDialog::getOpenFileName(this, QStringLiteral("执行成功，如果您希望结果可以转换成cxp格式的文件，您需要选择一个cxp文件的模板"), "./Template", "*.xml");
		QString templateName = "./Template/" + QString::fromStdString(m_scriptGenerator->currentKnowledge().m_strName)+".xml";
		if (!templateName.isEmpty())
		{	
			CxTransToXml trXml;
			//string readPath = CxObjectInitProperty::TemplatePath.toStdString() + "/八边形.xml";
			string readPath = toGbkString(templateName);
			if (trXml.readTemplate(readPath))
			{	
				string saveName = "C:\\Program Files\\xxcapp.xml";
				trXml.saveToFile(retObject, retParams, saveName);

				if (QMessageBox::question(this, QStringLiteral("调用成功"), QStringLiteral("您想现在使用工艺图表打开这个工艺文件，并转化成cxp文件存储吗？")) == QMessageBox::Yes)
				{

					if (!QProcess::startDetached(CxObjectInitProperty::CappPath , QStringList() << QString::fromStdString(saveName)))
					{
						QMessageBox::critical(this, QStringLiteral("调用失败"), QStringLiteral("启动工艺图表失败"));
					}
				}
			}
		}
		else
		{
			QMessageBox::question(this, QStringLiteral("调用成功"), QStringLiteral("执行成功"));
		}
		CxTree *newTree = new CxTree(0, true);
		newTree->setWindowTitle("Script out result");
		newTree->addObject(retObject);
		newTree->addCalcualteRule(m_scriptGenerator->calulateRule());
		newTree->addExtendsModule(m_scriptGenerator->extends());
		newTree->addInputParam(m_scriptGenerator->inputParams());
		for (int i = 0;i < retParams.size();i++)
		{
			string pDescription;
			string pOutType = "str";
			for (int j = 0;j < m_scriptGenerator->outparamList().size();j++)
			{
				if (m_scriptGenerator->outparamList()[j].m_strName.compare(retParams[i].first) == 0)
				{
					pDescription = m_scriptGenerator->outparamList()[j].m_strDescription;
					pOutType = m_scriptGenerator->outparamList()[j].m_strType;
					break;
				}
			}

			newTree->addOutputParam(CxOutputParam(retParams[i].first, 
				retParams[i].second + "  Description :" + pDescription, pOutType));
		}
		newTree->expandResultItem();
		emit newWidget(newTree);
	}
	else
	{
		QMessageBox::critical(this, QStringLiteral("调用脚本失败"), QString::fromStdString(m_scriptGenerator->lastError()) +
			QStringLiteral("\n请检查您设置的输入输出参数、规则等是否正确，再重新执行"));
	} 
}

void CxTree::call() 
{
	std::vector<CxInputParam *> params = toInputParam();
	CxSetparamDialog dialog(params, this);
	if (dialog.exec() == QDialog::Accepted)
	{
		QList<QTreeWidgetItem *> itemList = m_inputParamsItem->takeChildren();
		foreach (QTreeWidgetItem *it, itemList)
			DEL_OBJ(it);
		addInputParam(params);
		setScriptParams();
		string fileName = m_scriptGenerator->scriptFileName() + ".py";
		m_scriptGenerator->writeScript(fileName);
		emit scriptFileSaved(fileName);
		call( m_scriptGenerator->scriptFileName());
	}
} 

void CxTree::removeItem(QTreeWidgetItem *rItem)
{
	if (!rItem) return;
	QTreeWidgetItem *cParent = rItem->parent();
	if (cParent)
	{
		cParent->removeChild(rItem);
		DEL_OBJ(rItem);
	}
}

void CxTree::resetTree()
{
	clear();
	m_inputParamsItem = new QTreeWidgetItem(QStringList() << QStringLiteral("输入参数"));
	m_limitsParamItem = new QTreeWidgetItem(QStringList() << QStringLiteral("限定条件"));
	m_extendModuleItem = new QTreeWidgetItem(QStringList() << QStringLiteral("扩展模块"));
	m_outputItem = new QTreeWidgetItem(QStringList() << QStringLiteral("输出"));
	m_outParamItem = new QTreeWidgetItem(m_outputItem, QStringList() << QStringLiteral("输出参数"));
	m_outObjectItem = new QTreeWidgetItem(m_outputItem, QStringList() << QStringLiteral("输出对象"));
	m_calRuleItem = new QTreeWidgetItem(QStringList() << QStringLiteral("计算规则"));
	addTopLevelItems(QList<QTreeWidgetItem *> () << m_inputParamsItem << m_limitsParamItem << m_outputItem << m_calRuleItem << m_extendModuleItem);

	setHeaderLabels(QStringList() << QStringLiteral("名称") << QStringLiteral("类型") << QStringLiteral("值"));
	addInputParam(m_scriptGenerator->inputParams());
#ifdef NEED_LIMITERS
	addLimits(m_scriptGenerator->limiters());
#endif
	addCalcualteRule(m_scriptGenerator->calulateRule());
	addExtendsModule(m_scriptGenerator->extends());
	addOutputParam(m_scriptGenerator->outparamList());
	addObject(m_scriptGenerator->objectList());

}

void CxTree::editParam()
{
	editParam(currentItem());
}

void CxTree::editParam(QTreeWidgetItem *cItem)
{
	if (!cItem) return;

	QString previous = cItem->text(0);
	QString current;
	if (cItem->parent() == m_calRuleItem || cItem->parent() == m_limitsParamItem)
	{
		if (cItem->childCount() != 0)
		{
			CxRuleDialog dialog(this, m_scriptGenerator->objectNames(), QStringList() << cItem->text(0)
				<< cItem->child(0)->text(0) << cItem->child(0)->text(1));
			if (dialog.exec() == QDialog::Accepted)
			{
				std::vector<std::string> newRules = dialog.rule();
				current = QString::fromStdString(newRules[0]);
				cItem->setText(0, current);
				cItem->child(0)->setText(0, QString::fromStdString(newRules[1]));
				cItem->child(0)->setText(1, QString::fromStdString(newRules[2]));
			}
		}

	}
	else if (cItem->parent() == m_inputParamsItem)
	{
		CxParamDialog pDialog(this, toInputParam(cItem));
		if (pDialog.exec() == QDialog::Accepted)
		{
			CxInputParam *p =  pDialog.param();
			current = QString::fromStdString(p->m_name);
			updateParam(cItem, p);
		}
	}
	else if (cItem->parent() == m_outParamItem)
	{
		CxAttributeDialog attDialog(QStringLiteral("说明："), toOutputParam(cItem), this, true);
		if (attDialog.exec() == QDialog::Accepted)
		{
			CxOutputParam p = attDialog.param();
			current = QString::fromStdString(p.m_strName);
			updateParam(cItem, p);
		}
	}

	if (current != previous)
	{
		emit paramNameChanged(previous, current);
		setScriptParams();
	}
}

void CxTree::treeItemDoubleClicked(QTreeWidgetItem * item, int column)
{
	if (!item) return;
	if (item->parent() == m_calRuleItem || item->parent() == m_inputParamsItem || item->parent() == m_outParamItem
		|| item->parent() == m_limitsParamItem)
	{
		editParam();
		item->setFlags(item->flags() & ~(Qt::ItemIsEditable));
	}
	else if (item->data(column, Qt::UserRole).toInt() & ITEM_EDITABLE)
	{
		item->setFlags(item->flags() | Qt::ItemIsEditable);
	}
	else
	{
		item->setFlags(item->flags() & ~(Qt::ItemIsEditable));
	}
}

void CxTree::treeItemChanged(QTreeWidgetItem *cur, QTreeWidgetItem *pre)
{
	if (!cur)
		return;
	QFont f = cur->font(0);
	f.setBold(true);
	cur->setFont(0, f);
	if (pre)
	{
		f.setBold(false);
		pre->setFont(0, f);
	}
}

void CxTree::treeItemContentChanged(QTreeWidgetItem * , int )
{
	if (!m_loading)
	{
		emit treeContentChanged();
	}
}

QTreeWidgetItem *CxTree::objectBottomItem(QTreeWidgetItem *current)
{
	QTreeWidgetItem *topItem;
	if (current && current->parent())
	{
		topItem = new QTreeWidgetItem(current->parent());
	}
	else
	{
		topItem = new QTreeWidgetItem(m_outObjectItem);
	}
	return topItem;
}

QTreeWidgetItem *CxTree::topItem(QTreeWidgetItem *cItem)
{
	if (!cItem) return 0;

	while (cItem->parent())
	{
		cItem = cItem->parent();
	}

	return cItem;
}

void CxTree::addObject(const std::vector<CxExternalObject *> &externalObjects)
{
	for (int i = 0;i < externalObjects.size();i++)
	{
		QTreeWidgetItem *topItem = new QTreeWidgetItem(m_outObjectItem);
		addObject(externalObjects[i], topItem);
	}

	expandAll();
	for (int i = 0;i < 2;i++)
		resizeColumnToContents(i);
}

void CxTree::addObject(CxExternalObject *obj, QTreeWidgetItem *parentItem)
{
	if (!parentItem || !obj) return;
	QStringList title;
	title.append(QString::fromStdString(obj->objName));
	title.append(QString::fromStdString(obj->clsName));
	parentItem->setText(0, title[0]);
	parentItem->setText(1, title[1]);
	parentItem->setData(0, Qt::UserRole, ITEM_EDITABLE | ITEM_OBJECT);
	QTreeWidgetItem *propertyItem = 0;
	for (int i = 0;i < obj->attri.size();i++)
	{
		propertyItem = new QTreeWidgetItem(parentItem);
		propertyItem->setText(0, QString::fromStdString(obj->attri[i].first));
		propertyItem->setText(1, QString::fromStdString(obj->attri[i].second.first));
		propertyItem->setText(2, QString::fromStdString(obj->attri[i].second.second));

		// TEMPORARY ALLOW MODIFY
		propertyItem->setData(0, Qt::UserRole, ITEM_EDITABLE);
		propertyItem->setData(1, Qt::UserRole, ITEM_EDITABLE);
		propertyItem->setData(2, Qt::UserRole, ITEM_EDITABLE);
	}

	for (int i = 0;i < obj->children.size();i++)
	{
		QTreeWidgetItem *childItem = new QTreeWidgetItem(propertyItem);
		addObject(obj->children[i], childItem);
	}

	parentItem->setExpanded(true);
}

std::vector<CxExternalObject *> CxTree::toObject()
{
	std::vector<CxExternalObject *> retObj;
	for (int i = 0;i < m_outObjectItem->childCount();i++)
	{
		retObj.push_back(toObject(m_outObjectItem->child(i)));
	}

	return retObj;
}

void CxTree::addInputParam(const std::vector<CxInputParam *> &params)
{
	for (int i = 0;i < params.size();i++)
	{
		addInputParam(params[i]);
	}
}

std::vector<CxInputParam *> CxTree::toInputParam()
{
	std::vector<CxInputParam *> interfaceParam;
	for (int i = 0;i < m_inputParamsItem->childCount();i++)
	{
		CxInputParam *pParam = toInputParam(m_inputParamsItem->child(i));
		if (pParam)
			interfaceParam.push_back(pParam);
	}

	return interfaceParam;
}

CxInputParam * CxTree::toInputParam(QTreeWidgetItem *nameItem)
{
	if (!nameItem) return 0;
	CxInputParam *pParam = new CxInputParam(nameItem->text(0).toStdString(), nameItem->child(0)->text(1).toStdString());
	pParam->m_type = nameItem->child(1)->text(1).toStdString();
	pParam->m_description = nameItem->child(2)->text(1).toStdString();
	pParam->m_min = nameItem->child(3)->text(1).toStdString();
	pParam->m_max = nameItem->child(4)->text(1).toStdString();
	pParam->m_value = nameItem->child(5)->text(1).toStdString();

	return pParam;
}

void CxTree::updateParam(QTreeWidgetItem *pItem, CxInputParam *pParam)
{
	pItem->setText(0, QString::fromStdString(pParam->m_name));
	pItem->child(0)->setText(1, QString::fromStdString(pParam->m_nullAllowed));

	pItem->child(1)->setText(1, QString::fromStdString(pParam->m_type));
	pItem->child(2)->setText(1, QString::fromStdString(pParam->m_description));
	pItem->child(3)->setText(1, QString::fromStdString(pParam->m_min));
	pItem->child(4)->setText(1, QString::fromStdString(pParam->m_max));
	pItem->child(5)->setText(1, QString::fromStdString(pParam->m_value));
}

void CxTree::addCalcualteRule(CxCalculateRule *cr)
{
	std::vector<CxCalculateRulePrivate> rulePrivate = cr->calculateParam();

	for (int i = 0;i < rulePrivate.size();i++)
	{
		addCalculate(rulePrivate[i], m_calRuleItem);
	}
}

CxCalculateRule *CxTree::toCalculateRule()
{
	CxCalculateRule *cp = new CxCalculateRule();
	for (int i = 0;i < m_calRuleItem->childCount();i++)
	{
		QTreeWidgetItem *nameItem = m_calRuleItem->child(i);
		std::vector<std::string> vec;
		vec.push_back(nameItem->text(0).toStdString());
		vec.push_back(nameItem->child(0)->text(0).toStdString());
		vec.push_back(nameItem->child(0)->text(1).toStdString());
		cp->addParam(vec);
	}

	return cp;
}

void CxTree::addLimits(CxCalculateRule *cr)
{
	std::vector<CxCalculateRulePrivate> rulePrivate = cr->calculateParam();

	for (int i = 0;i < rulePrivate.size();i++)
	{
		addCalculate(rulePrivate[i], m_limitsParamItem);
	}
}

CxCalculateRule *CxTree::toLimits()
{
	CxCalculateRule *cp = new CxCalculateRule();
	for (int i = 0;i < m_limitsParamItem->childCount();i++)
	{
		QTreeWidgetItem *nameItem = m_limitsParamItem->child(i);
		std::vector<std::string> vec;
		vec.push_back(nameItem->text(0).toStdString());
		vec.push_back(nameItem->child(0)->text(0).toStdString());
		vec.push_back(nameItem->child(0)->text(1).toStdString());
		cp->addParam(vec);
	}

	return cp;
}

void CxTree::addExtendsModule(const std::vector<std::pair<string, string> > &extendMod)
{
	for (int i = 0;i < extendMod.size();i++)
	{
		QTreeWidgetItem *pro = new QTreeWidgetItem(m_extendModuleItem, QStringList() << QString::fromStdString(extendMod[i].first) 
			<< QString::fromStdString(extendMod[i].second));
		pro->setData(0, Qt::UserRole, ITEM_EDITABLE);
		pro->setData(1, Qt::UserRole, ITEM_EDITABLE);
	}
}

std::vector<std::pair<string, string> > CxTree::toExtendsModule()
{
	std::vector<std::pair<string, string> > ex;
	for (int i = 0;i < m_extendModuleItem->childCount();i++)
	{
		ex.push_back(std::make_pair(m_extendModuleItem->child(i)->text(0).toStdString(), m_extendModuleItem->child(i)->text(1).toStdString()));
	}
	return ex;
}

void CxTree::addOutputParam(const std::vector<CxOutputParam> &outParams)
{
	for (int i = 0;i < outParams.size();i++)
	{
		addOutputParam(outParams[i]);
	}
}

std::vector<CxOutputParam> CxTree::toOutputParams()
{
	std::vector<CxOutputParam> retParams;
	for (int i = 0;i < m_outParamItem->childCount();i++)
	{
		retParams.push_back(toOutputParam(m_outParamItem->child(i)));
	}
	return retParams;
}

CxOutputParam CxTree::toOutputParam(QTreeWidgetItem *item)
{
	if (!item) return CxOutputParam();
	return CxOutputParam(item->text(0).toStdString(), item->text(2).toStdString(), item->text(1).toStdString());
}

void CxTree::updateParam(QTreeWidgetItem *pItem, const CxOutputParam &oParam)
{
	if (!pItem) return;
	pItem->setText(0, QString::fromStdString(oParam.m_strName));
	pItem->setText(2, QString::fromStdString(oParam.m_strDescription));
	pItem->setText(1, QString::fromStdString(oParam.m_strType));
}

CxExternalObject *CxTree::toObject(QTreeWidgetItem *topItem)
{
	CxExternalObject *retObj = new CxExternalObject();
	retObj->objName = topItem->text(0).toStdString();
	retObj->clsName = topItem->text(1).toStdString();

	for (int i = 0;i < topItem->childCount();i++)
	{
		QTreeWidgetItem *childItem = topItem->child(i);
		retObj->attri.push_back(std::make_pair(childItem->text(0).toStdString(),
			std::make_pair(childItem->text(1).toStdString(), childItem->text(2).toStdString())));
		if (childItem->childCount() != 0)
		{
			// child 递归添加
			for (int j = 0;j < childItem->childCount();j++)
			{
				CxExternalObject *childObject =toObject(childItem->child(j));
				retObj->children.push_back(childObject);
			}
		}
	}

	return retObj;
}

QString CxTree::cueText()
{
	QString returnText;

// 	std::vector<ExternalObject *> objs = toObject();
// 	std::vector<WParamDefine *> inParams = toInputParam();
// 	WCalculateRule *rules = toCalculateRule();
// 	std::vector<std::pair<string, string> > extends = toExtendsModule();
// 	std::vector<OutputParam> outParams = toOutputParams();
// 
// 	for (int i = 0; i < rules->calculateParam().size();i++)
// 	{
// 		returnText += QString("%1 : %2\n").arg(QString::fromStdString(rules->calculateParam()[i].m_strName)).arg(QString::fromStdString(rules->calculateParam()[i].m_strStatement));
// 	}

	return returnText;
}

void CxTree::editParam(const QString &paramName)
{
	foreach (QTreeWidgetItem *editableItem, QList<QTreeWidgetItem *>() << m_inputParamsItem << m_outParamItem << m_calRuleItem)
	{
		for (int i = 0;i < editableItem->childCount();i++)
		{
			if (editableItem->child(i)->text(0) == paramName)
			{
				editParam(editableItem->child(i));
				return;
			}
		}
	}
}

QString CxTree::getParamText(const QString &paramName)
{
	QString retTooltip;
	foreach (QTreeWidgetItem *editableItem, QList<QTreeWidgetItem *>() << m_inputParamsItem << m_outParamItem << m_calRuleItem)
	{
		for (int i = 0;i < editableItem->childCount();i++)
		{
			QTreeWidgetItem *chi = editableItem->child(i);
			if (chi->text(0) == paramName)
			{
				retTooltip = QString("%1%2\n").arg(QStringLiteral("参数名称:")).arg(chi->text(0));

				if (editableItem == m_inputParamsItem)
				{
					retTooltip.prepend(QStringLiteral("输入参数\n"));
					retTooltip += QString("%1%2").arg(QStringLiteral("参数值:")).arg(chi->child(5)->text(1));
				}
				else if (editableItem == m_outParamItem)
				{
					retTooltip.prepend(QStringLiteral("输出参数\n"));
					retTooltip += QString("%1%2").arg(QStringLiteral("参数说明:")).arg(chi->text(2));
				}
				else
				{
					retTooltip.prepend(QStringLiteral("计算规则\n"));
					retTooltip += QString("%1%2").arg(QStringLiteral("参数说明:")).arg(chi->child(0)->text(0));
				}
				return retTooltip;
			}
		}
	}

	return retTooltip;
}