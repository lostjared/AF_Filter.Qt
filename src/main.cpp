/*
 
 Alpha Flame Filter
 written by Jared Bruni
 http://github.com/lostjared
 
 License: GPL
 */
#include"main_window.h"

int main(int argc, char **argv) {
    QApplication app(argc, argv);
    FilterWindow fwindow;
    fwindow.show();
    return app.exec();
}
