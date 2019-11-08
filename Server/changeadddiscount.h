#ifndef CHANGEADDDISCOUNT_H
#define CHANGEADDDISCOUNT_H

#include <QDialog>
#include "global.h"
#include <QDebug>
#include "databases.h"
namespace Ui {
class changeAddDiscount;
}

class changeAddDiscount : public QDialog
{
    Q_OBJECT

public:
    explicit changeAddDiscount(QWidget *parent = nullptr);
    ~changeAddDiscount();

private slots:
    void on_button_ok_clicked();

private:
    Ui::changeAddDiscount *ui;
    databases *usedb;
    int check(QString discound);
};

#endif // CHANGEADDDISCOUNT_H
