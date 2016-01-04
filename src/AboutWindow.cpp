#include "AboutWindow.h"
#include "modelizer.h"

AboutWindow::AboutWindow(QWidget *parent /*= 0*/) : QDialog(parent)
{
	ui.setupUi(this);
	setFixedSize(width(), height());

	//remove icon and question mark button
	setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);

	//set version string
	ui.lblVersion->setText("Version " + QString(MODELIZER_VERSION_STR));
}

AboutWindow::~AboutWindow()
{

}
