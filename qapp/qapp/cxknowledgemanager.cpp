#include "cxknowledgemanager.h"

#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QFileDialog>
#include <QUuid>
#include <QFile>
#include <QMessageBox>

CxKnowlegdeManager::CxKnowlegdeManager(QWidget *parent)
	:QDialog(parent)
	, m_xmlHelper(new CxXmlHelper(""))
	, m_isSave(false)
{
	initWindow();
}

CxKnowlegdeManager::CxKnowlegdeManager(const CxKnowlegdeDesc &knowledge, QWidget *parent)
	: QDialog(parent)
	, m_xmlHelper(new CxXmlHelper(""))
	, m_isSave(true)
{
	initWindow();
	setKnowledget(knowledge);
}

CxKnowlegdeDesc CxKnowlegdeManager::toKnowledge()
{
	CxKnowlegdeDesc desc;
	desc.m_strName = m_cbName->currentText().toStdString();
	desc.m_strDescription = m_descEdit->text().toStdString();
	desc.m_strFileVersion = m_fileversionEdit->text().toStdString();
	desc.m_strID = m_idEdit->text().toStdString();
	desc.m_strVersion = m_versionEdit->text().toStdString();
	desc.m_strPixFilename = m_pixfileEdit->text().toStdString();
	desc.m_strFileName = m_nameEdit->text().toStdString();
	return desc;
}

void CxKnowlegdeManager::setKnowledget(const CxKnowlegdeDesc &desc)
{
	m_nameEdit->setText(QString::fromStdString(desc.m_strFileName));
	m_descEdit->setText(QString::fromStdString(desc.m_strDescription));
	m_fileversionEdit->setText(QString::fromStdString(desc.m_strFileVersion));
	m_idEdit->setText(QString::fromStdString(desc.m_strID));
	m_versionEdit->setText(QString::fromStdString(desc.m_strVersion));
	m_pixfileEdit->setText(QString::fromStdString(desc.m_strPixFilename));
	m_cbName->clear();
	m_cbName->addItem(QString::fromStdString(desc.m_strName));
}

CxKnowlegdeManager::~CxKnowlegdeManager()
{

}

void CxKnowlegdeManager::initWindow()
{
	QVBoxLayout *mainLayout = new QVBoxLayout(this);
	QGridLayout *conLayout = new QGridLayout();
	m_cbName = new QComboBox();
	m_fileversionEdit = new QLineEdit();
	m_descEdit = new QLineEdit();
	m_idEdit = new QLineEdit();
	m_idEdit->setReadOnly(true);
	m_nameEdit = new QLineEdit();
	m_versionEdit = new QLineEdit();
	m_pixfileEdit = new QLineEdit();
	QLabel *filePath = new QLabel(QStringLiteral("文件路径:"));
	m_btnGenerateUID = new QPushButton(QStringLiteral("生成新UID((&G)"));
	m_btnLoadfile = new QPushButton(QStringLiteral("选择图片(&S)"));
	QHBoxLayout *btnLayout = new QHBoxLayout();
	conLayout->addWidget(new QLabel(QStringLiteral("名称:")), 0, 0, 1, 1);
	conLayout->addWidget(m_cbName, 0, 1, 1, 1);
	conLayout->addWidget(new QLabel(QStringLiteral("知识描述:")), 1, 0, 1, 1);
	conLayout->addWidget(m_descEdit, 1, 1, 1, 1);
	conLayout->addWidget(new QLabel(QStringLiteral("文件版本:")), 2, 0, 1, 1);
	conLayout->addWidget(m_fileversionEdit, 2, 1, 1, 1);
	conLayout->addWidget(new QLabel(QStringLiteral("ID标识:")), 3, 0, 1, 1);
	conLayout->addWidget(m_idEdit, 3, 1, 1, 1);
	conLayout->addWidget(m_btnGenerateUID, 3, 2, 1, 1);
	conLayout->addWidget(new QLabel(QStringLiteral("版本号:")), 4, 0, 1, 1);
	conLayout->addWidget(m_versionEdit, 4, 1, 1, 1);
	conLayout->addWidget(new QLabel(QStringLiteral("附图:")), 5, 0, 1, 1);
	conLayout->addWidget(m_pixfileEdit, 5, 1, 1, 1);
	conLayout->addWidget(m_btnLoadfile, 5, 2, 1, 1);
	conLayout->addWidget(filePath, 6, 0, 1, 1);
	conLayout->addWidget(m_nameEdit, 6, 1, 1, 1);

	QPushButton *dir = new QPushButton(QStringLiteral("搜索目录(&F)"));
	QPushButton *open = new QPushButton(QStringLiteral("打开文件(O)"));
	QPushButton *sa = new QPushButton(QStringLiteral("保存(S):"));
	QPushButton *can = new QPushButton(QStringLiteral("取消(&C)"));
	QPushButton *showPix = new QPushButton(QStringLiteral("显示图片(&P)"));
	btnLayout->addStretch();
	btnLayout->addWidget(dir);
	btnLayout->addWidget(open);
	btnLayout->addWidget(showPix);
	btnLayout->addWidget(sa);
	btnLayout->addWidget(can);

	mainLayout->addLayout(conLayout);
	mainLayout->addLayout(btnLayout);

	connect(sa, SIGNAL(clicked()), SLOT(save()));
	connect(dir, SIGNAL(clicked()), SLOT(searchDirectory()));
	connect(open, SIGNAL(clicked()), SLOT(openFile()));
	connect(can, SIGNAL(clicked()), SLOT(reject()));
	connect(showPix, SIGNAL(clicked()), SLOT(displayPix()));
	connect(m_btnLoadfile, SIGNAL(clicked()), SLOT(selectPix()));
	connect(m_btnGenerateUID, SIGNAL(clicked()), SLOT(generateUID()));
	connect(m_cbName, SIGNAL(currentTextChanged(const QString &)), SLOT(descriptionChanged(const QString)));

	m_btnGenerateUID->setEnabled(m_isSave);
	m_btnLoadfile->setEnabled(m_isSave);
	m_cbName->setEditable(m_isSave);
	filePath->setVisible(!m_isSave);
	m_nameEdit->setVisible(!m_isSave);
	dir->setVisible(!m_isSave);
	open->setVisible(!m_isSave);
	sa->setVisible(m_isSave);
	m_nameEdit->setReadOnly(true);

	setWindowTitle(QStringLiteral("知识管理"));
	resize(400, 300);
}

void CxKnowlegdeManager::displayPix()
{
	QString pixname = m_pixfileEdit->text();
	if (pixname.isEmpty() || !QFile::exists(pixname))
	{
		QMessageBox::warning(this, QStringLiteral("打开失败"),
			QStringLiteral("打开图片失败，图片为空或图片不存在"));
		return;
	}

	QLabel *pixLa = new QLabel();
	pixLa->setWindowTitle(QStringLiteral("工艺附图显示"));
	pixLa->setPixmap(QPixmap(pixname));
	pixLa->show();
}

void CxKnowlegdeManager::searchDirectory()
{
	QString dirName = QFileDialog::getExistingDirectory(this, "", "select a dir");
	if (!dirName.isEmpty())
	{
		if (m_xmlHelper->read(dirName.toStdString()))
		{
			m_knowledges = m_xmlHelper->knowlegdes();
			reset();
		}
	}
}

void CxKnowlegdeManager::openFile()
{
	if (!m_cbName->currentText().isEmpty())
	{
		QString name = m_cbName->currentText();
		std::map<std::string, CxKnowlegdeDesc>::iterator it = m_knowledges.begin();
		while (it != m_knowledges.end())
		{
			if (QString::fromStdString(it->second.m_strName) == name)
			{
				emit fileSelected(it->second.m_strFileName);
				return;
			}

			it++;
		}
	}
}

void CxKnowlegdeManager::save()
{
	QDialog::accept();
}

void CxKnowlegdeManager::descriptionChanged(const QString &desc)
{
	std::map<std::string, CxKnowlegdeDesc>::iterator it = m_knowledges.begin();
	while (it != m_knowledges.end())
	{
		if (desc == QString::fromStdString(it->second.m_strName))
		{
			fillInfo(it->second);
			break;
		}
		it++;
	}
}

void CxKnowlegdeManager::reset()
{
	m_cbName->clear();
	std::map<std::string, CxKnowlegdeDesc>::iterator it = m_knowledges.begin();
	while (it != m_knowledges.end())
	{
		CxKnowlegdeDesc desc = it->second;
		m_cbName->addItem(QString::fromStdString(it->second.m_strName));
		fillInfo(desc);
		it++;
	}
}

void CxKnowlegdeManager::fillInfo(const CxKnowlegdeDesc &kn)
{
	m_descEdit->setText(QString::fromStdString(kn.m_strDescription));
	m_fileversionEdit->setText(QString::fromStdString(kn.m_strFileVersion));
	m_idEdit->setText(QString::fromStdString(kn.m_strID));
	m_nameEdit->setText(QString::fromStdString(kn.m_strFileName));
	m_versionEdit->setText(QString::fromStdString(kn.m_strVersion));
	m_pixfileEdit->setText(QString::fromStdString(kn.m_strPixFilename));
}

void CxKnowlegdeManager::selectPix()
{
	QString pixName = QFileDialog::getOpenFileName(this, QStringLiteral("选择图片"), ".", "图片文件 (*.png *.jpg *.bmp)");
	if (!pixName.isEmpty())
	{
		m_pixfileEdit->setText(pixName);
	}
}

void CxKnowlegdeManager::generateUID()
{
	m_idEdit->setText(QUuid::createUuid().toString());
}