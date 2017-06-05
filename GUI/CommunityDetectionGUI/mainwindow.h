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

private slots:
    void on_button_graph_path_clicked();

    void on_button_load_graph_clicked();

    void on_button_comm1_clicked();

private:
    Ui::MainWindow *ui;



public:
    Graph g;
};

#endif // MAINWINDOW_H
