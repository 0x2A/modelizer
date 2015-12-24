#include "OpenModelDialog.h"
#include <QtWidgets/QDialogButtonBox>
#include "modelizer.h"

OpenModelDialog::OpenModelDialog(QWidget *parent,
	const QString &caption,
	const QString &directory,
	const QString &filter) : QFileDialog(parent, caption, directory, filter)
{
	setOption(QFileDialog::DontUseNativeDialog, true);
}

void OpenModelDialog::AddCheckBoxes()
{
	QGridLayout* mainLayout = dynamic_cast<QGridLayout*>(layout());
	if (!mainLayout)
	{
		modelizer::Log->AppendError("Unable to create openFile dialog!");
		return;
	}
	QHBoxLayout *hbl = new QHBoxLayout(this);

	//add some widgets
	m_Cb = new QCheckBox("My checkbox");
	hbl->addWidget(m_Cb);

	int numRows = mainLayout->rowCount();

	// add the new layout to the bottom of mainLayout
	// and span all columns
	mainLayout->addLayout(hbl, numRows, 0, 1, -1);
}

int OpenModelDialog::exec()
{
	bool r = QFileDialog::exec();
	if (!r) return false;



	return true;
}
