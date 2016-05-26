#include <vmtkLevelSetSegmentation.h>
#include <vtkSimpleImageToImageFilter.h>
#include <vtkvmtkGeodesicActiveContourLevelSetImageFilter.h>
#include <vtkvmtkCurvesLevelSetImageFilter.h>
#include <vtkvmtkThresholdSegmentationLevelSetImageFilter.h>
#include "NullPointerException.h"
#include <vtkvmtkLaplacianSegmentationLevelSetImageFilter.h>
#include <vtkImageMathematics.h>
vmtkLevelSetSegmentation::vmtkLevelSetSegmentation()
{
	    PropagationScaling = 0.0;
        CurvatureScaling = 0.0;
        AdvectionScaling = 1.0;
		FeatureDerivativeSigma = 0.0;

		EdgeWeight = 0.0;
		SmoothingIterations = 5;
		SmoothingTimeStep = 0.1;
		SmoothingConductance = 0.8;
		NumberOfIterations = 0;
		IsoSurfaceValue = 0.0;

		levelSetTypeName = LevelSetType::GEODESIC;
		featureImageTypeName = FeatureImageType::GRADIENT;
}


vmtkLevelSetSegmentation::~vmtkLevelSetSegmentation()
{
	if(InitializationImage)
	{
		InitializationImage->Delete();
	}
}



void vmtkLevelSetSegmentation::LevelSetEvolutionGEODESIC(vtkImageData*FeatureImage,vtkImageData*LevelSetsInput,vtkImageData*LevelSetsOutput)
{
		if(FeatureImage == 0|| LevelSetsInput == 0||LevelSetsOutput == 0)
		{
		    throw NullPointerException(":LevelSetEvolutionGEODESIC(vtkImageData*FeatureImage,vtkImageData*LevelSetsInput,vtkImageData*LevelSetsOutput):3�����������Ѿ������ڴ�");
		}
		vtkSmartPointer<vtkvmtkGeodesicActiveContourLevelSetImageFilter> levelSetsG = vtkSmartPointer<vtkvmtkGeodesicActiveContourLevelSetImageFilter>::New();	
		levelSetsG->SetFeatureImage(FeatureImage);
		levelSetsG->SetDerivativeSigma(FeatureDerivativeSigma);
		levelSetsG->SetAutoGenerateSpeedAdvection(1);
		levelSetsG->SetPropagationScaling(PropagationScaling);
		levelSetsG->SetCurvatureScaling(CurvatureScaling);
		levelSetsG->SetAdvectionScaling(AdvectionScaling);



	    levelSetsG->SetInputData(LevelSetsInput);	
        levelSetsG->SetNumberOfIterations(NumberOfIterations);
        levelSetsG->SetIsoSurfaceValue(IsoSurfaceValue);
        levelSetsG->SetMaximumRMSError(MaximumRMSError);
        levelSetsG->SetInterpolateSurfaceLocation(1);		
        levelSetsG->SetUseImageSpacing(1);
		levelSetsG->Update();

		LevelSetsOutput->DeepCopy(levelSetsG->GetOutput());
	
}

void vmtkLevelSetSegmentation::LevelSetEvolutionCURVES(vtkImageData*FeatureImage,vtkImageData*LevelSetsInput,vtkImageData*LevelSetsOutput)
{
		if(FeatureImage == 0|| LevelSetsInput == 0||LevelSetsOutput == 0)
		{
		    throw NullPointerException(":LevelSetEvolutionCURVES(vtkImageData*FeatureImage,vtkImageData*LevelSetsInput,vtkImageData*LevelSetsOutput):3�����������Ѿ������ڴ�");
		}
		vtkSmartPointer<vtkvmtkCurvesLevelSetImageFilter>levelSetsC = vtkSmartPointer<vtkvmtkCurvesLevelSetImageFilter>::New();
		levelSetsC->SetFeatureImage(FeatureImage);
        levelSetsC->SetDerivativeSigma(FeatureDerivativeSigma);
        levelSetsC->SetAutoGenerateSpeedAdvection(1);
        levelSetsC->SetPropagationScaling(PropagationScaling);
        levelSetsC->SetCurvatureScaling(CurvatureScaling);
        levelSetsC->SetAdvectionScaling(AdvectionScaling);


		levelSetsC->SetInputData(LevelSetsInput);	
        levelSetsC->SetNumberOfIterations(NumberOfIterations);
        levelSetsC->SetIsoSurfaceValue(IsoSurfaceValue);
        levelSetsC->SetMaximumRMSError(MaximumRMSError);
        levelSetsC->SetInterpolateSurfaceLocation(1);		
        levelSetsC->SetUseImageSpacing(1);
		levelSetsC->Update();

		LevelSetsOutput->DeepCopy(levelSetsC->GetOutput());
}

void vmtkLevelSetSegmentation::LevelSetEvolutionTHRESHOLD(vtkImageData*originalImage,vtkImageData*LevelSetsInput,vtkImageData*LevelSetsOutput,double UpperThreshold ,double LowerThreshold)
{
		if(originalImage == 0|| LevelSetsInput == 0||LevelSetsOutput == 0)
		{
		    throw NullPointerException("void vmtkLevelSetSegmentation::LevelSetEvolutionTHRESHOLD(vtkImageData*originalImage,vtkImageData*LevelSetsInput,vtkImageData*LevelSetsOutput,double UpperThreshold ,double LowerThreshold):ǰ��3�����������Ѿ������ڴ�");
		}
		vtkSmartPointer<vtkvmtkThresholdSegmentationLevelSetImageFilter> levelSetsT = vtkSmartPointer<vtkvmtkThresholdSegmentationLevelSetImageFilter>::New();
		 
		 levelSetsT->SetFeatureImage(originalImage);

		double*scalarRange =  originalImage->GetScalarRange();
		 
		if (LowerThreshold != unassinedThreshHold)
		{
		   levelSetsT->SetLowerThreshold(LowerThreshold);
		}else
		{        
		   levelSetsT->SetLowerThreshold(scalarRange[0]-1.0);
		}

		if (UpperThreshold != unassinedThreshHold)
		{
		   levelSetsT->SetUpperThreshold(UpperThreshold);
		}else
		{
		     levelSetsT->SetUpperThreshold(scalarRange[1]+1.0);
		}
        levelSetsT->SetEdgeWeight(EdgeWeight);
        levelSetsT->SetSmoothingIterations(SmoothingIterations);
        levelSetsT->SetSmoothingTimeStep(SmoothingTimeStep);
        levelSetsT->SetSmoothingConductance(SmoothingConductance);
        levelSetsT->SetPropagationScaling(PropagationScaling);
        levelSetsT->SetCurvatureScaling(CurvatureScaling);


		levelSetsT->SetInputData(LevelSetsInput);	
        levelSetsT->SetNumberOfIterations(NumberOfIterations);
        levelSetsT->SetIsoSurfaceValue(IsoSurfaceValue);
        levelSetsT->SetMaximumRMSError(MaximumRMSError);
        levelSetsT->SetInterpolateSurfaceLocation(1);		
        levelSetsT->SetUseImageSpacing(1);
		levelSetsT->Update();

		LevelSetsOutput->DeepCopy(levelSetsT->GetOutput());

}

void vmtkLevelSetSegmentation::LevelSetEvolutionLAPLACIAN(vtkImageData*originalImage,vtkImageData*LevelSetsInput,vtkImageData*LevelSetsOutput)
{
			if(originalImage == 0|| LevelSetsInput == 0||LevelSetsOutput == 0)
			{
			throw NullPointerException("void vmtkLevelSetSegmentation::LevelSetEvolutionLAPLACIAN(vtkImageData*originalImage,vtkImageData*LevelSetsInput,vtkImageData*LevelSetsOutput):ǰ��3�����������Ѿ������ڴ�");
			}
			vtkSmartPointer<vtkvmtkLaplacianSegmentationLevelSetImageFilter>levelSetsL = vtkvmtkLaplacianSegmentationLevelSetImageFilter::New();
			levelSetsL->SetFeatureImage(originalImage);
			levelSetsL->SetPropagationScaling(-PropagationScaling);
			levelSetsL->SetCurvatureScaling(CurvatureScaling);

			levelSetsL->SetInputData(LevelSetsInput);	
			levelSetsL->SetNumberOfIterations(NumberOfIterations);
			levelSetsL->SetIsoSurfaceValue(IsoSurfaceValue);
			levelSetsL->SetMaximumRMSError(MaximumRMSError);
			levelSetsL->SetInterpolateSurfaceLocation(1);		
			levelSetsL->SetUseImageSpacing(1);
			levelSetsL->Update();

			LevelSetsOutput->DeepCopy(levelSetsL->GetOutput());
}

void vmtkLevelSetSegmentation::MergeMergeLevelSet(vtkImageData*LevelSetInput1,vtkImageData*LevelSetInput2)
{
	if(LevelSetInput1 == 0|| LevelSetInput2 == 0)
	{
		throw NullPointerException("����ͼ���������Ϊ��ָ��");
	}
	vtkSmartPointer<vtkImageMathematics> minFilter = vtkSmartPointer<vtkImageMathematics>::New();
	minFilter->SetOperationToMin();
	minFilter->SetInput1Data(LevelSetInput1);
	minFilter->SetInput2Data(LevelSetInput2);
	minFilter->Update();
	LevelSetInput1->DeepCopy(minFilter->GetOutput());
}

void vmtkLevelSetSegmentation::Execute()
{
	log4cpp::Category& rootLog  = log4cpp::Category::getRoot();
	log4cpp::Category& subLog = log4cpp::Category::getInstance(std::string("sub1"));
	if(Image == 0)
	{
		rootLog.error("Error:vmtkLevelSetSegmentation::Execute()û��ͼ��");
		subLog.error("Error:vmtkLevelSetSegmentation::Execute()û��ͼ��");
		return;
	}

	vtkSmartPointer<vtkImageCast> cast = vtkSmartPointer<vtkImageCast>::New();
	cast->SetInputData(Image);
	cast->SetOutputScalarTypeToFloat();
	cast->Update();
	Image = cast->GetOutput();

	if(InitializationImage == 0)
	{
		InitializationImage = vtkImageData::New();
		InitializationImage->DeepCopy(Image);
	}

	if(FeatureImage == 0)
	{
		FeatureImage = vtkImageData::New();
		vmtkImageFeature* 
		if(levelSetTypeName == LevelSetType::GEODESIC || levelSetTypeName ==  LevelSetType::LAPLACIAN)
		{
			switch (featureImageTypeName)
			{
			case VTKGRADIENT:
				break;
			case GRADIENT:
				break;
			case UPWIND:
				break;
			case FWHM:
				break;
			default:
				break;
			}
		   	
		}else if(levelSetTypeName == LevelSetType::THRESHOLD || levelSetTypeName == LevelSetType::LAPLACIAN)
		{
			FeatureImage->DeepCopy(Image);
		}
		
	}




}
