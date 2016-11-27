#include "gscenetest.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	GSceneTest w;
	w.show();
	return a.exec();
}
