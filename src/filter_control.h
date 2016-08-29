#ifndef __FILTER_CONTROL_H__
#define __FILTER_CONTROL_H__


#include<QtGui>
#include<QtCore>

class FilterWindow;

class FilterControl : QDialog {
    
    Q_OBJECT
public:
    FilterControl(QWidget *parent = 0);
    
public slots:
    void setPos(int pos);
    
private:
    friend class FilterWindow;
    QLabel *filter_label, *slider_index;
    QComboBox *filter_combo;
    QSlider *slider;
    FilterWindow *parent_window;
};

extern const char *filter_names[];

#endif
