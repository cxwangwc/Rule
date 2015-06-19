#include "cxmainwindow.h"
#include "cxobjectinit.h"

#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QFile f(":/skin/skin.qss");
	if (f.open(QIODevice::ReadOnly))
	{
		a.setStyleSheet(f.readAll());
		f.close();
	}
	CxObjectInitProperty::initCXObject();

	CxMainWindow w;
	w.show();

	return a.exec();
}
