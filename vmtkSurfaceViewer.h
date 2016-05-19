#ifndef VMTK_SURFACE_VIEWER_HPP_
#define VMTK_SURFACE_VIEWER_HPP_
using namespace std;
#include <vtkRenderer.h>
#include <vtkActor.h>
#include <vtkImageData.h>
#include <vtkPolyData.h>
#include <string>
#include <log4cpp/Category.hh>
#include <log4cpp/PropertyConfigurator.hh>
class vmtkSurfaceViewer
{
public:
	vmtkSurfaceViewer(vtkRenderer* grenderer);
	virtual ~vmtkSurfaceViewer();
	void setRenderer();
	void buildViewWithTag();
	void buildView();
private:
	int FlatInterpolation;


	double color[3];
	//renderer从外面传进来的实例，不是本类方法中形成的
	vtkRenderer* renderer;

	//Actor，是本类方法中形成的
	vtkPolyData * Surface;

	//这里面有一个Actor是在这里实例化的,为什么选择在这里实例化,它有一条删除的语句
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

private:
	vector<double> getEnumerateColorTransferFunctionParam(int NumberOfColors);
	void SetSurfaceRepresentation(std::string Representation);

};

#endif