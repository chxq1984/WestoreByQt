#include "changeadddiscount.h"
#include "ui_changeadddiscount.h"
#include <QMessageBox>
changeAddDiscount::changeAddDiscount(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::changeAddDiscount)
{
    ui->setupUi(this);
    usedb=new databases;
    usedb->connect_database();
}

changeAddDiscount::~changeAddDiscount()
{
    delete ui;
}

void changeAddDiscount::on_button_ok_clicked()
{
    QString disone=ui->line_one->text();
    QString distwo=ui->line_two->text();
    QString disthree=ui->line_three->text();

    if(!(disone!=""&&distwo!=""&&disthree!=""))
    {
        QMessageBox msgBox;
        msgBox.setText("请将表单填写完整！");
        msgBox.exec();
        return;
    }
    int flagOne=check(disone);
    int flagTwo=check(distwo);
    int flagThree=check(disthree);
    if(!(flagOne&&flagTwo&&flagThree))
    {
        QMessageBox msgBox;
        msgBox.setText("请输入合法数据！");
        msgBox.exec();
        return;
    }

    if(disone=="1"&&distwo=="1"&&disthree=="1")
    {
        QString command="delete from discountadd where goodsname='"+global_goodsname+"'";
        usedb->query->exec(command);
        command="update goods set discount_add=0 where goods_name='"+global_goodsname+"'";
        usedb->query->exec(command);
        QMessageBox msgBox;
        msgBox.setText("已移出折扣列表！");
        msgBox.exec();
        this->hide();
        return;
    }

    QString command="update discountadd set dcount_one="+disone+",dcount_two="+distwo+",dcount_three="+disthree;
    command+=" where goodsname='"+global_goodsname+"'";
    usedb->query->exec(command);
    QMessageBox msgBox;
    msgBox.setText("已修改折扣信息！");
    msgBox.exec();
    this->hide();
}
int changeAddDiscount::check(QString discound)
{
    for(int m=0;m<discound.length();m++)
    {
        if(!((discound[m]>='0'&&discound[m]<='9')||discound[m]=='.'))
        {
            return 0;
        }
    }
    float discountN=discound.toFloat();
    if(!(discountN>0&&discountN<=1))
    {
        return 0;
    }
}
