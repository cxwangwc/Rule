#ifndef CXDIALOG_H
#define CXDIALOG_H

#include "cxtree.h"

class CxHighlighter;
class CxTextEdit;

class VariableEdit : public QLineEdit
{
	Q_OBJECT

public:
	/*^[_a-zA-Z][_a-zA-Z0-9]*$*/
	VariableEdit(const QString &te = "", QWidget *parent = 0);
	~VariableEdit();
};

class CxBaseDialog : public QDialog
{
public:
	CxBaseDialog(QWidget *parent = 0);
	virtual ~CxBaseDialog();

	void setDefinedObjects(const std::vector<std::string> &);
	bool checkIfNameUsed(const QString &name);

	virtual bool inputValid();

protected:
	virtual void initLayout();
	QHBoxLayout *m_btnLayout;
	std::vector<std::string> m_definedObjects;
};

class CxConfigObjectDialog : public CxBaseDialog
{
	Q_OBJECT

public:
	CxConfigObjectDialog(CxExternalObject *obj, QWidget *parent = 0);
	~CxConfigObjectDialog();

protected:
	void initLayout();
	bool inputValid();

	private slots:
		void accept();

private:
	std::vector<QLineEdit *> m_edits;
	CxExternalObject *m_configObj;
}; 

class CxObjectDialog : public CxBaseDialog
{
	Q_OBJECT

public:
	CxObjectDialog(std::vector<std::string> &classList, QWidget *parent = 0);
	~CxObjectDialog();

	std::string selectClass();
	CxExternalObject *createdObj();

protected:
	void initLayout();

	private slots:
		void accept();

private:
	QComboBox *m_cls;
	CxExternalObject *m_createdObject;
	std::vector<std::string> m_clsList;
};

class CxParamDialog : public CxBaseDialog
{
	Q_OBJECT

public:
	CxParamDialog(QWidget *parent = 0, CxInputParam *param = 0);
	CxInputParam *param();

protected:
	void initLayout();
	bool inputValid();

	private slots:
		void accept();
		void typeChanged(int );

private:
	void initParam(CxInputParam *param);


	QLineEdit *m_nameEdit;
	QLineEdit *m_descEdit;
	QLineEdit *m_maxEdit;
	QLineEdit *m_minEdit;
	QLineEdit *m_valueEdit;

	std::vector<QLineEdit *> m_edits;
	QComboBox *m_allowNull;
	QComboBox *m_typeBox;
	QStringList m_keys;

	CxInputParam *m_definedParam;
};

class CxRuleDialog : public CxBaseDialog
{
	Q_OBJECT

public:
	CxRuleDialog(QWidget *parent = 0, const std::vector<std::string> &paramList = std::vector<std::string>()
		, const QStringList &ruleValue = QStringList());
	std::vector<std::string> rule();

protected:
	void initLayout();
	bool inputValid();

	private slots:
		void accept();

private:
	std::vector<QLineEdit *> m_edits;
	std::vector<std::string> m_paramList;
	QStringList m_ruleValues;
	QStringList m_keys;
	CxTextEdit *m_stateEdit;
};

class CxModuleDialog : public CxBaseDialog
{
	Q_OBJECT

public:
	CxModuleDialog(QWidget *parent = 0);
	std::vector<std::string> rule();

protected:
	void initLayout();
	bool inputValid();

	private slots:
		void accept();

private:
	std::vector<QLineEdit *> m_edits;
	QStringList m_keys;
}; 

class CxAttributeDialog : public CxBaseDialog
{
	Q_OBJECT

public:
	CxAttributeDialog(QString textDesc, const CxOutputParam &outParam,
		QWidget *parent = 0, bool isAttribute = false);
	CxAttributeDialog(QString textDesc,
		QWidget *parent = 0, bool isAttribute = false);

	CxOutputParam param();
	void setOutParam(const CxOutputParam &);

protected:
	void initLayout();
	bool inputValid();

	private slots:
		void accept();

private:
	QLineEdit *m_nameEdit;
	QLineEdit *m_descriptionEdit;
	QComboBox *m_typeBox;

	QString m_textDesc;
	bool m_isAttribute;
}; 

class CxSetparamDialog : public CxBaseDialog
{
	Q_OBJECT

public:
	CxSetparamDialog(std::vector<CxInputParam *> &, QWidget *parent = 0);
	~CxSetparamDialog();

	private slots:
		void accept();

private:
	void initLayout();
	bool inputValid();

	std::vector<CxInputParam *> & m_inputParams;
	std::vector<QLabel *> m_paramLabel;
	std::vector<QLineEdit *> m_paramEdit;
};

#endif