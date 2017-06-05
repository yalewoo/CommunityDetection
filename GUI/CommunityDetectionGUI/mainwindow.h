#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "../../Graph.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);



private:
    Ui::MainWindow *ui;



public:
    Graph g;
    Communities c1;
    Communities c2;
private slots:
    void on_pushButton_clicked();
    void on_loadcomm1_clicked();
    void on_loadcomm2_clicked();
};

#endif // MAINWINDOW_H
