#ifndef VTK_UPDATE_CUBE_CALLBACK_
#define VTK_UPDATE_CUBE_CALLBACK_
#include "vtkCommand.h"
#include "vtkObject.h"
#include "vtkPolyData.h"
#include "vtkSmartPointer.h"
#include "vtkBoxWidget.h"
#include "vtkCubeSource.h"
#include "vtkActor.h"
class vtkUpdateCubeCallback:public vtkCommand
{
public:
	static vtkUpdateCubeCallback* New()
	{
		return new vtkUpdateCubeCallback;
	}

	virtual void Execute(vtkObject *caller, unsigned long ev,
                void *callData)
	{
		
		if( ev == vtkCommand::EndInteractionEvent|| ev == vtkCommand::EnableEvent)
		{
			vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
			vtkBoxWidget* boxWidget = dynamic_cast<vtkBoxWidget*>(caller);
			if(boxWidget != 0)
			{
				boxWidget->GetPolyData(polyData);
				polyData->ComputeBounds();
				if(cubeSource != 0 )
				{
					cubeSource->SetBounds(polyData->GetBounds());
					cubeSource->Modified();

				}

				if(cubeActor != 0)
				{
					cubeActor->VisibilityOn();
				}
			}
			
		}
		

	}
	void setCubeActor( vtkActor* va)
	{
		cubeActor = va;
	}
	void setCubeSource(vtkCubeSource* cs)
	{
		cubeSource = cs;
	}
protected:
	vtkUpdateCubeCallback()
	{

	}
private:
	vtkCubeSource * cubeSource;
	vtkActor* cubeActor;
};

#endif