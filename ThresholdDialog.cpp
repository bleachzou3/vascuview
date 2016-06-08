#include "ThresholdDialog.h"
#include "qregexp.h"
#include <iostream>
ThresholdDialog::ThresholdDialog(QWidget*parent):QDialog(parent)
{
	setupUi(this);
	okButton->setEnabled(false);
	//QRegExp regExp("\~(-?\d+)(\.\d+)?$");
	
	//lowerThresholdLine->setValidator(new QRegExpValidator(regExp,this));
	//upperThresholdLine->setValidator(new QRegExpValidator(regExp,this));
	
	connect(lowerThresholdLine,SIGNAL(textChanged(const QString &)),this,SLOT(lineEdit_textChanged(const QString &)));
	connect(upperThresholdLine,SIGNAL(textChanged(const QString &)),this,SLOT(lineEdit_textChanged(const QString &)));
	
}
void ThresholdDialog::lineEdit_textChanged(const QString &text)
{

	
	if(upperThresholdLine->hasAcceptableInput()&&lowerThresholdLine->hasAcceptableInput())
	{
		okButton->setEnabled(true);
	}
	
}
