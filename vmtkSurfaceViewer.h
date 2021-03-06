#ifndef VMTK_SURFACE_VIEWER_HPP_
#define VMTK_SURFACE_VIEWER_HPP_
using namespace std;
#include <vtkRenderer.h>
#include <vtkActor.h>
#include <vtkImageData.h>
#include <vtkPolyData.h>
#include <string>
#include "Uncopyable.h"
#include <log4cpp/Category.hh>
#include <log4cpp/PropertyConfigurator.hh>
class vmtkSurfaceViewer:public vtkObjectBase
{
public:
	static vmtkSurfaceViewer* New();
	void setRenderer(vtkRenderer*_renderer);
	void buildViewWithTag();
	void buildView();
	void setSurface(vtkPolyData* _Surface);
	void setDisplay(int _display);
	void setOpacity(double _opacity);
protected:
	virtual ~vmtkSurfaceViewer();
	vmtkSurfaceViewer();
private:
	int FlatInterpolation;


	double color[3];
	//renderer从外面传进来的实例，不是本类方法中形成的
	vtkRenderer* renderer;

	//外面传进来的
	vtkPolyData * Surface;

	//这里面Actor是在这里实例化的,为什么选择在这里实例化,它有一条删除的语句
	vtkActor* Actor;

    std::string ArrayName;

	int DisplayCellData;

	double ScalarRange[2];

	int Grayscale;

	std::string ColorMap;

	int NumberOfColors;

	double Opacity;

	double Linewidth;

	std::string Representation;

	int Display;

	std::string RegionTagArrayName;

	std::map<double,int> TagSet;

	int NumberOfRegions;

private:
	vector<double> getEnumerateColorTransferFunctionParam(int NumberOfColors);
	void SetSurfaceRepresentation(std::string Representation);

	

};

#endif