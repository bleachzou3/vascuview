#include <QApplication>
#include "QtVTKRenderWindows.h"
#include <log4cpp/Category.hh>
#include <log4cpp/PropertyConfigurator.hh>


int main( int argc, char** argv )
{
	try
	{
		log4cpp::PropertyConfigurator::configure("log4cpp.conf");
	}
	catch (log4cpp::ConfigureFailure& f)
	{
		std::cout << "Configure Problem " << f.what() << std::endl;
		return -1;
	}
	log4cpp::Category& rootLog  = log4cpp::Category::getRoot();
	rootLog.info("程序开始执行");
	log4cpp::Category& subLog = log4cpp::Category::getInstance(std::string("sub1"));
	subLog.info("程序开始执行");
  // QT Stuff
  QApplication app( argc, argv );

  QtVTKRenderWindows myQtVTKRenderWindows(argc, argv);
  myQtVTKRenderWindows.show();

  return app.exec();
}