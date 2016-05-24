#include "vmtkImageInitialization.h"

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

	}

		 
}