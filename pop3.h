#ifndef POP3_H
#define POP3_H
#include <QObject>
#include <QString>
#include <QTcpSocket>

class POP3 : public QObject
{
public:
    POP3(){}
    ~POP3(){}
    void Initialize(QString &username1, QString &password1, QString &server1, int port1);

    bool ConnectToHost();   //连接host
    bool Login();    //登录
    int ListMail();    //列出邮件信息,并返回邮件数量
    QByteArray Retrieve(int num);    //获得第num封邮件的邮件头
    void Quit();    //发送完毕，断开连接
    QString response();    //接收从服务器发回的信息，返回服务器的单行应答
    QByteArray getMail(QString);//获得单封邮件的全部内容

private slots:
    void send(QString text);   //向服务器发送指令

private:
    QTcpSocket* socket;  //套接字，用于连接，读写数据

    QString username;  //用户名
    QString password;  //用户密码
    QString serverAddress;  //服务器地址
    int serverPort;  //服务器端口

    /*void displayError(QAbstractSocket::SocketError);*/
};

#endif // POP3_H
