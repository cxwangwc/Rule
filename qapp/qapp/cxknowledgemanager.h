#ifndef CXKNOWLEGDEMANAGER_H
#define CXKNOWLEGDEMANAGER_H

#include <QDialog>
#include "cxxmlhelper.h"

class QPushButton;
class QComboBox;
class QLineEdit;

/*
	遍历类内数据成员

class A(object):
	def __init__(self):	
		self.a = 1
		self.b = 2

	def test(self):
		for i,j in vars(self).items():
		print j
 */

class CxKnowlegdeManager : public QDialog
{
	Q_OBJECT

public:
	CxKnowlegdeManager(QWidget *parent = 0);
	CxKnowlegdeManager(const CxKnowlegdeDesc &, QWidget *parent = 0);
	~CxKnowlegdeManager();

	CxKnowlegdeDesc toKnowledge();
	void setKnowledget(const CxKnowlegdeDesc &);

signals:
	void fileSelected(const std::string &);

private slots:
	void searchDirectory();
	void openFile();
	void save();
	void descriptionChanged(const QString &desc);
	void selectPix();
	void generateUID();
	void displayPix();

private:
	void initWindow();

	void reset();
	void fillInfo(const CxKnowlegdeDesc &);

	CxXmlHelper *m_xmlHelper;

	QComboBox *m_cbName;
	QLineEdit *m_descEdit;
	QLineEdit *m_fileversionEdit;
	QLineEdit *m_idEdit;
	QLineEdit *m_nameEdit;
	QLineEdit *m_versionEdit;
	QLineEdit *m_pixfileEdit;

	QPushButton *m_btnGenerateUID;
	QPushButton *m_btnLoadfile;

	std::map<std::string, CxKnowlegdeDesc> m_knowledges;

	bool m_isSave;
};

#endif