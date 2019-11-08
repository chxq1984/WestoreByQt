#ifndef TOTALSAILMETHOD_H
#define TOTALSAILMETHOD_H

#include <QDialog>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QtDebug>
#include "command_explain.h"
#include "databases.h"
namespace Ui {
class totalSailMethod;
}

class totalSailMethod : public QDialog
{
    Q_OBJECT

public:
    explicit totalSailMethod(QWidget *parent = nullptr);
    ~totalSailMethod();
private slots:
    void changediscount();//修改折扣力度

    void on_button_add_clicked();

private:
    Ui::totalSailMethod *ui;
    QStandardItemModel *model;
    void disPlay(int row,QString line);
    void set_listhead();
    command_explain *explain;
    databases *usedb;
    void setlist_button(int row);//设置列表按钮
};

#endif // TOTALSAILMETHOD_H
