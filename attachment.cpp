#include"attachment.h"

Attachment::Attachment(QFile* file):MailFile(file)
{
    this->header="Content-transfer-encoding:Base64\r\n";
    this->header +="Content-type:application/octet-stream;name=\""+fileName+"\"\r\n";
    this->header +="Content-disposition:attachment;filename=\""+fileName+"\"\r\n\r\n\r\n";
}

Attachment::~Attachment()
{

}

QString& Attachment::changeToString()
{
    mailContent=header;
    mailContent +=MailFile::changeToString();
    mailContent +="\r\n";
    return mailContent;
}
