#include "cxdialog.h"
#include "cxhighlighter.h"
#include "cxtextedit.h"
#include "cxobjectinit.h"

#include <QMessageBox>
#include <QDoubleValidator>
#include <QRegExp>
#include <QGroupBox>
#include <QValidator>

VariableEdit::VariableEdit(const QString &te, QWidget *parent)
	:QLineEdit(te, parent)
{
	QRegExp regx("^[_a-zA-Z][_a-zA-Z0-9]*$");
	QValidator *validator = new QRegExpValidator(regx);
	setValidator(validator);
}

VariableEdit::~VariableEdit()
{

}

CxBaseDialog::CxBaseDialog(QWidget *parent)
	:QDialog(parent)
{

}

CxBaseDialog::~CxBaseDialog()
{

}

void CxBaseDialog::setDefinedObjects(const std::vector<std::string> &dObjs)
{
	m_definedObjects = dObjs;
}

bool CxBaseDialog::checkIfNameUsed(const QString &name)
{
	if (name.isEmpty())
	{
		QMessageBox::warning(this, QStringLiteral("参数名称不能为空"),QStringLiteral("参数名称不能为空"));
		return true;
	}
	for (int i = 0;i < m_definedObjects.size();i++)
	{
		if (m_definedObjects[i].compare(name.toStdString()) == 0)
		{
			QMessageBox::warning(this, QStringLiteral("参数名称已被使用"),
				QStringLiteral("参数名称 ")+ name + QStringLiteral("已经被使用了，请重新命名"));;
			return true;
		}
	}

	return false;
}

bool CxBaseDialog::inputValid()
{
	return true;
}

void CxBaseDialog::initLayout()
{
	m_btnLayout = new QHBoxLayout();
	QPushButton *m_btnOK = new QPushButton(QStringLiteral("确认(&O))"));
	QPushButton *m_btnC = new QPushButton(QStringLiteral("取消(&C))"));
	connect(m_btnOK, SIGNAL(clicked()), SLOT(accept()));
	connect(m_btnC, SIGNAL(clicked()), SLOT(reject()));
	m_btnLayout->addStretch();
	m_btnLayout->addWidget(m_btnOK);
	m_btnLayout->addWidget(m_btnC);
}


CxConfigObjectDialog::CxConfigObjectDialog(CxExternalObject *obj, QWidget *parent)
	: CxBaseDialog(parent)
	, m_configObj(obj)
{
	Q_ASSERT(m_configObj);
	setWindowTitle(QStringLiteral("输出对象属性设置"));
	initLayout();
}

CxConfigObjectDialog::~CxConfigObjectDialog()
{

}

void CxConfigObjectDialog::initLayout()
{
	CxBaseDialog::initLayout();
	QVBoxLayout *widLayout = new QVBoxLayout(this);
	QGridLayout *mainLayout = new QGridLayout();
	mainLayout->addWidget(new QLabel("Object name:"), 0, 0, 1, 1);
	QLineEdit *objEdit = new VariableEdit(QString::fromStdString(m_configObj->objName));
	QLineEdit *clsEdit = new QLineEdit(QString::fromStdString(m_configObj->clsName));
	mainLayout->addWidget(objEdit, 0, 1, 1, 1);
	mainLayout->addWidget(new QLabel("Class name:"), 1, 0, 1, 1);
	mainLayout->addWidget(clsEdit, 1, 1, 1, 1);
	m_edits.push_back(objEdit);
	m_edits.push_back(clsEdit);
	clsEdit->setReadOnly(true);

	int i = 0;
	for (i = 0;i < m_configObj->attri.size();i++)
	{
		QLabel *label = new QLabel(QString::fromStdString(m_configObj->attri[i].first)+QString("(%1 )").arg(QString::fromStdString(m_configObj->attri[i].second.first)));
		QLineEdit *valEdit = new QLineEdit(QString::fromStdString(m_configObj->attri[i].second.second));
		mainLayout->addWidget(label, i+2, 0, 1, 1);
		mainLayout->addWidget(valEdit, i+2, 1, 1, 1);
		m_edits.push_back(valEdit);
	}

	widLayout->addLayout(mainLayout);
	widLayout->addLayout(m_btnLayout);
}

bool CxConfigObjectDialog::inputValid()
{
	if (checkIfNameUsed(m_edits[0]->text()))
	{
		return false;
	}

	return true;
}

void CxConfigObjectDialog::accept()
{
	if (inputValid())
	{
		m_configObj->objName = m_edits[0]->text().toStdString();
		for (int i = 0;i < m_configObj->attri.size();i++)
		{
			if (i+2 <= m_edits.size())
				m_configObj->attri[i].second.second = m_edits[2+i]->text().toStdString();
		}

		CxBaseDialog::accept();
	}
}

CxObjectDialog::CxObjectDialog(std::vector<std::string> &classList, QWidget *parent)
	: CxBaseDialog(parent)
	, m_createdObject(0)
	, m_clsList(classList)
{
	initLayout();
	setWindowTitle(QStringLiteral("选择输出对象类型"));
}

CxObjectDialog::~CxObjectDialog()
{

}

std::string CxObjectDialog::selectClass()
{
	return m_cls->currentText().toStdString();
}

CxExternalObject *CxObjectDialog::createdObj()
{
	return m_createdObject;
}

void CxObjectDialog::accept()
{
	// modify : READ from script to READ from INI file
	//m_createdObject = CxScriptGenerator::createObject(selectClass());
	m_createdObject = CxObjectInitProperty::createObject(QString::fromStdString(selectClass()));
	if (!m_createdObject) return;
	CxConfigObjectDialog dialog(m_createdObject);
	if (dialog.exec() == QDialog::Accepted)
	{
		CxBaseDialog::accept();
	}
}

void CxObjectDialog::initLayout()
{
	CxBaseDialog::initLayout();
	QVBoxLayout *mainLayout = new QVBoxLayout(this);

	QLabel *la = new QLabel("Select class :");
	m_cls = new QComboBox();
	for (int i = 0;i < m_clsList.size();i++)
	{
		m_cls->addItem(QString::fromStdString(m_clsList[i]));
	}

	QHBoxLayout *selLayout = new QHBoxLayout();
	selLayout->addWidget(la);
	selLayout->addWidget(m_cls);
	mainLayout->addLayout(selLayout);
	mainLayout->addLayout(m_btnLayout);
}

CxParamDialog::CxParamDialog(QWidget *parent, CxInputParam *param)
	:CxBaseDialog(parent)
	, m_definedParam(param)
{
	initLayout();
	if (m_definedParam)
		initParam(m_definedParam);
	setWindowTitle(QStringLiteral("输入参数属性设置"));
}

CxInputParam *CxParamDialog::param()
{
	QStringList typeList = QStringList() << "int" << "float" << "str";
	QStringList nullList = QStringList() << "true" << "false";
	if (!m_definedParam)
	{
		m_definedParam = new CxInputParam("newPara", nullList.at(m_allowNull->currentIndex()).toStdString());
	}
	else
	{
		m_definedParam->m_nullAllowed = nullList.at(m_allowNull->currentIndex()).toStdString();
	}
	m_definedParam->m_extraParams.clear();
	
	m_definedParam->m_name = m_nameEdit->text().toStdString();
	m_definedParam->m_min = m_minEdit->text().toStdString();
	m_definedParam->m_max = m_maxEdit->text().toStdString();
	m_definedParam->m_description = m_descEdit->text().toStdString();
	
	m_definedParam->m_value = m_valueEdit->text().isEmpty() ? "0" : m_valueEdit->text().toStdString();
	m_definedParam->m_type = typeList.at(m_typeBox->currentIndex()).toStdString();
	return m_definedParam;
}

void CxParamDialog::initParam(CxInputParam *param)
{
	m_nameEdit->setText(QString::fromStdString(param->m_name));
	m_maxEdit->setText(QString::fromStdString(param->m_max));
	m_minEdit->setText(QString::fromStdString(param->m_min));
	m_descEdit->setText(QString::fromStdString(param->m_description));
	m_valueEdit->setText(QString::fromStdString(param->m_value));
	QStringList typeList = QStringList() << "int" << "float" << "str";
	QStringList nullList = QStringList() << "true" << "false";
	m_typeBox->setCurrentIndex(typeList.indexOf(QString::fromStdString(param->m_type)));
	m_allowNull->setCurrentIndex(nullList.indexOf(QString::fromStdString(param->m_nullAllowed)));
}

void CxParamDialog::initLayout()
{
	CxBaseDialog::initLayout();

	m_keys = QStringList() << "name" << "description" << "max" << "min" << "value";
	QVBoxLayout *widLayout = new QVBoxLayout(this);
	QGridLayout *mainLayout = new QGridLayout();
	m_allowNull = new QComboBox();
	m_allowNull->addItems(QStringList() << QStringLiteral("可以") << QStringLiteral("不可以"));
	m_typeBox = new QComboBox();
	m_typeBox->addItems(QStringList() << QStringLiteral("整数")<< QStringLiteral("小数") <<QStringLiteral("文本"));
	connect(m_typeBox, SIGNAL(currentIndexChanged(int)), SLOT(typeChanged(int)));

	QLabel *nameLabel = new QLabel(QStringLiteral("参数名称："));
	QLabel *descLabel = new QLabel(QStringLiteral("参数说明："));
	QLabel *maxLabel = new QLabel(QStringLiteral("最大值："));
	QLabel *minLabel = new QLabel(QStringLiteral("最小值："));
	QLabel *valueLabel = new QLabel(QStringLiteral("值："));
	QLabel *nullLabel = new QLabel(QStringLiteral("可否为空："));
	QLabel *typeLabel = new QLabel(QStringLiteral("类型："));
	m_nameEdit = new VariableEdit();
	m_descEdit = new QLineEdit();
	m_maxEdit = new QLineEdit();
	m_minEdit = new QLineEdit();
	m_valueEdit = new QLineEdit();

	mainLayout->addWidget(nameLabel, 0, 0, 1, 1);
	mainLayout->addWidget(m_nameEdit, 0, 1, 1, 1);
	mainLayout->addWidget(descLabel, 1, 0, 1, 1);
	mainLayout->addWidget(m_descEdit, 1, 1, 1, 1);
	mainLayout->addWidget(maxLabel, 2, 0, 1, 1);
	mainLayout->addWidget(m_maxEdit, 2, 1, 1, 1);
	mainLayout->addWidget(minLabel, 3, 0, 1, 1);
	mainLayout->addWidget(m_minEdit, 3, 1, 1, 1);
	mainLayout->addWidget(valueLabel, 4, 0, 1, 1);
	mainLayout->addWidget(m_valueEdit, 4, 1, 1, 1);

	m_maxEdit->setValidator(new QDoubleValidator());
	m_minEdit->setValidator(new QDoubleValidator());
	mainLayout->addWidget(nullLabel, 5, 0, 1, 1);
	mainLayout->addWidget(m_allowNull, 5, 1, 1, 1);
	mainLayout->addWidget(typeLabel, 6, 0, 1, 1);
	mainLayout->addWidget(m_typeBox, 6, 1, 1, 1);

	typeChanged(0);
	widLayout->addLayout(mainLayout);
	widLayout->addLayout(m_btnLayout);
}

bool CxParamDialog::inputValid()
{
	if (checkIfNameUsed(m_nameEdit->text()))
	{
		return false;
	}

	bool allowNull = m_allowNull->currentIndex() == 0;

	if (m_typeBox->currentIndex() != 2)
	{
		if (m_maxEdit->text().isEmpty() || m_minEdit->text().isEmpty())
		{
			return true;
		}
		else
		{
			double value = m_valueEdit->text().toDouble();
			double mi = m_minEdit->text().toDouble();
			double ma = m_maxEdit->text().toDouble();
			if (value >= mi && value <= ma)
			{
				return true;
			}
			else
			{
				QMessageBox::warning(this, QStringLiteral("输入有误"), QStringLiteral("您输入的值不在定义的区间内，请重新输入."));
				return false;
			}
		}
	}
	else if (!allowNull && m_valueEdit->text().isEmpty())
	{
		QMessageBox::warning(this, QStringLiteral("输入有误"), QStringLiteral("您输入的值不能为空."));
		return false;
	}

	return true;
}

void CxParamDialog::accept()
{
	if (inputValid())
	{
		CxBaseDialog::accept();
	}
}

void CxParamDialog::typeChanged(int index)
{
	if (index != 2)
	{
		m_valueEdit->setValidator(new QDoubleValidator());
	}
	else
	{
		const QValidator *cd = m_valueEdit->validator();
		if (cd)
		{
			DEL_OBJ(cd);
		}
	}
}

CxRuleDialog::CxRuleDialog(QWidget *parent, const std::vector<std::string> &paramList, const QStringList &ruleValue)
	: CxBaseDialog(parent)
	, m_paramList(paramList)
	, m_ruleValues(ruleValue)
	, m_keys(QStringList()<< QStringLiteral("规则名称：") << QStringLiteral("规则说明："))
{
	initLayout();
	setWindowTitle(QStringLiteral("规则设置"));
	resize(600, 400);
}

std::vector<std::string> CxRuleDialog::rule()
{
	std::vector<std::string> retRule;

	for (int i = 0;i < m_keys.size();i++)
	{
		retRule.push_back(m_edits[i]->text().toStdString());
	}
	retRule.push_back(m_stateEdit->toPlainText().toStdString());
	return retRule;
}

void CxRuleDialog::initLayout()
{
	CxBaseDialog::initLayout();

	QVBoxLayout *widLayout = new QVBoxLayout(this);
	QGridLayout *mainLayout = new QGridLayout();
	int row = 0;
	foreach (QString key, m_keys)
	{
		QLabel *des = new QLabel(key);
		QLineEdit *edit = new QLineEdit();
		mainLayout->addWidget(des, row, 0, 1, 1);
		mainLayout->addWidget(edit, row, 1, 1, 1);
		m_edits.push_back(edit);
		if (row < m_ruleValues.size())
			edit->setText(m_ruleValues.at(row));
		row++;
	}
	QStringList keys;
	for (int i = 0;i < m_paramList.size();i++)
		keys.append(QString::fromStdString(m_paramList[i]));
	m_stateEdit = new CxTextEdit(this, keys);
	if (m_ruleValues.size() >= 3)
		m_stateEdit->setPlainText(m_ruleValues.at(2));
	new CxHighlighter(m_stateEdit->document());
	mainLayout->addWidget(new QLabel(QStringLiteral("规则语句：")), 2, 0, 1, 1);
	mainLayout->addWidget(m_stateEdit, 2, 1, 3, 1);

	widLayout->addLayout(mainLayout);
	widLayout->addLayout(m_btnLayout);
}

bool CxRuleDialog::inputValid()
{
	if (checkIfNameUsed(m_edits[0]->text()))
	{
		return false;
	}
	return true;
}

void CxRuleDialog::accept()
{
	if (inputValid())
	{
		CxBaseDialog::accept();
	}
}

CxModuleDialog::CxModuleDialog(QWidget *parent )
	: CxBaseDialog(parent)
	, m_keys(QStringList()<<QStringLiteral("模块名称：")<<QStringLiteral("模块说明："))
{
	initLayout();
	setWindowTitle(QStringLiteral("待添加模块属性设置"));
}

std::vector<std::string> CxModuleDialog::rule()
{
	std::vector<std::string> retRule;

	for (int i = 0;i < m_keys.size();i++)
	{
		retRule.push_back(m_edits[i]->text().toStdString());
	}
	return retRule;
}

void CxModuleDialog::initLayout()
{
	CxBaseDialog::initLayout();

	QVBoxLayout *widLayout = new QVBoxLayout(this);
	QGridLayout *mainLayout = new QGridLayout();
	int row = 0;
	foreach (QString key, m_keys)
	{
		QLabel *des = new QLabel(key);
		QLineEdit *edit = new QLineEdit();
		mainLayout->addWidget(des, row, 0, 1, 1);
		mainLayout->addWidget(edit, row, 1, 1, 1);
		m_edits.push_back(edit);
		row++;
	}

	widLayout->addLayout(mainLayout);
	widLayout->addLayout(m_btnLayout);
}

bool CxModuleDialog::inputValid()
{
	return true;
}

void CxModuleDialog::accept()
{
	if (inputValid())
	{
		CxBaseDialog::accept();
	}
}

CxAttributeDialog::CxAttributeDialog(QString textDesc, QWidget *parent, bool isAttribute)
	: CxBaseDialog(parent)
	, m_textDesc(textDesc)
	, m_isAttribute(isAttribute)
{
	initLayout();
}

CxAttributeDialog::CxAttributeDialog(QString textDesc, const CxOutputParam &outParam,
	QWidget *parent, bool isAttribute)
	: CxBaseDialog(parent)
	, m_textDesc(textDesc)
	, m_isAttribute(isAttribute)
{
	initLayout();
	setOutParam(outParam);
}

void CxAttributeDialog::setOutParam(const CxOutputParam &oParam)
{
	m_nameEdit->setText(QString::fromStdString(oParam.m_strName));
	m_descriptionEdit->setText(QString::fromStdString(oParam.m_strDescription));
	m_typeBox->setCurrentText(QString::fromStdString(oParam.m_strType));
}

CxOutputParam CxAttributeDialog::param()
{
	CxOutputParam p;
	p.m_strName = m_nameEdit->text().toStdString();
	p.m_strDescription = m_descriptionEdit->text().toStdString();
	p.m_strType = m_typeBox->currentText().toStdString();
	return p;
}

void CxAttributeDialog::initLayout()
{
	CxBaseDialog::initLayout();

	QVBoxLayout *widLayout = new QVBoxLayout(this);
	QGridLayout *mainLayout = new QGridLayout();

	QLabel *name = new QLabel(QStringLiteral("名称:"));
	//m_nameEdit = new VariableEdit();
	m_nameEdit = new QLineEdit();
	QLabel *desc = new QLabel(m_textDesc);
	m_descriptionEdit = new QLineEdit();
	QLabel *type = new QLabel(QStringLiteral("类型:"));
	m_typeBox = new QComboBox();
	m_typeBox->addItems(QStringList() << "int" << "float" << "str");
	mainLayout->addWidget(name, 0, 0, 1, 1);
	mainLayout->addWidget(m_nameEdit, 0, 1, 1, 1);
	mainLayout->addWidget(desc, 1, 0, 1, 1);
	mainLayout->addWidget(m_descriptionEdit, 1, 1, 1, 1);
	mainLayout->addWidget(type, 2, 0, 1, 1);
	mainLayout->addWidget(m_typeBox, 2, 1, 1, 1);

	widLayout->addLayout(mainLayout);
	widLayout->addLayout(m_btnLayout);
	setWindowTitle(QStringLiteral("新增参数属性设置"));
}

bool CxAttributeDialog::inputValid()
{
	QString name = m_nameEdit->text();
	if (m_isAttribute && checkIfNameUsed(name))
	{
		return false;
	}

	return true;
}

void CxAttributeDialog::accept()
{
	if (inputValid())
	{
		CxBaseDialog::accept();
	}
}


CxSetparamDialog::CxSetparamDialog(std::vector<CxInputParam *> &iParams, QWidget *parent)
	: m_inputParams(iParams)
{
	initLayout();
	setWindowTitle(QStringLiteral("参数设置"));
}

CxSetparamDialog::~CxSetparamDialog()
{

}

void CxSetparamDialog::accept()
{
	if (inputValid())
	{
		for (int i = 0;i < m_inputParams.size();i++)
		{
			m_inputParams[i]->m_value = m_paramEdit[i]->text().toStdString();
		}
		CxBaseDialog::accept();
	}
}

bool CxSetparamDialog::inputValid()
{
	bool ret = false;
	for (int i = 0;i < m_inputParams.size();i++)
	{
		// 不允许为空
		if (m_inputParams[i]->m_nullAllowed.compare("false") == 0)
		{
			if (m_paramEdit[i]->text().isEmpty())
			{
				QMessageBox::warning(this, QStringLiteral("参数设置错误"),
					QStringLiteral("参数%1不能为空，请重新设置.").arg(QString::fromStdString(m_inputParams[i]->m_name)));
				return false;
			}
		}
		else if (m_inputParams[i]->m_nullAllowed.compare("true") == 0 && m_paramEdit[i]->text().isEmpty())
		{
			m_paramEdit[i]->setText("0");
		}
	}

	return true;
}

void CxSetparamDialog::initLayout()
{
	CxBaseDialog::initLayout();
	QVBoxLayout *mainLayout = new QVBoxLayout(this);
	QGroupBox *contentGrp = new QGroupBox(QStringLiteral("参数编辑"));
	QGridLayout *contentsLayout = new QGridLayout(contentGrp);
	for (int i = 0;i < m_inputParams.size();i++)
	{
		QLabel *label = new QLabel(QString::fromStdString(m_inputParams[i]->m_name));
		QLineEdit *edit = new QLineEdit(QString::fromStdString(m_inputParams[i]->m_value));
		if (m_inputParams[i]->m_type.compare("str") != 0)
			edit->setValidator(new QDoubleValidator());
		label->setBuddy(edit);
		edit->setToolTip(QString::fromStdString(m_inputParams[i]->m_description));
		contentsLayout->addWidget(label, i, 0, 1, 1);
		contentsLayout->addWidget(edit, i, 1, 1, 1);
		contentsLayout->addWidget(new QLabel(QString::fromStdString(m_inputParams[i]->m_description)), i, 2, 1, 1);
		m_paramLabel.push_back(label);
		m_paramEdit.push_back(edit);
	}
	mainLayout->addWidget(contentGrp);
	mainLayout->addLayout(m_btnLayout);
}