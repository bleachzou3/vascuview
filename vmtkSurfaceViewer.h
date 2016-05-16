#ifndef VMTK_SURFACE_VIEWER_HPP_
#define VMTK_SURFACE_VIEWER_HPP_

#include <vtkRenderer.h>
#include <vtkActor.h>
#include <vtkImageData.h>
#include <vtkPolyData.h>
#include <string>
class vmtkSurfaceViewer
{
public:
	vmtkSurfaceViewer(vtkRenderer* grenderer);
	virtual ~vmtkSurfaceViewer();
	void setRenderer();
	void buildViewWithTag();
	void buildView();
private:
	//renderer从外面传进来的实例，不是本类方法中形成的
	vtkRenderer* renderer;

	//Actor，是本类方法中形成的
	vtkPolyData * Surface;

	//这里面有一个Actor是在这里实例化的
	vtkActor* Actor;

    std::string ArrayName;

	int DisplayCellData;

	double ScalarRange[2];
};

#endif