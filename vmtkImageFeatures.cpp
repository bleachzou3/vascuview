#include "vmtkImageFeature.h"
#include "vtkvmtkFWHMFeatureImageFilter.h"
#include "vtkvmtkUpwindGradientMagnitudeImageFilter.h"
#include "vtkPointData.h"
#include "vtkDataArray.h"
#include "vtkvmtkSigmoidImageFilter.h"
#include "vtkvmtkBoundedReciprocalImageFilter.h"
#include "NullPointerException.h"
#include "vtkvmtkGradientMagnitudeRecursiveGaussianImageFilter.h"
#include "vtkvmtkGradientMagnitudeImageFilter.h"
#include <vtkObjectFactory.h>
#include <log4cpp/Category.hh>
#include <log4cpp/PropertyConfigurator.hh>
vtkStandardNewMacro(vmtkImageFeature);
vmtkImageFeature::vmtkImageFeature()
{
	 Dimensionality = 3;
     DerivativeSigma = 0.0;
     SigmoidRemapping = 0;
     UpwindFactor = 1.0;	 
	 FWHMRadius[0]=3;
	 FWHMRadius[1]=3;
	 FWHMRadius[2]=3;
	 FWHMBackgroundValue = 0.0;
}

vmtkImageFeature::~vmtkImageFeature()
{


}

void vmtkImageFeature::BuildVTKGradientBasedFeatureImage(vtkImageData*originalData,vtkImageData*res)
{
     log4cpp::Category& rootLog  = log4cpp::Category::getRoot();
	 log4cpp::Category& subLog = log4cpp::Category::getInstance(std::string("sub1")); 
	if(res == 0)
	{
		throw  NullPointerException("BuildVTKGradientBasedFeatureImage(vtkImageData*originalData,vtkImageData* res)：res是空指针，请初始化");
	}
	rootLog.info("void vmtkImageFeature::BuildVTKGradientBasedFeatureImage(vtkImageData*originalData,vtkImageData*res) start......");
	subLog.info("void vmtkImageFeature::BuildVTKGradientBasedFeatureImage(vtkImageData*originalData,vtkImageData*res) start......");

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

void vmtkImageFeature::BuildFWHMBasedFeatureImage(vtkImageData*originalData,vtkImageData* res)
{
	    log4cpp::Category& rootLog  = log4cpp::Category::getRoot();
	    log4cpp::Category& subLog = log4cpp::Category::getInstance(std::string("sub1")); 
	    if(res == 0)
		{
			throw NullPointerException("BuildFWHMBasedFeatureImage(vtkImageData*originalData,vtkImageData* res)：res是空指针，请初始化");
		}
		rootLog.info("void vmtkImageFeature::BuildFWHMBasedFeatureImage(vtkImageData*originalData,vtkImageData* res) start......");
		subLog.info("void vmtkImageFeature::BuildFWHMBasedFeatureImage(vtkImageData*originalData,vtkImageData* res) start.......");
		vtkSmartPointer<vtkImageCast> cast = vtkSmartPointer<vtkImageCast>::New();        
		cast->SetInputData(originalData);
        cast->SetOutputScalarTypeToFloat();
        cast->Update();
		
		vtkSmartPointer<vtkvmtkFWHMFeatureImageFilter> fwhmFeatureImageFilter = vtkSmartPointer<vtkvmtkFWHMFeatureImageFilter>::New();
        fwhmFeatureImageFilter->SetInputConnection(cast->GetOutputPort());
		
        fwhmFeatureImageFilter->SetRadius(FWHMRadius);
        fwhmFeatureImageFilter->SetBackgroundValue(FWHMBackgroundValue);
        fwhmFeatureImageFilter->Update();
	
		res->DeepCopy(fwhmFeatureImageFilter->GetOutput());
}

void vmtkImageFeature::BuildUpwindGradientBasedFeatureImage(vtkImageData*originalData,vtkImageData* res)
{
	    log4cpp::Category& rootLog  = log4cpp::Category::getRoot();
	    log4cpp::Category& subLog = log4cpp::Category::getInstance(std::string("sub1")); 
	    if(res == 0)
		{
			throw NullPointerException("BuildUpwindGradientBasedFeatureImage(vtkImageData*originalData,vtkImageData* res)：res是空指针，请初始化");
		}
		rootLog.info("void vmtkImageFeature::BuildUpwindGradientBasedFeatureImage(vtkImageData*originalData,vtkImageData* res) start......");
		subLog.info("void vmtkImageFeature::BuildUpwindGradientBasedFeatureImage(vtkImageData*originalData,vtkImageData* res) start......");
		vtkSmartPointer<vtkImageCast> cast = vtkSmartPointer<vtkImageCast>::New();        
		cast->SetInputData(originalData);
        cast->SetOutputScalarTypeToFloat();
        cast->Update();
	    vtkSmartPointer<vtkvmtkUpwindGradientMagnitudeImageFilter> gradientMagnitude = vtkSmartPointer<vtkvmtkUpwindGradientMagnitudeImageFilter>::New();
        gradientMagnitude->SetInputConnection(cast->GetOutputPort());
        gradientMagnitude->SetUpwindFactor(UpwindFactor);
        gradientMagnitude->Update();
		vtkImageData* featureImage;
        //featureImage = None
        if (SigmoidRemapping==1)
		{
			//scalarRange =
            double *scalarRange = gradientMagnitude->GetOutput()->GetPointData()->GetScalars()->GetRange();
			//gradientMagnitude->GetOutput()->GetPointData();

            double inputMinimum = scalarRange[0];
            double inputMaximum = scalarRange[1];
            double alpha = - (inputMaximum - inputMinimum) / 6.0;
            double beta = (inputMaximum + inputMinimum) / 2.0;
			vtkSmartPointer<vtkvmtkSigmoidImageFilter> sigmoid = vtkSmartPointer<vtkvmtkSigmoidImageFilter>::New();
            sigmoid->SetInputConnection(gradientMagnitude->GetOutputPort());
            sigmoid->SetAlpha(alpha);
            sigmoid->SetBeta(beta);
            sigmoid->SetOutputMinimum(0.0);
            sigmoid->SetOutputMaximum(1.0);
            sigmoid->Update();
            featureImage = sigmoid->GetOutput();
		}
        else
		{
            
			vtkSmartPointer<vtkvmtkBoundedReciprocalImageFilter> boundedReciprocal = vtkSmartPointer<vtkvmtkBoundedReciprocalImageFilter>::New();	
            boundedReciprocal->SetInputConnection(gradientMagnitude->GetOutputPort());
            boundedReciprocal->Update();
            featureImage = boundedReciprocal->GetOutput();
		}
		res->DeepCopy(featureImage);
}

void vmtkImageFeature::BuildGradientBasedFeatureImage(vtkImageData*originalData,vtkImageData* res)
{
	    log4cpp::Category& rootLog  = log4cpp::Category::getRoot();
	    log4cpp::Category& subLog = log4cpp::Category::getInstance(std::string("sub1")); 
		if(res == 0||res->GetReferenceCount() < 1)
		{
			throw NullPointerException("BuildGradientBasedFeatureImage(vtkImageData*originalData,vtkImageData* res)：res是空指针，请初始化");
		}
		rootLog.info("void vmtkImageFeature::BuildGradientBasedFeatureImage(vtkImageData*originalData,vtkImageData* res)  start.....");
		subLog.info("void vmtkImageFeature::BuildGradientBasedFeatureImage(vtkImageData*originalData,vtkImageData* res)  start.....");
		vtkSmartPointer<vtkImageCast> cast = vtkSmartPointer<vtkImageCast>::New();        
		cast->SetInputData(originalData);
		
        cast->SetOutputScalarTypeToFloat();
		
        cast->Update();

		rootLog.debug("void vmtkImageFeature::BuildGradientBasedFeatureImage(vtkImageData*originalData,vtkImageData* res)  getting ImageFilter.....");
		subLog.debug("void vmtkImageFeature::BuildGradientBasedFeatureImage(vtkImageData*originalData,vtkImageData* res)  getting ImageFilter.....");
		vtkSimpleImageToImageFilter*gradientMagnitude = 0;

		vtkImageData* tempData = 0;
        if (DerivativeSigma > 0.0)
		{
			vtkvmtkGradientMagnitudeRecursiveGaussianImageFilter* gradientMagnitude2 = vtkvmtkGradientMagnitudeRecursiveGaussianImageFilter::New();

			gradientMagnitude2->SetInputData(cast->GetOutput());
            //gradientMagnitude2->SetInputConnection(cast->GetOutputPort());
            gradientMagnitude2->SetSigma(DerivativeSigma);
            gradientMagnitude2->SetNormalizeAcrossScale(0);
            gradientMagnitude2->Update();
			gradientMagnitude = gradientMagnitude2;
			tempData = gradientMagnitude2->GetOutput();
		}
        else
		{
            
		    vtkvmtkGradientMagnitudeImageFilter* gradientMagnitude3 = vtkvmtkGradientMagnitudeImageFilter::New();
				
			gradientMagnitude3->SetInputData(cast->GetOutput());
            //gradientMagnitude3->SetInputConnection(cast->GetOutputPort());
			
            gradientMagnitude3->Update();
		    
			gradientMagnitude = gradientMagnitude3;
			tempData = gradientMagnitude3->GetOutput();
		}
        //featureImage = None
		rootLog.debug("void vmtkImageFeature::BuildGradientBasedFeatureImage(vtkImageData*originalData,vtkImageData* res)  getting FeatureImage.....");
		subLog.debug("void vmtkImageFeature::BuildGradientBasedFeatureImage(vtkImageData*originalData,vtkImageData* res)  getting FeatureImage.....");
		vtkSmartPointer<vtkImageData> featureImage;
        if (SigmoidRemapping==1)
		{
            double*scalarRange =  gradientMagnitude->GetOutput()->GetPointData()->GetScalars()->GetRange();
            double inputMinimum = scalarRange[0];
            double inputMaximum = scalarRange[1];
            double  alpha = - (inputMaximum - inputMinimum) / 6.0;
            double  beta = (inputMaximum + inputMinimum) / 2.0;
			vtkSmartPointer<vtkvmtkSigmoidImageFilter> sigmoid = vtkSmartPointer<vtkvmtkSigmoidImageFilter>::New();
			sigmoid->SetInputData(tempData);
            sigmoid->SetAlpha(alpha);
            sigmoid->SetBeta(beta);
            sigmoid->SetOutputMinimum(0.0);
            sigmoid->SetOutputMaximum(1.0);
            sigmoid->Update();
            featureImage = sigmoid->GetOutput();
		}
        else
		{

			vtkSmartPointer<vtkvmtkBoundedReciprocalImageFilter> boundedReciprocal = vtkSmartPointer<vtkvmtkBoundedReciprocalImageFilter>::New();
			
			boundedReciprocal->SetInputData(tempData);
		

             boundedReciprocal->Update();
		
            featureImage = boundedReciprocal->GetOutput();
		}
	
		res->DeepCopy(featureImage);
		gradientMagnitude->Delete();
}