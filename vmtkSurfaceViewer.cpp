#include "vmtkSurfaceViewer.h"
#include "NullPointerException.h"
#include <vtkPolyDataMapper.h>
#include <vtkPointData.h>
#include <vtkCellData.h>
vmtkSurfaceViewer::vmtkSurfaceViewer(vtkRenderer* grender)
{
	if(grender == 0)
	{
		throw NullPointerException("没有传入grender"); 
	}
	renderer = grender;
	DisplayCellData = 0;
}

vmtkSurfaceViewer::~vmtkSurfaceViewer()
{

}

void vmtkSurfaceViewer::buildView()
{
    //当前的唯一渲染实例带着这Actor,这里看起来挺危险的
	if(Surface == 0)
	{
		throw NullPointerException("vmtkSurfaceViewer::buildView(),surface不能为空指针需要传进来.");
	}

	if(Actor)
	{		
		renderer->RemoveActor(Actor);
		Actor->Delete();
	
	}

	if(Surface)
	{
		vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
		mapper->SetInputData(Surface);
		vtkSmartPointer<vtkDataArray> array;
		if(ArrayName != "")
		{
			if(DisplayCellData == 0)
			{
				Surface->GetPointData()->SetActiveScalars(ArrayName.c_str());
				array = Surface->GetPointData()->GetScalars();
				
			}else
			{
				Surface->GetCellData()->SetActiveScalars(ArrayName.c_str());
				array = Surface->GetCellData()->GetScalars();
				mapper->SetScalarModeToUseCellData();
			}
			
		}
	}


	
}