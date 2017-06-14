#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <set>
#include <ctime>
#include <vector>
#include <iostream>
#include <unordered_set>

#include <cstdlib>
#include <cstdio>
using std::vector;
using std::swap;
using std::set;
using std::pair;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    vector<double> vmod;

private:
    Ui::MainWindow *ui;

public slots:
  void changeValue11(int value);
  void changeValue12(int value);
  void changeValue21(int value);
  void changeValue22(int value);
private slots:
  void on_pushButton_clicked();
};

#endif // MAINWINDOW_H
