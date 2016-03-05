#ifndef CHECKMAIL_H
#define CHECKMAIL_H

#include<QDialog>
#include<QtGui>
#include "message.h"
#include "mailheader.h"
#include "pop3.h"

class checkmail:public QDialog
{
    Q_OBJECT
public:
    checkmail(QWidget *parent=0);
    void login();
    void connectpop(QString mailaddress);

signals:
    void finishcheck();

private:
    QPoint  dragPosition;
    void mouseMoveEvent(QMouseEvent *e);  //重定义鼠标事件
    void mousePressEvent(QMouseEvent *e);

    POP3 *pop3;
    QLabel *getmail;
    void checkmails(QString mailaddress);
};

#endif // CHECKMAIL_H
