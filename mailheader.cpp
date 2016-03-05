#include "mailheader.h"
#include <QDebug>
#include <QTextCodec>
#include<QRegExp>

MailHeader::MailHeader(QByteArray head)
{
    mailContent=head.data();
    From=getFrom(mailContent);
    Subject=getSubject(mailContent);
    date=getDate(mailContent);
    type=getType(mailContent);
    encoding=getEncoding(mailContent);
    //content=getContent(mailContent);
}

QString MailHeader::getFrom(QString mailContent)
{
    QString from;int i=0;
    qint64 topPosition,endPosition,Length;/*
    topPosition=mailContent.indexOf("From: ")+6;       //完整截取发件人部分，考虑了多行的情况
    endPosition=mailContent.indexOf("\r\n",topPosition);
    while(mailContent.at(endPosition+2)==' '||mailContent.at(endPosition+2)=='\t')
    {
        if(mailContent.at(endPosition+3)=='='){
            i++;
        }
        endPosition=mailContent.indexOf("\r\n",endPosition+2);
    }
    Length=endPosition-topPosition;
    from=mailContent.mid(topPosition,Length);*/
    mailContent.replace("\r\n","\n");
    QRegExp regexp("\\nfrom:\\s?(.*)\\n");
    regexp.setMinimal(true);//
    regexp.setCaseSensitivity(Qt::CaseInsensitive);//
    int pos=regexp.indexIn(mailContent);
    if(pos > -1){
        from=regexp.cap(1);
    }


    //qDebug()<<"From: "<<from;
    if(from.contains("=?")){                           ///如果主题中需解码文字，解码后在其后加上邮箱
        From=DecodeCode(from,i);
        topPosition=from.indexOf("<");
        endPosition=from.indexOf(">",topPosition)+1;
        From=From.append(from.mid(topPosition,endPosition));
    }
    else From=from;
    qDebug()<<"From: "<<From;
    return From;
}

QString MailHeader::getSubject(QString mailContent)
{
    QString subject;int i=0;
    qint64 topPosition,endPosition,Length;
   /* topPosition=mailContent.indexOf("Subject: ")+9;
    endPosition=mailContent.indexOf("\r\n",topPosition);
    while(mailContent.at(endPosition+2)==' '||mailContent.at(endPosition+2)=='\t')
    {
        i++;
        endPosition=mailContent.indexOf("\r\n",endPosition+2);
    }
    Length=endPosition-topPosition;
    subject=mailContent.mid(topPosition,Length);
    qDebug()<<"Subject:"<<subject;*/

    mailContent.replace("\r\n","\n");
    QRegExp regexp("\\nsubject:\\s?(.*)\\n");//
    regexp.setMinimal(true);//
    regexp.setCaseSensitivity(Qt::CaseInsensitive);//

    int pos=regexp.indexIn(mailContent);
    if(pos > -1){
        subject=regexp.cap(1);
    }
    if(subject.contains("=?"))
        Subject=DecodeCode(subject,i);
    else Subject=subject;
    qDebug()<<"Subject:"<<Subject;
    return Subject;
}

QString MailHeader::getDate(QString mailContent)
{
    qint64 topPosition,endPosition,Length;
    /*topPosition=mailContent.indexOf("Date: ")+6;
    endPosition=mailContent.indexOf("\r\n",topPosition);
    while(mailContent.at(endPosition+2)==' '||mailContent.at(endPosition+2)=='\t')
    {
        endPosition=mailContent.indexOf("\r\n",endPosition+2);
    }
    Length=endPosition-topPosition;
    date=mailContent.mid(topPosition,Length);*/


    mailContent.replace("\r\n","\n");
    QRegExp regexp("\\ndate:\\s?(.*)\\n");//
    regexp.setMinimal(true);//
    regexp.setCaseSensitivity(Qt::CaseInsensitive);//

    int pos=regexp.indexIn(mailContent);
    if(pos > -1){
        date=regexp.cap(1);
    }

    qDebug()<<"Date: "<<date;
    return date;
}

QString MailHeader::getFrom()
{
    return From;
}

QString MailHeader::getSubject()
{
    return Subject;
}

QString MailHeader::getDate()
{
    return date;
}

QString MailHeader::DecodeCode(QString string,int line)
{
    QString str;
    QByteArray ba;
    QByteArray content=string.toAscii();
    QList<QByteArray> s;
    s=content.split('?');
    QByteArray read1,read2,read;
    read1=s.at(1);///////
    read2=s.at(2);//////编码方式
    for(int i=0;i<=line;i++){
        read=read.append(s.at(3+4*i));
        qDebug()<<i<<read<<endl;
    }
    QTextCodec *tc=QTextCodec::codecForName(read1);
    if(read2=="Q"){
       /* QByteArray output;
        for(int i=0;i<read.length();++i){
            while(read.at(i)=='='&&(i+3)<read.length()){
                output=output.append(read.mid(i,i+2));
                i=i+3;
            }
            if(output!="")ba=ba.append(QByteArray::fromHex(output));
            ba=ba.append(read.at(i));
        }
        //*/ba=QByteArray::fromHex(read);
    }
    if(read2=="B"){
        ba=QByteArray::fromBase64(read);
    }
    str=tc->toUnicode(ba);
    return str;
}

/*QByteArray& MailHeader::fromQuotedPrintable(const QString & input)
{
    // 0 1 2 3 4 5 6 7 8 9 : ; < = > ? @ A B C D E F
    const int hexVal[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 0, 0, 0, 0, 10, 11, 12, 13, 14, 15};
    QByteArray *output = new QByteArray();
    for(int i=0;i<input.length();++i){
        if (input.at(i).toAscii()=='='){
            output->append((hexVal[input.at(++i).toAscii()-'0']<<4)+hexVal[input.at(++i).toAscii()-'0']);
        }else{
            output->append(input.at(i).toAscii());
        }
    }
    return *output;
}
*/

/*QString MailHeader::getContent(QString mailContent)
{
    QString type,encoding;
    mailContent.replace("\r\n","\n");
    QRegExp regexp("\\ncontent-type:(.*);");//
    regexp.setMinimal(true);//
    regexp.setCaseSensitivity(Qt::CaseInsensitive);//

    int pos=regexp.indexIn(mailContent);
    if(pos > -1){
        type=regexp.cap(1);
    }
    regexp.setPattern("\\ncontent-transfer-encoding:(.*)\\n");
    pos=regexp.indexIn(mailContent);
    if(pos > -1){
        encoding=regexp.cap(1);
    }
    qDebug()<<type<<encoding;
    regexp.setPattern("text/plain");
    if(type.contains(regexp)){
        regexp.setPattern("\\n\\n([\\s\\S]*)\\n.\\n");
        int pos=regexp.indexIn(mailContent);
        if(pos > -1){
            content=regexp.cap(1);
            qDebug()<<content;
        }


    }


}*/

QString MailHeader::getType(QString)
{
    QString type;
    mailContent.replace("\r\n","\n");
    QRegExp regexp("\\ncontent-type:(.*);");//
    regexp.setMinimal(true);//
    regexp.setCaseSensitivity(Qt::CaseInsensitive);//

    int pos=regexp.indexIn(mailContent);
    if(pos > -1){
        type=regexp.cap(1);
    }
    qDebug()<<type;
   return type;

}

QString MailHeader::getEncoding(QString)
{
    QString encoding;
    mailContent.replace("\r\n","\n");
    QRegExp regexp("\\ncontent-transfer-encoding:(.*)\\n");//
    regexp.setMinimal(true);//
    regexp.setCaseSensitivity(Qt::CaseInsensitive);//

   int pos=regexp.indexIn(mailContent);
    if(pos > -1){
        encoding=regexp.cap(1);
    }
    qDebug()<<encoding;
   return encoding;

}

QString MailHeader::getType()
{
    return type;
}

QString MailHeader::getEncoding()
{
    return encoding;
}

