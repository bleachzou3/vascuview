#ifndef VMTK_IMAGE_FEATURES_CPP_
#define VMTK_IMAGE_FEATURES_CPP_
using namespace std;
#include <vector>
#include <vtkImageData.h>
#include <vtkImageCast.h>
#include <vtkSmartPointer.h>
#include <vtkImageGradientMagnitude.h>
#include <vtkImageMathematics.h>
#include <vtkObjectBase.h>
#include "Uncopyable.h"
enum FeatureImageType
{
	VTKGRADIENT,GRADIENT,UPWIND,FWHM
};
class vmtkImageFeature:public  vtkObjectBase
{
private:
  int Dimensionality;
  double DerivativeSigma;
  int SigmoidRemapping;
 
  double FWHMRadius[3];
  
  double FWHMBackgroundValue;

  double UpwindFactor;
protected:
	vmtkImageFeature();
	virtual ~vmtkImageFeature();
public:

	

	static vmtkImageFeature* New();
	vtkTypeMacro(vmtkImageFeature,vtkObjectBase);
	//originalData是原始图像,res图像是结果图像，传进来之前已经初始化，或者说已经分配内存，不是在这个函数内部分配内存
	void BuildVTKGradientBasedFeatureImage(vtkImageData*originalData,vtkImageData* res);


	//originalData是原始图像,res图像是结果图像，传进来之前已经初始化，或者说已经分配内存，不是在这个函数内部分配内存
	void BuildFWHMBasedFeatureImage(vtkImageData*originalData,vtkImageData* res);


	//originalData是原始图像,res图像是结果图像，传进来之前已经初始化，或者说已经分配内存，不是在这个函数内部分配内存
	void BuildUpwindGradientBasedFeatureImage(vtkImageData*originalData,vtkImageData* res);

	//originalData是原始图像,res图像是结果图像，传进来之前已经初始化，或者说已经分配内存，不是在这个函数内部分配内存
	void BuildGradientBasedFeatureImage(vtkImageData*originalData,vtkImageData* res);

	//
	void setSigmoidRemapping(int _sigmoidRemapping);
    
	//
	void setDerivativeSigma(double _DerivativeSigma);

	void setUpwindFactor(double _upwindFactor);

	void setFWHMRadius(double _FWHMRadius[3]);

	void setFWHMBackgroundValue(double _FWHMBackgroundValue);
};

#endif
