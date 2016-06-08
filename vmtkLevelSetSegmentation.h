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

	//�ⲿ�����
	vtkImagePlaneWidget* planeWidget[3];

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

	void LevelSetEvolutionCURVES();

	void LevelSetEvolutionTHRESHOLD(double UpperThreshold = unassinedThreshHold,double LowerThreshold = unassinedThreshHold);

	void LevelSetEvolutionLAPLACIAN();

	/*
	*�ϲ�ͼ��ķ���,�ϲ�֮��Ľ������LevelSetInput1
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