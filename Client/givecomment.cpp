#include "givecomment.h"
#include "ui_givecomment.h"
#include <QIntValidator>
giveComment::giveComment(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::giveComment)
{
    ui->setupUi(this);
    ui->line_score->setValidator(new QIntValidator(1,5,this));
    communicate=new TCP;
    communicate->SetUp();
    connect(communicate->tcpClient, SIGNAL(readyRead()), this, SLOT(ReadData()));

}

giveComment::~giveComment()
{
    delete ui;
}
void giveComment::ReadData()
{
    QByteArray buffer = communicate->tcpClient->readAll();
    if(!buffer.isEmpty())
    {
        SolveCommand(buffer);
    }
}
void giveComment::on_button_ok_clicked()
{
    QString score=this->ui->line_score->text();
    QString comment=this->ui->text_comment->toPlainText();
    if(!(score!=""&&comment!=""))
    {
        QMessageBox msgBox;
        msgBox.setText("请将表格填写完整！");
        msgBox.exec();
        return;
    }
    int scoreNumber=score.toInt();
    if(scoreNumber==0)
    {
        QMessageBox msgBox;
        msgBox.setText("评分最低分为1分！");
        msgBox.exec();
        return;
    }
    QString data="1005#"+global_mainwindow_name+"#"+comment+"#"+global_login_username+"#"+score+"#";
    communicate->ConnectToHost("127.0.0.1",8888);
    communicate->SendDate(data);
}
void giveComment::SolveCommand(QString command)
{
    if(command=="1005")
    {
        QMessageBox msgBox;
        msgBox.setText("评论已上传成功！");
        msgBox.exec();
    }
    else if(command=="1015")
    {
        QMessageBox msgBox;
        msgBox.setText("您已对此商品进行过评论！");
        msgBox.exec();
    }
    else if(command=="1045")
    {
        QMessageBox msgBox;
        msgBox.setText("您还未购买过此商品！");
        msgBox.exec();
    }
    communicate->DisConnect();
    this->hide();
}
