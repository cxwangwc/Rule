#ifndef CXLISTWIDGET_H
#define CXLISTWIDGET_H

#include <QListWidget>

class CxListWidget : public QListWidget
{
	Q_OBJECT

public:
	CxListWidget(QWidget *parent = 0);
	~CxListWidget();

	public slots:
		void paramNameChanged(QString previous, QString current);

signals:
	void paramClicked(const QString &paramName);
	void paramEntered(const QString &paramName);

	private slots:
		void listItemDoubleClicked(QListWidgetItem *item);
		void listItemEntered(QListWidgetItem *item);

};

#endif