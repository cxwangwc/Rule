#include "cxlistwidget.h"

#include <QListWidgetItem>

CxListWidget::CxListWidget(QWidget *parent)
	:QListWidget(parent)
{
	setMouseTracking(true);
	connect(this, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(listItemDoubleClicked(QListWidgetItem *)));
	connect(this, SIGNAL(itemEntered(QListWidgetItem *)), SLOT(listItemEntered(QListWidgetItem *)));
}

CxListWidget::~CxListWidget()
{

}

void CxListWidget::listItemDoubleClicked(QListWidgetItem * item)
{
	if (!item) return;

	emit paramClicked(item->text());
}

void CxListWidget::listItemEntered(QListWidgetItem *item)
{
	if (!item) return;

	emit paramEntered(item->text());
}

void CxListWidget::paramNameChanged(QString previous, QString current)
{
	for (int i = 0;i < count();i++)
	{
		if (item(i)->text() == previous)
		{
			item(i)->setText(current);
			return;
		}
	}
}