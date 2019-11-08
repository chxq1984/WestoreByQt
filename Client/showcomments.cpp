#include "showcomments.h"
#include "ui_showcomments.h"

showComments::showComments(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::showComments)
{
    ui->setupUi(this);
    model=new QStandardItemModel;
    communicate=new TCP;
    communicate->SetUp();
    connect(communicate->tcpClient, SIGNAL(readyRead()), this, SLOT(ReadData()));
    communicate->ConnectToHost("127.0.0.1",8888);
    QString data="2004#"+global_mainwindow_name+"#";
    communicate->SendDate(data);
    explain=new command_explain;
    set_listhead();
}

showComments::~showComments()
{
    delete ui;
}
void showComments::ReadData()
{
    QByteArray buffer = communicate->tcpClient->readAll();
    if(!buffer.isEmpty())
    {
        SolveCommand(buffer);
    }
}
void showComments::SolveCommand(QString command)
{
    explain->decoding_list(command);
    for(int m=0;m<explain->line_number;m++)
    {
        disPlay(m,explain-> all_line_information.at(m));
    }
}
void showComments::set_listhead()
{
    model->setHorizontalHeaderItem(0,new QStandardItem("用户名"));
    model->setHorizontalHeaderItem(1,new QStandardItem("评价"));
    this->ui->table_comment->setModel(model);
}
void showComments::disPlay(int rol,QString line)
{
    explain->decoding(line);
    for(int m=0;m<explain->command_set.size()-1;m++)
    {
        this->model->setItem(rol,m,new QStandardItem(explain->command_set[m+1]));//行,列
    }
}
