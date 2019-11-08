#include "sailmethod.h"
#include "ui_sailmethod.h"

sailMethod::sailMethod(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::sailMethod)
{
    ui->setupUi(this);
}

sailMethod::~sailMethod()
{
    delete ui;
}

void sailMethod::on_button_total_clicked()
{
    showTotal=new totalSailMethod;
    showTotal->resize(811,446);
    showTotal->show();
    this->hide();
}

void sailMethod::on_button_add_clicked()
{
    showAdd=new addsailmethod;
    showAdd->resize(1130,800);
    showAdd->show();
    this->hide();
}
