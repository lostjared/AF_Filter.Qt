#ifndef __AF_FILTER_H__WIN
#define __AF_FILTER_H__WIN

#include<QtGui>
#include<QtCore>
#include<iostream>
#include "alphaflame.h"


class FilterWindow;

class FilterControl : QDialog {
    Q_OBJECT
public:
    FilterControl(QWidget *parent = 0);
public slots:
    void updateIndex(int pos);
    void setPos(int pos);
    void setSliderText(int pos);
    void checkChanged(int chk);
private:
    friend class FilterWindow;
    QLabel *filter_label, *slider_index, *red_label, *green_label, *blue_label, *red_index, *green_index, *blue_index;
    QComboBox *filter_combo, *rgb_combo;
    QSlider *slider, *slider_red, *slider_green, *slider_blue;
    QCheckBox *neg_box;
    FilterWindow *parent_window;
};

extern const char *filter_names[];

class FilterWindow : public QMainWindow {
    
    Q_OBJECT
    
public:
    FilterWindow(QWidget *parent = 0);
    void paintEvent(QPaintEvent *event);
    void updateScreen();
    void resizeEvent(QResizeEvent *event);
    void filterChange(bool neg_state, unsigned int red, unsigned int green, unsigned int blue, int rev, int filter, int iteration);
public slots:
    void fileLoad();
    void fileSave();
    void fileSet();
    void fileExit();
    
    void editCopy();
    void editPaste();
    
    void helpHowTo();
    void helpAbout();
    
    
private:
    void createMenu();
    
    QMenu *file_menu, *edit_menu, *help_menu;
    QAction *file_load, *file_save, *file_exit, *file_setsource;
    QAction *edit_copy, *edit_paste;
    QAction *help_howto, *help_about;
    
    QImage original_image, current_image;
    bool image_set;
    FilterControl *filter_control;
};



#endif
