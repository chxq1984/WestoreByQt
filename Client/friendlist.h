#ifndef FRIENDLIST_H
#define FRIENDLIST_H

#include <QDialog>
#include "tcp.h"
#include "globals.h"
#include <QStandardItemModel>
#include <QStandardItem>
#include "command_explain.h"
#include "addfriend.h"
namespace Ui {
class friendlist;
}

class friendlist : public QDialog
{
    Q_OBJECT

public:
    explicit friendlist(QWidget *parent = nullptr);
    ~friendlist();
private slots:
    void ReadData();
    void on_button_addfreind_clicked();
    void deletefriend();
private:
    Ui::friendlist *ui;
    TCP *communicate;
    void SolveCommand(QString command);
    QStandardItemModel *model;
    void disPlay(int row,QString line);
    void set_listhead();
    command_explain *explain;
    void setlist_button(int row);//设置列表按钮
    addFriend *show_addfriend;
};

#endif // FRIENDLIST_H
