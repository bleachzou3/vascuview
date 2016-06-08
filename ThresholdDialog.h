#ifndef THRESHOLD_DIALOG_HPP_
#define THRESHOLD_DIALOG_HPP_
#include <qdialog.h>
#include <ui_Threshold.h>
class ThresholdDialog:public QDialog,public Ui::Threshold
{
	Q_OBJECT
public:
	ThresholdDialog(QWidget*parent = 0);
private slots:
	void lineEdit_textChanged(const QString &text);
};


#endif