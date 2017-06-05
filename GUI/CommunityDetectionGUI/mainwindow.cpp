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
    ui->path->setText(fileName);
}

void MainWindow::on_pushButton_clicked()
{
    QString fn = ui->path->toPlainText();
    fn.remove("file:///");
    g.load(fn.toStdString().c_str());

    QString t(g.print().c_str());
    ui->graph_info->setText(t);
}

void MainWindow::on_loadcomm1_clicked()
{
    QString fn = ui->path->toPlainText();
    fn.remove("file:///");
    c1.load(fn.toStdString().c_str());

    c1.calcModularity(g);
    QString t(c1.print(true).c_str());
    ui->comm1_info->setText(t);

    double nmi = c1.calcNMI(c2);
    QString s;
    s.sprintf("NMI = %lf", nmi);
    ui->comm12->setText(s);
}

void MainWindow::on_loadcomm2_clicked()
{
    QString fn = ui->path->toPlainText();
    fn.remove("file:///");
    c2.load(fn.toStdString().c_str());

    c2.calcModularity(g);
    QString t(c2.print(true).c_str());
    ui->comm2_info->setText(t);

    double nmi = c2.calcNMI(c1);
    QString s;
    s.sprintf("NMI = %lf", nmi);
    ui->comm12->setText(s);
}
