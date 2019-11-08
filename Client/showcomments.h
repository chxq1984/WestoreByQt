#ifndef SHOWCOMMENTS_H
#define SHOWCOMMENTS_H

#include <QDialog>
#include "globals.h"
#include "tcp.h"
#include "command_explain.h"
#include <QStandardItem>
#include <QStandardItemModel>
namespace Ui {
class showComments;
}

class showComments : public QDialog
{
    Q_OBJECT

public:
    explicit showComments(QWidget *parent = nullptr);
    ~showComments();

private slots:
    void ReadData();
private:
    Ui::showComments *ui;
    TCP *communicate;
    void set_listhead();
    void disPlay(int row,QString line);
    command_explain *explain;
    void SolveCommand(QString command);
    QStandardItemModel*model;
};

#endif // SHOWCOMMENTS_H
