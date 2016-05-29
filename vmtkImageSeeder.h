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
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include "Uncopyable.h"
class vmtkImageSeeder:public vtkObjectBase
{
public:
	vtkTypeMacro(vmtkImageSeeder,vtkObjectBase);
	static vmtkImageSeeder* New();
protected:
	vmtkImageSeeder();
	~vmtkImageSeeder();
public:
	void execute();
private:
	//�ⲿ�����ʵ��
	vtkImageData*Image;

	//�ⲿ�����ʵ��
	vtkRenderer*Renderer;

	//�ⲿ�����plane
	vtkImagePlaneWidget*planes[3];

	int Display;

	int KeepSeeds;

    /**
	*���ഴ��
	*/
	vtkSmartPointer<vtkCellPicker> Picker;

	/**
	*���ഴ��
	*/
	vtkSmartPointer<vtkPolyData> Seeds;

	/**
	*
	*/
	vtkSmartPointer<vtkActor> SeedActor;
	std::string ArrayName;

private:
	void InitializeSeeds();



	void WidgetsOn();

	void WidgetsOff();
public:
	void BuildView();

	void setRenderer(vtkRenderer* renderer);

	void setImage(vtkImageData*_image);

	void setDisplay(int display);

	

	

};

#endif