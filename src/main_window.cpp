#include "main_window.h"


FilterWindow::FilterWindow(QWidget *parent) : QMainWindow(parent) {
    setGeometry(100, 100, 800, 600);
    setWindowTitle("AF_Filter ");
    image_set = false;
    createMenu();
    filter_control = new FilterControl(this);
    filter_control->setGeometry(100, 710, 800, 80);
    filter_control->setWindowTitle("Controls");
    //Qt::WindowFlags flags = filter_control->windowFlags();
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
    
    file_setsource = new QAction(tr("Set Current as Source"), this);
    file_setsource->setShortcut(tr("Ctrl+E"));
    file_setsource->setStatusTip(tr("Set Current as Source"));
    file_menu->addAction(file_setsource);
    connect(file_setsource, SIGNAL(triggered()), this, SLOT(fileSet()));
    
    file_exit = new QAction(tr("E&xit"), this);
    file_menu->addAction(file_exit);
    connect(file_exit, SIGNAL(triggered()), this, SLOT(fileExit()));
    
    edit_menu = menuBar()->addMenu(tr("&Edit"));
    
    edit_undo = new QAction(tr("&Undo"), this);
    edit_undo->setShortcut(tr("Ctrl+Z"));
    edit_undo->setStatusTip(tr("Undo operation"));
    edit_menu->addAction(edit_undo);
    connect(edit_undo, SIGNAL(triggered()), this, SLOT(editUndo()));
    
    edit_redo = new QAction(tr("&Redo"), this);
    edit_redo->setShortcut(tr("Ctrl+R"));
    edit_redo->setStatusTip(tr("Redo operation"));
    edit_menu->addAction(edit_redo);
    connect(edit_redo, SIGNAL(triggered()), this, SLOT(editRedo()));
    
    edit_menu->addSeparator();
    
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
        original_image = current_image;
        image_list.push_back(original_image);
    	image_set = true;
    	updateScreen();
    }
    
}
void FilterWindow::fileSave() {
    QString file_name = QFileDialog::getSaveFileName(this, "Save as PNG", "", "Image (*.png)");
    if(file_name != "") current_image.save(file_name, "PNG"); }

void FilterWindow::fileSet() {
    if(image_set) {
        image_list.push_back(original_image);
        original_image = current_image;
    }
}

void FilterWindow::fileExit() {
    std::cout << "Exiting Application...\n";
    QApplication::quit();
}

void FilterWindow::editUndo() {
     if(image_list.size() > 0) {
    	QImage image = image_list.back();
    	image_list.pop_back();
        redo_list.push_back(current_image);
        original_image = image;
        current_image = image;
    	updateScreen();
    }
}

void FilterWindow::editRedo() {
    if(redo_list.size() > 0) {
        QImage image = redo_list.back();
        redo_list.pop_back();
        image_list.push_back(current_image);
        current_image = image;
        original_image = image;
        updateScreen();
    }
}

void FilterWindow::editCopy() {
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setImage(current_image);
}

void FilterWindow::editPaste() {
    
    QClipboard *clipboard = QApplication::clipboard();
    const QMimeData *mimedata = clipboard->mimeData();
    
    if(mimedata->hasImage()) {
        original_image = qvariant_cast<QImage>(mimedata->imageData());
        current_image = original_image;
        image_set = true;
        updateScreen();
    }
}

void FilterWindow::helpHowTo() {
    QMessageBox::information(this, "Information On How to Use", "Fill this out");
}

void FilterWindow::helpAbout() {
    QMessageBox::information(this, "About AF_Filter.Qt", "<b>AlphaFlame Filter written by Jared Bruni</b>\nhttp://lostsidedead.com");
}

void FilterWindow::filterChange(bool neg_state, unsigned int red, unsigned int green, unsigned int blue, int rev, int filter, int index) {
    // preform filter action
    current_image = original_image;
    alphaFlame(current_image,neg_state, red,green,blue,rev,filter, index);
    updateScreen();
}

// dialog FilterControl

const char *filter_names[] = { "AC Self AlphaBlend", "Reverse Self AlphaBlend",
    "Opposite Self AlphaBlend", "AC2 Distort", "Reverse Distort", "Opposite Distort",
    "Full Distort", "A New One", "AC NewOne", "AC Thought Filter", "Line Draw",
    "Gradient Square", "Color Wave", "Pixelated Gradient", "Combined Gradient",
    "Diagonal", "Average", "Average Divide", "Cos/Sin Multiply", "Modulus Multiply",
    "Positive/Negative", "z+1 Blend", "Diamond Pattern", "Pixelated Shift","Pixelated Mix",
    "Color Accumulate", "Color Accumulate #2", "Color Accumulate #3", "Angle",
    "Vertical Average", "Circular Blend", "Average Blend", "~Divide", "Mix", "Random Number",
    "Gradient Repeat", 0 };


FilterControl::FilterControl(QWidget *parent) : QDialog(parent) {
    
    filter_label = new QLabel("Filter: ", this);
    filter_label->setGeometry(10, 15, 30, 20);
    
    filter_combo = new QComboBox(this);
    filter_combo->setGeometry(50, 15, 300, 25);
    
    for(unsigned int i = 0; filter_names[i] != 0; ++i)
        filter_combo->addItem(filter_names[i]);
    
    slider = new QSlider(Qt::Horizontal, this);
    slider->setGeometry(360, 15, 400, 25);
    slider->setMinimum(1);
    slider->setMaximum(512);
    slider->setSingleStep(1);
    slider->setSliderPosition(1);
    slider_index = new QLabel("<b>1</b>", this);
    slider_index->setGeometry(770,15,30,25);
    
    parent_window = (FilterWindow*)parent;
    
    connect(slider, SIGNAL(valueChanged(int)), this, SLOT(setPos(int)));
    connect(filter_combo, SIGNAL(currentIndexChanged(int)), this, SLOT(updateIndex(int)));
    
    red_label = new QLabel("Red: ", this);
    red_label->setGeometry(10, 45, 25, 20);
    slider_red = new QSlider(Qt::Horizontal, this);
    slider_red->setGeometry(40, 45, 100, 20);
    slider_red->setMinimum(0);
    slider_red->setMaximum(255);
    slider_red->setSingleStep(1);
    slider_red->setSliderPosition(0);
    red_index = new QLabel("<b>0</b>", this);
    red_index->setGeometry(145, 45, 25, 20);
    
    connect(slider_red, SIGNAL(valueChanged(int)), this, SLOT(setSliderText(int)));
    
    green_label = new QLabel("Green: ", this);
    green_label->setGeometry(170, 45, 35, 20);
    slider_green = new QSlider(Qt::Horizontal, this);
    slider_green->setGeometry(210, 45, 100, 20);
    slider_green->setMinimum(0);
    slider_green->setMaximum(255);
    slider_green->setSingleStep(1);
    slider_green->setSliderPosition(0);
    green_index = new QLabel("<b>0</b>", this);
    green_index->setGeometry(315, 45, 25, 20);
    
    connect(slider_green, SIGNAL(valueChanged(int)), this, SLOT(setSliderText(int)));
    
    blue_label = new QLabel("Blue: ", this);
    blue_label->setGeometry(340, 45, 25, 20);
    slider_blue = new QSlider(Qt::Horizontal, this);
    slider_blue->setGeometry(370, 45, 100, 20);
    slider_blue->setMinimum(0);
    slider_blue->setMaximum(255);
    slider_blue->setSingleStep(1);
    slider_blue->setSliderPosition(0);
    blue_index = new QLabel("<b>0</b>", this);
    blue_index->setGeometry(475, 45, 25, 20);
    
    connect(slider_blue, SIGNAL(valueChanged(int)), this, SLOT(setSliderText(int)));
    
    neg_box = new QCheckBox("Negate", this);
    neg_box->setGeometry(495, 45, 100, 20);
    
    connect(neg_box, SIGNAL(stateChanged(int)), this, SLOT(checkChanged(int)));
    
    rgb_combo = new QComboBox(this);
    rgb_combo->setGeometry(565, 45, 100, 25);
    rgb_combo->addItem("RGB");
    rgb_combo->addItem("BGR");
    rgb_combo->addItem("BRG");
    rgb_combo->addItem("GRB");
    
    connect(rgb_combo, SIGNAL(currentIndexChanged(int)), this, SLOT(updateIndex(int)));
    
    setAsSource = new QPushButton("Set Current", this);
    setAsSource->setGeometry(675, 45, 100, 20);
    connect(setAsSource, SIGNAL(clicked()), this, SLOT(setSource()));
}

void FilterControl::setSource() {
    parent_window->fileSet();
}

void FilterControl::updateIndex(int) {
    parent_window->filterChange((neg_box->isChecked()) ? true : false, slider_red->sliderPosition(), slider_green->sliderPosition(), slider_blue->sliderPosition(), rgb_combo->currentIndex(), filter_combo->currentIndex(), slider->sliderPosition());
}

void FilterControl::checkChanged(int) {
    updateIndex(0);
}

void FilterControl::setPos(int pos) {
    QString text;
    QTextStream stream(&text);
    stream << "<b>" << pos << "</b>";
    slider_index->setText(text);
    parent_window->filterChange((neg_box->isChecked()) ? true : false, slider_red->sliderPosition(), slider_green->sliderPosition(), slider_blue->sliderPosition(), rgb_combo->currentIndex(), filter_combo->currentIndex(), pos);
}

void FilterControl::setSliderText(int) {
    QString red_text;
    QTextStream red_stream(&red_text);
    red_stream << "<b>" << slider_red->sliderPosition() << "</b>";
    red_index->setText(red_text);
    QString green_text;
    QTextStream green_stream(&green_text);
    green_stream << "<b>" << slider_green->sliderPosition() << "</b>";
    green_index->setText(green_text);
    QString blue_text;
    QTextStream blue_stream(&blue_text);
    blue_stream << "<b>" << slider_blue->sliderPosition() << "</b>";
    blue_index->setText(blue_text);
    parent_window->filterChange((neg_box->isChecked()) ? true : false, slider_red->sliderPosition(), slider_green->sliderPosition(), slider_blue->sliderPosition(), rgb_combo->currentIndex(), filter_combo->currentIndex(), slider->sliderPosition());
}


