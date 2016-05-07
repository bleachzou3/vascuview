#include <vmtkLevelSetSegmentation.h>
#include <vtkSimpleImageToImageFilter.h>
#include <vtkvmtkGeodesicActiveContourLevelSetImageFilter.h>
#include <vtkvmtkCurvesLevelSetImageFilter.h>
#include <vtkvmtkThresholdSegmentationLevelSetImageFilter.h>
#include "NullPointerException.h"
#include <vtkvmtkLaplacianSegmentationLevelSetImageFilter.h>

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
}


vmtkLevelSetSegmentation::~vmtkLevelSetSegmentation()
{
}



void vmtkLevelSetSegmentation::LevelSetEvolutionGEODESIC(vtkImageData*FeatureImage,vtkImageData*LevelSetsInput,vtkImageData*LevelSetsOutput)
{
		if(FeatureImage == 0|| LevelSetsInput == 0||LevelSetsOutput == 0)
		{
		    throw NullPointerException(":LevelSetEvolutionGEODESIC(vtkImageData*FeatureImage,vtkImageData*LevelSetsInput,vtkImageData*LevelSetsOutput):3个参数必须已经分配内存");
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
		    throw NullPointerException(":LevelSetEvolutionCURVES(vtkImageData*FeatureImage,vtkImageData*LevelSetsInput,vtkImageData*LevelSetsOutput):3个参数必须已经分配内存");
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
		    throw NullPointerException("void vmtkLevelSetSegmentation::LevelSetEvolutionTHRESHOLD(vtkImageData*originalImage,vtkImageData*LevelSetsInput,vtkImageData*LevelSetsOutput,double UpperThreshold ,double LowerThreshold):前面3个参数必须已经分配内存");
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
			throw NullPointerException("void vmtkLevelSetSegmentation::LevelSetEvolutionLAPLACIAN(vtkImageData*originalImage,vtkImageData*LevelSetsInput,vtkImageData*LevelSetsOutput):前面3个参数必须已经分配内存");
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