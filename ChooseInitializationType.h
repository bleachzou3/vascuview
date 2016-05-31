#ifndef CHOOSE_INITIALIZATION_TYPE_HPP_
#define CHOOSE_INITIALIZATION_TYPE_HPP_
#include "ui_ChooseInitializationType.h"
#include <qdialog.h>
class ChooseInitializationType:public QDialog,public Ui::InitializationDialog
{
	Q_OBJECT
public:
	ChooseInitializationType(QWidget* parent = 0);
};

#endif