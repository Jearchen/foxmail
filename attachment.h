#ifndef ATTACHMENT_H
#define ATTACHMENT_H

#include"mailFile.h"

class Attachment:public MailFile{
public:
    Attachment(QFile* file);
    ~Attachment();
    QString& changeToString();
protected:
    QString header;
    QString mailContent;

};

#endif // ATTACHMENT_H
