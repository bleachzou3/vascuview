#ifndef VMTK_LEVEL_SET_SEGMENTATION_HPP_
#define VMTK_LEVEL_SET_SEGMENTATION_HPP_
#include <vtkImageData.h>
#include <vtkSmartPointer.h>
#include <log4cpp/Category.hh>
#include <log4cpp/PropertyConfigurator.hh>
#include <vtkImageCast.h>
#include "vmtkImageFeature.h"
#include "vmtkImageSeeder.h"

const double unassinedThreshHold = 100000;
enum LevelSetType
{
	GEODESIC,CURVES,THRESHOLD,LAPLACIAN
};
class vmtkLevelSetSegmentation:public vtkObjectBase
{
private:
	double FeatureDerivativeSigma;
	double PropagationScaling;
	double CurvatureScaling;
	double AdvectionScaling;


	double EdgeWeight;
	int SmoothingIterations;
	double SmoothingTimeStep;
	double SmoothingConductance;

	int NumberOfIterations;
	double IsoSurfaceValue;
	double MaximumRMSError;

	LevelSetType levelSetTypeName;

	FeatureImageType featureImageTypeName;
	//外部分配
	vtkImageData* Image;


	//InitializationImage这个类分配内存
	vtkImageData* InitializationImage;

	//FeatureImage这个类分配内存
	vtkImageData* FeatureImage;

	//ImageSeeder这个类分配的内存
	vmtkImageSeeder* ImageSeeder;

public:
	static vmtkLevelSetSegmentation* New();
	vtkTypeMacro(vmtkLevelSetSegmentation,vtkObjectBase);
	

protected:
		vmtkLevelSetSegmentation();
	virtual~vmtkLevelSetSegmentation();
	
private:
	/*
	  以下的四个方法图像指针都不能为空
	*/
    void LevelSetEvolutionGEODESIC(vtkImageData*FeatureImage,vtkImageData*LevelSetsInput,vtkImageData*LevelSetsOutput);

	void LevelSetEvolutionCURVES(vtkImageData*FeatureImage,vtkImageData*LevelSetsInput,vtkImageData*LevelSetsOutput);

	void LevelSetEvolutionTHRESHOLD(vtkImageData*originalImage,vtkImageData*LevelSetsInput,vtkImageData*LevelSetsOutput,double UpperThreshold = unassinedThreshHold,double LowerThreshold = unassinedThreshHold);

	void LevelSetEvolutionLAPLACIAN(vtkImageData*originalImage,vtkImageData*LevelSetsInput,vtkImageData*LevelSetsOutput);

	/*
	*合并图像的方法,合并之后的结果放在LevelSetInput1
	*/
	void MergeMergeLevelSet(vtkImageData*LevelSetInput1,vtkImageData*LevelSetInput2);
public:
	void Execute();


	
};
#endif