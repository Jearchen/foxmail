#ifndef MAILHEADER_H
#define MAILHEADER_H
#include <QString>

class MailHeader
{
public:
    MailHeader(QByteArray);

////////...从源码中提取发件人、主题、日期、邮件类型...///
    QString getFrom(QString);
    QString getSubject(QString);
    QString getDate(QString);
    QString getType(QString);
    QString getEncoding(QString);
    //QString getContent(QString);

///////.......返回信息........///////////
    QString getFrom();
    QString getSubject();
    QString getDate();
    QString getType();
    QString getEncoding();
   // QString getContent();

    QString DecodeCode(QString,int);//解码函数base64与hex
   // QByteArray& fromQuotedPrintable(const QString &);
private:
    QString mailContent;
    QString From;
    QString Subject;
    QString date;
    QString type;
    QString encoding;
    //QString content;
};

#endif // MAILHEADER_H
