#ifndef __AF_FILTER_H__WIN
#define __AF_FILTER_H__WIN

#include<QtGui>
#include<QtCore>
#include<iostream>

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

class FilterWindow : public QMainWindow {
    
    Q_OBJECT
    
public:
    FilterWindow(QWidget *parent = 0);
    void paintEvent(QPaintEvent *event);
    void updateScreen();
    void resizeEvent(QResizeEvent *event);
    
public slots:
    void fileLoad();
    void fileSave();
    void fileExit();
    
    void editCopy();
    void editPaste();
    
    void helpHowTo();
    void helpAbout();
    
    void filterChange(int filter);
    
private:
    void createMenu();
    
    QMenu *file_menu, *edit_menu, *help_menu;
    QAction *file_load, *file_save, *file_exit;
    QAction *edit_copy, *edit_paste;
    QAction *help_howto, *help_about;
    
    QImage current_image;
    bool image_set;
    FilterControl *filter_control;
};



#endif
