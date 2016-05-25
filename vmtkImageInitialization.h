#ifndef VMTK_IMAGE_INITIALIZATION_HPP
#define VMTK_IMAGE_INITIALIZATION_HPP
#include "vmtkImageSeeder.h"
#include "vmtkSurfaceViewer.h"
#include <vtkImageData.h>
#include <log4cpp/Category.hh>
#include <log4cpp/PropertyConfigurator.hh>
#include <vtkImageCast.h>
#include <vtkSmartPointer.h>
#include <vtkImageMathematics.h>
#include <vtkImageShiftScale.h>
#include <vtkRenderer.h>
class vmtkImageInitialization
{
public:
	vmtkImageInitialization();
	~vmtkImageInitialization();
	void execute();
private:
	vtkImageData*Image;
	vtkRenderer* Renderer;
	
	vmtkImageSeeder* ImageSeeder;
	vmtkSurfaceViewer* SurfaceViewer;

	
	int Interactive;
	int NegateImage;
public:
	void setImageData(vtkImageData*localImageData);
};
#endif