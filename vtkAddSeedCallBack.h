#ifndef VTK_ADD_SEED_CALL_BACK_
#define VTK_ADD_SEED_CALL_BACK_
#include <vtkCommand.h>
#include <vtkImageData.h>
#include <vtkImagePlaneWidget.h>
#include <vtkPolyData.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
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
		if(RenderWindowInteractor->GetControlKey() == 0)
		{
			return;
		}
		double cursorData[4] = {0.0,0.0,0.0,0.0};
		vtkImagePlaneWidget* obj = dynamic_cast<vtkImagePlaneWidget*>(caller);
		obj->GetCursorData(cursorData);
		double spacing[3];
		double origin[3];
		Image->GetSpacing(spacing);
		Image->GetOrigin(origin);
		double point[3]={0.0,0.0,0.0};
		point[0] = cursorData[0]*spacing[0]+origin[0];
		point[1] = cursorData[1]*spacing[1]+origin[1];
		point[2] = cursorData[2]*spacing[2]+origin[2];
		Seeds->GetPoints()->InsertNextPoint(point);
		Seeds->Modified();
		renderer->Render();
		
		
	}
	void setImage(vtkImageData*localImage)
	{
		Image = localImage;
	}
	void setSeeds(vtkPolyData*localSeeds)
	{
		Seeds = localSeeds;
	}
	void setRenderer(vtkRenderer* localRender)
	{
		renderer = localRender;
	}

	void setRenderWindowInteractor(vtkRenderWindowInteractor* interactor)
	{
		RenderWindowInteractor = interactor;
	}
protected:
	vtkAddSeedCallBack()
	{
		
	}
private:
	/*
	*需要调用者传入
	*/
	vtkImageData* Image;
	/*
	*需要调用者传入
	*/
	vtkPolyData* Seeds;

	/*
	*需要调用者传入
	*/
	vtkRenderer* renderer;

	/**
	*需要调用者传入
	*/

	vtkRenderWindowInteractor*RenderWindowInteractor;

};



#endif