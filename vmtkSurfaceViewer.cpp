#include "vmtkSurfaceViewer.h"
#include "NullPointerException.h"
#include <vtkPolyDataMapper.h>
#include <vtkPointData.h>
#include <vtkCellData.h>
#include <vtkLookupTable.h>
#include <vtkScalarsToColors.h>
#include <vtkColorTransferFunction.h>
#include <vtkProperty.h>
vmtkSurfaceViewer::vmtkSurfaceViewer(vtkRenderer* grender)
{
	if(grender == 0)
	{
		throw NullPointerException("没有传入grender"); 
	}
	renderer = grender;
	DisplayCellData = 0;
	ScalarRange[0] = 0.0;
	ScalarRange[1] = 0.0;
	color[0] = -1.0;
	color[1] = -1.0;
	color[2] = -1.0;
	Opacity = 1.0;
	Linewidth = 1;
	Grayscale = 0;
	FlatInterpolation = 0;
	ColorMap = "cooltowarm";
	Display = 1;
	Representation = "surface";
	NumberOfColors = 256;
}

vmtkSurfaceViewer::~vmtkSurfaceViewer()
{

}

void vmtkSurfaceViewer::buildView()
{
    //当前的唯一渲染实例带着这Actor,这里看起来挺危险的
	
	log4cpp::Category& rootLog  = log4cpp::Category::getRoot();
	log4cpp::Category& subLog = log4cpp::Category::getInstance(std::string("sub1"));

	if(Surface == 0)
	{
		throw NullPointerException("vmtkSurfaceViewer::buildView(),surface不能为空指针需要传进来.");
	}

	if(Actor)
	{		
		renderer->RemoveActor(Actor);
		Actor->Delete();
	
	}else
	{
		throw NullPointerException("vmtkSurfaceViewer::buildView(),vtkActor不能为空指针需要传进来.");
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

			if(ScalarRange[1] > ScalarRange[0])
			{
				mapper->SetScalarRange(ScalarRange);
			}else
			{
				mapper->SetScalarRange(array->GetRange(0));
			}
			if(Grayscale)
			{
				vtkSmartPointer<vtkLookupTable> lut = vtkSmartPointer<vtkLookupTable>::New();
				lut->SetValueRange(0.0,1.0);
				lut->SetSaturationRange(0.0,0.0);
				mapper->SetLookupTable(lut);
			}
		}else
		{
			mapper->ScalarVisibilityOff();
		}
		rootLog.info("vmtkSurfaceViewer.cpp buildView() ColorMap" + ColorMap);
		subLog.info("vmtkSurfaceViewer.cpp buildView() ColorMap" + ColorMap);

		vtkLookupTable * lut;
		if(ColorMap == "grayscale")
		{ 
			
			lut = dynamic_cast<vtkLookupTable*>(mapper->GetLookupTable());
			lut->SetNumberOfTableValues(NumberOfColors);
			lut->SetValueRange(0.0,1.0);
			lut->SetSaturationRange(0.0,1.0);
			lut->Build();
			mapper->SetLookupTable(lut);
		}

		if(ColorMap == "rainbow")
		{
			lut = dynamic_cast<vtkLookupTable*>(mapper->GetLookupTable());
			lut->SetHueRange(0.666667,0.0);
			lut->SetSaturationRange(0.75,0.75);
			lut->SetValueRange(1.0,1.0);
			lut->SetAlphaRange(1.0,1.0);
			lut->SetNumberOfColors(NumberOfColors);
			lut->Build();
			mapper->SetLookupTable(lut); 
		}

		if(ColorMap == "blackbody")
		{
			lut = dynamic_cast<vtkLookupTable*>(mapper->GetLookupTable());
			lut->SetNumberOfColors(NumberOfColors);
			vtkSmartPointer<vtkColorTransferFunction> colorTransferFunction = vtkSmartPointer<vtkColorTransferFunction>::New();
			colorTransferFunction->SetColorSpaceToRGB();
			colorTransferFunction->AddRGBPoint(0,0.0,0.0,0.0);
			colorTransferFunction->AddRGBPoint(0.4,0.901961,0.0,0.0);
			colorTransferFunction->AddRGBPoint(0.8,0.901961,0.901961,0.0);
			colorTransferFunction->AddRGBPoint(1.0,1.0,1.0,1.0);
			vector<double> param = getEnumerateColorTransferFunctionParam(NumberOfColors);
			for(int i = 0;i < NumberOfColors;i ++)
			{
			  double* cc = colorTransferFunction->GetColor(param[i]);
			  lut->SetTableValue(i,cc[0],cc[1],cc[2],1.0);
			
			}
			lut->Build();
			mapper->SetLookupTable(lut);

		}
		if(ColorMap == "cooltowarm")
		{
			lut = dynamic_cast<vtkLookupTable*>(mapper->GetLookupTable());
			lut->SetNumberOfColors(NumberOfColors);
			vtkSmartPointer<vtkColorTransferFunction> colorTransferFunction = vtkSmartPointer<vtkColorTransferFunction>::New();
            colorTransferFunction->SetColorSpaceToDiverging();
            colorTransferFunction->AddRGBPoint(0,0.231373,0.298039,0.752941);
            colorTransferFunction->AddRGBPoint(0.5,0.865003,0.865003,0.865003);
            colorTransferFunction->AddRGBPoint(1.0,0.705882,0.0156863,0.14902);
			vector<double> param = getEnumerateColorTransferFunctionParam(NumberOfColors);
			for(int i = 0;i < NumberOfColors;i ++)
			{
			  double* cc = colorTransferFunction->GetColor(param[i]);
			  lut->SetTableValue(i,cc[0],cc[1],cc[2],1.0);
			
			}
			lut->Build();
			mapper->SetLookupTable(lut);
		}

		Actor = vtkActor::New();
		Actor->SetMapper(mapper);
		if(color[0] >= 0.0)
			Actor->GetProperty()->SetColor(color);
		Actor->GetProperty()->SetOpacity(Opacity);
		Actor->GetProperty()->SetLineWidth(Linewidth);
		if(FlatInterpolation)
		{
		   Actor->GetProperty()->SetInterpolationToFlat();
		}
		SetSurfaceRepresentation(Representation);
		renderer->AddActor(Actor);
		if(Display)
		{
			renderer->Render();
		}



	}


	
}

vector<double> vmtkSurfaceViewer::getEnumerateColorTransferFunctionParam(int NumberOfColors)
{
	vector<double> res;
	for(int i = 0; i < NumberOfColors; i++)
	{
		res.push_back((i*1.0)/NumberOfColors);
	}


	return res;
}

void vmtkSurfaceViewer::SetSurfaceRepresentation(string representation)
{
	if(representation == "surface")
	{
		  Actor->GetProperty()->SetRepresentationToSurface();
          Actor->GetProperty()->EdgeVisibilityOff();
	}else if(representation == "edges")
	{
		  Actor->GetProperty()->SetRepresentationToSurface();
          Actor->GetProperty()->EdgeVisibilityOn();
	}else if(representation == "wireframe")
	{
		Actor->GetProperty()->SetRepresentationToWireframe();
        Actor->GetProperty()->EdgeVisibilityOff();
	}
	Representation = representation;

}