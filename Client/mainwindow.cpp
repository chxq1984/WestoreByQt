#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDesktopServices>
#include <QUrl>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    page_number=1;
    ui->setupUi(this);
    model=new QStandardItemModel;
    set_listhead();

    communicate=new TCP;
    communicate->SetUp();
    connect(communicate->tcpClient, SIGNAL(readyRead()), this, SLOT(ReadData()));
    communicate->ConnectToHost("127.0.0.1",8888);

    QString data = "1000#1#";
    communicate->SendDate(data);
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(5*60*1000);//间隔时间ms

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_triggered()
{
    personcentrals=new personcentral;
    personcentrals->resize(400,400);
    personcentrals->show();
}
void MainWindow::disPlay(int rol,QString line)
{

    explain->decoding(line);

    for(int m=0;m<explain->command_set.size()-1;m++)
    {
        this->model->setItem(rol,m,new QStandardItem(explain->command_set[m+1]));//行,列
    }
    setlist_button(rol);//添加按钮

}

void MainWindow::ReadData()
{
    line_number=0;
    explain=new command_explain;
    QByteArray buffer = communicate->tcpClient->readAll();
    if(!buffer.isEmpty())
    {
        SolveCommand(buffer);
    }
}
void MainWindow::refreshdata(int page)
{
    this->model->clear();
    set_listhead();
    QString data = "1000#"+QString::number(page)+"#";
    communicate->SendDate(data);

}
void MainWindow::on_button_refresh_clicked()
{
    page_number=1;
    refreshdata(page_number);
}

void MainWindow::on_button_pre_clicked()
{
    if(page_number-1==0)
        return;
    page_number--;
    refreshdata(page_number);
}

void MainWindow::on_button_next_clicked()
{
    if(line_number!=10)
        return;
    else {
        page_number++;
        refreshdata(page_number);
    }
}
void MainWindow::update()
{
    page_number=1;
    refreshdata(page_number);
}
void MainWindow::setlist_button(int row)
{
    QPushButton *button_comment = new QPushButton("评价");
    QPushButton *button_buy = new QPushButton("购买");
    QPushButton *button_car = new QPushButton("购物车");
    QPushButton *button_collect = new QPushButton("收藏");
    QPushButton *button_showcomment=new QPushButton("查看评价");

    ui->table_goods->setIndexWidget(model->index(row,5), button_comment);
    ui->table_goods->setIndexWidget(model->index(row,6), button_buy);
    ui->table_goods->setIndexWidget(model->index(row,7), button_car);
    ui->table_goods->setIndexWidget(model->index(row,8), button_collect);
    ui->table_goods->setIndexWidget(model->index(row,9),button_showcomment);

    connect(button_buy, SIGNAL(clicked()), this, SLOT(changeto_buyingpage()));//点击购买按钮
    connect(button_car,SIGNAL(clicked()),this,SLOT(addToCar()));//加入购物车
    connect(button_collect,SIGNAL(clicked()),this,SLOT(addToCollection()));//加入收藏夹
    connect(button_comment,SIGNAL(clicked()),this,SLOT(changeToCommentPage()));//转到评价界面
    connect(button_showcomment,SIGNAL(clicked()),this,SLOT(changeToShowComment()));//转到查看评价界面

}
void MainWindow::set_listhead()
{
    model->setHorizontalHeaderItem(0,new QStandardItem("名称"));
    model->setHorizontalHeaderItem(1,new QStandardItem("单价"));
    model->setHorizontalHeaderItem(2,new QStandardItem("数量"));
    model->setHorizontalHeaderItem(3,new QStandardItem("评分"));
    model->setHorizontalHeaderItem(4,new QStandardItem("销量"));
    model->setHorizontalHeaderItem(5,new QStandardItem("评价"));
    model->setHorizontalHeaderItem(6,new QStandardItem("购买"));
    model->setHorizontalHeaderItem(7,new QStandardItem("购物车"));
    model->setHorizontalHeaderItem(8,new QStandardItem("收藏"));
    model->setHorizontalHeaderItem(9,new QStandardItem("查看评价"));
    this->ui->table_goods->setModel(model);
}
void MainWindow::changeto_buyingpage()
{
    global_mainwindow_row=ui->table_goods->currentIndex().row();//得到当前的行
    global_mainwindow_name=model->item(global_mainwindow_row, 0)->text();//获取名称
    buypage=new buying_page;
    buypage->show();
}
void MainWindow::SolveCommand(QString data)
{
    if(data=="1002")
    {
        QMessageBox msgBox;
        msgBox.setText("成功加入购物车！");
        msgBox.exec();
        return;
    }
    else  if(data=="4001"||data=="4011")
    {
        if(data=="4011")
        {
            QMessageBox msgBox;
            msgBox.setText("该商品已在收藏夹中！");
            msgBox.exec();
            return;
        }
        QMessageBox msgBox;
        msgBox.setText("收藏成功！");
        msgBox.exec();
        return;
    }
    else if(data=="1044")//请求页无数据
    {
        page_number--;
        refreshdata(page_number);
        return;
    }
    else if(data=="1422")
    {
        QMessageBox msgBox;
        msgBox.setText("商品已加入购物车！");
        msgBox.exec();
        return;
    }
    QStringList all_goods_information=data.split("/");
    line_number=all_goods_information.count()-1;
    for(int m=0;m<all_goods_information.count()-1;m++)
    {
        disPlay(m,all_goods_information.at(m));
    }
}
void MainWindow::addToCar()
{
    int row=ui->table_goods->currentIndex().row();//得到当前行
    QString goodsName=model->item(row, 0)->text();//获取商品名称
    QString data="1002#"+goodsName+"#"+global_login_username+"#";
    communicate->SendDate(data);
}
void MainWindow::addToCollection()
{
    int row=ui->table_goods->currentIndex().row();//得到当前行
    QString goodsName=model->item(row, 0)->text();//获取商品名称
    QString data="4001#"+goodsName+"#"+global_login_username+"#";
    communicate->SendDate(data);
}
void MainWindow::changeToCommentPage()
{
    global_mainwindow_row=ui->table_goods->currentIndex().row();//得到当前的行
    global_mainwindow_name=model->item(global_mainwindow_row, 0)->text();//获取名称
    commentPage=new giveComment;
    commentPage->resize(600,600);
    commentPage->show();
}
void MainWindow::changeToShowComment()
{
    int row=ui->table_goods->currentIndex().row();//得到当前行
    global_mainwindow_name=model->item(row, 0)->text();//获取商品名称
    showcommentPage=new showComments;
    showcommentPage->resize(500,500);
    showcommentPage->show();
}

void MainWindow::on_action_2_triggered()
{
    QDesktopServices::openUrl(QUrl("http://localhost/tp5/public/index.php/index/index/login"));
}
