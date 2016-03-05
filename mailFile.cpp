#include"mailFile.h"

MailFile::MailFile(QFile* file):info(*file)
{
    file->open(QIODevice::ReadOnly);
    this->content=file->readAll().toBase64();
    this->filePath=file->fileName();
    this->fileName=info.fileName();
    file->close();
}

MailFile::~MailFile()
{

}

QString& MailFile::changeToString()
{
    for(int i=75;i<content.size();i=i+76){  //base64编码每76字符一行
        content.insert(i,"\r\n");
    }
    return content;
}

QString MailFile::getFilePath()
{
    return filePath;
}

