#include "mainwindow.h"
#include "ui_mainwindow.h"




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);



    connect(ui->sliderp11, SIGNAL(valueChanged(int)), this, SLOT(changeValue11(int)));
    connect(ui->sliderp11_2, SIGNAL(valueChanged(int)), this, SLOT(changeValue12(int)));
    connect(ui->sliderp11_3, SIGNAL(valueChanged(int)), this, SLOT(changeValue21(int)));
    connect(ui->sliderp11_4, SIGNAL(valueChanged(int)), this, SLOT(changeValue22(int)));

    connect(ui->spinp11, SIGNAL(valueChanged(double)), this, SLOT(on_pushButton_clicked()));
    connect(ui->spinp11_2, SIGNAL(valueChanged(double)), this, SLOT(on_pushButton_clicked()));
    connect(ui->spinp11_3, SIGNAL(valueChanged(double)), this, SLOT(on_pushButton_clicked()));
    connect(ui->spinp11_4, SIGNAL(valueChanged(double)), this, SLOT(on_pushButton_clicked()));


}

MainWindow::~MainWindow()
{
    delete ui;
}




void MainWindow::changeValue11(int value)
{
    int pos = ui->sliderp11->value();
    ui->spinp11->setValue(pos/100.0);
}
void MainWindow::changeValue12(int value)
{
    int pos = ui->sliderp11_2->value();
    ui->spinp11_2->setValue(pos/100.0);
}
void MainWindow::changeValue21(int value)
{
    int pos = ui->sliderp11_3->value();
    ui->spinp11_3->setValue(pos/100.0);
}
void MainWindow::changeValue22(int value)
{
    int pos = ui->sliderp11_4->value();
    ui->spinp11_4->setValue(pos/100.0);
}



#define HIERARCHY_NUM 2
#define LAYER_NUM 2

bool RUN_HICODE = false;


void rand_inplace(vector<int>& arr)
{
    int len = arr.size();
    for(int i=0;i<len;++i)
        swap(arr[i],arr[rand()%(len-i)+i]);
}

#define NODE_NUM 3000	//结点个数

void MainWindow::on_pushButton_clicked()
{
    vmod.clear();
    QString qstring1;
    QString qstring2;
    int n11 = 25;
        int n12 = 100;
        int n21 = 20;
        int n22 = 60;
        double p11 = ui->spinp11->value();
        double p12 = ui->spinp11_2->value();
        double p21 = ui->spinp11_3->value();
        double p22 = ui->spinp11_4->value();

        vector<vector<int> > parameter_num;
                    vector<vector<double> > parameter_p;
                    vector<int> ivec;
                    vector<double> dvec;

                    /*ivec.push_back(25);   */      ivec.push_back(n11);  //第一层大社团个数
                    /*ivec.push_back(100);  */  	ivec.push_back(n12); //第一层小社团个数
                    /*dvec.push_back(0.005);*/	dvec.push_back(p11); //第一层大社团概率
                    /*dvec.push_back(0.2);	*/	dvec.push_back(p12); 	//第一次小社团概率

                    parameter_num.push_back(ivec);
                    parameter_p.push_back(dvec);

                    ivec.clear();
                    dvec.clear();

                    /*ivec.push_back(20);   */		ivec.push_back(n21); //第二层大社团个数
                    /*ivec.push_back(60);	*/		ivec.push_back(n22); //第二层小社团个数
                    /*dvec.push_back(0.004);*/		dvec.push_back(p21); //第二层大社团概率
                    /*dvec.push_back(0.12);	*/		dvec.push_back(p22); //第二次小社团概率
                    parameter_num.push_back(ivec);
                    parameter_p.push_back(dvec);

                    ivec.clear();
                    dvec.clear();

                    vector<vector<int> > matrix(NODE_NUM,vector<int>(NODE_NUM,0));

                    vector<set<int> > community;

                    vector<int> arr(NODE_NUM);

                    for(int i=0;i<NODE_NUM;++i)
                        arr[i] = i;

                    srand((unsigned)time(NULL));

                   // printf("%f", parameter_p[0][0]);

                    for(int i=0;i<LAYER_NUM;++i)
                    {
                        for(int j=0;j<HIERARCHY_NUM;++j)
                        {
                            //parameter_num = [[25,100],[20,60]]
                            //parameter_num = [[n11,n12],[n21,n22]]
                            int n = parameter_num[i][j];

                            for(int k=0;k<n;++k)
                            {
                                //社团k的结点个数
                                int per_num = NODE_NUM/n;

                                int edge = parameter_p[i][j]*per_num*(per_num-1)/2;

                                set<int> comm;

                                for(int i=k * per_num;i<(k+1) * per_num;++i)
                                    comm.insert(i);

                                while(edge>0)
                                {
                                    int im = rand() % (per_num) + k * per_num;
                                    int in = rand() % (per_num) + k * per_num;

                                    if(im!=in && matrix[im][in]==0)
                                    {
                                        matrix[im][in]=1;
                                        --edge;
                                    }
                                }

                                community.push_back(comm);
                            }
                        }

                        if(i==LAYER_NUM-1)
                            break;

                        rand_inplace(arr);

                        int len = community.size();

                        for(int i=0;i<len;++i)
                        {
                            set<int> iset = community[i];
                            set<int> tmp;

                            set<int>::iterator it = iset.begin();

                            while(it!=iset.end())
                            {
                                tmp.insert(arr[*it]);
                                ++it;
                            }

                            community[i] = tmp;
                        }

                        int row = matrix.size();
                        int col = matrix[0].size();

                        vector<vector<int> > tmp(matrix);

                        for(int i=0;i<row;++i)
                            for(int j=0;j<col;++j)
                                tmp[i][j]=0;

                        for(int i=0;i<row;++i)
                        {
                            for(int j=0;j<col;++j)
                            {
                                if(matrix[i][j]==1)
                                {
                                    tmp[arr[i]][arr[j]]=1;
                                }
                            }
                        }

                        matrix = tmp;

                        for(int i=0;i<NODE_NUM;++i)
                            arr[i] = i;
                    }

                    //FILE * graph = fopen("./graph","w");

                    int row = matrix.size();
                    int col = matrix[0].size();

                    //fprintf(graph,"%d\n",NODE_NUM);

                    int total_edge_num = 0;

                    for(int i=0;i<row;++i)
                    {
                        for(int j=0;j<col;++j)
                        {
                            if(matrix[i][j]==1)
                            {
                                matrix[j][i]=1;
                            }
                        }
                    }



                    //FILE* mod = fopen("./mod.txt","w");

                    int len = community.size();

                    int index = 0;

                    for(int i=0;i<NODE_NUM;++i)
                    {
                        for(int j=0;j<NODE_NUM;++j)
                        {
                            if(matrix[i][j]==1)
                                ++total_edge_num;
                        }
                    }



                    /*parameter_num[0][0]=25; */
                    /*parameter_num[0][1]=125; */
                    /*parameter_num[1][0]=145; */
                    /*parameter_num[1][1]=205; */

                    // parameter_num[0][0]=20;
                    // parameter_num[0][1]=120;
                    // parameter_num[1][0]=145;
                    // parameter_num[1][1]=445 ;

                    parameter_num[0][1] = parameter_num[0][0] + parameter_num[0][1];
                    parameter_num[1][0] = parameter_num[0][1] + parameter_num[1][0];
                    parameter_num[1][1] = parameter_num[1][0] + parameter_num[1][1];

                    vector<pair<int,int>> ipvec;

                    for(int i=0;i<LAYER_NUM;++i)
                    {
                        for(int j=0;j<HIERARCHY_NUM;++j)
                        {
                            while(index<parameter_num[i][j])
                            {
                                int edge = 0;
                                int degree = 0;

                                set<int> iset = community[index];

                                set<int>::iterator it = iset.begin();

                                while(it!=iset.end())
                                {
                                    for(int ii=0;ii<NODE_NUM;++ii)
                                    {
                                        if(matrix[*it][ii]==1)
                                        {
                                            ++degree;
                                        }
                                    }

                                    set<int>::iterator it_inner = iset.begin();

                                    while(it_inner!=iset.end())
                                    {
                                        if(it_inner==it)
                                            ++it_inner;

                                        if(it_inner==iset.end())
                                            break;

                                        if(matrix[*it][*it_inner]==1)
                                            ++edge;
                                        ++it_inner;
                                    }

                                    ++it;
                                }
                                ipvec.push_back(pair<int,int>(edge,degree));
                                ++index;
                            }
                        }
                    }

                    index = 0;

                    for(int i=0;i<LAYER_NUM;++i)
                    {
                        for(int j=0;j<HIERARCHY_NUM;++j)
                        {
                            double ans = 0;

                            while(index<parameter_num[i][j])
                            {
                                double tmp1 = static_cast<double>(ipvec[index].first)/total_edge_num;
                                double tmp2 = static_cast<double>(ipvec[index].second)/total_edge_num;

                                tmp2*=tmp2;

                                ans+=(tmp1-tmp2);
                                ++index;
                            }
                            vmod.push_back(ans);
                            //fprintf(mod,"%.4f\n",ans);
                            qstring1.sprintf("%.4f\n",ans);
                            qstring2 += qstring1;

                            //allmod[allmod.size() - 1].push_back(ans);
                        }
                    }


                    ui->textEdit->setText(qstring2);


                    ui->mod->setValue(vmod[0]);
                    ui->mod_2->setValue(vmod[1]);
                    ui->mod_3->setValue(vmod[2]);
                    ui->mod_4->setValue(vmod[3]);

                    ui->pmod->setValue(vmod[0] * 100);
                    ui->pmod_2->setValue(vmod[1] * 100);
                    ui->pmod_3->setValue(vmod[2] * 100);
                    ui->pmod_4->setValue(vmod[3] * 100);


}
