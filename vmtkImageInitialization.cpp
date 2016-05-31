#include "vmtkImageInitialization.h"
#include <vtkObjectFactory.h>
#include "ChooseInitializationType.h"
#include <vtkIdList.h>
#include "ThresholdDialog.h"
#include<qmessagebox.h>
#include <vtkImageThreshold.h>
#include <vtkvmtkCollidingFrontsImageFilter.h>
vtkStandardNewMacro(vmtkImageInitialization);
vmtkImageInitialization::vmtkImageInitialization()
{
	Interactive = 1;
	NegateImage = 0;
}

vmtkImageInitialization::~vmtkImageInitialization()
{

}

void vmtkImageInitialization::setImageData(vtkImageData* localImageData)
{
	Image = localImageData;
}
void vmtkImageInitialization::execute()
{
	log4cpp::Category& rootLog  = log4cpp::Category::getRoot();
	log4cpp::Category& subLog = log4cpp::Category::getInstance(std::string("sub1"));
	if(Image == 0)
	{
	   rootLog.info("vmtkImageInitialization::execute(),Image不能为空，请传入");
	   subLog.info("vmtkImageInitialization::execute(),Image不能为空，请传入");
       return;
	}
	if(Renderer == 0)
	{
	   rootLog.info("vmtkImageInitialization::execute(),Renderer不能为空，请传入");
	   subLog.info("vmtkImageInitialization::execute(),Renderer不能为空，请传入");
	   return;
	}

	
	vtkSmartPointer<vtkImageCast> cast = vtkSmartPointer<vtkImageCast>::New();
	cast->SetInputData(Image);
	cast->SetOutputScalarTypeToFloat();
	cast->Update();
	Image = cast->GetOutput();
	
	if(NegateImage)
	{
		double* scalarRange = Image->GetScalarRange();
		vtkSmartPointer<vtkImageMathematics> negate = vtkSmartPointer<vtkImageMathematics>::New();
		negate->SetInputData(Image);
		negate->SetOperationToMultiplyByK();
		negate->SetConstantK(-1.0);
		negate->Update();
		vtkSmartPointer<vtkImageShiftScale> shiftScale = vtkSmartPointer<vtkImageShiftScale>::New();
		shiftScale->SetInputConnection(negate->GetOutputPort());
		shiftScale->SetShift(scalarRange[1]+scalarRange[0]);
		shiftScale->SetOutputScalarTypeToFloat();
		shiftScale->Update();
		Image = shiftScale->GetOutput();
	}

	if(Interactive)
	{
		bool endInitialization = false;
		while(!endInitialization)
		{
			ChooseInitializationType* dialog = new ChooseInitializationType;
			if(dialog->collidingfrontsRadioButton->isChecked())
			{

			}
			delete dialog;
		}
	}

		 
}
void vmtkImageInitialization::CollidingFrontsInitialize()
{
	log4cpp::Category& rootLog  = log4cpp::Category::getRoot();
	log4cpp::Category& subLog = log4cpp::Category::getInstance(std::string("sub1"));
	rootLog.info("Colliding fronts initialization.");
	subLog.info("Colliding fronts initialization.");

	vtkSmartPointer<vtkIdList> seedIds1 = vtkSmartPointer<vtkIdList>::New();
	vtkSmartPointer<vtkIdList> seedIds2 = vtkSmartPointer<vtkIdList>::New();
	if(Interactive)
	{
		ThresholdDialog* dialog = new ThresholdDialog;
		dialog->show();
		QString upper = dialog->upperThresholdLine->text();
		QString lower = dialog->lowerThresholdLine->text();
		UpperThreshold =  upper.toDouble();
		LowerThreshold = lower.toDouble();
		delete dialog;
		vtkSmartPointer<vtkPolyData> seeds = SeedInput(QString("Please place two seeds"),2);
		seedIds1->InsertNextId(Image->FindPoint(seeds->GetPoint(0)));
		seedIds2->InsertNextId(Image->FindPoint(seeds->GetPoint(1)));

	}else
	{
		seedIds1->InsertNextId(Image->ComputePointId(SourcePoints));
		seedIds2->InsertNextId(Image->ComputePointId(TargetPoints));
	}

	double*scalarRange = Image->GetScalarRange();
	vtkImageData* thresholdImage = Image;

	//我的这里,和原来的py不太一样
	vtkSmartPointer<vtkImageThreshold>  threshold = vtkSmartPointer<vtkImageThreshold>::New();
	threshold->SetInputData(Image);
	threshold->ThresholdBetween(std::min(LowerThreshold,UpperThreshold),std::max(LowerThreshold,UpperThreshold));    	
	threshold->ReplaceInOff();
	threshold->ReplaceOutOn();
	threshold->SetOutValue(scalarRange[0]-scalarRange[1]);
	threshold->Update();

	scalarRange = threshold->GetOutput()->GetScalarRange();
	thresholdImage = threshold->GetOutput();

	double scale = 1.0;
	if(scalarRange[1] - scalarRange[0] > 0.0)
		scale = 1.0 /(scalarRange[0] - scalarRange[1]);
	
	vtkSmartPointer<vtkImageShiftScale> shiftScale = vtkSmartPointer<vtkImageShiftScale>::New();
	shiftScale->SetInputData(thresholdImage);
	shiftScale->SetShift(-scalarRange[0]);
	shiftScale->SetScale(scale);
	shiftScale->SetOutputScalarTypeToFloat();
	shiftScale->Update();

	vtkImageData* speedImage = shiftScale->GetOutput();

	vtkSmartPointer<vtkvmtkCollidingFrontsImageFilter> collidingFronts = vtkSmartPointer<vtkvmtkCollidingFrontsImageFilter>::New();

	collidingFronts->SetInputData(speedImage);
	collidingFronts->SetSeeds1(seedIds1);
	collidingFronts->SetSeeds2(seedIds2);
	collidingFronts->ApplyConnectivityOn();
	collidingFronts->StopOnTargetsOn();
	collidingFronts->Update();

	vtkSmartPointer<vtkImageMathematics> subtract = vtkSmartPointer<vtkImageMathematics>::New();
	subtract->SetInputConnection(collidingFronts->GetOutputPort());
	subtract->SetOperationToAddConstant();
	subtract->SetConstantC(-10.0*collidingFronts->GetNegativeEpsilon());
	subtract->Update();

	vtkImageData* InitialLevelSets = vtkImageData::New();
	InitialLevelSets->DeepCopy(subtract->GetOutput());

	IsoSurfaceValue = 0.0;


}

void vmtkImageInitialization::setRenderer(vtkRenderer*_renderer)
{
	Renderer = _renderer;
}

void vmtkImageInitialization::setImageSeeder(vmtkImageSeeder* _imageSeeder)
{
	ImageSeeder = _imageSeeder;
}

void vmtkImageInitialization::setSurfaceViewer(vmtkSurfaceViewer* _surfaceViewer)
{
	SurfaceViewer = _surfaceViewer;
}

void vmtkImageInitialization::setNegateImage(int _negateImage)
{
	NegateImage = _negateImage;
}

vtkSmartPointer<vtkPolyData> vmtkImageInitialization::SeedInput(const QString & message,int NumberOfSeeds)
{
	int invalid = 1;
	while(invalid)
	{
		invalid = 0;
		QMessageBox messageDialog(QMessageBox::Warning,"Information",message+"(click on the image while pressing Ctrl).\n",QMessageBox::Ok,NULL);
		ImageSeeder->InitializeSeeds();
		Renderer->Render();
		if(NumberOfSeeds > 0 )
		{
			if(ImageSeeder->getSeeds()->GetNumberOfPoints() != NumberOfSeeds)
			{
			  QMessageBox messageDialog(QMessageBox::Warning,"Information","Invalid selection. Please place exactly "+QString(NumberOfSeeds)+"seeds.\n",QMessageBox::Ok,NULL); 
			  invalid = 1;
			  continue;
			}
		
		}
	
	}
	vtkSmartPointer<vtkPolyData> seeds = vtkSmartPointer<vtkPolyData>::New();
	seeds->DeepCopy(ImageSeeder->getSeeds());
	return seeds;

}