#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "personcentral.h"
#include "command_explain.h"
#include "buying_page.h"
#include <QStandardItemModel>
#include <QStandardItem>
#include <QTcpSocket>
#include <QTimer>
#include "tcp.h"
#include "givecomment.h"
#include "showcomments.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_action_triggered();
    void ReadData();
    //void ReadError(QAbstractSocket::SocketError);
    void on_button_refresh_clicked();

    void on_button_pre_clicked();

    void on_button_next_clicked();
    void update();//计时器自动刷新
    void changeto_buyingpage();//button_buy的槽函数，跳转至购买界面
    void addToCar();//购物车
    void addToCollection();//收藏夹
    void changeToCommentPage();//跳转至评价界面
    void changeToShowComment();//跳转至查看评价界面
    void on_action_2_triggered();

private:
    Ui::MainWindow *ui;
    QStandardItemModel *model;
    personcentral *personcentrals;
    command_explain *explain;
    giveComment *commentPage;
    showComments *showcommentPage;
    void disPlay(int row,QString line);//给表格添加数据
    //QTcpSocket*tcpClient;
    buying_page*buypage;
    int page_number;
    int line_number;//返回的信息条数
    void change_page();
    void refreshdata(int page);
    void setlist_button(int row);//设置列表按钮
    void set_listhead();
    TCP *communicate;
    void SolveCommand(QString data);
};

#endif // MAINWINDOW_H
