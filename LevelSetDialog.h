#ifndef LEVEL_SET_DIALOG_HPP_
#define LEVEL_SET_DIALOG_HPP_
#include <qdialog.h>
#include "ui_LevelSetDialog.h"

class LevelSetDialog:public QDialog,public Ui::LevelSetDialog
{
	Q_OBJECT
public:
	LevelSetDialog(QWidget*parent=0);
};

#endif