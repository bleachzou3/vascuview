#include <vmtkLevelSetSegmentation.h>
#include <vtkSimpleImageToImageFilter.h>
#include <vtkvmtkGeodesicActiveContourLevelSetImageFilter.h>
#include <vtkvmtkCurvesLevelSetImageFilter.h>
#include <vtkvmtkThresholdSegmentationLevelSetImageFilter.h>
#include "NullPointerException.h"
#include <vtkvmtkLaplacianSegmentationLevelSetImageFilter.h>
#include <vtkImageMathematics.h>
#include <vtkObjectFactory.h>
#include <vtkMarchingCubes.h>
#include <vmtkImageInitialization.h>
#include "LevelSetDialog.h"
vtkStandardNewMacro(vmtkLevelSetSegmentation);
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

		NegateForInitialization = 0;
		levelSetTypeName = LevelSetType::GEODESIC;
		featureImageTypeName = FeatureImageType::GRADIENT;
}


vmtkLevelSetSegmentation::~vmtkLevelSetSegmentation()
{
	if(InitializationImage !=0 && InitializationImage->GetReferenceCount()>0)
	{
		InitializationImage->Delete();
	}
	if(ImageSeeder != 0 && ImageSeeder->GetReferenceCount() >0 )
	{
		ImageSeeder->Delete();
	}
	

}



void vmtkLevelSetSegmentation::LevelSetEvolutionGEODESIC()
{      
	    log4cpp::Category& rootLog  = log4cpp::Category::getRoot();
	    log4cpp::Category& subLog = log4cpp::Category::getInstance(std::string("sub1"));
		if(FeatureImage == 0|| LevelSetsInput == 0||LevelSetsOutput == 0||
			FeatureImage->GetReferenceCount()< 1 || LevelSetsInput->GetReferenceCount() < 1|| LevelSetsOutput->GetReferenceCount() < 1)
		{
		    
			rootLog.info("void vmtkLevelSetSegmentation::LevelSetEvolutionGEODESIC() FeatureImage")
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
		if(LevelSetsOutput != 0 && LevelSetsOutput->GetReferenceCount()>0)
			LevelSetsOutput->Delete();

		LevelSetsOutput = vtkImageData::New();
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

void vmtkLevelSetSegmentation::MergeMergeLevelSet(vtkImageData*LevelSetInput1,vtkImageData*LevelSetInput2)
{
	if(LevelSetInput1 == 0|| LevelSetInput2 == 0)
	{
		throw NullPointerException("输入图像参数不能为空指针");
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
		rootLog.error("Error:vmtkLevelSetSegmentation::Execute()没有图像");
		subLog.error("Error:vmtkLevelSetSegmentation::Execute()没有图像");
		return;
	}

	vtkSmartPointer<vtkImageCast> cast = vtkSmartPointer<vtkImageCast>::New();
	cast->SetInputData(Image);
	cast->SetOutputScalarTypeToFloat();
	cast->Update();
	Image = cast->GetOutput();

	if(InitializationImage == 0 || InitializationImage->GetReferenceCount() < 1)
	{
		InitializationImage = vtkImageData::New();
		InitializationImage->DeepCopy(Image);
	}

	if(FeatureImage == 0)
	{
		FeatureImage = vtkImageData::New();
		
		if(levelSetTypeName == LevelSetType::GEODESIC || levelSetTypeName ==  LevelSetType::LAPLACIAN)
		{
			vmtkImageFeature* imageFeatures = vmtkImageFeature::New();
			
			switch (featureImageTypeName)
			{
			case VTKGRADIENT:
				imageFeatures->BuildVTKGradientBasedFeatureImage(Image,FeatureImage);
				break;
			case GRADIENT:
				imageFeatures->BuildGradientBasedFeatureImage(Image,FeatureImage);
				break;
			case UPWIND:
				imageFeatures->BuildUpwindGradientBasedFeatureImage(Image,FeatureImage);
				break;
			case FWHM:
				imageFeatures->BuildFWHMBasedFeatureImage(Image,FeatureImage);
				break;
			default:
				break;
			}

			imageFeatures->Delete();
		   	
		}else if(levelSetTypeName == LevelSetType::THRESHOLD || levelSetTypeName == LevelSetType::LAPLACIAN)
		{
			FeatureImage->DeepCopy(Image);
		}else
		{
			   rootLog.error("Unsupported LevelSetsType");
		       subLog.error("Unsupported LevelSetsType");
		}

		
	}
	if( NumberOfIterations != 0)
	{
         
	}


	ImageSeeder = vmtkImageSeeder::New();
	ImageSeeder->setRenderer(Renderer);
	ImageSeeder->setImage(InitializationImage);
	ImageSeeder->setDisplay(0);
	ImageSeeder->execute();
	ImageSeeder->BuildView();

	SurfaceViewer = vmtkSurfaceViewer::New();

	SurfaceViewer->setRenderer(Renderer);

	if(LevelSets != 0 && LevelSets->GetReferenceCount() > 0)
	{
		DisplayLevelSetSurface(LevelSets,0.0);
	}
	
	vii = vmtkImageInitialization::New();

	vii->setImageData(InitializationImage);
	
	vii->setRenderer(Renderer);

	vii->setImageSeeder(ImageSeeder);

	vii->setSurfaceViewer(SurfaceViewer);

	vii->setNegateImage(NegateForInitialization);

	int endSegmentation = 0;

	while(endSegmentation == 0)
	{
	  
		if(InitialLevelSets == 0|| InitialLevelSets->GetReferenceCount() < 1)
		{
			vii->execute();
			LevelSetsInput = vii->getInitialLevelSets();
			vii->setInitialLevelSets(0);
		}else
		{
			LevelSetsInput = InitialLevelSets;
			InitialLevelSets = 0;
			DisplayLevelSetSurface(LevelSetsInput,IsoSurfaceValue);
		}
	    
		bool endEvolution = false;
		while(!endEvolution)
		{
			LevelSetDialog * levelSetDialog = new LevelSetDialog;
			levelSetDialog->show();
			if(levelSetDialog->ExitRadioButton->isChecked())
			{
				return;
			}else if(levelSetDialog->AcceptRadioButton->isChecked())
			{
				endEvolution = true;
			}else{
			    try
				{
					NumberOfIterations = levelSetDialog->NumberOfIterationsLineEdit->text().toInt();
					PropagationScaling = levelSetDialog->PropagationScalingLineEdit->text().toDouble();
					CurvatureScaling = levelSetDialog->CurvatureScalingLineEdit->text().toDouble();
					AdvectionScaling = levelSetDialog->AdvectionScalingLineEdit->text().toDouble();
				}catch(exception e)
				{
					rootLog.error("void vmtkLevelSetSegmentation::Execute() wrong parameter");
		            subLog.error("void vmtkLevelSetSegmentation::Execute() wrong parameter"); 
					continue;
				}
			}
			if(endEvolution)
				break;
			switch (levelSetTypeName)
			{
			case GEODESIC:
				break;
			case CURVES:
				break;
			case THRESHOLD:
				break;
			case LAPLACIAN:
				break;
			default:
				break;
			}
		}
	
	}
	


}

void vmtkLevelSetSegmentation::DisplayLevelSetSurface(vtkImageData*_levelSets,double _value)
{
	vtkSmartPointer<vtkMarchingCubes> marchingCubes = vtkSmartPointer<vtkMarchingCubes>::New();
	marchingCubes->SetInputData(_levelSets);
	marchingCubes->SetValue(0,_value);
	marchingCubes->Update();
    
	SurfaceViewer->setSurface(marchingCubes->GetOutput());
	SurfaceViewer->setDisplay(0);
	SurfaceViewer->setOpacity(0.5);
	SurfaceViewer->buildView();


}
