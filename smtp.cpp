#include "smtp.h"
#include <QDebug>
#include<QDateTime>

Smtp::Smtp(QString &hostName, int &port, QString &user, QString &pass, QString &sender,QStringList &rcpts)
{
    this->hostName=hostName;
    this->port=port;
    this->user=user;
    this->pass=pass;
    this->sender=sender;
    this->rcpts=rcpts;
    if(port==25)
        socket=new QTcpSocket(this);
    else
        socket = new QSslSocket(this);

}

Smtp::Smtp(QString hostName, int port, QString user, QString pass)
{
    this->hostName=hostName;
    this->port=port;
    this->user=user;
    this->pass=pass;
    if(port==25)
        socket=new QTcpSocket(this);
    else
        socket = new QSslSocket(this);
}

Smtp::~Smtp(){
    delete socket;
}

bool Smtp::connectToHost()
{
    if(port==25 || port==587)
        socket->connectToHost(hostName,port);//与邮件服务器连接
    else
        ((QSslSocket*) socket)->connectToHostEncrypted(hostName, port);

    if(!socket->waitForConnected(3000))//如果连接不上，输出connect failed
        qDebug("connect failed");

    if( response().left(3)=="220"){
        qDebug("connect!");
        return true;
    }
    else
        return false;
}

void Smtp::send(QString text)
{
    socket->write(text.toUtf8()+"\r\n");//向服务器发送信息
}

bool Smtp::login()
{
    send("helo there");//smtp命令，连接上服务器后，先向服务器发送helo命令
    qDebug()<<"response:"<<response();//输出服务器返回信息

    if (port==587) {
               // send a request to start TLS handshake
               send("STARTTLS");

               // Wait for the server's response
               qDebug()<<"response:"<<response();


               ((QSslSocket*) socket)->startClientEncryption();

               if (!((QSslSocket*) socket)->waitForEncrypted(3000))
                   qDebug() << ((QSslSocket*) socket)->errorString();

               // Send ELHO one more time
               send("helo there");

               // Wait for the server's response
               qDebug()<<"response:"<<response();

           }


    //将用户名和密码转成Base64编码
    user=user.toUtf8().toBase64();
    pass=pass.toUtf8().toBase64();

    send("auth login");//发出登陆的命令
    qDebug()<<"response:"<<response();

    //发送用户名和密码
    send(user);
    qDebug()<<"response:"<<response();
    send(pass);
    if(response().left(3)=="235"){
        qDebug("login success");
        return true;
    }
    else
        return false;
}

bool Smtp::sendMail(MailContent& email)
{


    send("mail from:<"+sender+">");//发出邮件发送者地址
    qDebug()<<"response:"<<response();

    for(int i=0;i<rcpts.count();i++)//输出每个邮件接受者地址
    {
        if(rcpts.at(i)!=""){          //！！！！！！！！！！！！！！！！！！！
            qDebug()<<rcpts.at(i);
        send("rcpt to:<"+rcpts.at(i)+">");
        qDebug()<<"response:"<<response();
        }
    }
    //qDebug()<<rcpts;

    send("data");//开始发送邮件内容
    qDebug()<<"response:"<<response();

    send(email.changeToString());
    //socket->waitForBytesWritten(300000);

    send(".");//邮件内容需以"\r\n.\r\n"结尾

    //socket->waitForBytesWritten(300000);


    if(response().left(3)=="250"){
        qDebug("mail accepted");
        return true;
    }
    else{
        qDebug("mail rejected");
        return false;
    }


}

void Smtp::quit()
{
    send("quit");//断开连接
    qDebug()<<"response:"<<response();
}

QString Smtp::response()
{
    QString respon;
    if(!socket->waitForReadyRead(30000))
        qDebug("response failed");

    while(socket->canReadLine()){   //接受服务器发回的信息，用while循环是为了避免接受服务器发出的“ ”之类的无内容的回复
          respon=socket->readLine();
          if(respon[3] ==' ')//to avoid response code=" "
              break;
    }
          return respon;
}
void Smtp::saveSendMail(MailContent& email, QString box)
{

    QDateTime dt;
    QTime time;
    QDate date;
    dt.setTime(time.currentTime());
    dt.setDate(date.currentDate());
    QString currentDate = dt.toString("yyyy-MM-dd-hh-mm-ss");

    QFile file(sender+"/"+box+"/"+currentDate+".txt");
    if(!file.open(QIODevice::WriteOnly))
        qDebug("open file failed");
    QString rcpt=rcpts.join(";");

    file.write("RCPT TO: "+rcpt.toUtf8()+"\r\n");
    file.write("SUBJECT: "+email.getSubject().toLocal8Bit()+"\r\n");
    file.write("DATE: "+currentDate.toUtf8()+"\r\n");
    file.write("CONTENT: "+email.getContent().toLocal8Bit()+"\r\n");


    for(int i=0;i<email.getAttachment().count();i++){
        file.write("ATTACHMENT. "+QString::number(i,10).toUtf8()+". "+email.getAttachment().at(i)->getFilePath().toUtf8()+"\r\n");
    }
    file.close();
}

