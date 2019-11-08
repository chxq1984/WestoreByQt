#include "goodsin.h"
#include "ui_goodsin.h"
#include <QMessageBox>
#include <QIntValidator>
goodsin::goodsin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::goodsin)
{
    ui->setupUi(this);
    ui->line_number->setValidator(new QIntValidator(1,10000,this));
}

goodsin::~goodsin()
{
    delete ui;
}

void goodsin::on_button_ok_clicked()//上架商品
{
    int flag= check();
    if(!flag)
    {
        return;
    }
    database.connect_database();
    QString goods_name=ui->line_name->text();
    QString goods_price=ui->line_price->text();
    QString goods_number=ui->line_number->text();

    database.query->exec("select price from goods where goods_name='"+goods_name+"'");
    if(database.query->next())//若已存在商品则考虑价格
    {
        if(database.query->value(0)==goods_price)//价格相同则更改数量即可
        {
            database.query->exec("update goods set remain_number=remain_number+"+goods_number+" where goods_name='"+goods_name+"'");
        }
        else
        {
            QMessageBox msgBox;
            msgBox.setText("您设置的新单价与原单价不同");
            msgBox.setInformativeText("您是否选择将单价进行更新？");
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            msgBox.setDefaultButton(QMessageBox::No);
            int ret = msgBox.exec();
            if(ret==QMessageBox::Yes)
            {
                database.query->exec("update goods set price="+goods_price+" where goods_name='"+goods_name+"'");
            }
            database.query->exec("update goods set remain_number=remain_number+"+goods_number+" where goods_name='"+goods_name+"'");
        }
    }
    else
    {
         QString command="insert into goods (goods_name,price,remain_number) values";
         command+="('"+goods_name+"',"+goods_price+","+goods_number+")";
         bool a=database.query->exec(command);
        if(!a)
        {
            QMessageBox msgBox;
            msgBox.setText("插入新商品信息出错！");
            msgBox.exec();
        }
    }
    bool a=database.query->exec("insert into shopper_in (in_number,flag_id) values("+goods_number+",1)");
    if(!a)//insert into shopper_in (in_number,flag_id) values(goods_number,1)"
    {
        QMessageBox msgBox;
        msgBox.setText("插入新商品进货信息出错！");
        msgBox.exec();
    }
    QMessageBox msgBox;
    msgBox.setText("已成功上架商品！");
    msgBox.exec();
    this->hide();
}
int goodsin::check()
{
    QString goodsname=ui->line_name->text();
    QString goodsprice=ui->line_price->text();
    QString goodsnumber=ui->line_number->text();
    if(!(goodsname!=""&&goodsprice!=""&&goodsnumber!=""))
    {
        QMessageBox msgBox;
        msgBox.setText("表格未填写完整！");
        msgBox.exec();
        return 0;
    }
    int numberN=goodsnumber.toInt();
    if(numberN==0)
    {
        QMessageBox msgBox;
        msgBox.setText("至少应上架一件商品！");
        msgBox.exec();
        return 0;
    }
    for(int m=0;m<goodsprice.length();m++)
    {
        if(!((goodsprice[m]>='0'&&goodsprice[m]<='9')||goodsprice[m]=='.'))
        {
            QMessageBox msgBox;
            msgBox.setText("请输入合法价格信息！");
            msgBox.exec();
            return 0;
        }
    }
    return 1;
}
