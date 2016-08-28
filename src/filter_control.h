#ifndef __FILTER_CONTROL_H__
#define __FILTER_CONTROL_H__


#include<QtGui>
#include<QtCore>

class FilterWindow;

class FilterControls : QDialog {
public:
    FilterControls(QWidget *parent = 0);
private:
    friend class FilterWindow;
};

#endif
