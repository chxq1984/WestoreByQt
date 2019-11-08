#ifndef GOODSIN_H
#define GOODSIN_H

#include <QDialog>
#include "databases.h"
namespace Ui {
class goodsin;
}

class goodsin : public QDialog
{
    Q_OBJECT

public:
    explicit goodsin(QWidget *parent = nullptr);
    ~goodsin();

private slots:
    void on_button_ok_clicked();

private:
    Ui::goodsin *ui;
    databases database;
    int check();
};

#endif // GOODSIN_H
