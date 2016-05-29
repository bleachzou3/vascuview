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

	//Image外面传进来
	vtkImageData*Image;

   //Image外面传进来
	vtkRenderer* Renderer;
	
	//ImageSeeder外面传进来
	vmtkImageSeeder* ImageSeeder;


	//SurfaceViewer外面传进来
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