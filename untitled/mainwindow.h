#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_insert_clicked();

    void on_pushButton_clicked();

    void on_Minify_clicked();

    void on_pushButton_4_clicked();

    void on_Json_clicked();

    void on_Compress_clicked();

    void on_Decompress_clicked();

    void on_Format_clicked();

    void on_comboBox_activated(int index);

    void on_Graph_clicked();

    void on_save_clicked();

    void on_enter_clicked();

    void on_undo_clicked();

    void on_redo_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
