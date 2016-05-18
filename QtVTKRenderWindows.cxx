#include "ui_QtVTKRenderWindows.h"
#include "QtVTKRenderWindows.h"

#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include "vtkResliceImageViewer.h"
#include "vtkResliceCursorLineRepresentation.h"
#include "vtkResliceCursorThickLineRepresentation.h"
#include "vtkResliceCursorWidget.h"
#include "vtkResliceCursorActor.h"
#include "vtkResliceCursorPolyDataAlgorithm.h"
#include "vtkResliceCursor.h"
#include "vtkDICOMImageReader.h"
#include "vtkCellPicker.h"
#include "vtkProperty.h"
#include "vtkPlane.h"
#include "vtkImageData.h"
#include "vtkCommand.h"
#include "vtkPlaneSource.h"
#include "vtkLookupTable.h"
#include "vtkImageMapToWindowLevelColors.h"
#include "vtkInteractorStyleImage.h"
#include "vtkImageSlabReslice.h"
#include "vtkBoundedPlanePointPlacer.h"
#include "vtkDistanceWidget.h"
#include "vtkDistanceRepresentation.h"
#include "vtkHandleRepresentation.h"
#include "vtkResliceImageViewerMeasurements.h"
#include "vtkDistanceRepresentation2D.h"
#include "vtkPointHandleRepresentation3D.h"
#include "vtkPointHandleRepresentation2D.h"
#include "qfiledialog.h"
#include  "vtkLight.h"
#include "vtkHideCubeCallback.h"
#include "vtkUpdateCubeCallback.h"
#include <string>
#include "vtkResliceCursorCallback.h"
#include "VascuviewUtility.h"
#include "vtkXMLImageDataWriter.h"
#include "vtkDataObject.h"
//----------------------------------------------------------------------------


QtVTKRenderWindows::QtVTKRenderWindows( ):flag(false),boxWidgetOn(false)
{
  this->ui = new Ui_QtVTKRenderWindows;
  this->ui->setupUi(this);

  init3DWidget();
  connectActions();
 



};



void QtVTKRenderWindows::init3DWidget()
{

	 reader =  vtkSmartPointer< vtkDICOMImageReader >::New();
	 readerVti = vtkSmartPointer<vtkXMLImageDataReader>::New();
	 cubeActor = vtkSmartPointer<vtkActor>::New();
	 cubeSource = vtkSmartPointer<vtkCubeSource>::New();
}

 void QtVTKRenderWindows::openDirectoryDicom()
{

	/*
	if(flag)
	{
		return;
	}
	*/
	  
    
		    QFileDialog::Options options = QFileDialog::DontResolveSymlinks | QFileDialog::ShowDirsOnly;  
    options |= QFileDialog::DontUseNativeDialog;  
    QString directory = QFileDialog::getExistingDirectory(this,  
                                    tr("Open Directory"),  
                                    "/home",  
                                    options);  

	//如果没有选择目录就为空
	if(directory.isEmpty())
	{
		return;
	}
	flag = true;
	readerFlag = 0;
	this->ui->statusBar->showMessage(directory);
	






	//---------------------------------------
	//this->ui->view4->GetRenderWindow()->ClearInRenderStatus();
	//  vtkSmartPointer< vtkDICOMImageReader > reader =
    //vtkSmartPointer< vtkDICOMImageReader >::New();
  //reader->SetDirectoryName("E:/ZHANG_XIANGJU");
	  
  reader->SetDirectoryName(directory.toStdString().c_str());
  reader->Update();


  int imageDims[3];
  reader->GetOutput()->GetDimensions(imageDims);

  //记得删除
  //---------------------
  //if(planeWidget[0] != 0)
   // cout << planeWidget[0]->GetReferenceCount()<<endl;
  //-----------------------------------

  for (int i = 0; i < 3; i++)
    {
		

		   if(riw[i] == 0)
		   {
             riw[i] = vtkSmartPointer< vtkResliceImageViewer >::New();
		   }
    }

 
  this->ui->view1->SetRenderWindow(riw[0]->GetRenderWindow());
  riw[0]->SetupInteractor(
      this->ui->view1->GetRenderWindow()->GetInteractor());

  this->ui->view2->SetRenderWindow(riw[1]->GetRenderWindow());
  riw[1]->SetupInteractor(
      this->ui->view2->GetRenderWindow()->GetInteractor());

  this->ui->view3->SetRenderWindow(riw[2]->GetRenderWindow());
  riw[2]->SetupInteractor(
      this->ui->view3->GetRenderWindow()->GetInteractor());

  for (int i = 0; i < 3; i++)
    {
    // make them all share the same reslice cursor object.
    vtkResliceCursorLineRepresentation *rep =
      vtkResliceCursorLineRepresentation::SafeDownCast(
          riw[i]->GetResliceCursorWidget()->GetRepresentation());
    riw[i]->SetResliceCursor(riw[0]->GetResliceCursor());

    rep->GetResliceCursorActor()->
      GetCursorAlgorithm()->SetReslicePlaneNormal(i);

    riw[i]->SetInputData(reader->GetOutput());
    riw[i]->SetSliceOrientation(i);
    riw[i]->SetResliceModeToAxisAligned();
	
    }

  vtkSmartPointer<vtkCellPicker> picker =
    vtkSmartPointer<vtkCellPicker>::New();
  picker->SetTolerance(0.005);

  vtkSmartPointer<vtkProperty> ipwProp =
    vtkSmartPointer<vtkProperty>::New();

  vtkSmartPointer< vtkRenderer > ren =
    vtkSmartPointer< vtkRenderer >::New();

 //自己后期添加，想试着在同一个QTVTKrenderer实例中载入不同的图片。
  vtkSmartPointer<vtkRenderWindow> new1 = vtkSmartPointer<vtkRenderWindow>::New();
  this->ui->view4->SetRenderWindow(new1);

  if(boxWidget == 0)
  {
	  boxWidget = vtkSmartPointer<vtkBoxWidget>::New();
  }
  //-----------------------------------------------------------------



   this->ui->view4->GetRenderWindow()->AddRenderer(ren);
  vtkRenderWindowInteractor *iren = this->ui->view4->GetInteractor();
  boxWidget->SetInteractor(iren);


  for (int i = 0; i < 3; i++)
    {
      if(planeWidget[i] == 0)
	  {	
	     planeWidget[i] = vtkSmartPointer<vtkImagePlaneWidget>::New();
		 // planeWidget[i] = vtkSmartPointer<vtkvmtkImagePlaneWidget>::New();
	  }
	
	  
		planeWidget[i]->SetInteractor( iren );
	
		planeWidget[i]->SetPicker(picker);
		planeWidget[i]->RestrictPlaneToVolumeOn();
		double color[3] = {0, 0, 0};
		color[i] = 1;
		planeWidget[i]->GetPlaneProperty()->SetColor(color);

		color[0] /= 4.0;
		color[1] /= 4.0;
		color[2] /= 4.0;
		riw[i]->GetRenderer()->SetBackground( color );

		planeWidget[i]->SetTexturePlaneProperty(ipwProp);
		planeWidget[i]->TextureInterpolateOff();
		planeWidget[i]->SetResliceInterpolateToLinear();
		
		planeWidget[i]->SetInputConnection(reader->GetOutputPort());
		
		//planeWidget[i]->SetInputData(reader->GetOutput());
		planeWidget[i]->SetPlaneOrientation(i);
		planeWidget[i]->SetSliceIndex(imageDims[i]/2);
		planeWidget[i]->DisplayTextOn();
		  
		planeWidget[i]->SetDefaultRenderer(ren);
		  
		planeWidget[i]->SetWindowLevel(1358, -27);
		planeWidget[i]->On();
		planeWidget[i]->InteractionOn();
	  
    }

  vtkSmartPointer<vtkResliceCursorCallback> cbk =
    vtkSmartPointer<vtkResliceCursorCallback>::New();
 
  for (int i = 0; i < 3; i++)
    {
		
    cbk->IPW[i] = planeWidget[i];
	  
    cbk->RCW[i] = riw[i]->GetResliceCursorWidget();
    riw[i]->GetResliceCursorWidget()->AddObserver(
        vtkResliceCursorWidget::ResliceAxesChangedEvent, cbk );
    riw[i]->GetResliceCursorWidget()->AddObserver(
        vtkResliceCursorWidget::WindowLevelEvent, cbk );
    riw[i]->GetResliceCursorWidget()->AddObserver(
        vtkResliceCursorWidget::ResliceThicknessChangedEvent, cbk );
    riw[i]->GetResliceCursorWidget()->AddObserver(
        vtkResliceCursorWidget::ResetCursorEvent, cbk );
    riw[i]->GetInteractorStyle()->AddObserver(
        vtkCommand::WindowLevelEvent, cbk );

    // Make them all share the same color map.
    riw[i]->SetLookupTable(riw[0]->GetLookupTable());
    planeWidget[i]->GetColorMap()->SetLookupTable(riw[0]->GetLookupTable());
    //planeWidget[i]->GetColorMap()->SetInput(riw[i]->GetResliceCursorWidget()->GetResliceCursorRepresentation()->GetColorMap()->GetInput());
    planeWidget[i]->SetColorMap(riw[i]->GetResliceCursorWidget()->GetResliceCursorRepresentation()->GetColorMap());



    }




  this->ui->view1->show();
  this->ui->view2->show();
  this->ui->view3->show();
 
  //当重新打开一张图像的时候，这些按钮的可见性需要重新设置
  this->ui->clickedVtkBoxWidget->setVisible(true);
  this->ui->clickedVtkBoxWidget->setChecked(false);
  this->ui->croppedImageButton->setVisible(false);





  //释放上一次遗留的

  //这一句我加的

 // connectActions();
}


void QtVTKRenderWindows::connectActions()
{
	  // Set up action signals and slots
  connect(this->ui->actionExit, SIGNAL(triggered()), this, SLOT(slotExit()));
  connect(this->ui->resliceModeCheckBox, SIGNAL(stateChanged(int)), this, SLOT(resliceMode(int)));
  connect(this->ui->thickModeCheckBox, SIGNAL(stateChanged(int)), this, SLOT(thickMode(int)));
  this->ui->thickModeCheckBox->setEnabled(0);

  connect(this->ui->radioButton_Max, SIGNAL(pressed()), this, SLOT(SetBlendModeToMaxIP()));
  connect(this->ui->radioButton_Min, SIGNAL(pressed()), this, SLOT(SetBlendModeToMinIP()));
  connect(this->ui->radioButton_Mean, SIGNAL(pressed()), this, SLOT(SetBlendModeToMeanIP()));
  this->ui->blendModeGroupBox->setEnabled(0);

  connect(this->ui->resetButton, SIGNAL(pressed()), this, SLOT(ResetViews()));
  connect(this->ui->AddDistance1Button, SIGNAL(pressed()), this, SLOT(AddDistanceMeasurementToView1()));
  connect(this->ui->clickedVtkBoxWidget,SIGNAL(clicked(bool)),this,SLOT(IsShowBoxWidget(bool)));
  connect(this->ui->actionOpenDicomDirectory,SIGNAL(triggered()),this,SLOT(openDirectoryDicom()));
  connect(this->ui->croppedImageButton,SIGNAL(clicked()),this,SLOT(croppedImageActivity()));
  connect(this->ui->actionActionOpenVti,SIGNAL(triggered()),this,SLOT(openVtiDicom()));

}
void QtVTKRenderWindows::slotExit()
{
  qApp->exit();
}

void QtVTKRenderWindows::resliceMode(int mode)
{
  this->ui->thickModeCheckBox->setEnabled(mode ? 1 : 0);
  this->ui->blendModeGroupBox->setEnabled(mode ? 1 : 0);

  for (int i = 0; i < 3; i++)
    {
    riw[i]->SetResliceMode(mode ? 1 : 0);
    riw[i]->GetRenderer()->ResetCamera();
    riw[i]->Render();
    }
}

void QtVTKRenderWindows::thickMode(int mode)
{
  for (int i = 0; i < 3; i++)
    {
    riw[i]->SetThickMode(mode ? 1 : 0);
    riw[i]->Render();
    }
}

void QtVTKRenderWindows::SetBlendMode(int m)
{
  for (int i = 0; i < 3; i++)
    {
    vtkImageSlabReslice *thickSlabReslice = vtkImageSlabReslice::SafeDownCast(
        vtkResliceCursorThickLineRepresentation::SafeDownCast(
          riw[i]->GetResliceCursorWidget()->GetRepresentation())->GetReslice());
    thickSlabReslice->SetBlendMode(m);
    riw[i]->Render();
    }
}

void QtVTKRenderWindows::SetBlendModeToMaxIP()
{
  this->SetBlendMode(VTK_IMAGE_SLAB_MAX);
}

void QtVTKRenderWindows::SetBlendModeToMinIP()
{
  this->SetBlendMode(VTK_IMAGE_SLAB_MIN);
}

void QtVTKRenderWindows::SetBlendModeToMeanIP()
{
  this->SetBlendMode(VTK_IMAGE_SLAB_MEAN);
}

void QtVTKRenderWindows::ResetViews()
{
	/*
  // Reset the reslice image views
  for (int i = 0; i < 3; i++)
    {
    riw[i]->Reset();
    }

  // Also sync the Image plane widget on the 3D top right view with any
  // changes to the reslice cursor.
  for (int i = 0; i < 3; i++)
    {
    vtkPlaneSource *ps = static_cast< vtkPlaneSource * >(
        planeWidget[i]->GetPolyDataAlgorithm());
    ps->SetNormal(riw[0]->GetResliceCursor()->GetPlane(i)->GetNormal());
    ps->SetCenter(riw[0]->GetResliceCursor()->GetPlane(i)->GetOrigin());

    // If the reslice plane has modified, update it on the 3D widget
    this->planeWidget[i]->UpdatePlacement();
    }

  // Render in response to changes.
  this->Render();
  */
}

void QtVTKRenderWindows::Render()
{
  for (int i = 0; i < 3; i++)
    {
    riw[i]->Render();
    }
  this->ui->view3->GetRenderWindow()->Render();
}

void QtVTKRenderWindows::AddDistanceMeasurementToView1()
{
  this->AddDistanceMeasurementToView(1);
}


void QtVTKRenderWindows::IsShowBoxWidget(bool visible)
{   
	/*
	*
	*/
	//flag代表有没有图像读进来
	if(!flag)
	{
		return;
	}
	if(visible)
	{
		boxWidgetOn = true;

		boxWidget->SetPriority(2);
		boxWidget->SetHandleSize(5E-3);
		if(readerFlag == 0)
		{
		  boxWidget->SetInputConnection(reader->GetOutputPort());
		}else if(readerFlag == 1)
		{
		   boxWidget->SetInputConnection(readerVti->GetOutputPort());
		}
		boxWidget->PlaceWidget();
		boxWidget->RotationEnabledOff();
		vtkSmartPointer<vtkHideCubeCallback> hcc = vtkSmartPointer<vtkHideCubeCallback>::New();
		hcc->setObject(cubeActor);
		vtkSmartPointer<vtkUpdateCubeCallback> ucc = vtkSmartPointer<vtkUpdateCubeCallback>::New();
		ucc->setCubeActor(cubeActor);
		ucc->setCubeSource(cubeSource);

		boxWidget->AddObserver(vtkCommand::StartInteractionEvent,hcc);
		boxWidget->AddObserver(vtkCommand::EndInteractionEvent,ucc);
		boxWidget->AddObserver(vtkCommand::EnableEvent,ucc);
		boxWidget->AddObserver(vtkCommand::DisableEvent,hcc);
	

		boxWidget->On();
	}else
	{
		boxWidgetOn = false;

		boxWidget->Off();
		
	}
}
void QtVTKRenderWindows::AddDistanceMeasurementToView(int i)
{
  // remove existing widgets.
  if (this->DistanceWidget[i])
    {
    this->DistanceWidget[i]->SetEnabled(0);
    this->DistanceWidget[i] = NULL;
    }

  // add new widget
  this->DistanceWidget[i] = vtkSmartPointer< vtkDistanceWidget >::New();
  this->DistanceWidget[i]->SetInteractor(
    this->riw[i]->GetResliceCursorWidget()->GetInteractor());

  // Set a priority higher than our reslice cursor widget
  this->DistanceWidget[i]->SetPriority(
    this->riw[i]->GetResliceCursorWidget()->GetPriority() + 0.01);

  vtkSmartPointer< vtkPointHandleRepresentation2D > handleRep =
    vtkSmartPointer< vtkPointHandleRepresentation2D >::New();
  vtkSmartPointer< vtkDistanceRepresentation2D > distanceRep =
    vtkSmartPointer< vtkDistanceRepresentation2D >::New();
  distanceRep->SetHandleRepresentation(handleRep);
  this->DistanceWidget[i]->SetRepresentation(distanceRep);
  distanceRep->InstantiateHandleRepresentation();
  distanceRep->GetPoint1Representation()->SetPointPlacer(riw[i]->GetPointPlacer());
  distanceRep->GetPoint2Representation()->SetPointPlacer(riw[i]->GetPointPlacer());

  // Add the distance to the list of widgets whose visibility is managed based
  // on the reslice plane by the ResliceImageViewerMeasurements class
  this->riw[i]->GetMeasurements()->AddItem(this->DistanceWidget[i]);

  this->DistanceWidget[i]->CreateDefaultRepresentation();
  this->DistanceWidget[i]->EnabledOn();
}

QtVTKRenderWindows:: ~QtVTKRenderWindows()
{
	//如果没有这句话，当boxWiget还在显示时如果程序退出，会出错，具体什么原因就不知道了
	if(boxWidget != 0)
	{
	  boxWidget->Off();
	}
}

/**
*里面主要动作从vmtkimagevoiselector.py中获取
*/
void QtVTKRenderWindows::croppedImageActivity()
{
	//没有图像读进来，直接返回
	if(!flag)
	{
		return;
	}

	log4cpp::Category& rootLog  = log4cpp::Category::getRoot();	
	log4cpp::Category& subLog = log4cpp::Category::getInstance(std::string("sub1"));
	//让用户选择一个文件名进行保存截取的图像
	QString filename = QFileDialog::getSaveFileName(this,  
    tr("Save Image"),  
    "",  
    tr("*.vti")); //选择路径  
	std::string cppFileName;
	if(filename.isEmpty())
	{
	   rootLog.info("QtVTKRenderWindows类croppedImageActivity():没有选择文件");
	   subLog.info("QtVTKRenderWindows类croppedImageActivity():没有选择文件");
	   return;
	}else
	{
		cppFileName = filename.toStdString();
		rootLog.info("QtVTKRenderWindows类croppedImageActivity():"+cppFileName);
		subLog.info("QtVTKRenderWindows类croppedImageActivity():"+cppFileName);
	}


    
	

	vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
	boxWidget->GetPolyData(polyData);
	polyData->ComputeBounds();

	double bounds[6];
	polyData->GetBounds(bounds);

	
	rootLog.info("QtVTKRenderWindows类croppedImageActivity()%lf , %lf , %lf , %lf , %lf , %lf" , bounds[0],bounds[1],bounds[2],bounds[3],bounds[4],bounds[5]);
	subLog.info("QtVTKRenderWindows类croppedImageActivity()%lf , %lf , %lf , %lf , %lf , %lf" , bounds[0],bounds[1],bounds[2],bounds[3],bounds[4],bounds[5]);

	vtkSmartPointer<vtkImageData> croppedImageData = vtkSmartPointer<vtkImageData>::New();
	//cout << croppedImageData->GetReferenceCount() << endl;
	if(readerFlag == 0 )
	{
	   VascuviewUtility::extractVoi(reader->GetOutput(),bounds,croppedImageData);
	}else if(readerFlag == 1)
	{
		VascuviewUtility::extractVoi(readerVti->GetOutput(),bounds,croppedImageData);
	}
	vtkSmartPointer<vtkXMLImageDataWriter> vtiWriter = vtkSmartPointer<vtkXMLImageDataWriter>::New();

	vtiWriter->SetInputData(croppedImageData);
	vtiWriter->SetFileName(filename.toStdString().c_str());

	if(vtiWriter->Write())
	{
		rootLog.info("QtVTKRenderWindows类croppedImageActivity():保存成功"+cppFileName);
		subLog.info("QtVTKRenderWindows类croppedImageActivity():保存成功"+cppFileName);
	}else
	{
		rootLog.info("QtVTKRenderWindows类croppedImageActivity():保存失败"+cppFileName);
		subLog.info("QtVTKRenderWindows类croppedImageActivity():保存失败"+cppFileName);
	}
	

	
	
}


void QtVTKRenderWindows::openVtiDicom()
{
	cout << "helloOpenVtiDicom" << endl;
	QString fileName = QFileDialog::getOpenFileName(this,  
                                                tr("打开文件"),  
                                                "/",  
                                                tr(  "vti文件(*.vti)"));  




	//如果没有选择目录就为空
	if(fileName.length() == 0)
	{

		return;
	}
	flag = true;
	readerFlag = 1 ;
	this->ui->statusBar->showMessage(fileName);
	






	//---------------------------------------
	//this->ui->view4->GetRenderWindow()->ClearInRenderStatus();
	//  vtkSmartPointer< vtkDICOMImageReader > reader =
    //vtkSmartPointer< vtkDICOMImageReader >::New();
  //reader->SetDirectoryName("E:/ZHANG_XIANGJU");
  
	readerVti->SetFileName(fileName.toStdString().c_str());
    readerVti->Update();


  int imageDims[3];
  readerVti->GetOutput()->GetDimensions(imageDims);

  //记得删除
  //---------------------
  //if(planeWidget[0] != 0)
   // cout << planeWidget[0]->GetReferenceCount()<<endl;
  //-----------------------------------

  for (int i = 0; i < 3; i++)
    {
		

		   if(riw[i] == 0)
		   {
             riw[i] = vtkSmartPointer< vtkResliceImageViewer >::New();
		   }
    }

 
  this->ui->view1->SetRenderWindow(riw[0]->GetRenderWindow());
  riw[0]->SetupInteractor(
      this->ui->view1->GetRenderWindow()->GetInteractor());

  this->ui->view2->SetRenderWindow(riw[1]->GetRenderWindow());
  riw[1]->SetupInteractor(
      this->ui->view2->GetRenderWindow()->GetInteractor());

  this->ui->view3->SetRenderWindow(riw[2]->GetRenderWindow());
  riw[2]->SetupInteractor(
      this->ui->view3->GetRenderWindow()->GetInteractor());

  for (int i = 0; i < 3; i++)
    {
    // make them all share the same reslice cursor object.
    vtkResliceCursorLineRepresentation *rep =
      vtkResliceCursorLineRepresentation::SafeDownCast(
          riw[i]->GetResliceCursorWidget()->GetRepresentation());
    riw[i]->SetResliceCursor(riw[0]->GetResliceCursor());

    rep->GetResliceCursorActor()->
      GetCursorAlgorithm()->SetReslicePlaneNormal(i);

    riw[i]->SetInputData(readerVti->GetOutput());
    riw[i]->SetSliceOrientation(i);
    riw[i]->SetResliceModeToAxisAligned();
	
    }

  vtkSmartPointer<vtkCellPicker> picker =
    vtkSmartPointer<vtkCellPicker>::New();
  picker->SetTolerance(0.005);

  vtkSmartPointer<vtkProperty> ipwProp =
    vtkSmartPointer<vtkProperty>::New();

  vtkSmartPointer< vtkRenderer > ren =
    vtkSmartPointer< vtkRenderer >::New();

 //自己后期添加，想试着在同一个QTVTKrenderer实例中载入不同的图片。
  vtkSmartPointer<vtkRenderWindow> new1 = vtkSmartPointer<vtkRenderWindow>::New();
  this->ui->view4->SetRenderWindow(new1);

  if(boxWidget == 0)
  {
	  boxWidget = vtkSmartPointer<vtkBoxWidget>::New();
  }
  //-----------------------------------------------------------------



   this->ui->view4->GetRenderWindow()->AddRenderer(ren);
  vtkRenderWindowInteractor *iren = this->ui->view4->GetInteractor();
  boxWidget->SetInteractor(iren);


  for (int i = 0; i < 3; i++)
    {
      if(planeWidget[i] == 0)
	  {	
	     planeWidget[i] = vtkSmartPointer<vtkImagePlaneWidget>::New();
		 // planeWidget[i] = vtkSmartPointer<vtkvmtkImagePlaneWidget>::New();
	  }
	
	  
		planeWidget[i]->SetInteractor( iren );
	
		planeWidget[i]->SetPicker(picker);
		planeWidget[i]->RestrictPlaneToVolumeOn();
		double color[3] = {0, 0, 0};
		color[i] = 1;
		planeWidget[i]->GetPlaneProperty()->SetColor(color);

		color[0] /= 4.0;
		color[1] /= 4.0;
		color[2] /= 4.0;
		riw[i]->GetRenderer()->SetBackground( color );

		planeWidget[i]->SetTexturePlaneProperty(ipwProp);
		planeWidget[i]->TextureInterpolateOff();
		planeWidget[i]->SetResliceInterpolateToLinear();
		
		planeWidget[i]->SetInputConnection(readerVti->GetOutputPort());
		
		//planeWidget[i]->SetInputData(reader->GetOutput());
		planeWidget[i]->SetPlaneOrientation(i);
		planeWidget[i]->SetSliceIndex(imageDims[i]/2);
		planeWidget[i]->DisplayTextOn();
		  
		planeWidget[i]->SetDefaultRenderer(ren);
		  
		planeWidget[i]->SetWindowLevel(1358, -27);
		planeWidget[i]->On();
		planeWidget[i]->InteractionOn();
	  
    }

  vtkSmartPointer<vtkResliceCursorCallback> cbk =
    vtkSmartPointer<vtkResliceCursorCallback>::New();
 
  for (int i = 0; i < 3; i++)
    {
		
    cbk->IPW[i] = planeWidget[i];
	  
    cbk->RCW[i] = riw[i]->GetResliceCursorWidget();
    riw[i]->GetResliceCursorWidget()->AddObserver(
        vtkResliceCursorWidget::ResliceAxesChangedEvent, cbk );
    riw[i]->GetResliceCursorWidget()->AddObserver(
        vtkResliceCursorWidget::WindowLevelEvent, cbk );
    riw[i]->GetResliceCursorWidget()->AddObserver(
        vtkResliceCursorWidget::ResliceThicknessChangedEvent, cbk );
    riw[i]->GetResliceCursorWidget()->AddObserver(
        vtkResliceCursorWidget::ResetCursorEvent, cbk );
    riw[i]->GetInteractorStyle()->AddObserver(
        vtkCommand::WindowLevelEvent, cbk );

    // Make them all share the same color map.
    riw[i]->SetLookupTable(riw[0]->GetLookupTable());
    planeWidget[i]->GetColorMap()->SetLookupTable(riw[0]->GetLookupTable());
    //planeWidget[i]->GetColorMap()->SetInput(riw[i]->GetResliceCursorWidget()->GetResliceCursorRepresentation()->GetColorMap()->GetInput());
    planeWidget[i]->SetColorMap(riw[i]->GetResliceCursorWidget()->GetResliceCursorRepresentation()->GetColorMap());



    }




  this->ui->view1->show();
  this->ui->view2->show();
  this->ui->view3->show();
 
  //当重新打开一张图像的时候，这些按钮的可见性需要重新设置
  this->ui->clickedVtkBoxWidget->setVisible(true);
  this->ui->clickedVtkBoxWidget->setChecked(false);
  this->ui->croppedImageButton->setVisible(false);

}