#ifndef ADDSAILMETHOD_H
#define ADDSAILMETHOD_H

#include <QDialog>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QtDebug>
#include <QInputDialog>
#include <QDir>
#include <QMessageBox>
#include "command_explain.h"
#include "databases.h"
#include "changeadddiscount.h"
#include "global.h"
namespace Ui {
class addsailmethod;
}

class addsailmethod : public QDialog
{
    Q_OBJECT

public:
    explicit addsailmethod(QWidget *parent = nullptr);
    ~addsailmethod();
private slots:
    void changeDiscount();
    void on_button_add_clicked();

private:
    Ui::addsailmethod *ui;
    QStandardItemModel *model;
    void disPlay(int row,QString line);
    void set_listhead();
    command_explain *explain;
    databases *usedb;
    void setlist_button(int row);//设置列表按钮
    changeAddDiscount *show_changeAdd;
};

#endif // ADDSAILMETHOD_H
