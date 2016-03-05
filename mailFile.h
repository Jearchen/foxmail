#ifndef MAILFILE_H
#define MAILFILE_H

#include<QObject>
#include<QString>
#include<QFile>
#include<QStringList>
#include<QFileInfo>

class MailFile:public QObject{
public:
    MailFile(QFile* file);
    ~MailFile();
    virtual QString& changeToString();
    QString getFilePath();

protected:
    QString content;
    QString fileName;
    QString filePath;
    QFileInfo info;

};

#endif // MAILFILE_H
