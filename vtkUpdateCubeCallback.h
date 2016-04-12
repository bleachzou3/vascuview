#ifndef VTK_UPDATE_CUBE_CALLBACK_
#define VTK_UPDATE_CUBE_CALLBACK_
#include "vtkCommand.h"
#include "vtkObject.h"
#include "vtkPolyData.h"
#include "vtkSmartPointer.h"
#include "vtkBoxWidget.h"
#include "vtkCubeSource.h"
#include "vtkActor.h"
#include <log4cpp/Category.hh>
#include <log4cpp/PropertyConfigurator.hh>
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
	log4cpp::Category& rootLog  = log4cpp::Category::getRoot();
	
	log4cpp::Category& subLog = log4cpp::Category::getInstance(std::string("sub1"));
	
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

				}else
				{
			        rootLog.error("vtkUpdateCubeCallback：cubeSource没有设置");
					subLog.error("vtkUpdateCubeCallback：cubeSource没有设置");
				}

				if(cubeActor != 0)
				{
					cubeActor->VisibilityOn();
				}else
				{
					rootLog.error("vtkUpdateCubeCallback：cubeActor没有设置");
					subLog.error("vtkUpdateCubeCallback：cubeActor没有设置");
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