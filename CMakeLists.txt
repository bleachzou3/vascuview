cmake_minimum_required(VERSION 2.8.8)
if(POLICY CMP0020)
  cmake_policy(SET CMP0020 NEW)
endif()
if(POLICY CMP0025)
  cmake_policy(SET CMP0025 NEW) # CMake 3.0
endif()
if(POLICY CMP0043)
  cmake_policy(SET CMP0043 NEW) # CMake 3.0
endif()
if(POLICY CMP0053)
  cmake_policy(SET CMP0053 NEW) # CMake 3.1
endif()
project(vascuview)

find_package(VTK COMPONENTS
  vtkCommonCore
  vtkFiltersSources
  vtkGUISupportQt
  vtkIOImage
  vtkInteractionImage
  vtkalglib
vtkChartsCore
vtkCommonColor
vtkCommonComputationalGeometry
vtkCommonCore
vtkCommonDataModel
vtkCommonExecutionModel
vtkCommonMath
vtkCommonMisc
vtkCommonSystem
vtkCommonTransforms
vtkDICOMParser
vtkDomainsChemistry
vtkexodusII
vtkexpat
vtkFiltersAMR
vtkFiltersCore
vtkFiltersExtraction
vtkFiltersFlowPaths
vtkFiltersGeneral
vtkFiltersGeneric
vtkFiltersGeometry
vtkFiltersHybrid
vtkFiltersHyperTree
vtkFiltersImaging
vtkFiltersModeling
vtkFiltersParallel
vtkFiltersParallelImaging
vtkFiltersProgrammable
vtkFiltersSelection
vtkFiltersSMP
vtkFiltersSources
vtkFiltersStatistics
vtkFiltersTexture
vtkFiltersVerdict
vtkfreetype
vtkftgl
vtkGeovisCore
vtkgl2ps

vtkhdf5
vtkImagingColor
vtkImagingCore
vtkImagingFourier
vtkImagingGeneral
vtkImagingHybrid
vtkImagingMath
vtkImagingMorphological
vtkImagingSources
vtkImagingStatistics
vtkImagingStencil
vtkInfovisCore
vtkInfovisLayout
vtkInteractionImage
vtkInteractionStyle
vtkInteractionWidgets
vtkIOAMR
vtkIOCore
vtkIOEnSight
vtkIOExodus
vtkIOExport
vtkIOGeometry
vtkIOImage
vtkIOImport
vtkIOInfovis
vtkIOLegacy
vtkIOLSDyna
vtkIOMINC
vtkIOMovie
vtkIONetCDF
vtkIOParallel
vtkIOParallelXML
vtkIOPLY
vtkIOSQL
vtkIOVideo
vtkIOXML
vtkIOXMLParser
vtkjpeg
vtkjsoncpp
vtklibproj4
vtklibxml2
vtkMetaIO
vtknetcdf
vtkoggtheora
vtkParallelCore
vtkParseOGLExt
vtkpng
vtkRenderingAnnotation
vtkRenderingContext2D
vtkRenderingContextOpenGL
vtkRenderingCore
vtkRenderingFreeType
vtkRenderingGL2PS
vtkRenderingImage
vtkRenderingLabel
vtkRenderingLIC
vtkRenderingLOD
vtkRenderingOpenGL
vtkRenderingQt
vtkRenderingVolume
vtkRenderingVolumeOpenGL
vtksqlite
vtksys
vtktiff
vtkUtilitiesEncodeString
vtkUtilitiesHashSource
vtkverdict
vtkViewsContext2D
vtkViewsCore
vtkViewsInfovis
vtkViewsQt
vtkzlib
)
include(${VTK_USE_FILE})


if("${VTK_QT_VERSION}" STREQUAL "")
  message(FATAL_ERROR "VTK was not built with Qt")
endif()

# Set your files and resources here
set( Srcs QtVTKRenderWindowsApp.cxx QtVTKRenderWindows.cxx VascuviewUtility.cpp vmtkImageFeatures.cpp vmtkLevelSetSegmentation.cpp
  vmtkSurfaceViewer.cpp vmtkImageInitialization.cpp vmtkImageSeeder.cpp ChooseInitializationType.cpp ThresholdDialog.cpp YNDialog.cpp LevelSetDialog.cpp)

set( Hdrs QtVTKRenderWindows.h VascuviewUtility.h vtkResliceCursorCallback.h vtkUpdateCubeCallback.h vtkHideCubeCallback.h NullPointerException.h
   vmtkImageFeature.h vmtkLevelSetSegmentation.h vtkAddSeedCallBack.h vmtkSurfaceViewer.h vmtkImageSeeder.h vmtkImageInitialization.h
  Uncopyable.h ChooseInitializationType.h ThresholdDialog.h YNDialog.h LevelSetDialog.h)

set( MOC_Hdrs QtVTKRenderWindows.h )

set( UIs QtVTKRenderWindows.ui ChooseInitializationType.ui YNDialog.ui Threshold.ui LevelSetDialog.ui)
#加入log4cpp配置文件
set( LOGs log4cpp.conf )

set(QRCs icons.qrc)
set(LinkedVmtk 
vtkvmtkCommon
vtkvmtkComputationalGeometry
vtkvmtkContrib
vtkvmtkDifferentialGeometry
vtkvmtkIO
vtkvmtkITK
vtkvmtkMisc
vtkvmtkRendering
vtkvmtkSegmentation
)
set(LinkedLog
log4cpp
)
include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}  
)
#加入VMTK_LIB
include_directories(VMTK_LIB/include ..)
link_directories(VMTK_LIB/lib)  

#加入log4cpp
include_directories(LOG_HOME/include ..)
link_directories(LOG_HOME/lib)  

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

if(VTK_QT_VERSION VERSION_GREATER "4")
  find_package(Qt5 COMPONENTS Widgets REQUIRED QUIET)
  qt5_wrap_ui(UI_Srcs ${UIs})


  qt5_add_resources(QRC_Srcs ${QRCs} )

 source_group("Resources" FILES
    ${UIs}
    ${QRCs}
    ${EXE_ICON} # Not present
	${LOGs}
  )

  source_group("Generated" FILES
    ${UI_Srcs}
    ${MOC_Srcs}
    ${QRC_Srcs}
    ${QRC_Srcs}
  )

  # CMAKE_AUTOMOC in ON so the MocHdrs will be automatically wrapped.
  add_executable(QtVTKRenderWindows ${Srcs} ${Hdrs} ${UI_Srcs} ${MOC_Hdrs})
  qt5_use_modules(QtVTKRenderWindows Core Gui Widgets)
  target_link_libraries(QtVTKRenderWindows ${VTK_LIBRARIES} ${LinkedVmtk} ${LinkedLog})
else()
  find_package(Qt4 REQUIRED)
  include(${QT_USE_FILE})
  # Use what VTK built with
  set(QT_QMAKE_EXECUTABLE ${VTK_QT_QMAKE_EXECUTABLE} CACHE FILEPATH "")
  set(QT_MOC_EXECUTABLE ${VTK_QT_MOC_EXECUTABLE} CACHE FILEPATH "")
  set(QT_UIC_EXECUTABLE ${VTK_QT_UIC_EXECUTABLE} CACHE FILEPATH "")
  qt4_wrap_ui(UI_Srcs ${UIs})

  add_executable(QtVTKRenderWindows ${Srcs} ${Hdrs} ${UI_Srcs} ${MOC_Hdrs})
  target_link_libraries(QtVTKRenderWindows ${QT_LIBRARIES} ${VTK_LIBRARIES})
endif()