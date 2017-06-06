#include "comm.h"
#include "ui_comm.h"

#include <QFileDialog>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QList>
#include <QUrl>

Comm::Comm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Comm)
{
    ui->setupUi(this);
    setAcceptDrops(true);
}

Comm::~Comm()
{
    delete ui;
}


void Comm::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}
void Comm::dropEvent(QDropEvent *event)
{
    QList<QUrl> urls = event->mimeData()->urls();
    if (urls.isEmpty())
       return;
    QString fileName = urls.first().toLocalFile();



    comm1.load(fileName.toStdString().c_str());

    comm1.calcModularity(*pg);
    QString t(comm1.print().c_str());
    ui->comm_info->setText(t);

    ui->comm_name->setText(fileName);

    QString s, tmp;
    double nmi = comm1.calcNMI(*comm2);
    tmp.sprintf("NMI = %lf\n", nmi);
    s += tmp;

    vector<int> v_index;
    vector<double> v_value;
    string outdir = "f1/";
    Communities::mkdir(outdir);
    double p = Communities::Precision(comm1, *comm2, v_index, v_value, outdir).first;
    double r = Communities::Recall(comm1, *comm2, v_index, v_value, outdir).first;
    double f1 = Communities::F1Score(comm1, *comm2, outdir).first;

    tmp.sprintf("F1Score = %lf, Precision = %lf, Recall = %lf\n", f1, p, r);
    s += tmp;

    double jp = Communities::JaccardPrecision(comm1, *comm2, v_index, v_value, outdir).first;
    double jr = Communities::JaccardRecall(comm1, *comm2, v_index, v_value, outdir).first;
    double jf1 = Communities::JaccardF1Score(comm1, *comm2, outdir).first;

    tmp.sprintf("JF1Score = %lf,J Precision = %lf, JRecall = %lf", jf1, jp, jr);
    s += tmp;

    info->setText(s);



}