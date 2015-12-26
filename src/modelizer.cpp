
#include "modelizer.h"
#include <QtWidgets/QToolButton>
#include "OpenModelDialog.h"

LogWidget * modelizer::Log = nullptr;


modelizer::modelizer(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	InitToolbar();
	Log = ui.logWidget;


}

modelizer::~modelizer()
{

}

void modelizer::InitToolbar()
{
	QToolButton *tb = new QToolButton;
	tb->setDefaultAction(ui.actionImport);
	tb->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	ui.toolBar->addWidget(tb);

	tb = new QToolButton;
	tb->setDefaultAction(ui.actionExport);
	tb->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	ui.toolBar->addWidget(tb);

	ui.toolBar->addSeparator();

	tb = new QToolButton;
	tb->setDefaultAction(ui.actionBatch_convert);
	tb->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	ui.toolBar->addWidget(tb);

	ui.toolBar->addSeparator();

	tb = new QToolButton;
	tb->setDefaultAction(ui.actionSettings);
	tb->setToolButtonStyle(Qt::ToolButtonIconOnly);
	ui.toolBar->addWidget(tb);
}

void modelizer::on_actionImport_triggered()
{
	OpenModelDialog dialog(this);
	dialog.setFileMode(QFileDialog::ExistingFiles);

	QStringList filters;
	filters << "Model files (*.3ds *.blend *.dae *.collada *.fbx *.ifc *.ase *.dxf *.hmp *.md2 *.md3 *.md5 *.pk3 *.mdc *.mdl *.raw *.nff *.off *.ply *.stl *.x *.obj *.ogex *.smd *.vta *.lwo *.lxo *.lws *.ter *.3d *.ac *.ms3d *.cob *.scn *.q3bsp *.xgl *.zgl *.csm *.bvh *.b3d *.ndo *.mesh *.xml *.q3d *.q3s *.assbin *.gltf *.glb *.irrmesh)"
		<< "3ds Max 3DS(*.3ds)"
		<< "3ds Max ASE (*.ase)"
		<< "Blender 3D (*.blend)"
		<< "Collada (*.dae *.collada)"
		<< "Autodesk (*.fbx)"
		<< "Industry Foundation Classes (IFC/Step) (*.ifc)"
		<< "Wavefront Object (*.obj)"
		<< "glTF (*.gltf *.glb)"
		<< "XGL (*.xgl *.zgl)"
		<< "Stanford Polygon Library (*.ply)"
		<< "AutoCAD DXF (*.dxf)"
		<< "LightWave (*.lwo)"
		<< "LightWave Scene (*.lws)"
		<< "Modo (*.lxo)"
		<< "Stereolithography (*.stl)"
		<< "DirectX X (*.x)"
		<< "AC3D (*.ac)"
		<< "Milkshape 3D (*.ms3d)"
		<< "TrueSpace (*.cob *scn)"
		
		<< "Biovision BVH (*.bvh)"
		<< "CharacterStudio Motion (*.csm)"
		
		<< "Ogre XML (*.xml)"
		<< "Irrlicht Mesh (*.irrmesh)"
		<< "Irrlicht Scene (*.irr)"
		
		<< "Quake I (*.mdl)"
		<< "Quake II (*.md2)"
		<< "Quake III Mesh (*.md3)"
		<< "Quake III Map/BSP (*.pk3)"
		<< "Return to Castle Wolfenstein (*.pk3)"
		<< "Doom 3 (*.md5)"
		<< "Valve Model (*.smd *.vta)"
		<< "Open Game Engine Exchange (*.ogex)"
		<< "Unreal (*.3d)"
		
		<< "BlitzBasic 3D (*.b3d)"
		<< "Quick3D (*.q3d, *.q3s)"
		<< "Neutral File Format (*.nff)"
		<< "Sense8 WorldToolKit (*.nff)"
		<< "Object File Format (*.off)"
		<< "PovRAY Raw (*.raw)"
		<< "Terragen Terrain (*.ter)"
		<< "3D GameStudio (3DGS) (*.mdl)"
		<< "3D GameStudio (3DGS) Terrain (*.hmp)"
		<< "Izware Nendo (*.ndo)";

	dialog.setNameFilters(filters);
	dialog.setViewMode(QFileDialog::Detail);
	dialog.setOption(QFileDialog::DontUseNativeDialog, true);
	dialog.AddCheckBoxes();
	QStringList fileNames;
	if (dialog.exec())
	{
		fileNames = dialog.selectedFiles();

		QFileInfo info(fileNames.first());
		Log->AppendMessage("Loading model '" + info.fileName() + "'...");
	}
}
