#ifndef VTK_HIDE_CUBE_CALLBACK_
#define VTK_HIDE_CUBE_CALLBACK_
#include "vtkCommand.h"
#include "vtkObject.h"
#include "vtkBoxWidget.h"
#include "vtkActor.h"
#include "vtkCubeSource.h"

/*
*在使用这个类的时候，一定要把cubeActor传进来
*/
class vtkHideCubeCallback:public vtkCommand
{
public:

	static vtkHideCubeCallback* New()
	{
		return new vtkHideCubeCallback;
	}

	virtual void Execute(vtkObject *caller, unsigned long ev,
                void *callData)
	{
	
		
		if(ev == vtkCommand::DisableEvent||ev == vtkCommand::StartInteractionEvent)
		{
			if(cubeActor != 0)
			{
				cubeActor->VisibilityOff();
			}
		}

	}
	void setObject(vtkActor* ca)
	{
		cubeActor = ca;
	}
protected:
	vtkHideCubeCallback(){};
private:
	
	vtkActor* cubeActor;



};
#endif