#ifndef VMTK_LEVEL_SET_SEGMENTATION_HPP_
#define VMTK_LEVEL_SET_SEGMENTATION_HPP_
#include <vtkImageData.h>
#include <vtkSmartPointer.h>
#include <log4cpp/Category.hh>
#include <log4cpp/PropertyConfigurator.hh>
#include <vtkImageCast.h>
#include "vmtkImageFeature.h"
#include "vmtkImageSeeder.h"
#include "vmtkSurfaceViewer.h"
#include "vmtkImageInitialization.h"
const double unassinedThreshHold = 100000;
enum LevelSetType
{
	GEODESIC,CURVES,THRESHOLD,LAPLACIAN
};
class vmtkLevelSetSegmentation:public vtkObjectBase
{
private:
	int SigmoidRemapping;
	double FeatureDerivativeSigma;
	double PropagationScaling;
	double CurvatureScaling;
	double AdvectionScaling;
	double UpwindFactor;
	double FWHMRaidus[3];
	double FWHMBackgroundValue;

	double EdgeWeight;
	int SmoothingIterations;
	double SmoothingTimeStep;
	double SmoothingConductance;

	int NumberOfIterations;
	double IsoSurfaceValue;
	double MaximumRMSError;

	LevelSetType levelSetTypeName;

	FeatureImageType featureImageTypeName;

	int NegateForInitialization;
	//外部分配
	vtkImageData* Image;


	//InitializationImage这个类分配内存
	vtkImageData* InitializationImage;

	//FeatureImage这个类分配内存
	vtkImageData* FeatureImage;

	//ImageSeeder这个类分配的内存
	vmtkImageSeeder* ImageSeeder;

	//SurfaceViewer由这个类分配内存
	vmtkSurfaceViewer* SurfaceViewer;

	//外面传进来
	vtkRenderer* Renderer;

	//本实例分配内存
	vtkImageData* LevelSets;

	//本例分配内存
	vmtkImageInitialization* vii;

	vtkImageData* InitialLevelSets;

	//这个类从其它地方获得，或者说获得其它地方引用
	vtkImageData*LevelSetsInput;

	//这个类分配的内存
	vtkImageData*LevelSetsOutput;

	//外部分配的
	vtkImagePlaneWidget* planeWidget[3];

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
    void LevelSetEvolutionGEODESIC();

	void LevelSetEvolutionCURVES();

	void LevelSetEvolutionTHRESHOLD(double UpperThreshold = unassinedThreshHold,double LowerThreshold = unassinedThreshHold);

	void LevelSetEvolutionLAPLACIAN();

	/*
	*合并图像的方法,合并之后的结果放在LevelSetInput1
	*/
	void MergeLevelSet();

	void DisplayLevelSetSurface(vtkImageData*levelSets,double value = 0.0);
public:
	void Execute();

	void setImage(vtkImageData*_image);

	void setRenderer(vtkRenderer*_render);

	void setPlaneWidget(vtkImagePlaneWidget* _palneWidget[3]);




	
};
#endif