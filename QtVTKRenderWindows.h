#ifndef QtVTKRenderWindows_H
#define QtVTKRenderWindows_H

#include "vtkSmartPointer.h"
#include "vtkResliceImageViewer.h"
#include "vtkImagePlaneWidget.h"
#include "vtkDistanceWidget.h"
#include "vtkResliceImageViewerMeasurements.h"
#include "vtkBoxWidget.h"
#include "vtkDICOMImageReader.h"
#include "vtkCubeSource.h"
#include "vtkActor.h"
#include "vtkXMLImageDataReader.h"
#include <QMainWindow>

// Forward Qt class declarations
class Ui_QtVTKRenderWindows;

class QtVTKRenderWindows : public QMainWindow
{
  Q_OBJECT
public:

  // Constructor/Destructor
  QtVTKRenderWindows();
  ~QtVTKRenderWindows();

public slots:

  virtual void slotExit();
  virtual void resliceMode(int);
  virtual void thickMode(int);
  virtual void SetBlendModeToMaxIP();
  virtual void SetBlendModeToMinIP();
  virtual void SetBlendModeToMeanIP();
  virtual void SetBlendMode(int);
  virtual void ResetViews();
  virtual void Render();
  virtual void AddDistanceMeasurementToView1();
  virtual void AddDistanceMeasurementToView( int );

  //打开dicom图像所在的图像目录
  virtual void openDirectoryDicom();

  //打开vti格式的图像，主要是截取后的
  virtual void openVtiDicom();
  //点击单选按钮BoxVisible后的操作
  virtual void IsShowBoxWidget(bool visible);


  //进行图像截取并保存
  virtual void croppedImageActivity();
protected:
  vtkSmartPointer< vtkResliceImageViewer > riw[3];
  vtkSmartPointer< vtkImagePlaneWidget > planeWidget[3];
  vtkSmartPointer<vtkBoxWidget> boxWidget;
  // vtkSmartPointer<vtkvmtkImagePlaneWidget> planeWidget[3];
  vtkSmartPointer< vtkDistanceWidget > DistanceWidget[3];
  vtkSmartPointer< vtkResliceImageViewerMeasurements > ResliceMeasurements;

  //从目录下读取dicom图像
   vtkSmartPointer< vtkDICOMImageReader > reader ;

   //读取某个vti文件
   vtkSmartPointer<vtkXMLImageDataReader> readerVti;
   
   vtkSmartPointer<vtkActor> cubeActor;
   vtkSmartPointer<vtkCubeSource> cubeSource; 
  
 

protected slots:

private:

  // Designer form
  Ui_QtVTKRenderWindows *ui;
  //做一些连接操作
  void connectActions();

  //初始化一些3d组件,比如说一些vtkDicomImageReader
  void init3DWidget();

  //是否已经打开过图像
  bool flag;

  bool boxWidgetOn;

  //标记使用哪个reader,0:代表reader,1代表readerVti
  int readerFlag;
  
};

#endif // QtVTKRenderWindows_H