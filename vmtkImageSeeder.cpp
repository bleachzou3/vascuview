#include "vmtkImageSeeder.h"
#include <vtkObjectFactory.h>
vtkStandardNewMacro(vmtkImageSeeder);

vtkPolyData* vmtkImageSeeder::getSeeds()
{
	return Seeds;
}
vmtkImageSeeder::vmtkImageSeeder()
{
	Display = 1;
	ArrayName = "";
	KeepSeeds = 0;
}

vmtkImageSeeder::~vmtkImageSeeder()
{

}

void vmtkImageSeeder::execute()
{
	log4cpp::Category& rootLog  = log4cpp::Category::getRoot();
	log4cpp::Category& subLog = log4cpp::Category::getInstance(std::string("sub1"));
	if((Image == 0) & (Display == 1))
	{
		rootLog.error("vmtkImageSeeder::execute(),Image不能为空");
		subLog.error("vmtkImageSeeder::execute(),Image不能为空");
		return;
	}

    Picker = vtkSmartPointer<vtkCellPicker>::New();
	Picker->SetTolerance(0.005);

	vtkSmartPointer<vtkAddSeedCallBack> vasc = vtkSmartPointer<vtkAddSeedCallBack>::New();
	for(int i = 0; i < 3; i++)
	{
		planes[i]->AddObserver(vtkCommand::StartInteractionEvent,vasc);		
	}

	Seeds = vtkSmartPointer<vtkPolyData>::New();

	InitializeSeeds();
	BuildView();
	WidgetsOff();
	if(!KeepSeeds)
		Renderer->RemoveActor(SeedActor);




	
    

}
void vmtkImageSeeder::setDisplay(int display)
{
	Display = display;
}
void vmtkImageSeeder::setRenderer(vtkRenderer* renderer)
{
	Renderer = renderer;
}
void vmtkImageSeeder::BuildView()
{
	if(ArrayName != "")
		Image->GetPointData()->SetActiveScalars(ArrayName.c_str());
	int* wholeExtent;
	wholeExtent = Image->GetExtent();
	for(int i = 0; i < 3; i++)
	{
		planes[i]->KeyPressActivationOff();
	}

	vtkSmartPointer<vtkGlyph3D> glyphs = vtkSmartPointer<vtkGlyph3D>::New();

	vtkSmartPointer<vtkSphereSource> glyphSource  = vtkSmartPointer<vtkSphereSource>::New();

	glyphs->SetInputData(Seeds);
	glyphs->SetSourceConnection(glyphSource->GetOutputPort());
	glyphs->SetScaleModeToDataScalingOff();
	glyphs->SetScaleFactor(Image->GetLength()*0.01);
	
	vtkSmartPointer<vtkPolyDataMapper> glyphMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	glyphMapper->SetInputConnection(glyphs->GetOutputPort());

	SeedActor = vtkSmartPointer<vtkActor>::New();
	SeedActor->SetMapper(glyphMapper);

	SeedActor->GetProperty()->SetColor(1.0,0.0,0.0);
	Renderer->AddActor(SeedActor);
	WidgetsOn();
	if(Display == 1)
		Renderer->Render();

	
}
void vmtkImageSeeder::InitializeSeeds()
{
	Seeds->Initialize();
	vtkSmartPointer<vtkPoints> seedPoints = vtkSmartPointer<vtkPoints>::New();
	Seeds->SetPoints(seedPoints);

}

void vmtkImageSeeder::WidgetsOff()
{
	for(int i = 0; i < 3;i++)
	{
		planes[i]->Off();
	}
}

void vmtkImageSeeder::WidgetsOn()
{
	for(int i = 0; i < 3;i ++)
	{
		planes[i]->On();
	}
}

void vmtkImageSeeder::setImage(vtkImageData*_image)
{
	Image = _image;
}

