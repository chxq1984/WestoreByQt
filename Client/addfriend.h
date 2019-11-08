#ifndef ADDFRIEND_H
#define ADDFRIEND_H

#include <QDialog>
#include "tcp.h"
#include "globals.h"
namespace Ui {
class addFriend;
}

class addFriend : public QDialog
{
    Q_OBJECT

public:
    explicit addFriend(QWidget *parent = nullptr);
    ~addFriend();

private:
    Ui::addFriend *ui;
    TCP *communicate;
    void SolveCommand(QString command);
private slots:
    void ReadData();
    void on_button_addfriend_clicked();
};

#endif // ADDFRIEND_H
