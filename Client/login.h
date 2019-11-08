#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include "registered.h"
#include "mainwindow.h"
#include "forget_password.h"
#include "tcp.h"
#include "command_explain.h"
namespace Ui {
class login;
}

class login : public QDialog
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = nullptr);
    ~login();

private slots:
    void on_pushButton_clicked();

    void on_button_login_clicked();
    void ReadData();//保留
    void on_button_forget_clicked();

private:
    Ui::login *ui;
    registered *regis;
    MainWindow *mainwin;
    forget_password *forget;
    TCP *communicate;
    void SolveCommand(QString command);
    command_explain *explain;
};

#endif // LOGIN_H
