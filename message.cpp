#include "message.h"
#include <QDebug>
#include <QTextCodec>
#include <QStringList>

Message::Message(QByteArray mail)
{
    mailContent=mail.data();
    mailContent.replace("\r\n","\n");
    //type=getType();
    //encoding=getEncoding();
    //plaintext=setContent();
    //qDebug()<<plaintext;
   // content=getContent(mailContent);
   // decodedContent=parted(mailContent);
}


QString Message::getContentType(QString mailContent)
{
    qint64 topPosition,endPosition,Length;
    topPosition=mailContent.indexOf("Content-Type: ")+14;
    endPosition=mailContent.indexOf("\r\n",topPosition);
    while(mailContent.at(endPosition+2)==' '||mailContent.at(endPosition+2)=='\t')
    {
        endPosition=mailContent.indexOf("\r\n",endPosition+2);
    }
    Length=endPosition-topPosition;
    QString contentType=mailContent.mid(topPosition,Length);
    qDebug()<<"ContentType: "<<contentType;
    return contentType;
}

QString Message::getContent(QString content)
{
    qint64 topPosition,endPosition,Length;
    QString Content;//截取的所需要的部分邮件内容
    if(content==mailContent){
        topPosition=mailContent.indexOf("\r\n\r\n")+4;
        endPosition=mailContent.indexOf("\r\n.\r\n",topPosition);
        Length=endPosition-topPosition;
        Content=mailContent.mid(topPosition,Length);
        //qDebug()<<QObject::tr("邮件全文：")<<"(from getContent(QString))"<<endl<<Content;
    }else {
        topPosition=mailContent.indexOf("\r\n\r\n")+4;
        Content=mailContent.mid(topPosition);
        qDebug()<<QObject::tr("所截部分：")<<"(from getContent(QString))"<<endl<<Content;
    }
    return Content;
}

QString Message::getBoundary(QString contentType)
{
    qint64 topPosition,endPosition,Length;
    topPosition=contentType.indexOf("boundary=")+10;
    endPosition=contentType.indexOf("\"",topPosition);
    Length=endPosition-topPosition;
    QString boundary=contentType.mid(topPosition,Length);
    qDebug()<<"Boundary: "<<boundary;
    return boundary;
}

/*QString Message::getContent()
{
    return content;
}*/

QString Message::getmailContent()
{
    return mailContent;
}

QString Message::fromBase64(QString string)
{
    if(!string.contains("=?"))
        return string;
    QByteArray content=string.toLatin1();
    QList<QByteArray> s;
    s=content.split('?');
    QByteArray read1,read2;
    read1=s.at(1);
    read2=s.at(3);
    QTextCodec *tc=QTextCodec::codecForName(read1);
    QByteArray ba=QByteArray::fromBase64(read2);
    QString str=tc->toUnicode(ba);
    return str;
}

QString Message::getDecoded(QString content,QString charset,QString encoding)
{
    QTextCodec *tc=QTextCodec::codecForName(charset.toLatin1());
    QByteArray content1;
    if(encoding=="base64")
        content1=QByteArray::fromBase64(content.toLatin1());
    else
        content1=QByteArray::fromHex(content.toLatin1());
    QString str=tc->toUnicode(content1);
    return str;
}


QString Message::parted(QString content)
{
    QString contentType=getContentType(content);
    QString mechanism=getEncodingType(content);
    content=getContent(content);
    if(mechanism=="quoted-printable")
        decodedContent=QByteArray::fromHex(content.toLatin1());
    else if(contentType.contains("text")){
        decodedContent=decodedContent.append(getText(content));
    }
    else if(contentType.contains("Multipart")){
        QString boundary=getBoundary(contentType);

        QStringList s;
        s=content.split(boundary);
        int bodynum=s.size();
        for(int i=0;i<bodynum;i++){
            parted(s.at(i));
        }
    }//else if
    return decodedContent;
}

QString Message::getText(QString content)
{
    QString contentType=getContentType(content);
    QString charset=getCharset(contentType);
    QString encoding=getEncodingType(content);
    content=getContent(content);
    return(getDecoded(content,charset,encoding));
}

QString Message::getCharset(QString contentType)
{
    qint64 topPosition;
    topPosition=contentType.indexOf("charset=")+8;
    QString charset=contentType.mid(topPosition);
    qDebug()<<"Charset: "<<charset;
    return charset;
}

QString Message::getEncodingType(QString content)
{
    QString mechanism;
    qint64 topPosition,endPosition,Length;
    if(!content.contains("Content-Transfer-Encoding: ")){
        mechanism="7bit";
        return mechanism;
    }
    topPosition=content.indexOf("Content-Transfer-Encoding: ")+27;
    endPosition=content.indexOf("\r\n",topPosition);
    Length=endPosition-topPosition;
    mechanism=content.mid(topPosition,endPosition);
    qDebug()<<"Mechanism: "<<mechanism;
    return mechanism;
}

QString Message::getDecodedMail()
{
    return decodedContent;
}

/*QString Message::fromQuotedPrintable(QString pSrc)
{
    int nSrcLen=pSrc.length();
    int i=0;
    while (i<nSrcLen){
        if(qstrncmp(pSrc.mid(i),"=\r\n",3)==0){      // 软回车，跳过
            i+=3;
        }else{
            if (pSrc.at(i)=='='){       // 是编码字节
                pSrc.at(i+1).toLatin1();
                //replace(pSrc,"=%02X",pDst);
                pDst++;
                pSrc+=3;
                i+=3;
            }else {// 非编码字节
                *pDst++ = (unsigned char)*pSrc++;
                i++;
            }
        }
    }// 输出加个结束符
    *pDst = '/0';
    return pDst;
}*/

void Message::setContent(QString type,QString encoding)
{
    QByteArray text;
    QRegExp regexp;
    regexp.setMinimal(true);//
    regexp.setCaseSensitivity(Qt::CaseInsensitive);//

    if(type.contains("text/plain")){
        regexp.setPattern("\\n\\n([\\s\\S]*)\\n.\\n");
        int pos=regexp.indexIn(mailContent);
        if(pos > -1){
            plaintext=regexp.cap(1);
            qDebug()<<plaintext;
        }
        regexp.setPattern("base64");
        if(encoding.contains(regexp)){
            plaintext=text.fromBase64(plaintext.toUtf8());
        }
        else if(encoding.contains("rintable")){
            plaintext=plaintext;
        }
        else
            plaintext=plaintext;
    }
    else if(type.contains("text/html")){
        regexp.setPattern("\\n\\n([\\s\\S]*)\\n.\\n");
        int pos=regexp.indexIn(mailContent);
        if(pos > -1){
            html=regexp.cap(1);
           // qDebug()<<html;
        }
        if(encoding.contains("64")){
            html=text.fromBase64(html.toUtf8());
        }
        else if(encoding.contains("rintable")){
            html=html;
        }
        else
            html=html;
    }
    else if(type.contains("multipart")){
        regexp.setPattern("boundary=\"(.*)\"");
        int pos=regexp.indexIn(mailContent);
        if(pos > -1){
            boundary=regexp.cap(1);
            qDebug()<<boundary<<"boundary";
        }
        regexp.setPattern("\\ncontent-type:\\s?text/plain;");

        int index=regexp.indexIn(mailContent);
        int lastindex=mailContent.indexOf(boundary,index);
        QString mixplain=mailContent.mid(index,lastindex-index);
        //QString mixplain;
        //if(index>-1)
          //  mixplain=regexp.cap(0);
       // qDebug()<<mixplain;
        regexp.setPattern("\\n\\n([\\s\\S]*)--");
        pos=regexp.indexIn(mixplain);
        if(pos>-1)
            plaintext=regexp.cap(1);
        //qDebug()<<plaintext<<"mixedplain";
        regexp.setPattern("base64");
        if(mailContent.contains(regexp))
            plaintext=text.fromBase64(plaintext.toUtf8());


        regexp.setPattern("\\ncontent-type:\\s?text/html;");
         index=regexp.indexIn(mailContent);
         if(index>-1){
             lastindex=mailContent.indexOf(boundary,index);
            QString mixhtml=mailContent.mid(index,lastindex-index+boundary.size());
            //QString mixplain;
            //if(index>-1)
              //  mixplain=regexp.cap(0);
           // qDebug()<<mixhtml;
            /*regexp.setPattern("\\n\\n([\\s\\S]*)--"+boundary);
            pos=regexp.indexIn(mixhtml);
            if(pos>-1)
                html=regexp.cap(1);
            //html=html.left(html.size()-2);
            qDebug()<<html<<"mixedhtml";*/
            html=mixhtml.mid(mixhtml.indexOf("\\n\\n"),mixhtml.indexOf(boundary)-mixhtml.indexOf("\\n\\n")-2);
            //qDebug()<<html<<"mixhtml";
         }

    regexp.setPattern("content-Disposition:\\s?attachment;\\s?filename=\"(.*)\"\\n");
    index=regexp.indexIn(mailContent);
    if(index>-1){
        QString file;
        qDebug()<<index<<"file";
        QString filename=regexp.cap(1);
        qDebug()<<filename<<"name";
        filenames.append(filename);
         int lastindex=mailContent.indexOf(boundary,index);
         QString mixfile=mailContent.mid(index,lastindex-index+boundary.size());
        //regexp.setPattern("\\n\\n([\\s\\S]*)--");
        //pos=regexp.indexIn(mixfile);
        //if(pos>-1)
           // file=regexp.cap(1);
        //qDebug()<<plaintext<<"mixedplain";
         qDebug()<<mixfile;
         file=mixfile.mid(mixfile.indexOf("\\n\\n"),mixfile.indexOf(boundary)-mixfile.indexOf("\\n\\n")-2);
        //regexp.setPattern("base64");
        //if(mailContent.contains(regexp))
          //  file=text.fromBase64(file.toUtf8());
        files.append(file);

        qDebug()<<filenames<<files;



    }
    }






}

/*QString Message::getType()
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

QString Message::getEncoding()
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

}*/

QString Message::getplaintxet()
{
    return plaintext;
}

QString Message::gethtml()
{
   return html;
}

QStringList Message::getfilename()
{
    return filenames;
}

QStringList Message::getfile()
{
    return files;
}

