#include "addsailmethod.h"
#include "ui_addsailmethod.h"

addsailmethod::addsailmethod(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addsailmethod)
{
    ui->setupUi(this);

    model=new QStandardItemModel;
    explain=new command_explain;
    usedb=new databases;
    usedb->connect_database();
    set_listhead();
    QString command="select *from discountadd";
    usedb->query->exec(command);
    int row=0;
    while(usedb->query->next())
    {

        QString data="0000#";
        QString goodsname=usedb->query->value(0).toString();
        QString discount_one=usedb->query->value(1).toString();
        QString discount_two=usedb->query->value(2).toString();
        QString discount_three=usedb->query->value(3).toString();
        QString price=usedb->query->value(5).toString();

        float ndOne=discount_one.toFloat();
        float ndTwo=discount_two.toFloat();
        float ndThree=discount_three.toFloat();
        float nprice=price.toFloat();

        float npOne=ndOne*nprice;
        float npTwo=ndTwo*nprice;
        float npThree=ndThree*nprice;

        QString tpOne=QString::number(npOne,10,2)+" / "+discount_one;
        QString tpTwo=QString::number(npTwo,10,2)+" / "+discount_two;
        QString tpThree=QString::number(npThree,10,2)+" / "+discount_three;

        data+=goodsname+"#"+price+"#"+tpOne+"#"+tpTwo+"#"+tpThree+"#"+price+"#";

        disPlay(row++,data);
    }
}

addsailmethod::~addsailmethod()
{
    delete ui;
}

void addsailmethod::set_listhead()
{
    model->setHorizontalHeaderItem(0,new QStandardItem("商品名"));
    model->setHorizontalHeaderItem(1,new QStandardItem("原价"));
    model->setHorizontalHeaderItem(2,new QStandardItem("第一件折后价/折扣"));
    model->setHorizontalHeaderItem(3,new QStandardItem("第二件折后价/折扣"));
    model->setHorizontalHeaderItem(4,new QStandardItem("第三件折后价/折扣"));
    model->setHorizontalHeaderItem(5,new QStandardItem("管理"));
    this->ui->table_goods->setModel(model);
    ui->table_goods->setColumnWidth(2,200);
    ui->table_goods->setColumnWidth(3,200);
    ui->table_goods->setColumnWidth(4,200);
}
void addsailmethod::disPlay(int rol,QString line)
{
    explain->decoding(line);
    for(int m=0;m<explain->command_set.size()-1;m++)
    {
        this->model->setItem(rol,m,new QStandardItem(explain->command_set[m+1]));//行,列
    }
    setlist_button(rol);//添加按钮
}
void addsailmethod::setlist_button(int row)
{
    QPushButton *button_change = new QPushButton("管理");
    ui->table_goods->setIndexWidget(model->index(row,5), button_change);
    connect(button_change, SIGNAL(clicked()), this, SLOT(changeDiscount()));
}
void addsailmethod::changeDiscount()
{
    int row=ui->table_goods->currentIndex().row();
    global_goodsname=model->item(row,0)->text();

    show_changeAdd=new changeAddDiscount;
    show_changeAdd->resize(400,400);
    show_changeAdd->show();

}

void addsailmethod::on_button_add_clicked()
{
    bool ok;
    QString goodsname = QInputDialog::getText(this, tr("QInputDialog::getText()"),
                                         tr("请输入商品名"), QLineEdit::Normal,
                                         QDir::home().dirName(), &ok);
    if(goodsname=="")
        return;
    QString goodsprice;
    QString command="select price from goods where goods_name='"+goodsname+"'";
    usedb->query->exec(command);
    if(usedb->query->next())
    {
        goodsprice=usedb->query->value(0).toString();
        command="select *from discountadd where goodsname='"+goodsname+"'";
        usedb->query->exec(command);
        if(usedb->query->next())
        {
            QMessageBox msgBox;
            msgBox.setText("已添加此商品!");
            msgBox.exec();
        }
        else {
            command="insert into discountadd (goodsname,price) values('"+goodsname+"',"+goodsprice+")";
            bool b=usedb->query->exec(command);
            if(b)
            {
                command="update goods set discount_add=1 where goods_name='"+goodsname+"'";
                usedb->query->exec(command);
                QMessageBox msgBox;
                msgBox.setText("成功加入折扣！");
                msgBox.exec();
                this->hide();
            }
            else {
                QMessageBox msgBox;
                msgBox.setText("加入折扣出错！");
                msgBox.exec();
            }
        }

    }
    else {
        QMessageBox msgBox;
        msgBox.setText("不存在此商品！");
        msgBox.exec();
    }
}
