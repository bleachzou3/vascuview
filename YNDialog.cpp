#include "YNDialog.h"

YNDialog::YNDialog(QWidget* parent):QDialog(parent)
{
	setupUi(this);
	
}
void YNDialog::setMessage(const char * message)
{
	this->description->setText(QApplication::translate("Dialog",message, 0));
}