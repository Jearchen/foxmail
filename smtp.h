#ifndef SMTP_H
#define SMTP_H

#include <QObject>
#include <QTcpSocket>  //套接字类，Qt提供，用于接收发送数据，需在.pro文件中添加“QT +=network”
#include <QSslSocket>
#include <QString>
#include <QStringList>
#include <QByteArray>
#include"mailContent.h"
#include"attachment.h"
#include"mailFile.h"

class Smtp : public QObject
{
    Q_OBJECT
public:
    Smtp(QString &hostName, int &port, QString &user, QString &pass, QString &sender,QStringList &rcpts);
    Smtp(QString hostName, int port, QString user, QString pass);
    ~Smtp();

    //set and get info    //get/set数据成员的函数
    bool connectToHost();//连接host
    bool login();    //登陆
    bool sendMail(MailContent& email);//发送信息，包括登陆，邮件发送接收者，邮件内容等
    //void sendMail();    //发送信息，包括登陆，邮件发送接收者，邮件内容等
    void quit();    //发送完毕，断开连接
    QString response();     //接收从服务器发回的信息，以判断信息发送是否成功
    void saveSendMail(MailContent& email, QString box);

protected:
    QTcpSocket* socket;     //套接字，用于连接，读写数据
    QString hostName;       //host地址，可以是IP也可以是"smtp.sina.com"之类的
    int port;
    QString user;       //登陆名，需要用Base64加密
    QString pass;
    QString sender;     //发送邮件的地址
    QStringList rcpts;
    //QString subject;    //邮件主题
    //QString content;    //邮件内容

    void send(QString text);


};

#endif // SMTP_H
