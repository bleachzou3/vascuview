#ifndef VMTK_IMAGE_FEATURES_CPP_
#define VMTK_IMAGE_FEATURES_CPP_
using namespace std;
#include <vector>
#include <vtkImageData.h>
#include <vtkImageCast.h>
#include <vtkSmartPointer.h>
#include <vtkImageGradientMagnitude.h>
#include <vtkImageMathematics.h>
class vmtkImageFeature
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
	//originalData是原始图像,res图像是结果图像，传进来之前已经初始化，或者说已经分配内存，不是在这个函数内部分配内存
	void BuildVTKGradientBasedFeatureImage(vtkImageData*originalData,vtkImageData* res);


	//originalData是原始图像,res图像是结果图像，传进来之前已经初始化，或者说已经分配内存，不是在这个函数内部分配内存
	void BuildFWHMBasedFeatureImage(vtkImageData*originalData,vtkImageData* res);


	//originalData是原始图像,res图像是结果图像，传进来之前已经初始化，或者说已经分配内存，不是在这个函数内部分配内存
	void BuildUpwindGradientBasedFeatureImage(vtkImageData*originalData,vtkImageData* res);

	//originalData是原始图像,res图像是结果图像，传进来之前已经初始化，或者说已经分配内存，不是在这个函数内部分配内存
	void BuildGradientBasedFeatureImage(vtkImageData*originalData,vtkImageData* res);
};

#endif
