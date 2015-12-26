#pragma once


#include <QFileDialog>
#include <QtWidgets/QCheckBox>

class OpenModelDialog : public QFileDialog
{
	Q_OBJECT

public:
	OpenModelDialog(QWidget *parent = 0,
		const QString &caption = QString(),
		const QString &directory = QString(),
		const QString &filter = QString());

	void AddCheckBoxes();
	int exec();

	unsigned int GetFlags() { return flags; }

private:

	unsigned int flags;
	QCheckBox* m_CalcTangentSpace;
	QCheckBox* m_JoinIdenticalVertices;
	QCheckBox* m_MakeLeftHanded;
	QCheckBox* m_Triangulate;
	QCheckBox* m_GenNormals;
	QCheckBox* m_SplitLargeMeshes;
	QCheckBox* m_PreTransformVertices;
	QCheckBox* m_LimitBoneWeights;
	QCheckBox* m_ValidateDataStructure;
	QCheckBox* m_RemoveRedundantMaterials;
	QCheckBox* m_FixInfacingNormals;
	QCheckBox* m_SortByPType;
	QCheckBox* m_GenUVCoords;
	QCheckBox* m_TransformUVCoords;
	QCheckBox* m_OptimizeMeshes;
	QCheckBox* m_OptimizeGraph;
	QCheckBox* m_FlipUVs;
	QCheckBox* m_FlipWindingOrder;
	QCheckBox* m_Debone;
};