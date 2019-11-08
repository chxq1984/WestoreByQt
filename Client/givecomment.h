#ifndef GIVECOMMENT_H
#define GIVECOMMENT_H

#include <QDialog>
#include "globals.h"
#include "tcp.h"
#include <QMessageBox>
namespace Ui {
class giveComment;
}

class giveComment : public QDialog
{
    Q_OBJECT

public:
    explicit giveComment(QWidget *parent = nullptr);
    ~giveComment();
private slots:
    void ReadData();
    void on_button_ok_clicked();

private:
    Ui::giveComment *ui;
    TCP *communicate;
    void SolveCommand(QString command);
};

#endif // GIVECOMMENT_H
