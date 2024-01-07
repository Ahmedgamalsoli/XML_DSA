#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/icons/data-analytics.png"));
    a.setStyleSheet(
        "QMainWindow { background-color: #91C8E4; color: #ffffff; }"
        "QPushButton { background-color: #6499E9; color: #ffffff; padding: 8px 16px; }"
        "QPushButton:hover { background-color: #2980b9; }"
        "QPushButton:disabled { background-color: #749BC2; color: #ffffff; }"
        "QComboBox {background-color: #6499E9; color: #ffffff; border: 1px solid #2980b9; /* Dark blue border */padding: 3px;}"
        "QComboBox:hover {background-color: #2980b9; /* Darker blue on hover */}"
        "QLineEdit {background-color: #EEF5FF; /* White background */color: #333333; /* Dark text color */border: 1px solid #bdc3c7; /* Light gray border */padding: 5px;}"
        "QPlainTextEdit {background-color: #EEF5FF; /* White background */color: #333333; /* Dark text color */border: 1px solid #bdc3c7; /* Light gray border */}");
    MainWindow w;
    w.show();
    return a.exec();
}
