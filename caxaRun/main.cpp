#include "caxarun.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	CaxaRun w(true, 0);
	w.show();
	return a.exec();
}
