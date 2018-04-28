#include "comm.h"
#include "ui_comm.h"

#include <QFileDialog>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QList>
#include <QUrl>

#include "../../os.h"

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

    double f1_unweighted = Communities::F1_unweighted(comm1, *comm2);
    double f1_weighted = Communities::F1_weighted(*comm2, comm1);

    double jaccard_f1_unweighted = Communities::Jaccard_F1_unweighted(comm1, *comm2);
    double jaccard_f1_weighted = Communities::Jaccard_F1_weighted(*comm2, comm1);

    double f1_precision_unweighted = Communities::f1(comm1, *comm2);
    double f1_recall_unweighted = Communities::f1(*comm2, comm1);

    double f1_precision_weighted = Communities::wf1(comm1, *comm2);
    double f1_recall_weighted = Communities::wf1(*comm2, comm1);

    tmp.sprintf("F1 = %lf, wF1 = %lf\n", f1_unweighted, f1_weighted);
    s += tmp;
    tmp.sprintf("JF1 = %lf, wJF1 = %lf\n", jaccard_f1_unweighted, jaccard_f1_weighted);
    s += tmp;
    tmp.sprintf("F1_precision = %lf, wF1_precision = %lf\n", f1_precision_unweighted, f1_recall_unweighted);
    s += tmp;
    tmp.sprintf("F1_recall = %lf, wF1_recall = %lf\n", f1_precision_weighted, f1_recall_weighted);
    s += tmp;

    /*
    vector<int> v_index;
    vector<double> v_value;
    string outdir = "f1/";
    os::mkdir(outdir);
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

*/
    info->setText(s);

    ui->comm_name_2->setText("Detected  Communities");
    w2->ui->comm_name_2->setText("Truth Communities");

/*
    tmp.sprintf("%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf", nmi, f1, p, r, jf1, jp, jr);
    *cliq = tmp;
*/

    QTableWidget * tableWidget = ui->t;
    tableWidget->clearContents();

    tableWidget->setRowCount(comm1.size());     //设置行数为10
    tableWidget->setColumnCount(3);   //设置列数为5
    QStringList header;
    header<<"id"<<"size"<<"nodes";
    tableWidget->setHorizontalHeaderLabels(header);

    for (int i = 0; i < comm1.size(); ++i)
    {
        tableWidget->setItem(i,0,new QTableWidgetItem(QString::number(i)));
        tableWidget->setItem(i,1,new QTableWidgetItem(QString::number(comm1.comms[i].size())));
        QString qnodes;
        for (int j = 0; j < comm1.comms[i].nodes.size(); ++j)
        {
            qnodes += QString::number(comm1.comms[i].nodes[j]) + " ";
        }
        tableWidget->setItem(i,2,new QTableWidgetItem(qnodes));
    }

    tableWidget->selectRow(0);
    tableWidget->show();
}

void Comm::on_t_cellClicked(int row, int column)
{
    Community & c = comm1.comms[row];

    pair<double, int> res = Communities::findSimilar
            (c, *comm2);

    //pair<double, int> res = c.JaccardPrecision(*comm2, 0);
    double p = res.first;
    int index = res.second;

    //index = w2->ui->t->currentRow();

    vector<int> cap = Communities::intersection(c.nodes, comm2->comms[index].nodes);
    vector<int> join = Communities::setunion(c.nodes, comm2->comms[index].nodes);
    int len = cap.size();
    int joinlen = join.size();

    QString s;
    s.sprintf("%d / %d = %lf , jaccard = %lf ", len, c.nodes.size(), (double)len/c.nodes.size(), (double)len/joinlen);
    cinfo->setText(s);

    w2->ui->t->selectRow(index);
}
