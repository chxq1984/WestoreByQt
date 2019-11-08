#ifndef GOODS_H
#define GOODS_H

#include <QDialog>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QString>
#include <QDateTime>
#include "goodsin.h"
#include "databases.h"
#include <QTimer>
#include "sailmethod.h"
namespace Ui {
class goods;
}

class goods : public QDialog
{
    Q_OBJECT

public:
    explicit goods(QWidget *parent = nullptr);
    ~goods();

private slots:
    void on_button_in_clicked();

    void on_button_refresh_clicked();
    void update_goods();

    void on_button_sailmethod_clicked();

    void offGoods();//下架商品
    void changePrice();//更改价格
private:
    Ui::goods *ui;
    QStandardItemModel *model;
    goodsin *inshow;
    void disPlay();
    void set_tablehead();
    void refresh_table();
    void setlist_button(int row);
    QDateTime pre_time;
    databases database;
    sailMethod * showSailmethod;
    int grow;
    int gcol;
};

#endif // GOODS_H
