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
#include <qstring.h>
#include <vtkMarchingCubes.h>
enum ImageInitializationType
{
	COLLIDINGFRONTS,FASTMARCHING,THRESHOLD,ISOSURFACE,SEEDS
};
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
	//本类创建
	vtkPolyData*Surface;

	//Image外面传进来
	vtkImageData*Image;

   //Image外面传进来
	vtkRenderer* Renderer;
	
	//ImageSeeder外面传进来
	vmtkImageSeeder* ImageSeeder;


	//SurfaceViewer外面传进来
	vmtkSurfaceViewer* SurfaceViewer;

	//本类创建
	vtkImageData* InitialLevelSets;


	//本类创建的内存
	vtkImageData*MergedInitialLevelSets;

	double IsoSurfaceValue;



	int  SourcePoints[3];
	int  TargetPoints[3];
	int Interactive;
	int NegateImage;

	double LowerThreshold;
	double UpperThreshold;

	ImageInitializationType Method;
public:
	void setImageData(vtkImageData*localImageData);
	void setRenderer(vtkRenderer*_renderer);
	void setImageSeeder(vmtkImageSeeder* _imageSeeder);
	void setSurfaceViewer(vmtkSurfaceViewer* _surfaceViewer);
	void setNegateImage(int _negateImage);
	void CollidingFrontsInitialize();
	void MergeLevelSets();

	vtkImageData* getInitialLevelSets();

	void setInitialLevelSets(vtkImageData* _image);
private:
	vtkSmartPointer<vtkPolyData> SeedInput(const QString & message,int numberOfSeeds);

	void DisplayLevelSetSurface(vtkImageData*levelSets);
	
};
#endif