#include "goods.h"
#include "ui_goods.h"
#include <qDebug>
goods::goods(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::goods)
{
    ui->setupUi(this);

    model=new QStandardItemModel;
    refresh_table();
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update_goods()));//计时器
    timer->start(5*60*1000);//间隔时间ms
}

goods::~goods()
{
    delete ui;
}

void goods::on_button_in_clicked()
{
    inshow=new goodsin;
    inshow->resize(400,400);
    inshow->show();
}
void goods::set_tablehead()
{
    model->setHorizontalHeaderItem(0,new QStandardItem("名称"));
    model->setHorizontalHeaderItem(1,new QStandardItem("单价"));
    model->setHorizontalHeaderItem(2,new QStandardItem("数量"));
    model->setHorizontalHeaderItem(3,new QStandardItem("评分"));
    model->setHorizontalHeaderItem(4,new QStandardItem("销售量"));
    model->setHorizontalHeaderItem(5,new QStandardItem("下架"));
    model->setHorizontalHeaderItem(6,new QStandardItem("更改价格"));
    this->ui->table_goods->setModel(model);
}
void goods::disPlay()
{
    QString command="select *from goods";
    database.connect_database();
    database.query->exec(command);
    int row=0,col_sum=5;
    while(database.query->next())
    {
        for(int col=0;col<col_sum;col++)
        {
            QString content=database.query->value(col).toString();
            model->setItem(row,col,new QStandardItem(content));
        }
        setlist_button(row);
        row++;
    }
}
void goods::refresh_table()
{
    pre_time = QDateTime::currentDateTime();//更新时间
    this->model->clear();
    set_tablehead();
    disPlay();
}

void goods::on_button_refresh_clicked()
{
    refresh_table();
}
void goods::update_goods()
{
    refresh_table();
}

void goods::on_button_sailmethod_clicked()
{
    showSailmethod=new sailMethod;
    showSailmethod->resize(400,400);
    showSailmethod->show();
}
void goods::setlist_button(int row)
{
    QPushButton *button_offGoods=new QPushButton("下架");
    QPushButton *button_changePrice=new QPushButton("价格");

    ui->table_goods->setIndexWidget(model->index(row,5),button_offGoods);
    ui->table_goods->setIndexWidget(model->index(row,6),button_changePrice);

    connect(button_offGoods,SIGNAL(clicked()),this,SLOT(offGoods()));
    connect(button_changePrice,SIGNAL(clicked()),this,SLOT(changePrice()));
}
void goods::offGoods()
{
    grow=ui->table_goods->currentIndex().row();
    QString goodsName=model->item(grow,0)->text();
    QString command="delete from goods where goods_name='"+goodsName+"'";
    database.query->exec(command);

    QMessageBox msgBox;
    msgBox.setText("下架成功！");
    msgBox.exec();
}
void goods::changePrice()
{
    grow=ui->table_goods->currentIndex().row();
    QString goodsName=model->item(grow,0)->text();

    bool ok;
    QString text = QInputDialog::getText(this, tr("QInputDialog::getText()"),
                                         tr("请输入更改的商品价格"), QLineEdit::Normal,
                                         QDir::home().dirName(), &ok);
    for(int m=0;m<text.length();m++)
    {
        if(!(text[m]>='0'&&text[m]<='9'))
        {
            QMessageBox msgBox;
            msgBox.setText("请输入合法字符！");
            msgBox.exec();
            return;
        }
    }

    QString command="update goods set price="+text+" where goods_name='"+goodsName+"'";
    bool a=database.query->exec(command);
    if(a)
    {
        QMessageBox msgBox;
        msgBox.setText("修改完成！");
        msgBox.exec();
    }
}
