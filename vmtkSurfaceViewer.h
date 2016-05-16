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
	//renderer�����洫������ʵ�������Ǳ��෽�����γɵ�
	vtkRenderer* renderer;

	//Actor���Ǳ��෽�����γɵ�
	vtkPolyData * Surface;

	//��������һ��Actor��������ʵ������
	vtkActor* Actor;

    std::string ArrayName;

	int DisplayCellData;

	double ScalarRange[2];
};

#endif