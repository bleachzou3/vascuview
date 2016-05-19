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
	//renderer�����洫������ʵ�������Ǳ��෽�����γɵ�
	vtkRenderer* renderer;

	//Actor���Ǳ��෽�����γɵ�
	vtkPolyData * Surface;

	//��������һ��Actor��������ʵ������,Ϊʲôѡ��������ʵ����,����һ��ɾ�������
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