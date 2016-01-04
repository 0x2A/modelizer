
#include "modelizer.h"
#include <QtWidgets/QToolButton>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMessageBox>
#include <QMimeData>
#include <QUrl>
#include "OpenModelDialog.h"
#include "Model.h"
#include "AboutWindow.h"

LogWidget * modelizer::Log = nullptr;
Model * modelizer::m_Model = nullptr;
QMenu *helpMenu;
QToolButton *helpMenuButton;

modelizer::modelizer(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	InitToolbar();
	Log = ui.logWidget;

	QObject::connect(this, SIGNAL(BeginLoadModel(const QString, unsigned int)), this, SLOT(onLoadModel(const QString, unsigned int)));
	QObject::connect(this, SIGNAL(BeginSaveModel(const QString, const QString, unsigned int)), this, SLOT(onExportModel(const QString, const QString, unsigned int)));


	//build filter map
	Assimp::Exporter exporter;
	for (int i = 0; i < exporter.GetExportFormatCount(); i++)
	{
		auto descr = exporter.GetExportFormatDescription(i);
		QString s("%1 (*.%2)");
		s = s.arg(descr->description, descr->fileExtension);
		m_ExportTypeMap[s] = descr->fileExtension;
	}
}

modelizer::~modelizer()
{
	delete m_Model;
	m_Model = nullptr;
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

	/*tb = new QToolButton;
	tb->setDefaultAction(ui.actionBatch_convert);
	tb->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	ui.toolBar->addWidget(tb);

	ui.toolBar->addSeparator();*/

	tb = new QToolButton;
	tb->setDefaultAction(ui.actionSettings);
	tb->setToolButtonStyle(Qt::ToolButtonIconOnly);
	ui.toolBar->addWidget(tb);

	ui.toolBar->addSeparator();

	helpMenu = new QMenu(this);
	helpMenu->addAction(ui.actionHelp);
	helpMenu->addSeparator();
	helpMenu->addAction(ui.actionAboutQt);
	helpMenu->addAction(ui.actionAbout);

	QIcon icon7;
	icon7.addFile(QStringLiteral(":/modelizer/Resources/appbar.question.png"), QSize(), QIcon::Normal, QIcon::Off);
	helpMenuButton = new QToolButton(this);
	helpMenuButton->setDefaultAction(ui.actionOpenHelpMenu);
	helpMenuButton->setIcon(icon7);
	helpMenuButton->setMenu(helpMenu);
	helpMenuButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
	
	ui.toolBar->addWidget(helpMenuButton);

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

		if (fileNames.count() <= 0) return;

		QFileInfo info(fileNames.first());
		
		emit BeginLoadModel(fileNames.first(), dialog.GetFlags());
	}
}

void modelizer::onLoadModel(const QString path, unsigned int flags)
{

	Log->AppendMessage("Loading model '" + path + "'...");

	qApp->processEvents();
	delete m_Model;
	m_Model = nullptr;
	m_Model = Model::Load(path, flags);
	update();
}

void modelizer::onExportModel(const QString path, const QString extension, unsigned int flags)
{
	if (!m_Model) return;


	qApp->processEvents();

	//find format id from extension
	QString formatId = "";
	auto exporter = m_Model->GetExporter();
	for (int i = 0; i < exporter->GetExportFormatCount(); i++)
	{
		auto descr = exporter->GetExportFormatDescription(i);
		if (QString(descr->fileExtension).compare(extension, Qt::CaseInsensitive) == 0)
		{
			formatId = descr->id;
			break;
		}
	}

	//export model with given Flags
	m_Model->Export(path, formatId, flags);
	
}
void modelizer::on_actionBlinn_triggered()
{
	ui.actionBlinn->setChecked(true);
	ui.actionUnlit->setChecked(false);
	ui.actionWireframe->setChecked(false);
	ui.openGLWidget->SetRenderMode(RenderMode::blinn);
	update();
}

void modelizer::on_actionUnlit_triggered()
{
	ui.actionBlinn->setChecked(false);
	ui.actionUnlit->setChecked(true);
	ui.actionWireframe->setChecked(false);
	ui.openGLWidget->SetRenderMode(RenderMode::unlit);
	update();
}

void modelizer::on_actionWireframe_triggered()
{
	ui.actionBlinn->setChecked(false);
	ui.actionUnlit->setChecked(false);
	ui.actionWireframe->setChecked(true);
	ui.openGLWidget->SetRenderMode(RenderMode::wireframe);
	update();
}

void modelizer::on_actionExport_triggered()
{
	if (!m_Model) return;

	//build filter list from extension map
	QStringList filters;
	QMapIterator<QString, QString> i(m_ExportTypeMap);
	while (i.hasNext())
	{
		i.next();
		filters.push_back(i.key());
	}

	OpenModelDialog dialog(this);
	dialog.setFileMode(QFileDialog::AnyFile);
	dialog.setAcceptMode(QFileDialog::AcceptSave);
	dialog.setNameFilters(filters);
	dialog.setViewMode(QFileDialog::Detail);
	dialog.setOption(QFileDialog::DontUseNativeDialog, true);
	dialog.AddCheckBoxes();

	QStringList fileNames;
	if (dialog.exec())
	{
		fileNames = dialog.selectedFiles();
		if (fileNames.count() <= 0) return;

		QString fileName = fileNames.first();

		//find extension from filter name
		QString extension = m_ExportTypeMap[dialog.selectedNameFilter()];

		//Check if filename has extension, if not append extension
		QFileInfo info(fileName);
		if (info.suffix().isEmpty())
			fileName += "." + extension;

		//inform user about exporting file
		modelizer::Log->AppendMessage("Exporting model to '" + fileName + "'");

		emit BeginSaveModel(fileName, extension, dialog.GetFlags());
	}
}

void modelizer::on_actionOpenHelpMenu_triggered()
{
	helpMenuButton->showMenu();
}

void modelizer::on_actionAboutQt_triggered()
{
	QMessageBox::aboutQt(this);
}

void modelizer::on_actionAbout_triggered()
{
	AboutWindow *about = new AboutWindow(this);
	about->show();
}

void modelizer::dropEvent(QDropEvent *event)
{
	auto action = event->possibleActions();
	auto mdata = event->mimeData();
	if ((action & Qt::CopyAction || action & Qt::MoveAction))
		event->acceptProposedAction();

	QUrl url = event->mimeData()->text();
	QString path = url.toLocalFile();
	emit BeginLoadModel(path, 6333963); //default flags as integer since its shorter to write. Maybe create an extra window for this later or an option to define this
}

void modelizer::dragEnterEvent(QDragEnterEvent *event)
{
	//TODO: check for supported formats
	event->acceptProposedAction();
}

