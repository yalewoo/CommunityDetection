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

    setWindowTitle("Modularity,NMI,F1");

    setAcceptDrops(true);

    g.load("F:/HICODE_SUB/syn/3000_0611/graph");

    QString t(g.print().c_str());
    ui->graph_info->setText(t);


    ui->comm1->pg = &g;
    ui->comm1->comm2 = &ui->comm2->comm1;

    ui->comm2->pg = &g;
    ui->comm2->comm2 = &ui->comm1->comm1;

    ui->comm1->info = ui->info;
    ui->comm2->info = ui->info;

    ui->comm1->cinfo = ui->community_info;
    ui->comm2->cinfo = ui->community_info;

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

void MainWindow::updatasubinfo()
{
    QString s;
    s.sprintf("%d nodes", subnodes.size());
    ui->subinfo->setText(s);
}

void MainWindow::on_add1_clicked()
{
    vector<Community> & comms = ui->comm1->comm1.comms;
    for (size_t i = 0; i < comms.size(); ++i)
    {
        for (size_t j = 0; j < comms[i].size(); ++j)
        {
            subnodes.insert(comms[i].nodes[j]);
        }
    }
    updatasubinfo();
}

void MainWindow::on_add2_clicked()
{
    vector<Community> & comms = ui->comm2->comm1.comms;
    for (size_t i = 0; i < comms.size(); ++i)
    {
        for (size_t j = 0; j < comms[i].size(); ++j)
        {
            subnodes.insert(comms[i].nodes[j]);
        }
    }

    updatasubinfo();
}

void MainWindow::on_getsub_clicked()
{
    Graph subg = g.getSubGraph(subnodes);
    QString s;
    s.sprintf("%d nodes, %d edges", subg.getNodeNum(), subg.getEdgeNum());
    ui->subinfo->setText(s);
}
