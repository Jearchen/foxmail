#ifndef MESSAGE_H
#define MESSAGE_H
#include <QString>
#include<QStringList>
//////--------邮件体-------///////
class Message
{
public:
    Message(QByteArray);

////////...从源码中提取邮件类型...///
    QString getContentType(QString);
    QString getContent(QString);//从mailContent中获得邮件体
    QString getBoundary(QString);
    QString getCharset(QString);
    QString getEncodingType(QString);
    QString getDecoded(QString,QString,QString);
    QString parted(QString);//把邮件解构后调用getDecoded(QString)函数重组
    QString getText(QString);

    void setContent(QString type,QString encoding);
  //  QString getType();
   // QString getEncoding();

///////.......返回信息........///////////
    QString getmailContent();
    QString getContentType();

    QString getplaintxet();
    QString gethtml();
    QStringList getfilename();
    QStringList getfile();
    //QString getContent();

//////最终要调用的函数
    QString getDecodedMail();

    QString fromBase64(QString);
private:
    QString mailContent; //包括邮件头的原始邮件，接受来自服务器的ASCII码的邮件转化成QString形式
    QString plaintext,html;//,type,encoding;
    QString boundary;
    QStringList filenames;
    QStringList files;
public:
    QString content; //解码前的邮件体，去掉邮件头的部分
private:
    QString decodedContent; //解码后的邮件全文，由content解构解码而来
};

#endif // MESSAGE_H
