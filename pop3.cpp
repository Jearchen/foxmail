#include "pop3.h"
#include <QtNetwork>
#include <QDebug>
#include <QByteArray>

void POP3::Initialize(QString &username1, QString &password1, QString &server1, int port1)
{
    username=username1;
    password=password1;
    serverAddress=server1;
    serverPort=port1;
    socket=new QTcpSocket(this);
}

bool POP3::ConnectToHost()
{
    socket->abort();
    socket->connectToHost(serverAddress,serverPort);
    qDebug()<<serverAddress<<serverPort;
    if(socket->waitForConnected()==true){
        return true;
    }
    else {
        return false;
    }
}

bool POP3::Login()
{
    QString user=QString("USER %1").arg(username);
    QString pass=QString("PASS %1").arg(password);
    send(user);                        //发送用户名和密码
    if(response().left(3)=="+OK"){
        send(pass);
        if(response().left(3)=="+OK"){
            return true;
        }else{
            qDebug("login failed");
            return false;
        }
    }else{
        qDebug("login failed");
        return false;
    }
}

void POP3::send(QString text)
{
    qDebug()<<text;
    socket->write(text.toUtf8()+"\r\n");
}

QString POP3::response()
{
   /* QString respon;
    respon.clear();
    if(socket->waitForReadyRead()){
        while(socket->canReadLine()){
            respon=socket->readLine();
            qDebug()<<respon;
            if(socket->atEnd())
                break;
        }                    //接受服务器发回的信息
    }
    return respon;*/
    QString res;
    QString str;

      socket->waitForReadyRead(200);
      str=socket->readAll();
      res.append(str);
      socket->waitForReadyRead(200);
      str=socket->readAll();
      res.append(str);


      return res;
}

int POP3::ListMail()        //获取邮件数目
{
    QString sendCode="STAT";
    send(sendCode);
    QString retcode=response();

    qDebug()<<retcode;
    if(retcode.left(3)=="+OK"){
        QStringList s;
        QString read;
        int readint;
        s=retcode.split(" ");
        read = s.at(1);
        bool ok;
        ok= true;
        readint = read.toInt(&ok,10);

        return readint;
    }else{
        qDebug("Response failed!");
        return 0;
    }
}

QByteArray POP3::Retrieve(int num)
{
    QByteArray mailContent;
    QString sendCode=QString("TOP %1 0").arg(num,0,10);
    send(sendCode);

    while(1){
        socket->waitForReadyRead(200);
        mailContent.append(socket->readAll());
        if(mailContent.endsWith("\r\n.\r\n"))
            break;
    }

    if(mailContent.contains("+OK")){
        return mailContent;
    }else return "Error!";
}

void POP3::Quit()
{
    QString sendcode="QUIT";
    send(sendcode);
    QString retcode=response();
    if(retcode.left(3)=="+OK")
        socket->close();
}

QByteArray POP3::getMail(QString order)
{
    QByteArray mailContent;
    QString sendCode=QString("RETR %1").arg(order);
    send(sendCode);
    socket->waitForReadyRead(200);
    QString retCode=socket->readLine();
    if(retCode.left(3)=="+OK"){
    while(1){
        socket->waitForReadyRead(200);
        mailContent.append(socket->readAll());
        if(mailContent.endsWith("\r\n.\r\n"))
            break;
    }
    return mailContent;
    }else return "Error";
}
