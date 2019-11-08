#ifndef MONEYIN_H
#define MONEYIN_H

#include <QDialog>
#include <QString>
#include "globals.h"
#include <QTcpSocket>
#include <QMessageBox>
#include "tcp.h"
namespace Ui {
class moneyin;
}

class moneyin : public QDialog
{
    Q_OBJECT

public:
    explicit moneyin(QWidget *parent = nullptr);
    ~moneyin();

private slots:
    void on_pushButton_clicked();
    void ReadData();
private:
    Ui::moneyin *ui;
    TCP *communicate;
    void SolveCommand(QString command);
    float inmoney;
};

#endif // MONEYIN_H
