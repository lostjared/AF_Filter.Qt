#include "filter_control.h"
#include "main_window.h"


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
    filter_combo->setGeometry(50, 15, 300, 20);
    
    for(unsigned int i = 0; filter_names[i] != 0; ++i)
        filter_combo->addItem(filter_names[i]);
    
    slider = new QSlider(Qt::Horizontal, this);
    slider->setGeometry(360, 15, 400, 25);
    slider->setMinimum(1);
    slider->setMaximum(512);
    slider->setSingleStep(1);
    
    slider_index = new QLabel("0", this);
    slider_index->setGeometry(770,15,30,25);

    parent_window = (FilterWindow*)parent;

    connect(slider, SIGNAL(valueChanged(int)), this, SLOT(setPos(int)));
}

void FilterControl::setPos(int pos) {
    QString text;
    QTextStream stream(&text);
    stream << pos;
    slider_index->setText(text);
    parent_window->filterChange(pos);
}