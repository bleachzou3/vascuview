#ifndef VTK_HIDE_CUBE_CALLBACK_
#define VTK_HIDE_CUBE_CALLBACK_
#include "vtkCommand.h"
#include "vtkObject.h"
#include "vtkBoxWidget.h"
#include "vtkActor.h"
#include "vtkCubeSource.h"
#include <log4cpp/Category.hh>
#include <log4cpp/PropertyConfigurator.hh>
/*
*��ʹ��������ʱ��һ��Ҫ��cubeActor������
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
		log4cpp::Category& rootLog  = log4cpp::Category::getRoot();
	
	log4cpp::Category& subLog = log4cpp::Category::getInstance(std::string("sub1"));
		
		if(ev == vtkCommand::DisableEvent||ev == vtkCommand::StartInteractionEvent)
		{
			if(cubeActor != 0)
			{
				cubeActor->VisibilityOff();
			}else
			{
				rootLog.error("vtkHideCubeCallback:cubeActorû������");
				subLog.error("vtkHideCubeCallback:cubeActorû������");
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