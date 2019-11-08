#include "command_action.h"
#include <QSqlDatabase>//用于连接数据库
#include <QSqlQuery>
#include <qDebug>
#include <QMessageBox>
#include <QThread>
#include <QRandomGenerator>
command_action::command_action()
{
    qDebug()<<QThread::currentThreadId()<<"command action";//
}
void command_action:: decoding(QString frombuffer)
{
    int separate[40];
    int top=-1;
    for(int m=0;m<frombuffer.length();m++)
    {
        if(frombuffer[m]=='#')
        {
            separate[++top]=m;//记录#所在的位置
        }
    }
    command_set.push_back(frombuffer.mid(0,4));

    for(int m=0;m<top;m++)//command_set内即为分解的指令
    {
        command_set.push_back(frombuffer.mid(separate[m]+1,separate[m+1]-separate[m]-1));
    }
}
void command_action::connect_mysql()
{
    /*if(QSqlDatabase::contains("qt_sql_default_connection"))
    {
        db = QSqlDatabase::database("qt_sql_default_connection");
    }
    else
    {
        db = QSqlDatabase::addDatabase("QMYSQL");
    }*/
    quint32 value = QRandomGenerator::global()->generate();
    if ( true == QSqlDatabase::contains(QString::number(value)))
    {
        db = QSqlDatabase::database(QString::number(value));
    }
    else
    {
        db = QSqlDatabase::addDatabase("QMYSQL",QString::number(value));
    }
    db.setHostName("localhost");
    db.setDatabaseName("WeStore");
    db.setUserName("root");
    db.setPassword("453480049");
    if (!db.open())
        qDebug() << "Failed to connect to root mysql admin";
    else
        qDebug() << "open";
    query=new QSqlQuery(db);
    //db.exec("SET NAMES 'GBK'");//防止乱码
}
QString action_register::action()
{
    connect_mysql();
    if(command_set[0]=="2001")//查重账号
    {
        bool a=query->exec("select * from customer_login where account='"+command_set[1]+"'");

        if(a)
        {
            if(query->next())//有信息则进入,即存在账号
            {
                return "0000";
            }
            else
            {
                query->exec("insert into customer_login(account) values('"+command_set[1]+"')");
                return "0001";
                //insert into customer_login(account) values('command_set[1]')//插入账号
            }
        }
        else
        {
            QMessageBox msgBox;
            msgBox.setText("服务端数据库查找账号失败！");
            msgBox.exec();
        }
    }
    else if(command_set[0]=="2002")//将账号信息写入数据库
    {
        QString temp="update customer_login set password='"+command_set[1]+"',";
        temp+="email='"+command_set[2]+"',";
        temp+="IP='"+command_set[3]+"',";
        temp+="PORT='"+command_set[4]+"'";
        temp+="where account='"+command_set[5]+"'";
        //qDebug()<<temp;
        query->exec(temp);
        temp="select id from customer_login ";
        temp+="where account='"+command_set[5]+"'";
        qDebug()<<temp;
        query->exec(temp);
        query->next();
        QString temid=query->value(0).toString();
        temp="insert into customer_shop values ("+temid+",'"+command_set[5]+"',0)";
        qDebug()<<temp;
        bool a=query->exec(temp);
        if(a)
        {
            return "0002";
        }
        else
        {
            QMessageBox msgBox;
            msgBox.setText("服务端数据库写入账号信息失败！");
            msgBox.exec();
        }
    }
    else if(command_set[0]=="2000")//检测账号密码是否错误,登录
    {
        QString account=command_set[1];
        QString password =command_set[2];
        QString command="select password,username from customer_login where account='"+account+"'";
        //select password from customer_login where account='account';
        query->exec(command);
        query->next();
        if(query->value(0)==password)
        {
            QString username=query->value(1).toString();
            query->exec("update customer_login set last_login_time=null where account='"+account+"'");

            command="select balance from customer_shop where username='"+username+"'";
            query->exec(command);
            query->next();
            QString balance=query->value(0).toString();
            return "0003#"+username+"#"+balance+"#";//更新登录时间
        }
        else {
            return "0004#";
        }
    }
    else if(command_set[0]=="2010")//重置忘记密码
    {
        QString newpassword=command_set[1];
        QString emaile=command_set[2];
        QString command="update customer_login set password='"+newpassword+"'";
        command+="where email='"+emaile+"'";
        bool a=query->exec(command);
        if(a)
        {
            return "2010";
        }
        else {
            QMessageBox msgBox;
            msgBox.setText("新密码写入失败！");
            msgBox.exec();
        }
    }
    else if(command_set[0]=="0006")//设置用户名
    {
        int flag=0;
        QString command="select username from customer_login";
        query->exec(command);
        while(query->next())
        {
            if(query->value(0).toString()==command_set[1])
            {
                flag=1;
                break;
            }
        }
        if(flag)
            return "0007";
        else {
            query->exec("update customer_login set username='"+command_set[1]+"'"+"where account='"+command_set[2]+"'");
            command="select id from customer_login where account="+command_set[2];
            query->exec(command);
            query->next();
            QString id=query->value(0).toString();
            command="update customer_shop set username='"+command_set[1]+"' where flag_id="+id;
            bool a=query->exec(command);
            if(a)
            return "0006";
            else{
                QMessageBox msgBox;
                msgBox.setText("新用户名写入失败！");
                msgBox.exec();
            }
        }
    }
    else if(command_set[0]=="0008")//返回用户信息
    {
        QString command="select username,email,id from customer_login where account='"+command_set[1]+"'";
        query->exec(command);
        QString data_return="0008#";
        QString id;
        if(query->next())
        {
            data_return+=query->value(0).toString()+"#"+query->value(1).toString()+"#";
            id=query->value(2).toString();
        }
        else {
            QMessageBox msgBox;
            msgBox.setText("用户信息读取失败！");
            msgBox.exec();
        }
        command="select balance from customer_shop where flag_id="+id;
        query->exec(command);
        qDebug()<<command;
        if(query->next())
        {
            data_return+=query->value(0).toString()+"#";
        }
        else {
            QMessageBox msgBox;
            msgBox.setText("用户余额读取失败！");
            msgBox.exec();
        }
        return data_return;
    }
    else if(command_set[0]=="0009")//充值
    {
        //金额密码账号
        QString account=command_set[3];
        QString money_in=command_set[1];
        QString password=command_set[2];
        QString command="select password from customer_login where account='"+account+"'";
        bool a=query->exec(command);
        if(a&&query->next())
        {
            if(query->value(0).toString()==password)
            {
                QString command="update customer_shop set balance=balance+"+money_in;
                query->exec(command);
                return "0009";
            }
            else {
               return "0010";
            }
        }
        else {
            QMessageBox msgBox;
            msgBox.setText("用户密码读取失败！");
            msgBox.exec();
        }
    }
}
QString acton_shopping_list::action()
{
    qDebug()<<QThread::currentThreadId()<<" 商品列表";//

    connect_mysql();
    if(command_set[0]=="1000")//传回商品列表
    {
        int pagenum=command_set[1].toInt();
        int begin=(pagenum-1)*10;
        int end=pagenum*10;//name price number score salesnumber
        //QString command="select *from goods where id>begin and id<=end";
        QString command="select *from goods where remain_number>0";
        query->exec(command);
        QString return_data;
        int num=5;
        QStringList alldata;
        while(query->next())
        {
            QString oneline="1000#";
            for(int n=0;n<num;n++)
            oneline+=query->value(n).toString()+"#";
            oneline+="/";
            alldata.push_back(oneline);
        }
        //qDebug()<<qMin(end,alldata.size());
        for(int m=begin;m<qMin(end,alldata.size());m++)
        {
            return_data+=alldata.at(m);
        }
        //qDebug()<<return_data;
        if(return_data=="")
            return "1044";//此页没有商品
        return return_data;
    }
    else if(command_set[0]=="1001")//客户购买商品
    {// 1001+商品名称+数量+密码+账号
        QString goods_name=command_set[1];
        QString goods_number=command_set[2];
        QString password=command_set[3];
        QString account=command_set[4];
        QString username;
        QString command="select password,username from customer_login where account='"+account+"'";
        query->exec(command);
        if(query->next())//核对密码
        {
            if(password!=query->value(0).toString())
                return "1011";
            username=query->value(1).toString();
        }
        command="select price,remain_number from goods where goods_name='"+goods_name+"'";//获得商品价格
        QString command_custor="select balance from customer_shop where username='"+username+"'";
        query->exec(command);
        double temprice;
        if(query->next())
        {
            qDebug()<<query->value(0).toString()<<" "<<query->value(1).toString();
            int temnum=query->value(1).toInt();
            int num=goods_number.toInt();
            if(num>temnum)
                return "1011";//数量不足
            temprice=(query->value(0).toDouble())*num;

            if(num==1)
            {
                temprice*=global_distotal*global_disAddOne;
            }
            else if(num==2)
            {
                if(global_disAddTwo!=1)
                    temprice*=global_distotal*global_disAddTwo;
                else
                {
                    temprice*=global_distotal*global_disAddOne;
                }
            }
            else if(num>=3)
            {
                if(global_disAddThree!=1)
                    temprice*=global_distotal*global_disAddThree;
                else if(global_disAddTwo!=1)
                    temprice*=global_distotal*global_disAddTwo;
                else
                {
                    temprice*=global_distotal*global_disAddOne;
                }
            }
            //qDebug()<<"价格为"+QString::number(temprice,'10',5);
            query->exec(command_custor);
            query->next();
            double customer_balance=query->value(0).toDouble();

            if(customer_balance<temprice)//余额不足
            {
                return "1011";
            }
            else {


                command="start transaction";//开启事务
                query->exec(command);

                command="update goods set remain_number=remain_number-"+goods_number+" where goods_name='"+goods_name+"' and remain_number>="+goods_number;
                query->exec(command);

                command="SELECT ROW_COUNT()";
                query->exec(command);
                query->next();
                int effect=query->value(0).toInt();
                if(effect==0)//影响条数为0，即数量不足
                {
                    command="commit";
                    query->exec(command);//提交事务
                    return "1011";
                }
                command="commit";
                query->exec(command);//提交事务


                command="update customer_shop set balance=balance-";
                command+=QString::number(temprice,'10',5)+" where username='"+username+"'";
                bool a=query->exec(command);
                if(a)
                {
                    command="select *from having_buy where username='"+username+"'"+" and goodsname='"+goods_name+"'";
                    query->exec(command);

                    if(query->next())//已购买过商品则购买人数不增加，只增加销量
                    {
                        qDebug()<<command;
                        command="update goods set sales_number=sales_number+"+goods_number+" where goods_name='"+goods_name+"'";
                        query->exec(command);
                        command="insert into having_buy (username,goodsname,buyingtime)values ('"+username+"','"+goods_name+"',null)";
                        query->exec(command);
                        return "1001";
                    }
                    command="update goods set sales_number=sales_number+"+goods_number+",sales_person=sales_person+1 where goods_name ='"+goods_name+"'";
                    query->exec(command);
                    command="insert into having_buy (username,goodsname,buyingtime)values ('"+username+"','"+goods_name+"',null)";
                    query->exec(command);
                    return "1001";
                }
                else {
                    return "1011";
                }
            }

        }
    }
    else if(command_set[0]=="1111")//传回优惠信息
    {
        //1111+标志1+标志2+折扣一+折扣二1+折扣二2+折扣二3
        QString data="1111#";
        QString goodsname=command_set[1];
        QString command="select discount_all,discount_add from goods where goods_name='"+goodsname+"'";
        query->exec(command);
        query->next();
        QString discount_all=query->value(0).toString();
        QString discount_add=query->value(1).toString();

        global_flag_distotal=discount_all.toInt();
        global_flag_disadd=discount_add.toInt();

        data+=discount_all+"#"+discount_add+"#";
        if(discount_all!="0")
        {
            command="select discount from discounttotal where goodsname='"+goodsname+"'";
            query->exec(command);
            query->next();
            QString discount=query->value(0).toString();

            global_distotal=discount.toFloat();

            data+=discount+"#";
        }
        else {
           global_distotal=1;
           data+="1#";
        }
        if(discount_add!="0")
        {
            command="select dcount_one,dcount_two,dcount_three from discountadd where ";
            command+="goodsname='"+goodsname+"'";
            query->exec(command);
            query->next();
            QString dcount_one=query->value(0).toString();
            QString dcount_two=query->value(1).toString();
            QString dcount_three=query->value(2).toString();

            global_disAddOne=dcount_one.toFloat();
            global_disAddTwo=dcount_two.toFloat();
            global_disAddThree=dcount_three.toFloat();

            data+=dcount_one+"#"+dcount_two+"#"+dcount_three+"#";
        }
        else {
            global_disAddOne=1;
            global_disAddTwo=1;
            global_disAddThree=1;
            data+="1#1#1#";
        }
        return data;
    }
}
QString action_friend_list::action()
{
    connect_mysql();
    if(command_set[0]=="3006")//返回好友信息
    {
        QString username=command_set[1];
        QString return_data;
        QString command="select friend_username,friend_account from friend where user_name='"+username+"'";
        query->exec(command);
        while(query->next())
        {
            QString dataline="3006#";
            dataline+=query->value(0).toString()+"#";
            dataline+=query->value(1).toString()+"#/";
            return_data+=dataline;
        }
        return return_data;
    }
    else if(command_set[0]=="3000")//添加好友
    {//3000+好友账号+用户名
        QString friend_account=command_set[1];
        QString my_username=command_set[2];
        QString command="select username from customer_login where account='"+friend_account+"'";
        query->exec(command);
        if(query->next())
        {
            QString friend_username=query->value(0).toString();
            command="insert into friend (user_name,friend_username,friend_account)values('"+my_username+"','"+friend_username+"','"+friend_account+"')";
            qDebug()<<command;
            bool a=query->exec(command);
            if(a)
            {
                return "3000";
            }
            else {
                QMessageBox msgBox;
                msgBox.setText("写入好友信息失败！");
                msgBox.exec();
                return "3010";
            }
        }
        else {
            return "3010";
        }
    }
    else if(command_set[0]=="3001")//3001+好友账号+用户名,删除好友
    {
        QString friend_account=command_set[1];
        QString my_username=command_set[2];
        QString command="delete from friend where user_name='"+my_username+"' and friend_account="+friend_account;
        bool a=query->exec(command);
        if(a)
        {
            return "3001";
        }
        else {
            QMessageBox msgBox;
            msgBox.setText("删除好友失败！");
            msgBox.exec();
            return "3011";
        }
    }
}
QString acton_shopping_car::action()//购物车类
{
    connect_mysql();//1002+商品名称+用户名
    if(command_set[0]=="1002")//加入购物车
    {
        QString found="select * from shoppingcar where goodsname='"+command_set[1]+"'";
        query->exec(found);
        if(query->next())
        {
            return "1422";//商品已存在
        }

        QString command="select price from goods where goods_name='"+command_set[1]+"'";
        query->exec(command);
        query->next();
        double price=query->value(0).toDouble();
        command="insert into shoppingcar(username,goodsname,price,addtime)values";
        command+="('"+command_set[2]+"','"+command_set[1]+"',"+QString::number(price,'10',5)+",null)";
        qDebug()<<command;
        bool a=query->exec(command);
        if(a)
        {
            return "1002";
        }
    }
    else if(command_set[0]=="1200")//返回购物车列表
    {
        QString username=command_set[1];
        QString command="select goodsname,price from shoppingcar where username='"+username+"'";
        query->exec(command);
        QString data;
        while(query->next())
        {
            data+="1200#";
            QString goodsname=query->value(0).toString();
            QString goodsprice=query->value(1).toString();
            data+=goodsname+"#";
            data+=goodsprice+"#/";
        }
        return data;
    }
    else if(command_set[0]=="1004")//1004+商品名称+数量+价格+用户名/
    {
        //处理：在客户端存储余额，即在客户端即完成判断，因此数据在此处直接数据库处理即可
        QString goodsname=command_set[1];
        QString goodsnumber=command_set[2];
        QString username=command_set[4];
        QString command;

        command="select discount_all,discount_add from goods where goods_name='"+goodsname+"'";
        query->exec(command);
        query->next();
        int flag_distotal=query->value(0).toInt();
        int flag_disadd=query->value(1).toInt();
        float disall,disone,distwo,disthree;
        disall=disone=distwo=disthree=1;
        if(flag_distotal)
        {
            command="select discount from discounttotal where goodsname='"+goodsname+"'";
            query->exec(command);
            query->next();
            disall=query->value(0).toFloat();
        }
        if(flag_disadd)
        {
            command="select dcount_one,dcount_two,dcount_three from discountadd where goodsname='"+goodsname+"'";
            query->exec(command);
            query->next();
            disone=query->value(0).toFloat();
            distwo=query->value(1).toFloat();
            disthree=query->value(2).toFloat();
        }

        float goodsprice=command_set[3].toFloat();
        float totalprice=goodsprice*(goodsnumber.toInt());
        int num=goodsnumber.toInt();
        //处理折扣
        if(num==1)
        {
            totalprice*=disall*disone;
        }
        else if(num==2)
        {
            if(distwo!=1)
                totalprice*=disall*distwo;
            else
            {
                totalprice*=disall*disone;
            }
        }
        else if(num>=3)
        {
            if(disthree!=1)
                totalprice*=disall*disthree;
            else if(distwo!=1)
                totalprice*=disall*distwo;
            else
            {
                totalprice*=disall*disone;
            }
        }

        QString total=QString::number(totalprice,'10',5);


        command="start transaction";
        query->exec(command);
        command="update goods set remain_number=remain_number-"+goodsnumber+" where goods_name='"+goodsname+"' and remain_number>="+goodsnumber;
        query->exec(command);//减少商品数量
        command="SELECT ROW_COUNT()";
        query->exec(command);
        query->next();
        int effect=query->value(0).toInt();
        if(effect==0)
        {
            command="commit";
            query->exec(command);
            return "0";
        }
        command="commit";
        query->exec(command);


        command="update customer_shop set balance=balance-";
        command+=total+" where username='"+username+"'";

        qDebug()<<command;
        bool a=query->exec(command);//减少余额
        if(a)
        {
            command="select *from having_buy where username='"+username+"'"+" and goodsname='"+goodsname+"'";
            query->exec(command);//查看是否购买过此商品

            if(query->next())//已购买过商品则购买人数不增加，只增加销量
            {
                command="update goods set sales_number=sales_number+"+goodsnumber+" where goods_name='"+goodsname+"'";
                query->exec(command);//增加销量

                for(int m=0;m<goodsnumber.toInt();m++)//增加购买信息
                {
                    command="insert into having_buy (username,goodsname,buyingtime)values ('"+username+"','"+goodsname+"',null)";
                    query->exec(command);
                }
                command="delete from shoppingcar where goodsname='"+goodsname+"' and username='"+username+"'";
                query->exec(command);
                return "1";
            }
            command="update goods set sales_number=sales_number+"+goodsnumber+",sales_person=sales_person+1 where goods_name ='"+goodsname+"'";
            query->exec(command);//增加销量与购买人数
            for(int m=0;m<goodsnumber.toInt();m++)
            {
                command="insert into having_buy (username,goodsname,buyingtime)values ('"+username+"','"+goodsname+"',null)";
                query->exec(command);
            }
            command="delete from shoppingcar where goodsname='"+goodsname+"' and username='"+username+"'";
            query->exec(command);
            return "1";
        }
        else {
            return "0";
        }

    }
    else if(command_set[0]=="1102")//移出购物车
    {
        //1102+商品名称+用户名
        QString goodsname=command_set[1];
        QString username=command_set[2];
        QString command="delete from shoppingcar where username='"+username+"' and goodsname='"+goodsname+"'";
        bool a=query->exec(command);
        if(a)
            return "1";
        else {
            return "0";
        }
    }
}
QString action_collection::action()
{
    connect_mysql();//4001+商品名称+用户名
    if(command_set[0]=="4001")//加入收藏夹
    {
        QString goodsname=command_set[1];
        QString username=command_set[2];
        QString command="select *from collection where goodsname='"+goodsname+"' and username='"+username+"'";
        query->exec(command);
        if(query->next())
            return "4011";
        command="select price from goods where goods_name='"+command_set[1]+"'";
        query->exec(command);
        query->next();
        double price=query->value(0).toDouble();
        command="insert into collection values(default,'"+username+"','"+goodsname+"',"+QString::number(price,'10',5)+")";
        qDebug()<<command;
        bool a=query->exec(command);
        if(a)
        {
            return "4001";
        }
    }
    else if(command_set[0]=="4005")//返回收藏夹界面
    {
        QString username=command_set[1];
        QString command="select goodsname,price from collection where username='"+username+"'";
        QString data;
        query->exec(command);
        while(query->next())
        {
           data+="4005#";
           QString goodsname=query->value(0).toString();
           QString goodsprice=query->value(1).toString();
           data+=goodsname+"#";
           data+=goodsprice+"#/";
        }
        return data;
    }
    else if(command_set[0]=="4002")//取消收藏
    {
        QString goodsname=command_set[1];
        QString username=command_set[2];
        QString command="delete from collection where goodsname='"+goodsname+"' and username='"+username+"'";
        bool a=query->exec(command);
        if(a)
        {
            return "4002";
        }
        else {
            QMessageBox msgBox;
            msgBox.setText("数据库收藏失败！");
            msgBox.exec();
        }
    }
}
QString action_comment::action()
{//1005+商品名称+评价内容+用户名+评分
    connect_mysql();
    if(command_set[0]=="1005")//商品评价
    {//select id from goods where goods_name='Apple';
        QString goodsname=command_set[1];
        QString commentcontent=command_set[2];
        QString username=command_set[3];
        double score=command_set[4].toDouble();

        QString commandFound="select *from having_buy where username='"+username+"' and goodsname='"+goodsname+"'";
        query->exec(commandFound);
        if(!query->next())//未购买过此商品
        {
            return "1045";
        }

        QString command="select id,score,sales_person from goods where goods_name='"+goodsname+"'";
        query->exec(command);
        query->next();
        QString goodsid=query->value(0).toString();//商品id
        double goodscore=query->value(1).toDouble();//商品分数
        int goodsnumber=query->value(2).toInt();//商品数量
        command="select *from comment where id="+goodsid+" and username='"+username+"'";

        query->exec(command);
        if(query->next())//评论已存在
        {
            return "1015";
        }
        else
        {
            goodsnumber--;
            command="insert into comment values ("+goodsid+",'"+commentcontent+"','"+username+"')";
            query->exec(command);
            double sumscore=goodscore*goodsnumber+score;
            goodsnumber++;
            double endscore=1.0*sumscore/(goodsnumber);
            command="update goods set score="+QString::number(endscore,'f',1)+" where id="+goodsid;
            query->exec(command);
            return "1005";
        }
    }
    else if(command_set[0]=="2004")//返回评分与评价
    {//2004+评价+评价用户名/...
        QString goodsname=command_set[1];
        QString command="select content,username from goods natural join comment where goods_name='"+goodsname+"'";
        query->exec(command);
        QString data;
        while(query->next())
        {
            QString content=query->value(0).toString();
            QString username=query->value(1).toString();
            data+="2004#";
            data+=content+"#"+username+"#/";
        }
        return data;
    }
}
