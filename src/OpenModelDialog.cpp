#include "OpenModelDialog.h"
#include <QtWidgets/QDialogButtonBox>
#include "modelizer.h"
#include <assimp/postprocess.h>

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
	QGridLayout *hbl = new QGridLayout(this);

	//add some widgets
	m_CalcTangentSpace = new QCheckBox("Calculate tangents");
	m_CalcTangentSpace->setCheckState(Qt::Checked);
	hbl->addWidget(m_CalcTangentSpace, 0, 0);
	m_JoinIdenticalVertices = new QCheckBox("Join identical vertices");
	m_JoinIdenticalVertices->setCheckState(Qt::Checked);
	hbl->addWidget(m_JoinIdenticalVertices, 0, 1);
	m_MakeLeftHanded = new QCheckBox("Make left handed");
	hbl->addWidget(m_MakeLeftHanded, 0, 2);
	m_Triangulate = new QCheckBox("Triangulate");
	m_Triangulate->setCheckState(Qt::Checked);
	hbl->addWidget(m_Triangulate, 0, 3);
	m_GenNormals = new QCheckBox("Generates normals");
	hbl->addWidget(m_GenNormals, 1, 0);
	m_SplitLargeMeshes = new QCheckBox("Split large meshes");
	hbl->addWidget(m_SplitLargeMeshes, 1, 1);
	m_PreTransformVertices = new QCheckBox("PreTransform Vertices");
	hbl->addWidget(m_PreTransformVertices, 1, 2);
	m_LimitBoneWeights = new QCheckBox("Limit bone weights");
	m_LimitBoneWeights->setCheckState(Qt::Checked);
	hbl->addWidget(m_LimitBoneWeights, 1, 3);
	m_ValidateDataStructure = new QCheckBox("Validate data structure");
	m_ValidateDataStructure->setCheckState(Qt::Checked);
	hbl->addWidget(m_ValidateDataStructure, 2, 0);
	m_RemoveRedundantMaterials = new QCheckBox("Remove redundant materials");
	hbl->addWidget(m_RemoveRedundantMaterials, 2, 1);
	m_FixInfacingNormals = new QCheckBox("Fix infacing normals");
	m_FixInfacingNormals->setCheckState(Qt::Checked);
	hbl->addWidget(m_FixInfacingNormals, 2, 2);
	m_SortByPType = new QCheckBox("Sort by primitive type");
	m_SortByPType->setCheckState(Qt::Checked);
	hbl->addWidget(m_SortByPType, 2, 3);
	m_GenUVCoords = new QCheckBox("Generate UV coordinates");
	hbl->addWidget(m_GenUVCoords, 3, 0);
	m_TransformUVCoords = new QCheckBox("Transform UV coordinates");
	hbl->addWidget(m_TransformUVCoords, 3, 1);
	m_OptimizeMeshes = new QCheckBox("Optimize meshes");
	m_OptimizeMeshes->setCheckState(Qt::Checked);
	hbl->addWidget(m_OptimizeMeshes, 3, 2);
	m_OptimizeGraph = new QCheckBox("Optimize graph");
	m_OptimizeGraph->setCheckState(Qt::Checked);
	hbl->addWidget(m_OptimizeMeshes, 3, 3);
	m_FlipUVs = new QCheckBox("Flip UV coordinates");
	hbl->addWidget(m_FlipUVs, 4, 0);
	m_FlipWindingOrder = new QCheckBox("Flip vertex order");
	hbl->addWidget(m_FlipWindingOrder, 4, 1);
	m_Debone = new QCheckBox("Debone");
	hbl->addWidget(m_Debone, 4, 2);

	int numRows = mainLayout->rowCount();

	// add the new layout to the bottom of mainLayout
	// and span all columns
	mainLayout->addLayout(hbl, numRows, 0, 1, -1);
}

int OpenModelDialog::exec()
{
	bool r = QFileDialog::exec();
	if (!r) return false;

	flags = 0;
	if (m_CalcTangentSpace->checkState() == Qt::Checked)
		flags |= aiProcess_CalcTangentSpace;
	if (m_JoinIdenticalVertices->checkState() == Qt::Checked)
		flags |= aiProcess_JoinIdenticalVertices;
	if (m_MakeLeftHanded->checkState() == Qt::Checked)
		flags |= aiProcess_MakeLeftHanded;
	if (m_Triangulate->checkState() == Qt::Checked)
		flags |= aiProcess_Triangulate;
	if (m_GenNormals->checkState() == Qt::Checked)
		flags |= aiProcess_GenNormals;
	if (m_SplitLargeMeshes->checkState() == Qt::Checked)
		flags |= aiProcess_SplitLargeMeshes;
	if (m_PreTransformVertices->checkState() == Qt::Checked)
		flags |= aiProcess_PreTransformVertices;
	if (m_LimitBoneWeights->checkState() == Qt::Checked)
		flags |= aiProcess_LimitBoneWeights;
	if (m_ValidateDataStructure->checkState() == Qt::Checked)
		flags |= aiProcess_ValidateDataStructure;
	if (m_RemoveRedundantMaterials->checkState() == Qt::Checked)
		flags |= aiProcess_RemoveRedundantMaterials;
	if (m_FixInfacingNormals->checkState() == Qt::Checked)
		flags |= aiProcess_FixInfacingNormals;
	if (m_SortByPType->checkState() == Qt::Checked)
		flags |= aiProcess_SortByPType;
	if (m_GenUVCoords->checkState() == Qt::Checked)
		flags |= aiProcess_GenUVCoords;
	if (m_TransformUVCoords->checkState() == Qt::Checked)
		flags |= aiProcess_TransformUVCoords;
	if (m_OptimizeMeshes->checkState() == Qt::Checked)
		flags |= aiProcess_OptimizeMeshes;
	if (m_OptimizeGraph->checkState() == Qt::Checked)
		flags |= aiProcess_OptimizeGraph;
	if (m_FlipUVs->checkState() == Qt::Checked)
		flags |= aiProcess_FlipUVs;
	if (m_FlipWindingOrder->checkState() == Qt::Checked)
		flags |= aiProcess_FlipWindingOrder;
	if (m_Debone->checkState() == Qt::Checked)
		flags |= aiProcess_Debone;

	return true;
}
