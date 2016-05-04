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
  vector<int> FWHMRadius;
  
  double FWHMBackgroundValue;
public:
	vmtkImageFeature();
	virtual ~vmtkImageFeature();
	void BuildVTKGradientBasedFeatureImage(vtkImageData*originalData,vtkImageData* res);
	
};

#endif
