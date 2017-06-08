#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <QFileDialog>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QList>
#include <QUrl>

#include <QClipboard>


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

    ui->comm1->w2 = ui->comm2;
    ui->comm2->w2 = ui->comm1;

    ui->comm1->cliq = &res;
    ui->comm2->cliq = &res;
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



void MainWindow::on_pushButton_clicked()
{
    QClipboard *board = QApplication::clipboard();//使用 QApplication::clipboard() 函数获得系统剪贴板对象。这个函数的返回值是 QClipboard 指针。
    board->setText(res); //通过 setText()，setImage() 或者 setPixmap() 函数可以将数据放置到剪贴板内，也就是通常所说的剪贴或者复制的操作；
}
