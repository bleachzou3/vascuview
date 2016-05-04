#include "vmtkImageFeature.h"

vmtkImageFeature::vmtkImageFeature()
{
	 Dimensionality = 3;
     DerivativeSigma = 0.0;
     SigmoidRemapping = 0;
     UpwindFactor = 1.0;	 
	 FWHMRadius.push_back(3);
	 FWHMRadius.push_back(3);
	 FWHMRadius.push_back(3);
	 FWHMBackgroundValue = 0.0;
}

vmtkImageFeature::~vmtkImageFeature()
{


}

void vmtkImageFeature::BuildVTKGradientBasedFeatureImage(vtkImageData*originalData,vtkImageData*res)
{
	vtkSmartPointer<vtkImageCast> cast = vtkSmartPointer<vtkImageCast>::New();
	cast->SetInputData(originalData);
	cast->SetOutputScalarTypeToFloat();       
    cast->Update();

        
	vtkSmartPointer<vtkImageGradientMagnitude> gradientMagnitude = vtkSmartPointer<vtkImageGradientMagnitude>::New();
	gradientMagnitude->SetInputConnection(cast->GetOutputPort());
	gradientMagnitude->SetDimensionality(Dimensionality);
	gradientMagnitude->Update();

		

     
	vtkSmartPointer<vtkImageMathematics> imageAdd = vtkSmartPointer<vtkImageMathematics>::New();
    imageAdd->SetInputConnection(gradientMagnitude->GetOutputPort());
    imageAdd->SetOperationToAddConstant();
    imageAdd->SetConstantC(1.0);
    imageAdd->Update();

        
	vtkSmartPointer<vtkImageMathematics> imageInvert = vtkSmartPointer<vtkImageMathematics>::New();
    imageInvert->SetInputConnection(imageAdd->GetOutputPort());
    imageInvert->SetOperationToInvert();
    imageInvert->SetConstantC(1E20);
    imageInvert->DivideByZeroToCOn();
    imageInvert->Update();
	res->DeepCopy(imageInvert->GetOutput());

}