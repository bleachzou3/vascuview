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
#include "ThresholdDialog.h"
#include "YNDialog.h"
vtkStandardNewMacro(vmtkLevelSetSegmentation);
vmtkLevelSetSegmentation::vmtkLevelSetSegmentation():Image(0),InitializationImage(0),FeatureImage(0),ImageSeeder(0),SurfaceViewer(0),Renderer(0),LevelSets(0),InitialLevelSets(0),LevelSetsInput(0),LevelSetsOutput(0)
{

	    for(int i = 0; i < 3; i++)
		{
			planeWidget[i] = 0;
		}
		for(int i = 0; i < 3;i++)
		{
			FWHMRaidus[i] = 1.0;
		}
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
		SigmoidRemapping = 0;

		NegateForInitialization = 0;
		levelSetTypeName = LevelSetType::GEODESIC;
		featureImageTypeName = FeatureImageType::GRADIENT;

		UpwindFactor = 1.0;
		FWHMBackgroundValue = 0.0;
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
	if(FeatureImage != 0 && FeatureImage->GetReferenceCount() > 0)
	{
		FeatureImage->Delete();
	}
	if(SurfaceViewer != 0 && SurfaceViewer->GetReferenceCount() > 0 )
	{
		SurfaceViewer->Delete();
	}
	if(LevelSets != 0 && LevelSets->GetReferenceCount() > 0)
	{
		LevelSets->Delete();
	
	}
	if(LevelSetsOutput != 0&& LevelSetsOutput->GetReferenceCount() > 0)
	{
		LevelSetsOutput->Delete();
	}

}



void vmtkLevelSetSegmentation::LevelSetEvolutionGEODESIC()
{      
	    log4cpp::Category& rootLog  = log4cpp::Category::getRoot();
	    log4cpp::Category& subLog = log4cpp::Category::getInstance(std::string("sub1"));
		if(FeatureImage == 0|| LevelSetsInput == 0||
			FeatureImage->GetReferenceCount()< 1 || LevelSetsInput->GetReferenceCount() < 1)
		{
			subLog.info("void vmtkLevelSetSegmentation::LevelSetEvolutionGEODESIC() FeatureImage,LevelSetsInput,Invalid");
			rootLog.info("void vmtkLevelSetSegmentation::LevelSetEvolutionGEODESIC() FeatureImage,LevelSetsInput,Invalid");
			return;
		}
		subLog.info("void vmtkLevelSetSegmentation::LevelSetEvolutionGEODESIC() start...");
		rootLog.info("void vmtkLevelSetSegmentation::LevelSetEvolutionGEODESIC() start...");
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

void vmtkLevelSetSegmentation::LevelSetEvolutionCURVES()
{
		if(FeatureImage == 0|| LevelSetsInput == 0||
			FeatureImage->GetReferenceCount()< 1 || LevelSetsInput->GetReferenceCount() < 1)
		{
		    log4cpp::Category& rootLog  = log4cpp::Category::getRoot();
	        log4cpp::Category& subLog = log4cpp::Category::getInstance(std::string("sub1"));
			subLog.info("void vmtkLevelSetSegmentation::LevelSetEvolutionGEODESIC() FeatureImage,LevelSetsInput,Invalid");
			rootLog.info("void vmtkLevelSetSegmentation::LevelSetEvolutionGEODESIC() FeatureImage,LevelSetsInput,Invalid");
			return;
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
		if(LevelSetsOutput != 0 && LevelSetsOutput->GetReferenceCount()>0)
		           LevelSetsOutput->Delete();

		LevelSetsOutput = vtkImageData::New();
		LevelSetsOutput->DeepCopy(levelSetsC->GetOutput());
}

void vmtkLevelSetSegmentation::LevelSetEvolutionTHRESHOLD(double UpperThreshold ,double LowerThreshold)
{
			if(Image == 0|| LevelSetsInput == 0||
				Image->GetReferenceCount()< 1 || LevelSetsInput->GetReferenceCount() < 1)
			{
				log4cpp::Category& rootLog  = log4cpp::Category::getRoot();
				log4cpp::Category& subLog = log4cpp::Category::getInstance(std::string("sub1"));
				subLog.info("void vmtkLevelSetSegmentation::LevelSetEvolutionGEODESIC() Image,LevelSetsInput,Invalid");
				rootLog.info("void vmtkLevelSetSegmentation::LevelSetEvolutionGEODESIC() Image,LevelSetsInput,Invalid");
				return;
			}
		vtkSmartPointer<vtkvmtkThresholdSegmentationLevelSetImageFilter> levelSetsT = vtkSmartPointer<vtkvmtkThresholdSegmentationLevelSetImageFilter>::New();
		 
		levelSetsT->SetFeatureImage(Image);

		double*scalarRange =  Image->GetScalarRange();
		 
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
		if(LevelSetsOutput != 0 && LevelSetsOutput->GetReferenceCount()>0)
				LevelSetsOutput->Delete();

		LevelSetsOutput = vtkImageData::New();
		LevelSetsOutput->DeepCopy(levelSetsT->GetOutput());

}

void vmtkLevelSetSegmentation::LevelSetEvolutionLAPLACIAN()
{
			if(Image == 0|| LevelSetsInput == 0||
				Image->GetReferenceCount()< 1 || LevelSetsInput->GetReferenceCount() < 1)
			{
				log4cpp::Category& rootLog  = log4cpp::Category::getRoot();
				log4cpp::Category& subLog = log4cpp::Category::getInstance(std::string("sub1"));
				subLog.info("void vmtkLevelSetSegmentation::LevelSetEvolutionGEODESIC() Image,LevelSetsInput,Invalid");
				rootLog.info("void vmtkLevelSetSegmentation::LevelSetEvolutionGEODESIC() Image,LevelSetsInput,Invalid");
				return;
			}
			vtkSmartPointer<vtkvmtkLaplacianSegmentationLevelSetImageFilter>levelSetsL = vtkvmtkLaplacianSegmentationLevelSetImageFilter::New();
			levelSetsL->SetFeatureImage(Image);
			levelSetsL->SetPropagationScaling(-PropagationScaling);
			levelSetsL->SetCurvatureScaling(CurvatureScaling);

			levelSetsL->SetInputData(LevelSetsInput);	
			levelSetsL->SetNumberOfIterations(NumberOfIterations);
			levelSetsL->SetIsoSurfaceValue(IsoSurfaceValue);
			levelSetsL->SetMaximumRMSError(MaximumRMSError);
			levelSetsL->SetInterpolateSurfaceLocation(1);		
			levelSetsL->SetUseImageSpacing(1);
			levelSetsL->Update();
			if(LevelSetsOutput != 0 && LevelSetsOutput->GetReferenceCount()>0)
		          LevelSetsOutput->Delete();

		    LevelSetsOutput = vtkImageData::New();
			LevelSetsOutput->DeepCopy(levelSetsL->GetOutput());
}

void vmtkLevelSetSegmentation::MergeLevelSet()
{
	log4cpp::Category& rootLog  = log4cpp::Category::getRoot();
	log4cpp::Category& subLog = log4cpp::Category::getInstance(std::string("sub1"));
	if(LevelSetsOutput == 0 || LevelSetsOutput->GetReferenceCount() < 1)
	{
		rootLog.error("void vmtkLevelSetSegmentation::MergeLevelSet() LevelSetsOutput is invalid");
		subLog.error("void vmtkLevelSetSegmentation::MergeLevelSet() LevelSetsOutput is invalid");
		return;
	}
    

	if(LevelSets == 0 || LevelSets->GetReferenceCount() < 1)
	{
		LevelSets = vtkImageData::New();
		LevelSets->DeepCopy(LevelSetsOutput);
	}else
	{
		vtkSmartPointer<vtkImageMathematics> minFilter = vtkSmartPointer<vtkImageMathematics>::New();
		minFilter->SetOperationToMin();
		minFilter->SetInput1Data(LevelSets);
		minFilter->SetInput2Data(LevelSetsOutput);
		minFilter->Update();
		vtkImageData* tempLevelSets = LevelSets;


		LevelSets = vtkImageData::New();
		LevelSets->DeepCopy(minFilter->GetOutput());
		tempLevelSets->Delete();
	}


	
}

void vmtkLevelSetSegmentation::Execute()
{

	log4cpp::Category& rootLog  = log4cpp::Category::getRoot();
	log4cpp::Category& subLog = log4cpp::Category::getInstance(std::string("sub1"));
	if(Image == 0&& Image->GetReferenceCount() < 1)
	{
		rootLog.error("Error:vmtkLevelSetSegmentation::Execute()没有图像");
		subLog.error("Error:vmtkLevelSetSegmentation::Execute()没有图像");
		return;
	}
	for(int i = 0; i < 3; i++)
	{
		if(planeWidget[i] == 0 || planeWidget[i]->GetReferenceCount() < 1)
		{
			rootLog.error("Error:vmtkLevelSetSegmentation::Execute() vtkPlaneWidget invalid");
		    subLog.error("Error:vmtkLevelSetSegmentation::Execute() vtkPlaneWidget invalid");
			return;
		}
	}

	rootLog.debug("Error:vmtkLevelSetSegmentation::Execute()start...");
	subLog.debug("Error:vmtkLevelSetSegmentation::Execute()start...");
	vtkSmartPointer<vtkImageCast> cast = vtkSmartPointer<vtkImageCast>::New();
	cast->SetInputData(Image);
	cast->SetOutputScalarTypeToFloat();
	cast->Update();
	Image = cast->GetOutput();

   

	
	if(InitializationImage == 0 || InitializationImage->GetReferenceCount() < 1)
	{
		InitializationImage = Image;
	}

	rootLog.debug("info:vmtkLevelSetSegmentation::Execute()  getting Feature Image");
	subLog.debug("info:vmtkLevelSetSegmentation::Execute() getting Feature Image");
	if(FeatureImage == 0||FeatureImage->GetReferenceCount() < 1)
	{
		FeatureImage = vtkImageData::New();
		
		if(levelSetTypeName == LevelSetType::GEODESIC || levelSetTypeName ==  LevelSetType::LAPLACIAN)
		{
			vmtkImageFeature* imageFeatures = vmtkImageFeature::New();
			imageFeatures->setSigmoidRemapping(SigmoidRemapping);
			imageFeatures->setDerivativeSigma(FeatureDerivativeSigma);
			imageFeatures->setUpwindFactor(UpwindFactor);
			imageFeatures->setFWHMRadius(FWHMRaidus);
			imageFeatures->setFWHMBackgroundValue(FWHMBackgroundValue);
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

	rootLog.debug("info:vmtkLevelSetSegmentation::Execute()  initialize ImageSeeder and surfaceViewer");
	subLog.debug("info:vmtkLevelSetSegmentation::Execute() initialize ImageSeeder and surfaceViewer");
	ImageSeeder = vmtkImageSeeder::New();
	ImageSeeder->setRenderer(Renderer);
	ImageSeeder->setPlaneWidget(planeWidget);
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
			delete levelSetDialog;
			if(endEvolution)
				break;

			switch (levelSetTypeName)
			{
			case GEODESIC:
				LevelSetEvolutionGEODESIC();
				break;
			case CURVES:
				LevelSetEvolutionCURVES();
				break;
			case THRESHOLD:
			{
				ThresholdDialog* thresholdDialog = new ThresholdDialog;
				thresholdDialog->show();
				double upperThreshold = thresholdDialog->upperThresholdLine->text().toDouble();
				double lowerThreshold = thresholdDialog->lowerThresholdLine->text().toDouble();
				delete thresholdDialog;
				LevelSetEvolutionTHRESHOLD(upperThreshold,lowerThreshold);
				break;
			}
			case LAPLACIAN:
				LevelSetEvolutionLAPLACIAN();
				break;
			default:
				break;
			}
			DisplayLevelSetSurface(LevelSetsOutput);
			YNDialog *yndialog = new YNDialog;
			yndialog->setMessage("Accept result? (y/n):");
			if(yndialog->YRadioButton->isChecked())
			{
				endEvolution = true;
			}else
			{
				endEvolution = false;
			}
			delete yndialog;
			
		}

		YNDialog* yndialog = new YNDialog;
		yndialog->setMessage("Merge branch? (y/n):");
		if(yndialog->YRadioButton->isChecked())
		{
			MergeLevelSet();
		}else
		{
			delete yndialog;
			continue;
		}
		delete yndialog;

		if(LevelSets != 0&&LevelSets->GetReferenceCount()>0)
			DisplayLevelSetSurface(LevelSets);
		YNDialog* segBranch = new YNDialog;
		segBranch->setMessage("Segment another branch? (y/n):");
		if(segBranch->YRadioButton->isChecked())
		{
			endSegmentation = false;
		}else
		{
			endSegmentation = true;
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


void vmtkLevelSetSegmentation::setImage(vtkImageData*_image)
{
	Image = _image;
}

void vmtkLevelSetSegmentation::setRenderer(vtkRenderer*_render)
{
	Renderer = _render;
}


void vmtkLevelSetSegmentation::setPlaneWidget(vtkImagePlaneWidget* _planeWidget[3])
{
	for(int i = 0; i < 3; i++)
	{
		
		planeWidget[i] = _planeWidget[i];
		cout << "void vmtkLevelSetSegmentation::setPlaneWidget(vtkImagePlaneWidget* _planeWidget[3])" << _planeWidget[i] << ":" << planeWidget[i] << endl;
	}
}