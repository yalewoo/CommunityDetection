#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <QFileDialog>

map<string, string> Graph::config;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_button_graph_path_clicked()
{
    QString file = QFileDialog::getOpenFileName();
    ui->input_graph_path->setText(file);
}

void MainWindow::on_button_load_graph_clicked()
{
    g.load(ui->input_graph_path->text().toStdString().c_str());
    QString info;
    QString tmp;
    if (g.Weighted)
        tmp.sprintf("Weighted Graph ");
    else
        tmp.sprintf("Unweighted Graph ");

    info.sprintf("%d edges", g.edges.size());
    info = tmp + info;

    ui->label_graph_info->setText(info);
}

void MainWindow::on_button_comm1_clicked()
{
    QString file = QFileDialog::getOpenFileName();
    ui->input_comm1->setText(file);
}
