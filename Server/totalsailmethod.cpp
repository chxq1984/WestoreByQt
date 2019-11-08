#include "totalsailmethod.h"
#include "ui_totalsailmethod.h"
#include <QInputDialog>
#include <QDir>
#include <QMessageBox>
totalSailMethod::totalSailMethod(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::totalSailMethod)
{
    ui->setupUi(this);
    model=new QStandardItemModel;
    explain=new command_explain;
    usedb=new databases;
    usedb->connect_database();

    set_listhead();
    QString command="select *from discounttotal";
    usedb->query->exec(command);
    int row=0;
    while(usedb->query->next())
    {
        QString data="0000#";
        QString goodsname=usedb->query->value(0).toString();
        QString discount=usedb->query->value(1).toString();
        QString price=usedb->query->value(3).toString();

        float ndiscount=discount.toFloat();
        float nprice=price.toFloat();
        float totalprice=ndiscount*nprice;
        QString toprice=QString::number(totalprice,10,2);

        data+=goodsname+"#"+price+"#"+toprice+"#"+discount+"#";
        disPlay(row++,data);
    }

}

totalSailMethod::~totalSailMethod()
{
    delete ui;
}

void totalSailMethod::set_listhead()
{
    model->setHorizontalHeaderItem(0,new QStandardItem("商品名"));
    model->setHorizontalHeaderItem(1,new QStandardItem("原价"));
    model->setHorizontalHeaderItem(2,new QStandardItem("折后价"));
    model->setHorizontalHeaderItem(3,new QStandardItem("折扣"));
    model->setHorizontalHeaderItem(4,new QStandardItem("操作"));
    this->ui->table_discount->setModel(model);
}
void totalSailMethod::disPlay(int rol,QString line)
{
    explain->decoding(line);
    for(int m=0;m<explain->command_set.size()-1;m++)
    {
        this->model->setItem(rol,m,new QStandardItem(explain->command_set[m+1]));//行,列
    }
    setlist_button(rol);//添加按钮
}
void totalSailMethod::setlist_button(int row)
{
    QPushButton *button_change = new QPushButton("修改");
    ui->table_discount->setIndexWidget(model->index(row,4), button_change);
    connect(button_change, SIGNAL(clicked()), this, SLOT(changediscount()));
}
void totalSailMethod::changediscount()
{//5000+商品名称+折扣数
    bool ok;
    QString text = QInputDialog::getText(this, tr("QInputDialog::getText()"),
                                         tr("请输入折扣数"), QLineEdit::Normal,
                                         QDir::home().dirName(), &ok);
    if(text=="")
        return;
    for(int m=0;m<text.length();m++)
    {
        if(!((text[m]>='0'&&text[m]<='9')||text[m]=='.'))
        {
            QMessageBox msgBox;
            msgBox.setText("请输入合法折扣数据！");
            msgBox.exec();
            return;
        }
    }
    float discountN=text.toFloat();
    if(!(discountN>0&&discountN<=1))
    {
        QMessageBox msgBox;
        msgBox.setText("折扣范围为0~1！");
        msgBox.exec();
        return;
    }
    int row=ui->table_discount->currentIndex().row();
    QString goodsname=model->item(row,0)->text();

    if(text=="1")//移出
    {
        QString command="delete from discounttotal where goodsname='"+goodsname+"'";
        bool a=usedb->query->exec(command);
        command="update goods set discount_all=0 where goods_name='"+goodsname+"'";
        bool b=usedb->query->exec(command);
        QMessageBox msgBox;
        if(a&&b)
        {
            msgBox.setText("已移出折扣！");
            msgBox.exec();
        }
        this->hide();
        return;
    }

    QString command="update discounttotal set discount="+text+" where goodsname='"+goodsname+"'";
    bool a=usedb->query->exec(command);
    if(a)
    {
        QMessageBox msgBox;
        msgBox.setText("修改完成！");
        msgBox.exec();
        this->hide();
    }
    else {
        QMessageBox msgBox;
        msgBox.setText("修改出错！");
        msgBox.exec();
    }
}

void totalSailMethod::on_button_add_clicked()
{
    bool ok;
    QString goodsname = QInputDialog::getText(this, tr("QInputDialog::getText()"),
                                         tr("请输入商品名称"), QLineEdit::Normal,
                                         QDir::home().dirName(), &ok);
    if(goodsname=="")
        return;
    QString goodsprice;
    QString command="select price from goods where goods_name='"+goodsname+"'";
    usedb->query->exec(command);
    if(usedb->query->next())
    {
        goodsprice=usedb->query->value(0).toString();
        command="select *from discounttotal where goodsname='"+goodsname+"'";
        usedb->query->exec(command);
        if(usedb->query->next())
        {
            QMessageBox msgBox;
            msgBox.setText("已添加此商品!");
            msgBox.exec();
            return;
        }
        else {
            command="insert into discounttotal (goodsname,price) values('"+goodsname+"',"+goodsprice+")";
            bool b=usedb->query->exec(command);
            if(b)
            {
                command="update goods set discount_all=1 where goods_name='"+goodsname+"'";
                usedb->query->exec(command);
                QMessageBox msgBox;
                msgBox.setText("成功加入折扣！");
                msgBox.exec();
            }
            else {
                QMessageBox msgBox;
                msgBox.setText("加入折扣出错！");
                msgBox.exec();
            }
        }
        this->hide();

    }
    else {
        QMessageBox msgBox;
        msgBox.setText("不存在此商品！");
        msgBox.exec();
    }
}
