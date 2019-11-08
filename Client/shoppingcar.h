#ifndef SHOPPINGCAR_H
#define SHOPPINGCAR_H

#include <QDialog>
#include "globals.h"
#include "tcp.h"
#include "command_explain.h"
#include <QMessageBox>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QCheckBox>
#include <QLineEdit>
#include <QInputDialog>
#include <QDir>
namespace Ui {
class shoppingcar;
}

class shoppingcar : public QDialog
{
    Q_OBJECT

public:
    explicit shoppingcar(QWidget *parent = nullptr);
    ~shoppingcar();
private slots:
    void ReadData();
    void on_button_buy_clicked();
    void checkchange(int state);
    void on_button_out_clicked();

private:
    Ui::shoppingcar *ui;
    QStandardItemModel*model;
    TCP *communicate;
    void SolveCommand(QString command);
    command_explain *explain;
    void disPlay(int row,QString line);
    void set_listhead();
    void setlist_button(int row);
    void remainLack(QString data);//提示购物车某商品数量不足
    int number_line;//购物车条数
    int *checkstate;//是否被选中
};

#endif // SHOPPINGCAR_H
