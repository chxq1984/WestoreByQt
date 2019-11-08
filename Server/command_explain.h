#ifndef COMMAND_EXPLAIN_H
#define COMMAND_EXPLAIN_H
#include <QString>
#include <QStringList>
#include <QVector>

class command_explain
{
public:
    command_explain();
    void decoding(QString frombuffer);//传入QString类型的指令码将其解码至command_set中
    void decoding_list(QString data);//将列表信息进行解码
    QVector<QString> command_set;
    QStringList all_line_information;
    int line_number;

};

#endif // COMMAND_EXPLAIN_H
