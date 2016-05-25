#include "vmtkImageSeeder.h"

vmtkImageSeeder::vmtkImageSeeder()
{
	Display = 1;
	ArrayName = "";
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



	
    

}
void vmtkImageSeeder::BuildView()
{
	if(ArrayName != "")
		Image->GetPointData()->SetActiveScalars(ArrayName.c_str());
	int* wholeExtent;
	wholeExtent = Image->GetExtent();


	;
}
void vmtkImageSeeder::InitializeSeeds()
{
	Seeds->Initialize();
	vtkSmartPointer<vtkPoints> seedPoints = vtkSmartPointer<vtkPoints>::New();
	Seeds->SetPoints(seedPoints);

}

