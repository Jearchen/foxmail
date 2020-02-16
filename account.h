#ifndef ACCOUNT_H
#define ACCOUNT_H
#include<QDialog>
#include<QtGui>
#include<mainwindow.h>
#include<QFile>
#include<QTcpSocket>
#include<QComboBox>
#include<QLabel>
#include "setting.h"
#include "mainwindow.h"
class account:public QWidget
{
     Q_OBJECT
public:
    account(QWidget *parent = 0);
    QLineEdit *pwdedit;
    QLineEdit *e_mail_addressedit;
    QString response();

signals:
    void createmail();
    void mainwindowcase();

private slots:
    void manualAction();
    void servercase();
    void createaccount();

protected slots:
    void createButtoncase();


private:
    //setting my_father;
    //MainWindow my_parrent;
    QPushButton *ok;

    QToolButton *manualTool;
    int k;
    QWidget *manualDialog;

    QComboBox *serverTypeButton;
    QLineEdit *pop3portedit;
    QLabel *pop3add;
    QLineEdit *smtpedit;
    QLineEdit *smtpportedit;
    QLineEdit *pop3edit;
    QLabel *tip;  //提示是否create成功;
    QTcpSocket *socket;

    void setupUi();
    void judgeaccount();
    void send(QString text);
    void checkaccount();
    bool accountexit();

    QPoint  dragPosition;
    void mouseMoveEvent(QMouseEvent *e);  //重定义鼠标事件
    void mousePressEvent(QMouseEvent *e);
};

#endif // ACCOUNT_H
