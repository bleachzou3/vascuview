#include "VascuviewUtility.h"
#include "vtkImageData.h"
#include <vtkSmartPointer.h>
#include <vtkExtractVOI.h>
#include <algorithm>
vtkImageData* VascuviewUtility::extractVoi(vtkImageData*data,vector<double>&BoxBounds)
{
	cout << data->GetReferenceCount()<<endl;
	int wholeExtent[6];
	data->GetExtent(wholeExtent);
	double origin[3];
	double spacing[3];
	data->GetOrigin(origin);
	data->GetSpacing(spacing);

	int newVOI[6];
	newVOI[0] = max(wholeExtent[0],int(ceil((BoxBounds[0]-origin[0])/spacing[0])));
    newVOI[1] = min(wholeExtent[1],int(floor((BoxBounds[1]-origin[0])/spacing[0])));
    newVOI[2] = max(wholeExtent[2],int(ceil((BoxBounds[2]-origin[1])/spacing[1])));
    newVOI[3] = min(wholeExtent[3],int(floor((BoxBounds[3]-origin[1])/spacing[1])));
    newVOI[4] = max(wholeExtent[4],int(ceil((BoxBounds[4]-origin[2])/spacing[2])));
    newVOI[5] = min(wholeExtent[5],int(floor((BoxBounds[5]-origin[2])/spacing[2])));

	vtkSmartPointer<vtkExtractVOI> eVoi = vtkSmartPointer<vtkExtractVOI>::New();
	eVoi->SetInputData(data);
	eVoi->SetVOI(newVOI);
	eVoi->Update();
    


	

	
	//vtkImageData* result = vtkImageData::New();
	return eVoi->GetOutput();

}