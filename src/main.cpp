#include"main_window.h"
#include<iostream>

int main(int argc, char **argv) {
    QApplication app(argc, argv);
    FilterWindow fwindow;
    fwindow.show();
    app.exec();
	return 0;
}
