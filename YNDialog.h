#ifndef YN_DIALOG_HPP_
#define YN_DIALOG_HPP_
#include <qdialog.h>
#include <ui_YNDialog.h>
class YNDialog:public QDialog,public Ui::Dialog
{
	Q_OBJECT
public:
	YNDialog(QWidget*parent = 0);
	void setMessage(const char* message);

};


#endif
