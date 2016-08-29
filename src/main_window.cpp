#include "main_window.h"
#include "filter_control.h"


FilterWindow::FilterWindow(QWidget *parent) : QMainWindow(parent) {
    setGeometry(100, 100, 800, 600);
    setWindowTitle("AF_Filter ");
    image_set = false;
    createMenu();
    filter_control = new FilterControl(this);
    filter_control->setGeometry(100, 710, 800, 100);
    filter_control->setWindowTitle("Controls");
    Qt::WindowFlags flags = filter_control->windowFlags();
    filter_control->setWindowFlags(Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint | Qt::CustomizeWindowHint |  Qt::WindowStaysOnTopHint | Qt::Tool);
    filter_control->show();
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
    
    edit_menu = menuBar()->addMenu(tr("&Edit"));
    
    edit_copy = new QAction(tr("&Copy"), this);
    edit_copy->setShortcut(tr("Ctrl+C"));
    edit_copy->setStatusTip(tr("Copy Image"));
    edit_menu->addAction(edit_copy);
    connect(edit_copy, SIGNAL(triggered()), this, SLOT(editCopy()));
    
    edit_paste = new QAction(tr("&Paste"), this);
    edit_paste->setShortcut(tr("Ctrl+V"));
    edit_paste->setStatusTip(tr("Paste Image"));
    edit_menu->addAction(edit_paste);
    connect(edit_paste, SIGNAL(triggered()), this, SLOT(editPaste()));
    
    help_menu = menuBar()->addMenu(tr("&Help"));
    
    help_howto = new QAction(tr("&How to Use"), this);
    help_howto->setShortcut(tr("Ctrl+H"));
    help_howto->setStatusTip(tr("How to use"));
    help_menu->addAction(help_howto);
    connect(help_howto, SIGNAL(triggered()), this, SLOT(helpHowTo()));
    
    
    help_about = new QAction(tr("&About"), this);
    help_about->setShortcut(tr("Ctrl+A"));
    help_about->setStatusTip(tr("About this program"));
    help_menu->addAction(help_about);
    connect(help_about, SIGNAL(triggered()), this, SLOT(helpAbout()));
 }

void FilterWindow::paintEvent(QPaintEvent *) {
    QImage temp_image(size(), QImage::Format_ARGB32_Premultiplied);
    QPainter paint(&temp_image);
    paint.fillRect(QRect(QPoint(0,0), size()), QColor(0,0,0));
    if(image_set == true) {
        QRect src(QPoint(0, 0), size());
        QImage scaled_image = current_image.scaled(size(), Qt::KeepAspectRatio);
        QRect dst(QPoint(0,0),scaled_image.size());
        dst.moveCenter(src.center());
        paint.drawImage(dst.topLeft(), scaled_image);
    }
    QPainter flip_paint(this);
    flip_paint.drawImage(0, 0, temp_image);
}

void FilterWindow::resizeEvent(QResizeEvent *) {
    
}

void FilterWindow::updateScreen() {
    repaint();
}

void FilterWindow::fileLoad() {
    
    QString input_file = QFileDialog::getOpenFileName(this, "Select an Image", "", "Images (*.png *.xpm *.jpg)");
    
    if(input_file != "") {
        current_image = QImage(input_file);
    	image_set = true;
    	updateScreen();
    }
    
}
void FilterWindow::fileSave() {
    
}

void FilterWindow::fileExit() {
    std::cout << "Exiting Application...\n";
    QApplication::quit();
}

void FilterWindow::editCopy() {
    
}

void FilterWindow::editPaste() {
    
}

void FilterWindow::helpHowTo() {
    QMessageBox::information(this, "Information On How to Use", "Fill this out");
}

void FilterWindow::helpAbout() {
    QMessageBox::information(this, "About AF_Filter.Qt", "<b>AF_Filter written by Jared Bruni</b>\nhttp://lostsidedead.com");
}


