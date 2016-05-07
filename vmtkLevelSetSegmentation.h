#ifndef VMTK_LEVEL_SET_SEGMENTATION_HPP_
#define VMTK_LEVEL_SET_SEGMENTATION_HPP_
#include <vtkImageData.h>
enum LevelSetType
{
	GEODESIC,CURVES,THRESHOLD,LAPLACIAN
};
class vmtkLevelSetSegmentation
{
public:

	vmtkLevelSetSegmentation();
	virtual~vmtkLevelSetSegmentation();
	//levelSetInput要处理的图像数据，levelSetOutput处理完之后的图像数据，levelSetOutput传进来之前已经分配内存
	void LevelSetEvolution(LevelSetType type,vtkImageData*levelSetInput,vtkImageData*levelSetOutput);
	
};
#endif