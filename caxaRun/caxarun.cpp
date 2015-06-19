#include "caxarun.h"

#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTextBrowser>
#include <QMessageBox>
#include <QFileDialog>
#include <QHeaderView>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QTextCodec>
#include <QProcess>
#include <QSettings>

#include "cxtranstoxml.h"
#include "cxxmlhelper.h"

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

CaxaRun::CaxaRun(bool needsResult, QWidget *parent)
	: m_needsResult(needsResult)
	, QDialog(parent)
	, m_generator(new CxScriptGenerator(""))
	, m_resultBrowser(0)
	, m_savePath("C:\\Program Files\\xxcapp.xml")
{
	ui.setupUi(this);

	connect(ui.btn_open, SIGNAL(clicked()), SLOT(open()));
	connect(ui.btn_run, SIGNAL(clicked()), SLOT(run()));
	connect(ui.btn_quit, SIGNAL(clicked()), SLOT(close()));
	connect(ui.cb_knowledge, SIGNAL(currentIndexChanged(int)), SLOT(currentIndexChanged(int)));
	connect(ui.paramTable, SIGNAL(doubleClicked(const QModelIndex &)), SLOT(doubleClicked(const QModelIndex &)));

	connect(ui.btn_display, SIGNAL(clicked()), SLOT(displayInCapp()));
	connect(ui.btn_openFile, SIGNAL(clicked()), SLOT(openFromFile()));
	connect(ui.btn_save, SIGNAL(clicked()), SLOT(saveToFile()));

	m_model = new QStandardItemModel();
	ui.paramTable->setModel(m_model);
}

void CaxaRun::doubleClicked(const QModelIndex &index)
{
	if (index.isValid())
	{
		QStandardItem *item = m_model->item(index.row(), index.column());
		if (index.column() == 1)
			item->setFlags(item->flags() | Qt::ItemIsEditable);
		else
			item->setFlags(item->flags() & ~(Qt::ItemIsEditable));
	}
}

void CaxaRun::currentIndexChanged(int )
{
	QString text = ui.cb_knowledge->currentText();

	std::map<std::string, CxKnowlegdeDesc>::iterator it = m_knowledges.begin();
	while (it != m_knowledges.end())
	{
		if (QString::fromStdString(it->second.m_strName) == text)
		{

			setKnowlegde(it->second);
			break;
		}

		it++;
	}
}

CaxaRun::~CaxaRun()
{

}

void CaxaRun::open()
{
	QString fileName = QFileDialog::getExistingDirectory(this, "", "");
	if (!fileName.isEmpty())
	{
		CxXmlHelper helper("");
		string name = fileName.toStdString();
		QTextCodec *c = QTextCodec::codecForName("gb18030");
		if (c) 
		{
			name = c->fromUnicode(fileName).data();
		}

		if (helper.read(name))
		{
			m_knowledges = helper.knowlegdes();
			ui.cb_knowledge->clear();

			std::map<std::string, CxKnowlegdeDesc>::iterator it = m_knowledges.begin();
			while (it != m_knowledges.end())
			{  
				ui.cb_knowledge->addItem(QString::fromStdString(it->second.m_strName.c_str()));
				it++;
			}
		}
	}
}

void CaxaRun::openFromFile()
{
	QString fileName = QFileDialog::getOpenFileName(this, "", "", "Caxa Rule Files (*.rxl)");
	if (!fileName.isEmpty())
	{
		CxFlowParse parse;
		string name = fileName.toStdString();
		QTextCodec *c = QTextCodec::codecForName("gb18030");
		if (c) 
		{
			name = c->fromUnicode(fileName).data();
		}
		if (parse.setFileName(name))
		{
			ui.cb_knowledge->clear();
			CxKnowlegdeDesc desc = parse.knowledgetDescription();
			setKnowlegde(desc);
		}
	}
}

void CaxaRun::setKnowlegde(const CxKnowlegdeDesc &desc)
{
	if (ui.cb_knowledge->count() == 0)
	{
		ui.cb_knowledge->addItem(QString::fromStdString(desc.m_strName));
	}
	ui.le_desc->setText(QString::fromStdString(desc.m_strDescription));
	ui.le_fileVersion->setText(QString::fromStdString(desc.m_strFileVersion));
	ui.le_id->setText(QString::fromStdString(desc.m_strID));
	ui.le_verison->setText(QString::fromStdString(desc.m_strVersion));
	ui.le_pix->setText(QString::fromStdString(desc.m_strPixFilename));

	m_generator->setFile(desc.m_strFileName);
	if (!m_generator->readParams())
	{
		return;
	}

	setParam(m_generator->inputParams());
}

void CaxaRun::saveToFile()
{	
	//QString templateName = QFileDialog::getOpenFileName(this, QStringLiteral("执行成功，如果您希望结果可以转换成cxp格式的文件，您需要选择一个cxp文件的模板"), "./Template", "*.xml");
	QString templateName = "./Template/" + ui.cb_knowledge->currentText()+".xml";
	if (!QFileInfo::exists(templateName))
	{
		QMessageBox::warning(this, QStringLiteral("启动失败"), QStringLiteral("找不到对应的模板来显示 %1 工艺。").arg(templateName));
		return;
	}
	if (!templateName.isEmpty())
	{	
		CxTransToXml trXml;
		string readPath = toGbkString(templateName);
		if (trXml.readTemplate(readPath))
		{
			QString saveName = QFileDialog::getSaveFileName(this, QStringLiteral("请输入保存的文件名"), ".", "*.xml");
			if (saveName.isEmpty())
				return;
			string savePath = toGbkString(saveName);
			if(trXml.saveToFile(m_retObject, m_retParams, savePath))
			{
				QMessageBox::information(this, QStringLiteral("保存成功"), QStringLiteral("保存成工艺文件成功."));
			}
		}
	}
}

bool CaxaRun::save()
{
	//QString templateName = QFileDialog::getOpenFileName(this, QStringLiteral("选择一个cxp文件的模板"), "./Template", "*.xml");
	QString templateName = "./Template/" + ui.cb_knowledge->currentText()+".xml";
	if (!QFileInfo::exists(templateName))
	{
		QMessageBox::warning(this, QStringLiteral("启动失败"), QStringLiteral("找不到对应的模板来显示 %1 工艺。").arg(templateName));
		return false;
	}
	if (!templateName.isEmpty())
	{	
		CxTransToXml trXml;
		string readPath = toGbkString(templateName);
		if (trXml.readTemplate(readPath))
		{
			trXml.saveToFile(m_retObject, m_retParams, m_savePath.toStdString());
			return true;
		}
	}
	return false;
}

void CaxaRun::displayInCapp()
{
	QSettings st("./caxa.ini", QSettings::IniFormat);
	QString CappPath = st.value("CappPath").toString();

	if (save())
	{
		if (!QProcess::startDetached(CappPath , QStringList() << m_savePath))
		{
			QMessageBox::warning(this, QStringLiteral("调用失败"), QStringLiteral("启动工艺图表失败"));
		}
	}
	else
	{
		;
	}
}

void CaxaRun::run()
{
	for (int i = 0;i < m_inputParams.size();i++)
	{
		m_inputParams[i]->m_value = m_model->data(m_model->index(i, 1)).toString().toStdString();
	}
	qDeleteAll(m_retObject);
	m_retObject.clear();
	m_retParams.clear();
	m_generator->writeScript("CTMP.py");
	CxScriptGenerator::CallResult res = m_generator->callScript(m_retObject, m_retParams, "CTMP");
	if (res == CxScriptGenerator::CalculateRight)
	{
		QMessageBox::information(this, QStringLiteral("调用成功"), QStringLiteral("调用脚本成功"));
		if (m_needsResult)
		{
			//m_resultBrowser->clear();
			m_resultBrowser = new QTextBrowser();
			m_resultBrowser->append(QStringLiteral("参数结果 :\n"));

			for (int i = 0;i < m_retParams.size();i++)
			{
				string resultDisplay;
				string pDescription;
				string pOutType = "str";
				for (int j = 0;j < m_generator->outparamList().size();j++)
				{
					if (m_generator->outparamList()[j].m_strName.compare(m_retParams[i].first) == 0)
					{
						pDescription = m_generator->outparamList()[j].m_strDescription;
						pOutType = m_generator->outparamList()[j].m_strType;
						break;
					}
				}

				resultDisplay = m_retParams[i].first + " : " + m_retParams[i].second + "  ,Description :" + pDescription;
				m_resultBrowser->append(QString::fromStdString(resultDisplay));
			}

			m_resultBrowser->append(QStringLiteral("对象输出结果 :\n"));
			foreach (CxExternalObject *obj, m_retObject)
			{
				string retObjectString = obj->objName + "\r\n";
				for (size_t j = 0;j < obj->attri.size();j++)
				{
					retObjectString += "\t" + obj->attri[j].first + " : " + obj->attri[j].second.second + "\r\n";
				}
				m_resultBrowser->append(QString::fromStdString(retObjectString));
			}
			m_resultBrowser->resize(600, 400);
			m_resultBrowser->show();
		}
	}
	else if (res == CxScriptGenerator::ImportError || res == CxScriptGenerator::CalculateError)
	{
		QMessageBox::critical(this, QStringLiteral("调用脚本失败"), QString::fromStdString(m_generator->lastError()) +
			QStringLiteral("\n请检查您设置的输入输出参数、规则等是否正确，再重新执行"));
	}
	else if (res == CxScriptGenerator::CheckparamError)
	{
		QMessageBox::critical(this, QStringLiteral("调用脚本失败"),QStringLiteral("您输入的参数超出了您设定的限定条件，请检查您设置的输入参数，重新输入之后再执行。"));
	}
}

void CaxaRun::setParam(const std::vector<CxInputParam *> &inputParams)
{
	m_inputParams = inputParams;
	m_model->clear();
	ui.paramTable->verticalHeader()->setVisible(false);
	m_model->setHorizontalHeaderLabels(QStringList() << QStringLiteral("名称")
		<< QStringLiteral("值") 
		<< QStringLiteral("说明")
		<< QStringLiteral("最大")
		<< QStringLiteral("最小"));
	for (int i = 0;i < m_inputParams.size();i++)
	{
		m_model->insertRow(i);
		m_model->setData(m_model->index(i, 0), QString::fromStdString(m_inputParams[i]->m_name));
		if (m_inputParams[i]->m_type.compare("str") == 0)
		{
			m_model->setData(m_model->index(i, 1), QString::fromStdString(m_inputParams[i]->m_value));
		}
		else
		{
			m_model->setData(m_model->index(i, 1), QString::fromStdString(m_inputParams[i]->m_value).toFloat());
		}
		m_model->setData(m_model->index(i, 2), QString::fromStdString(m_inputParams[i]->m_description));
		m_model->setData(m_model->index(i, 3), QString::fromStdString(m_inputParams[i]->m_max));
		m_model->setData(m_model->index(i, 4), QString::fromStdString(m_inputParams[i]->m_min));
	}
}
