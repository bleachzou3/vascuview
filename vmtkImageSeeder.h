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
#include <string>
#include <vtkPointData.h>
#include <vtkGlyph3D.h>
#include "Uncopyable.h"
class vmtkImageSeeder:private Uncopyable
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

    /**
	*本类创建
	*/
	vtkSmartPointer<vtkCellPicker> Picker;

	/**
	*本类创建
	*/
	vtkSmartPointer<vtkPolyData> Seeds;

	std::string ArrayName;

private:
	void InitializeSeeds();

	void BuildView();

	

};

#endif