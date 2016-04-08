#ifndef VASCUVIEW_UTILITY_HPP_
#define VASCUVIEW_UITILITY_HPP
#include <vtkImageData.h>
#include <vector>
using namespace std;
class VascuviewUtility
{
public:
	VascuviewUtility();

	~VascuviewUtility(); 

public:
	static vtkImageData* extractVoi(vtkImageData*data,vector<double>&BoxBounds);
};
#endif