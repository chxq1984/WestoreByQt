#include "mysocket.h"
mysocket::mysocket()
{
    connect(this, SIGNAL(readyRead()), this, SLOT(recvData()));
}
void mysocket::recvData()
{
    QByteArray buffer = this->readAll();
    if(buffer.isEmpty()) return;
    frombuffer=buffer;
    choose_action();
}
void mysocket::choose_action()
{
    QString begin_four=frombuffer.mid(0,4);//根据开头四位进行选择
    if(begin_four=="2001"||begin_four=="2002"||begin_four=="2000"||begin_four=="2010"||begin_four=="0006"||begin_four=="0008"||begin_four=="0009")
    {
        coma=new action_register;
        if(begin_four=="2002")
        {
            frombuffer+=this->peerAddress().toString().split("::ffff:")[1]+"#"+QString::number(this->peerPort())+"#";//对数据加工，加入客户端IP地址与PORT，以便进行存储
            frombuffer+=current_account+"#";//用于找到数据库插入位置
        }
        else if(begin_four=="2001")
            current_account=frombuffer.mid(5,frombuffer.length()-6);
    }
    else if(begin_four=="1000"||begin_four=="1001"||begin_four=="1111")
    {
        coma=new acton_shopping_list;
    }
    else if(begin_four=="3006"||begin_four=="3000"||begin_four=="3001")//好友类
    {
        coma=new action_friend_list;
    }
    else if(begin_four=="1002"||begin_four=="1200"||begin_four=="1004"||begin_four=="1102")//购物车类
    {
        coma=new acton_shopping_car;
    }
    else if(begin_four=="4001"||begin_four=="4005"||begin_four=="4002")//收藏夹类
    {
        coma=new action_collection;
    }
    else if(begin_four=="1005"||begin_four=="2004")//评论类
    {
        coma=new action_comment;
    }
    //继续添加其余指令
    if(begin_four!="1004"&&begin_four!="1102")
    coma->decoding(frombuffer);
    else {
        //处理列表数据
        coma->explain=new command_explain;
        coma->explain->decoding_list(frombuffer);
        QString check;
        QString result;
        for(int m=0;m<coma->explain->line_number;m++)
        {
            check+="1";
            coma->command_set.clear();
            coma->decoding(coma->explain->all_line_information.at(m));
            result+=coma->action();
        }
        if(check==result)
        {
           QString response;
           if(begin_four=="1004")//购物车购买商品
           response="1";
           else if(begin_four=="1102")//移出购物车
           response="1102";

           //this->write(response.toLatin1());
           this->write(response.toUtf8());
        }
        else {
            //购买出错:数量不足/并发
            if(begin_four=="1004")
            {
                QString response="4004#"+result;
                //this->write(response.toLatin1());
                this->write(response.toUtf8());
            }
        }
         delete coma;
         return;
    }
    QString response=coma->action();
    if(response.length())//有回复内容
    {
        this->write(response.toUtf8());
    }
    delete coma;
}
