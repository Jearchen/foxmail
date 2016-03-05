#include"mailContent.h"
#include <QByteArray>
#include<QDebug>
#include<QDateTime>

MailContent::MailContent(QString sender, QStringList rcpts, QStringList ccs, QStringList bccs, QString subject, QString content)
{
    this->sender=sender;
    this->rcpts=rcpts;
    this->ccs=ccs;
    this->bccs=bccs;
    this->subject=subject;
    this->content=content;//.toUtf8().toBase64();//为了能成功发送中文，邮件内容使用Base64编码！！

}

MailContent::~MailContent(){}

void MailContent::addAttachment(Attachment* attachment)
{
    attachments.append(attachment);
}

QString& MailContent::changeToString()
{
    QDateTime time;
    QString t;
    time=time.currentDateTime();
    t=time.toString("ddd, dd MMM yyyy hh:mm:ss +0800");
    mailContent="Date: "+t+"\r\n";//日期
    mailContent +="From: <"+sender+">"+"\r\n";//邮件发送者地址
    for(int i=0;i<rcpts.count();i++){
        mailContent +="To: <"+rcpts.at(i)+">"+"\r\n";//邮件接受者地址
    }
    if(!ccs.isEmpty()){
        for(int i=0;i<ccs.count();i++)
            mailContent +="Cc: <"+ccs.at(i)+">\r\n";  //抄送
    }
    if(!bccs.isEmpty()){
        for(int i=0;i<bccs.count();i++)
            mailContent +="Bcc: <"+bccs.at(i)+">\r\n";  //暗送
    }

    QString sub=subject.toUtf8().toBase64();
    mailContent +="Subject: =?UTF-8?B?"+sub+"?="+"\r\n";//主题

    QString con=content.toUtf8().toBase64();

    mailContent +="Mime-Version:1.0\r\n";
    if(attachments.isEmpty()){
        mailContent +="Content-transfer-encoding:Base64\r\n";
        mailContent +="Content-type:text/plain;charset=utf-8\r\n\r\n";
        mailContent +=con+"\r\n";
    }
    else{
        mailContent +="Content-type:multipart/mixed;boundary=\"===ytttestformail\"\r\n\r\n\r\n";
        mailContent +="--===ytttestformail\r\n";
        mailContent +="Content-transfer-encoding:Base64\r\n";
        mailContent +="Content-type:text/plain;charset=utf-8\r\n\r\n";
        mailContent +=con+"\r\n\r\n";

        for(int i=0;i<attachments.size();i++){
            mailContent +="--===ytttestformail\r\n";
            mailContent +=attachments.at(i)->changeToString()+"\r\n";
        }
        mailContent +="--===ytttestformail--\r\n";
    }
    //qDebug()<<"mailcontent:"<<mailContent;
    return mailContent;


}

QList<Attachment*> MailContent::getAttachment()
{
    return attachments;
}

QString MailContent::getSubject()
{
    return subject;
}

QString MailContent::getContent()
{
    return content;
}
