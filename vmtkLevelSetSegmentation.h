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
	//levelSetInputҪ�����ͼ�����ݣ�levelSetOutput������֮���ͼ�����ݣ�levelSetOutput������֮ǰ�Ѿ������ڴ�
	void LevelSetEvolution(LevelSetType type,vtkImageData*levelSetInput,vtkImageData*levelSetOutput);
	
};
#endif