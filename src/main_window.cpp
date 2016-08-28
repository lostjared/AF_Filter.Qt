#include "main_window.h"


FilterWindow::FilterWindow(QWidget *parent) : QMainWindow(parent) {
    setGeometry(100, 100, 800, 600);
    setWindowTitle("AF_Filter ");
    createMenu();
}

void FilterWindow::createMenu() {
    file_menu = menuBar()->addMenu(tr("&File"));
    
    file_load = new QAction(tr("&Load Image"), this);
    file_load->setShortcut(tr("Ctrl+L"));
    file_load->setStatusTip(tr("Load an Image"));
    file_menu->addAction(file_load);
    connect(file_load, SIGNAL(triggered()), this, SLOT(fileLoad()));
    
    file_save = new QAction(tr("&Save Image"), this);
    file_save->setShortcut(tr("Ctrl+S"));
    file_save->setStatusTip(tr("Save Image As"));
    file_menu->addAction(file_save);
    connect(file_save, SIGNAL(triggered()), this, SLOT(fileSave()));
    
    file_exit = new QAction(tr("E&xit"), this);
    file_menu->addAction(file_exit);
    connect(file_exit, SIGNAL(triggered()), this, SLOT(fileExit()));
 }


void FilterWindow::fileLoad() {
    
}
void FilterWindow::fileSave() {
    
}

void FilterWindow::fileExit() {
    std::cout << "Exiting Application...\n";
    QApplication::quit();
    
}