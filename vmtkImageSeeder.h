#ifndef VMTK_IMAGE_SEEDER_HPP_
#define VMTK_IMAGE_SEEDER_HPP_
#include <vtkImageData.h>
#include <vtkRenderer.h>
#include <vtkImagePlaneWidget.h>
#include <vtkSmartPointer.h>
#include <vtkCellPicker.h>
#include <log4cpp/Category.hh>
#include <log4cpp/PropertyConfigurator.hh>
#include <vtkAddSeedCallBack.h>
class vmtkImageSeeder
{
public:
	vmtkImageSeeder();
	~vmtkImageSeeder();
public:
	void execute();
private:
	//外部传入的实例
	vtkImageData*Image;

	//外部传入的实例
	vtkRenderer*Renderer;

	//外部传入的plane
	vtkImagePlaneWidget*planes[3];

	int Display;
	

};

#endif