#ifndef VMTK_IMAGE_FEATURES_CPP_
#define VMTK_IMAGE_FEATURES_CPP_
using namespace std;
#include <vector>
#include <vtkImageData.h>
#include <vtkImageCast.h>
#include <vtkSmartPointer.h>
#include <vtkImageGradientMagnitude.h>
#include <vtkImageMathematics.h>
#include "Uncopyable.h"
enum FeatureImageType
{
	VTKGRADIENT,GRADIENT,UPWIND,FWHM
};
class vmtkImageFeature:private Uncopyable
{
private:
  int Dimensionality;
  double DerivativeSigma;
  int SigmoidRemapping;
  double UpwindFactor;
  int FWHMRadius[3];
  
  double FWHMBackgroundValue;
public:
	vmtkImageFeature();
	virtual ~vmtkImageFeature();
	//originalData��ԭʼͼ��,resͼ���ǽ��ͼ�񣬴�����֮ǰ�Ѿ���ʼ��������˵�Ѿ������ڴ棬��������������ڲ������ڴ�
	void BuildVTKGradientBasedFeatureImage(vtkImageData*originalData,vtkImageData* res);


	//originalData��ԭʼͼ��,resͼ���ǽ��ͼ�񣬴�����֮ǰ�Ѿ���ʼ��������˵�Ѿ������ڴ棬��������������ڲ������ڴ�
	void BuildFWHMBasedFeatureImage(vtkImageData*originalData,vtkImageData* res);


	//originalData��ԭʼͼ��,resͼ���ǽ��ͼ�񣬴�����֮ǰ�Ѿ���ʼ��������˵�Ѿ������ڴ棬��������������ڲ������ڴ�
	void BuildUpwindGradientBasedFeatureImage(vtkImageData*originalData,vtkImageData* res);

	//originalData��ԭʼͼ��,resͼ���ǽ��ͼ�񣬴�����֮ǰ�Ѿ���ʼ��������˵�Ѿ������ڴ棬��������������ڲ������ڴ�
	void BuildGradientBasedFeatureImage(vtkImageData*originalData,vtkImageData* res);
};

#endif
