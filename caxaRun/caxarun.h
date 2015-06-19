#ifndef CAXARUN_H
#define CAXARUN_H

#include "cxscriptgenerator.h"
#include "ui_cxbase.h"

class QTextBrowser;
class QStandardItemModel;
	
class CaxaRun : public QDialog
{
	Q_OBJECT

public:
	CaxaRun(bool needsResult = true, QWidget *parent = 0);
	~CaxaRun();

	void setParam(const std::vector<CxInputParam *> &);

	private slots:
		void open();
		void run();
		void openFromFile();
		void saveToFile();
		void displayInCapp();
		void currentIndexChanged(int );

	public slots:
		void doubleClicked(const QModelIndex &index);

private:
	void setKnowlegde(const CxKnowlegdeDesc &);
	bool save();

	QString m_savePath;

	QTextBrowser *m_resultBrowser;
	bool m_needsResult;
	QStandardItemModel *m_model;

	CxScriptGenerator *m_generator;

	std::map<std::string, CxKnowlegdeDesc> m_knowledges;
	std::vector<CxInputParam *> m_inputParams;

	Ui::CxBase ui;

	vector<CxExternalObject *> m_retObject;
	vector<pair<string, string> > m_retParams;
};

#endif // CAXARUN_H
