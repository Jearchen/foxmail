#include "setting.h"
#include <QDockWidget>
#include "QBitmap"
#include "QPalette"
#include <QDir>
#include <QMenu>
#include <QMouseEvent>
#include "account.h"
#include <QDir>

setting::setting(QWidget *parent): QWidget(parent,Qt::FramelessWindowHint)
{
    resize(637,549);

    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(QPixmap("://images/frame2.png")));
    this->setPalette(palette);

    QPushButton *close = new QPushButton(this);
    close->setGeometry(612,6,15,15);
    close->setObjectName("close");
    //close->setFocusPolicy(Qt::NoFocus);
    connect(close,SIGNAL(clicked()),this,SLOT(close()));

    QLabel *tool=new QLabel(this);
    tool->setGeometry(5,35,627,63);
    tool->setPixmap(QPixmap("://images/options.bmp"));


    accounttable = new QListWidget(this);
    accounttable->setGeometry(35,120,150,340);

    QPushButton *create = new QPushButton(this);
    create->setGeometry(35,465,50,25);
    create->setText(tr("Create"));

    QPushButton *deleteaccount = new QPushButton(this);
    deleteaccount->setGeometry(90,465,50,25);
    deleteaccount->setText(tr("Delete"));

    QPushButton *ok = new QPushButton(this);
    ok->setGeometry(490,515,60,23);
    ok->setText(tr("OK"));

    QPushButton *cancel = new QPushButton(this);
    cancel->setGeometry(555,515,60,23);
    cancel->setText(tr("CANCEL"));

    QLabel *e_mail=new QLabel(this);
    e_mail->setText(tr("E-mail: "));
    e_mail->setGeometry(268,180,40,22);
    e_mailedit=new QLineEdit(this);
    e_mailedit->setGeometry(320,183,270,18);
    QLabel *Password=new QLabel(this);
    Password->setText(tr("Password: "));
    Password->setGeometry(248,210,80,22);
    pwedit=new QLineEdit(this);
    pwedit->setGeometry(320,213,270,18);
    pwedit->setEchoMode(QLineEdit::Password);
    QLabel *name=new QLabel(this);
    name->setText(tr("Name: "));
    name->setGeometry(270,240,40,22);
    nameedit=new QLineEdit(this);
    nameedit->setGeometry(320,243,270,18);

    QLabel *type=new QLabel(this);
    type->setText(tr("TYPE: "));
    type->setGeometry(273,270,40,22);
    hosttype = new QLabel(this);
    hosttype->setGeometry(320,270,50,22);
    hosttype->setText(tr("POP3"));              //pop3设置为变量
    QLabel *pop3add=new QLabel(this);
    pop3add->setText(tr("Incoming: "));
    pop3add->setGeometry(253,300,80,22);
    pop3edit=new QLineEdit(this);
    pop3edit->setGeometry(320,303,160,18);
    QLabel *pop3port=new QLabel(this);
    pop3port->setText(tr("Port: "));
    pop3port->setGeometry(490,300,40,22);
    pop3portedit=new QLineEdit(this);
    pop3portedit->setGeometry(530,303,40,18);
    QLabel *smtpadd=new QLabel(this);
    smtpadd->setText(tr("Outgoing: "));
    smtpadd->setGeometry(254,330,80,22);
    smtpedit=new QLineEdit(this);
    smtpedit->setGeometry(320,333,160,18);
    QLabel *smtpport=new QLabel(this);
    smtpport->setText(tr("Port: "));
    smtpport->setGeometry(490,330,40,22);
    smtpportedit=new QLineEdit(this);
    smtpportedit->setGeometry(530,333,40,18);
    //smtpportedit->setText(tr("25"));

    connect(create,SIGNAL(clicked()),this,SLOT(contextCreateButton()));
    connect(cancel,SIGNAL(clicked()),this,SLOT(close()));
    connect(accounttable, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)), this, SLOT(changeShowAccount(QListWidgetItem*,QListWidgetItem*)));
    connect(deleteaccount,SIGNAL(clicked()),this,SLOT(deleteaccountslot()));
    connect(ok,SIGNAL(clicked()),this,SLOT(contextOkButton()));

    listaccount();
}

void setting::contextCreateButton()
{
    account *dialog=new account(this);
    dialog->setGeometry(60,50,497,384);
    dialog->setWindowModality(Qt::ApplicationModal);
    dialog->show();

    connect(dialog, SIGNAL(createmail()), this, SLOT(listaccount()));
}

void setting::listaccount()
{
    accounttable->clear();
    QFile file("account.txt");
    QString read;
    QStringList str;
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {

       while(!file.atEnd())
       {
            read=file.readLine();
            str=read.split(" ");
            accounttable->addItem(str.at(2));
       }
    }
    file.close();
    accounttable->setCurrentRow(0);
    if(accounttable->count()==1)
    {
        QFile file("account.txt");
        QString read;
        QStringList str;
        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        {

            read=file.readLine();
            str=read.split(" ");
            e_mailedit->setText(str.at(0));
            pwedit->setText(str.at(1));
            nameedit->setText(str.at(2));
            hosttype->setText(str.at(3));
            pop3edit->setText(str.at(4));
            pop3portedit->setText(str.at(5));
            smtpedit->setText(str.at(6));
            smtpportedit->setText(str.at(7));
        }
        file.close();
    }
    emit mainwindowcase();
}

void setting::changeShowAccount(QListWidgetItem* previous, QListWidgetItem* currentitem)
{
   // e_mailedit->setText(accounttable->currentItem()->text());
    if(accounttable->count()>1)
    {
        QFile file("account.txt");

        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
           QString read;
           QStringList str;

           while(!file.atEnd())
           {
                read=file.readLine();
                str=read.split(" ");
                if(accounttable->currentItem()->text().compare(str.at(2))==0)
                {
                    e_mailedit->setText(str.at(0));
                    pwedit->setText(str.at(1));
                    nameedit->setText(str.at(2));
                    hosttype->setText(str.at(3));
                    pop3edit->setText(str.at(4));
                    pop3portedit->setText(str.at(5));
                    smtpedit->setText(str.at(6));
                    smtpportedit->setText(str.at(7));
                    break;
                }
           }
         }
        file.close();
    }else
    {
        e_mailedit->setText("");
        pwedit->setText("");
        nameedit->setText("");
        hosttype->setText("");
        pop3edit->setText("");
        pop3portedit->setText("");
        smtpedit->setText("");
        smtpportedit->setText("");
    }
}

void setting::deleteaccountslot()
{
    int row=accounttable->currentRow();
    qDebug()<<row;

    //删除文件中的那行
    QString deleteitem;
    QString strall;
    QString read;
    QStringList str;
    int nLine=0;
    //int Index=0;
    nLine = accounttable->count();

    QFile readfile("account.txt");
    if(readfile.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&readfile);
        strall=stream.readAll();
    }
    readfile.close();
    //如果是直接从位置0开始删除\n算一个字符"abc\nme\ndo" \n的index是3要删除3+1个字符，即index+1个
    if(row==0)
    {
        int nIndex=strall.indexOf('\n');
        strall.remove(0,nIndex+1);
    }
    else
    {
        int nTemp=row;
        int nIndex=0,nIndex2=0;
        while(nTemp--)
        {
            //
            nIndex=strall.indexOf('\n',nIndex+1);//这里会更新nIndex
            if(nIndex!=-1)//说明是有效的
            {
                nIndex2=strall.indexOf('\n',nIndex+1);
            }
        }
        //删除的行不是最后一行（从nIndex+1这个位置起nIndex2-nIndex个字符全部抹去）
        if(row<nLine-1)
        {
            strall.remove(nIndex+1, nIndex2-nIndex);//不用减一
        }
        //删除的是最后一行（从nIndex起始len-nIndex个字符全抹去）
        //不能从nIndex+1处开始，
        else if(row==nLine-1)
        {
            int len=strall.length();
            strall.remove(nIndex,len-nIndex);
        }
    }
    QFile writefile("account.txt");
    if(writefile.open(QIODevice::WriteOnly))
    {
        QTextStream wrtstream(&writefile);
        wrtstream<<strall;
    }
    writefile.close();

    QString dir = accounttable->currentItem()->text();
    removeDirWithContent(dir);

    accounttable->takeItem(row);
    accounttable->setCurrentRow(0);
    emit mainwindowcase();
}

void setting::contextOkButton()
{
    /*
    if(accounttable->count()!=0)
    {
        QFile file("account.txt");
        if(!file.open(QIODevice::ReadWrite|QIODevice::Text))
        {
            qDebug()<<"Open file failure!";
        }

        QString read;
        QStringList str;
        QTextStream txtOutput(&file);
        while(!file.atEnd())
        {
             read=file.readLine();
             str=read.split(" ");
             if(accounttable->currentItem()->text().compare(str.at(2))==0)
             {

                 txtOutput<<e_mailedit->text()<<" ";
                 txtOutput<<pwedit->text()<<" ";
                 txtOutput<<nameedit->text()<<" ";
                 txtOutput<<hosttype->text()<<" ";
                 txtOutput<<pop3edit->text()<<" ";
                 txtOutput<<pop3portedit->text()<<" ";
                 txtOutput<<smtpedit->text()<<" ";
                 txtOutput<<smtpportedit->text();
                 break;
             }
        }
        file.close();
    }
    */
    //emit mainwindowcase();
    this->close();
}

bool setting::removeDirWithContent(const QString &dirName)
{
    static QVector<QString> dirNames;
    //static QString funcErrMsg="删除[%1]失败.";
    //static QString funcInfFndMsg="发现[%1].";
    //static QString funcInfDelMsg="删除[%1]成功.";
    QDir dir;
    QFileInfoList filst;
    QFileInfoList::iterator curFi;
    //初始化
    dirNames.clear();
    if(dir.exists()){
        dirNames<<dirName;
    }
    else{
        return true;
    }
    //遍历各级文件夹，并将这些文件夹中的文件删除
    for(int i=0;i<dirNames.size();++i){
        dir.setPath(dirNames[i]);
        filst=dir.entryInfoList(QDir::Dirs|QDir::Files|QDir::Readable|QDir::Writable|QDir::Hidden|QDir::NoDotAndDotDot ,QDir::Name);
        if(filst.size()>0){
            curFi=filst.begin();
            while(curFi!=filst.end()){
                 //遇到文件夹,则添加至文件夹列表dirs尾部
                if(curFi->isDir()){
                    dirNames.push_back(curFi->filePath());
                }else if(curFi->isFile()){
                    //遇到文件,则删除之
                    if(!dir.remove(curFi->fileName())){
                        return false;
                    }
                }
                curFi++;
            }//end of while
        }
    }
    //删除文件夹
    for(int i=dirNames.size()-1;i>=0;--i){
        dir.setPath(dirNames[i]);
        if(!dir.rmdir(".")){
            return false;
        }
    }
    return true;
}

void  setting::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {

            move(event->globalPos() - dragPosition);

            event->accept();

        }

}

void  setting::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {

            dragPosition = event->globalPos() - frameGeometry().topLeft();

            event->accept();

        }

}


