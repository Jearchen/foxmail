#ifndef MAILCONTENT_H
#define MAILCONTENT_H

#include<QString>
#include<QObject>
#include<QList>
#include<QStringList>
#include"attachment.h"

class MailContent:public QObject{
public:
    MailContent(QString sender,QStringList rcpts,QStringList ccs,QStringList bccs,QString subject,QString content);
    ~MailContent();

    void addAttachment(Attachment* attachment);
    QString& changeToString();

    QList<Attachment*> getAttachment();

    QString getSubject();
    QString getContent();

protected:
    QString sender;
    QStringList rcpts;
    QStringList ccs;
    QStringList bccs;
    QString subject;
    QString content;
    QString mailContent;
    QList<Attachment*> attachments;

};

#endif // MAILCONTENT_H
