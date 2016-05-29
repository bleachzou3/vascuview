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
#include <vtkObjectBase.h>

class vmtkImageInitialization:public vtkObjectBase
{
public:
	vtkTypeMacro(vmtkImageInitialization,vtkObjectBase);
	static vmtkImageInitialization* New();
	void execute();
protected:
	vmtkImageInitialization();
	~vmtkImageInitialization();
private:

	//Image���洫����
	vtkImageData*Image;

   //Image���洫����
	vtkRenderer* Renderer;
	
	//ImageSeeder���洫����
	vmtkImageSeeder* ImageSeeder;


	//SurfaceViewer���洫����
	vmtkSurfaceViewer* SurfaceViewer;



	
	int Interactive;
	int NegateImage;
public:
	void setImageData(vtkImageData*localImageData);
	void setRenderer(vtkRenderer*_renderer);
	void setImageSeeder(vmtkImageSeeder* _imageSeeder);
	void setSurfaceViewer(vmtkSurfaceViewer* _surfaceViewer);
	void setNegateImage(int _negateImage);
};
#endif