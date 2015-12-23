#include "modelizer.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	modelizer w;
	w.show();
	return a.exec();
}
