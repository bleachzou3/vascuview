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
#include <QMainWindow>

// Forward Qt class declarations
class Ui_QtVTKRenderWindows;

class QtVTKRenderWindows : public QMainWindow
{
  Q_OBJECT
public:

  // Constructor/Destructor
  QtVTKRenderWindows(int argc, char *argv[]);
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

  //�����ڵ�ͼ��Ŀ¼
  virtual void openDirectoryDicom();

  //�����ѡ��ťBoxVisible��Ĳ���
  virtual void IsShowBoxWidget(bool visible);

protected:
  vtkSmartPointer< vtkResliceImageViewer > riw[3];
  vtkSmartPointer< vtkImagePlaneWidget > planeWidget[3];
  vtkSmartPointer<vtkBoxWidget> boxWidget;
  // vtkSmartPointer<vtkvmtkImagePlaneWidget> planeWidget[3];
  vtkSmartPointer< vtkDistanceWidget > DistanceWidget[3];
  vtkSmartPointer< vtkResliceImageViewerMeasurements > ResliceMeasurements;

   vtkSmartPointer< vtkDICOMImageReader > reader ;
   vtkSmartPointer<vtkActor> cubeActor;
   vtkSmartPointer<vtkCubeSource> cubeSource; 
  
 

protected slots:

private:

  // Designer form
  Ui_QtVTKRenderWindows *ui;
  //��һЩ���Ӳ���
  void connectActions();

  //��ʼ��һЩ3d���,����˵һЩvtkDicomImageReader
  void init3DWidget();

  //�Ƿ��Ѿ��򿪹�ͼ��
  bool flag;
  
};

#endif // QtVTKRenderWindows_H