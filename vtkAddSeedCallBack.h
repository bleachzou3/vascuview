#ifndef VTK_ADD_SEED_CALL_BACK_
#define VTK_ADD_SEED_CALL_BACK_
#include <vtkCommand.h>
#include <vtkImageData.h>
class vtkAddSeedCallBack:public vtkCommand
{
public:
	static vtkAddSeedCallBack* New()
	{
		return new vtkAddSeedCallBack;
	}
	virtual void Execute(vtkObject *caller, unsigned long ev,
                void *callData)
	{

	}
protected:
	vtkAddSeedCallBack()
	{

	}
private:
	vtkImageData* Image;
};



#endif