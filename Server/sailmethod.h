#ifndef SAILMETHOD_H
#define SAILMETHOD_H

#include <QDialog>
#include "totalsailmethod.h"
#include "addsailmethod.h"
namespace Ui {
class sailMethod;
}

class sailMethod : public QDialog
{
    Q_OBJECT

public:
    explicit sailMethod(QWidget *parent = nullptr);
    ~sailMethod();

private slots:
    void on_button_total_clicked();

    void on_button_add_clicked();

private:
    Ui::sailMethod *ui;
    totalSailMethod  *showTotal;
    addsailmethod *showAdd;
};

#endif // SAILMETHOD_H
