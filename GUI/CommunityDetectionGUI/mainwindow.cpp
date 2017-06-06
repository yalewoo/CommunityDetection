#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <QFileDialog>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QList>
#include <QUrl>



map<string, string> Graph::config;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setAcceptDrops(true);

    g.load("F:/HICODE_SUB/result/syn/graph");

    QString t(g.print().c_str());
    ui->graph_info->setText(t);


    ui->comm1->pg = &g;
    ui->comm1->comm2 = &ui->comm2->comm1;

    ui->comm2->pg = &g;
    ui->comm2->comm2 = &ui->comm1->comm1;

    ui->comm1->info = ui->info;
    ui->comm2->info = ui->info;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}
void MainWindow::dropEvent(QDropEvent *event)
{
    QList<QUrl> urls = event->mimeData()->urls();
    if (urls.isEmpty())
       return;
    QString fileName = urls.first().toLocalFile();



    g.load(fileName.toStdString().c_str());

    QString t(g.print().c_str());
    ui->graph_info->setText(t);
}


