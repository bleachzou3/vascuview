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

	int NegateForInitialization;
	//�ⲿ����
	vtkImageData* Image;


	//InitializationImage���������ڴ�
	vtkImageData* InitializationImage;

	//FeatureImage���������ڴ�
	vtkImageData* FeatureImage;

	//ImageSeeder����������ڴ�
	vmtkImageSeeder* ImageSeeder;

	//SurfaceViewer�����������ڴ�
	vmtkSurfaceViewer* SurfaceViewer;

	//���洫����
	vtkRenderer* Renderer;

	//��ʵ�������ڴ�
	vtkImageData* LevelSets;

	//���������ڴ�
	vmtkImageInitialization* vii;

	vtkImageData* InitialLevelSets;

	//�����������ط���ã�����˵��������ط�����
	vtkImageData*LevelSetsInput;

	//����������ڴ�
	vtkImageData*LevelSetsOutput;

public:
	static vmtkLevelSetSegmentation* New();
	vtkTypeMacro(vmtkLevelSetSegmentation,vtkObjectBase);
	

protected:
		vmtkLevelSetSegmentation();
	virtual~vmtkLevelSetSegmentation();
	
private:
	/*
	  ���µ��ĸ�����ͼ��ָ�붼����Ϊ��
	*/
    void LevelSetEvolutionGEODESIC();

	void LevelSetEvolutionCURVES(vtkImageData*FeatureImage,vtkImageData*LevelSetsInput,vtkImageData*LevelSetsOutput);

	void LevelSetEvolutionTHRESHOLD(vtkImageData*originalImage,vtkImageData*LevelSetsInput,vtkImageData*LevelSetsOutput,double UpperThreshold = unassinedThreshHold,double LowerThreshold = unassinedThreshHold);

	void LevelSetEvolutionLAPLACIAN(vtkImageData*originalImage,vtkImageData*LevelSetsInput,vtkImageData*LevelSetsOutput);

	/*
	*�ϲ�ͼ��ķ���,�ϲ�֮��Ľ������LevelSetInput1
	*/
	void MergeMergeLevelSet(vtkImageData*LevelSetInput1,vtkImageData*LevelSetInput2);

	void DisplayLevelSetSurface(vtkImageData*levelSets,double value = 0.0);
public:
	void Execute();




	
};
#endif