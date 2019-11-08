#include "colletion.h"
#include "ui_colletion.h"

colletion::colletion(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::colletion)
{
    ui->setupUi(this);
    model=new QStandardItemModel;

    communicate=new TCP;
    communicate->SetUp();
    connect(communicate->tcpClient, SIGNAL(readyRead()), this, SLOT(ReadData()));
    communicate->ConnectToHost("127.0.0.1",8888);
    QString data="4005#"+global_login_username+"#";
    communicate->SendDate(data);
    explain=new command_explain;
    set_listhead();
}

colletion::~colletion()
{
    delete ui;
}
void colletion::set_listhead()
{
    model->setHorizontalHeaderItem(0,new QStandardItem("商品名"));
    model->setHorizontalHeaderItem(1,new QStandardItem("价格"));
    model->setHorizontalHeaderItem(2,new QStandardItem("操作"));
    this->ui->table_collection->setModel(model);
}
void colletion::ReadData()
{
    QByteArray buffer = communicate-> tcpClient->readAll();
    if(!buffer.isEmpty())
    {
        SolveCommand(buffer);
    }
}
void colletion::SolveCommand(QString command)
{
    if(command=="4002")
    {
        QMessageBox msgBox;
        msgBox.setText("已将此商品移出收藏夹");
        msgBox.exec();
        this->hide();
        return;
    }
    explain->decoding_list(command);
    for(int m=0;m<explain->line_number;m++)
    {
        disPlay(m,explain-> all_line_information.at(m));
    }
}
void colletion::disPlay(int rol,QString line)
{

    explain->decoding(line);
    for(int m=0;m<explain->command_set.size()-1;m++)
    {
        this->model->setItem(rol,m,new QStandardItem(explain->command_set[m+1]));//行,列
    }
    setlist_button(rol);//添加按钮

}
void colletion::setlist_button(int row)
{
    QPushButton *button_delete = new QPushButton("删除");
    ui->table_collection->setIndexWidget(model->index(row,2), button_delete);
    connect(button_delete, SIGNAL(clicked()), this, SLOT(deletecollect()));
}
void colletion::deletecollect()//移出收藏夹
{
    int row=ui->table_collection->currentIndex().row();
    QString goodsname=model->item(row,0)->text();
    QString data="4002#"+goodsname+"#";
    QString username=global_login_username;
    data+=username+"#";
    communicate->SendDate(data);
}
