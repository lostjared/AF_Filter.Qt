#ifndef __AF_FILTER_H__WIN
#define __AF_FILTER_H__WIN

#include<QtGui>
#include<QtCore>
#include<iostream>


class FilterWindow : public QMainWindow {
    
    Q_OBJECT
    
public:
    FilterWindow(QWidget *parent = 0);
    
public slots:
    void fileLoad();
    void fileSave();
    void fileExit();
    
    void editCopy();
    void editPaste();
    
    void helpHowTo();
    void helpAbout();
    
private:
    void createMenu();
    
    QMenu *file_menu, *edit_menu, *help_menu;
    QAction *file_load, *file_save, *file_exit;
    QAction *edit_copy, *edit_paste;
    QAction *help_howto, *help_about;
    
};




#endif
