#include "command_explain.h"

command_explain::command_explain()
{

}
void command_explain::decoding(QString frombuffer)
{
    command_set.clear();//初始化
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
    //qDebug()<<frombuffer.mid(0,4);
    for(int m=0;m<top;m++)//command_set内即为分解的指令
    {//qDebug()<<frombuffer.mid(separate[m]+1,separate[m+1]-separate[m]-1);
        command_set.push_back(frombuffer.mid(separate[m]+1,separate[m+1]-separate[m]-1));
    }
}
void command_explain::decoding_list(QString data)
{
    all_line_information=data.split("/");
    line_number=all_line_information.count()-1;
}
