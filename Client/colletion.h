#ifndef COLLETION_H
#define COLLETION_H

#include <QDialog>
#include "tcp.h"
#include "globals.h"
#include "command_explain.h"
#include <QStandardItem>
#include <QStandardItemModel>
#include <QPushButton>

namespace Ui {
class colletion;
}

class colletion : public QDialog
{
    Q_OBJECT

public:
    explicit colletion(QWidget *parent = nullptr);
    ~colletion();
private slots:
    void ReadData();
    void deletecollect();
private:
    Ui::colletion *ui;
    TCP*communicate;
    void SolveCommand(QString command);
    void set_listhead();
    command_explain *explain;
    QStandardItemModel*model;
    void disPlay(int row,QString line);
    void setlist_button(int row);//设置列表按钮
};

#endif // COLLETION_H
