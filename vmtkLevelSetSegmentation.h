#ifndef VMTK_LEVEL_SET_SEGMENTATION_HPP_
#define VMTK_LEVEL_SET_SEGMENTATION_HPP_
#include <vtkImageData.h>
#include <vtkSmartPointer.h>
const double unassinedThreshHold = 100000;
enum LevelSetType
{
	GEODESIC,CURVES,THRESHOLD,LAPLACIAN
};
class vmtkLevelSetSegmentation
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

public:

	
	vmtkLevelSetSegmentation();
	virtual~vmtkLevelSetSegmentation();
	//levelSetInput要处理的图像数据，levelSetOutput处理完之后的图像数据，levelSetOutput传进来之前已经分配内存
	


    void LevelSetEvolutionGEODESIC(vtkImageData*FeatureImage,vtkImageData*LevelSetsInput,vtkImageData*LevelSetsOutput);

	void LevelSetEvolutionCURVES(vtkImageData*FeatureImage,vtkImageData*LevelSetsInput,vtkImageData*LevelSetsOutput);

	void LevelSetEvolutionTHRESHOLD(vtkImageData*originalImage,vtkImageData*LevelSetsInput,vtkImageData*LevelSetsOutput,double UpperThreshold = unassinedThreshHold,double LowerThreshold = unassinedThreshHold);

	void LevelSetEvolutionLAPLACIAN(vtkImageData*originalImage,vtkImageData*LevelSetsInput,vtkImageData*LevelSetsOutput);
	
};
#endif