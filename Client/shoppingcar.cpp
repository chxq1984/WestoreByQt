#include "shoppingcar.h"
#include "ui_shoppingcar.h"
#include <QQueue>
shoppingcar::shoppingcar(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::shoppingcar)
{
    ui->setupUi(this);
    model=new QStandardItemModel;
    communicate=new TCP;
    communicate->SetUp();
    connect(communicate->tcpClient, SIGNAL(readyRead()), this, SLOT(ReadData()));
    communicate->ConnectToHost("127.0.0.1",8888);
    QString data="1200#"+global_login_username+"#";
    communicate->SendDate(data);
    explain=new command_explain;
    set_listhead();
}

shoppingcar::~shoppingcar()
{
    delete ui;
}
void shoppingcar::ReadData()
{
    QByteArray buffer = communicate->tcpClient->readAll();
    if(!buffer.isEmpty())
    {
        SolveCommand(buffer);
    }
}
void shoppingcar::SolveCommand(QString command)
{
    if(command=="1")
    {
        QMessageBox msgBox;
        msgBox.setText("购买成功！");
        msgBox.exec();
        this->hide();
        return;
    }
    else if(command=="1102")
    {
        QMessageBox msgBox;
        msgBox.setText("已移出购物车！");
        msgBox.exec();
        this->hide();
        return;
    }
    else if(command.mid(0,4)=="4004")//购买商品数量不足
    {//  4004#11011
        QStringList list=command.split("#");
        remainLack(list[1]);
        this->hide();
        return;
    }

    explain->decoding_list(command);
    number_line=explain->line_number;
    checkstate=new int[number_line]();

    for(int m=0;m<explain->line_number;m++)
    {
        disPlay(m,explain-> all_line_information.at(m));
    }
}
void shoppingcar::set_listhead()
{
    model->setHorizontalHeaderItem(0,new QStandardItem("商品名称"));
    model->setHorizontalHeaderItem(1,new QStandardItem("价格"));
    model->setHorizontalHeaderItem(2,new QStandardItem("选择"));
    model->setHorizontalHeaderItem(3,new QStandardItem("数量"));
    this->ui->table_shoppingcar->setModel(model);
}
void shoppingcar::disPlay(int rol,QString line)
{

    explain->decoding(line);
    for(int m=0;m<explain->command_set.size()-1;m++)
    {
        this->model->setItem(rol,m,new QStandardItem(explain->command_set[m+1]));//行,列
    }
    setlist_button(rol);//添加按钮
    this->model->setItem(rol,3,new QStandardItem(0));//行,列
}
void shoppingcar::setlist_button(int row)
{
    QCheckBox *box_check = new QCheckBox();
    ui->table_shoppingcar->setIndexWidget(model->index(row,2), box_check);
    connect(box_check, SIGNAL(stateChanged(int)), this, SLOT(checkchange(int)));
}


void shoppingcar::on_button_buy_clicked()//购买
{
    int line_number=model->rowCount();
    int isChecked=0;
    int isFill=1;//是否将表格填写完整
    int isNotAllZero=0;//是否输入全为0
    int isAllNumber=1;//是否输入全为数字
    for(int m=0;m<line_number;m++)
    {
        if(checkstate[m])
        {
            isChecked=1;
            QString goodsnumber=model->item(m,3)->text();

            for(int k=0;k<goodsnumber.length();k++)
            {
                if(!(goodsnumber[k]>="0"&&goodsnumber[k]<="9"))
                {
                    isAllNumber=0;
                    break;
                }
            }
            if(!isAllNumber)
                break;

            if(goodsnumber=="")
                isFill=0;
            isNotAllZero+=goodsnumber.toInt();
        }
    }
    if(!isAllNumber)
    {
        QMessageBox msgBox;
        msgBox.setText("请输入数字！");
        msgBox.exec();
        return;
    }
    if(!isChecked)
    {
        QMessageBox msgBox;
        msgBox.setText("您未选择任何商品！");
        msgBox.exec();
        return;
    }
    if(!isFill)
    {
        QMessageBox msgBox;
        msgBox.setText("您未将选中的商品购买数量填写完整！");
        msgBox.exec();
        return;
    }
    if(!isNotAllZero)
    {
        QMessageBox msgBox;
        msgBox.setText("您未购买任何商品！");
        msgBox.exec();
        return;
    }

    bool ok;
    QString text = QInputDialog::getText(this, tr("请输入密码"),
            tr("密码："), QLineEdit::Password,
            "", &ok);

    if(text=="")
        return;

   if(text!=global_login_password)
   {
       QMessageBox msgBox;
       msgBox.setText("密码不正确！");
       msgBox.exec();
       return;
   }


    QString data;
    float totalprice=0;
    for(int m=0;m<line_number;m++)
    {
        if(checkstate[m])//1004+商品名称+数量+价格+用户名/。。。
        {
            QString goodsname=model->item(m,0)->text();
            QString goodsnumber=model->item(m,3)->text();
            if(goodsnumber=="0")//若输入数量为0则直接跳过
            {
                continue;
            }
            QString goodsprice=model->item(m,1)->text();
            data+="1004#";
            float price=goodsprice.toFloat();
            int number=goodsnumber.toInt();
            totalprice+=price*number;
            data+=goodsname+"#"+goodsnumber+"#"+goodsprice+"#";
            data+=global_login_username+"#/";
        }

    }

    if(global_balance>=totalprice)
    communicate->SendDate(data);
    else {
        QMessageBox msgBox;
        msgBox.setText("余额不足！");
        msgBox.exec();
    }
}
void shoppingcar::checkchange(int state)
{
    int row=ui->table_shoppingcar->currentIndex().row();
    if (state == Qt::Checked) // 选中
    {
        checkstate[row]=1;
    }
    else if(state==Qt::Unchecked)//未选中
    {
        checkstate[row]=0;
    }
}

void shoppingcar::on_button_out_clicked()//移出购物车
{
    int line_number=model->rowCount();
    QString data;
    for(int m=0;m<line_number;m++)
    {
        if(checkstate[m])//1102+商品名称+用户名/。。。
        {
            data+="1102#";
            QString goodsname=model->item(m,0)->text();
            data+=goodsname+"#"+global_login_username+"#/";
        }
    }
    qDebug()<<data;
    communicate->SendDate(data);
}
void shoppingcar::remainLack(QString data)
{
    QString wrongGoods;
    //2019.10.24.0:09  data即为1011格式的数据，1为购买成功，0为失败
    QQueue<int> mark;
    for(int n=0;n<data.length();n++)
    {
        if(data[n]=="0")
        {
            mark.push_back(n);
        }
    }
    int line_number=model->rowCount();
    int flag=-1;
    for(int m=0;m<line_number&&mark.size()>0;m++)
    {
        int first=mark.front();
        if(checkstate[m])
        {
            QString goodsnumber=model->item(m,3)->text();
            if(goodsnumber=="0")
                continue;
            flag++;
        }
        if(first==flag)//此处即为出错位置
        {
            QString goodsname=model->item(m,0)->text();
            wrongGoods+=goodsname+"数量不足\n";
            mark.pop_front();
        }
    }
    wrongGoods+="其余商品已完成购买";
    QMessageBox msgBox;
    msgBox.setText(wrongGoods);
    msgBox.exec();

}
