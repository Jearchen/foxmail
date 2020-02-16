#include "checkmail.h"
#include<QtNetwork>
#include<QPushButton>
checkmail::checkmail(QWidget *parent):QDialog(parent,Qt::FramelessWindowHint)
{
    QCoreApplication::processEvents();

    QWidget::resize(460,189);


    //生成一张位图
    QBitmap objBitmap(size());
    //QPainter用于在位图上绘画
    QPainter painter(&objBitmap);
    //填充位图矩形框(用白色填充)
    painter.fillRect(rect(),Qt::white);
    painter.setBrush(QColor(0,0,0));
    //在位图上画圆角矩形(用黑色填充)
    painter.drawRoundedRect(this->rect(),5,5);
    //使用setmask过滤即可
    setMask(objBitmap);
    //设置背景
    QPixmap pixmap("://images/frame5.png");
    QPalette palette;
    palette.setBrush(backgroundRole(), QBrush(pixmap));
    setPalette(palette);

    QPushButton *close = new QPushButton(this);
    close->setGeometry(435,6,15,15);
    close->setObjectName("close");
    connect(close,SIGNAL(clicked()),this,SLOT(close()));

    QLabel *label=new QLabel (this); //初始化qlabel
    QMovie *pm = new QMovie("://images/getmail.gif");   //设定要显示的图片
    label->setMovie(pm); //将图片加载到label上
    label->setGeometry(25, 32, 150, 100 ); //屏幕大小，初始位置
    label->setScaledContents(true);
    label->show();
    pm->start();

    getmail = new QLabel(this);
    getmail->setGeometry(180,50,200,40);
    getmail->setText("             Doing Connect..");
    getmail->setFixedWidth(200);
    getmail->setFixedHeight(80);

}

void checkmail::connectpop(QString mailaddress)
{
    QCoreApplication::processEvents();

    QFile file("account.txt");
    QString sender, password, portstr,host;
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
       QString read;
       QStringList str;

       while(!file.atEnd())
       {
            read=file.readLine();
            str=read.split(" ");
            if(mailaddress.compare(str.at(0))==0)
            {
                sender=str.at(0);
                password=str.at(1);
                host = str.at(4);
                //qDebug()<<host<<"host";
                portstr=str.at(5);
                //qDebug()<<portstr<<"port";
                break;
            }
       }
     }
    file.close();
    int port;
    bool ok1;
    ok1=true;
    port=portstr.toInt(&ok1,10);

    pop3 = new POP3;

    pop3->Initialize(sender, password ,host,port);

    qDebug()<<host<<" "<<port;

    if(pop3->ConnectToHost())
    {
        if(pop3->Login())
        {
            getmail->setText("            Get mails......");
            QCoreApplication::processEvents();
            checkmails(mailaddress);
            pop3->Quit();
            emit finishcheck();
        }
    }
   else{   getmail->setText("      Get Messages Failed");}
}

void checkmail::checkmails(QString mailaddress)
{
    int mailsum=0;
    mailsum=pop3->ListMail();//获取邮件的数量

    if(mailsum>0)
        getmail->setText(tr("             There are %1 emails").arg(mailsum));
    else getmail->setText(tr("             There is no emails"));

    QString from, subject, date, content1;

    for(int i=1;i<=mailsum;i++)
    {
        QByteArray mailContent=pop3->Retrieve(i); //调用获取邮件函数保存邮件
        MailHeader mail(mailContent);
        QString order=QString::number(i,10);
        from = mail.getFrom();
        subject = mail.getSubject();
        date = mail.getDate();

        QByteArray mail2=pop3->getMail(order);
        Message mail1(mail2);
        //qDebug()<<mail2;
        qDebug()<<mail.getType()<<mail.getEncoding()<<"type&encoding";

        mail1.setContent(mail.getType(),mail.getEncoding());
        content1=mail1.gethtml();
        if(content1==""){
            content1=mail1.getplaintxet();
        }
        else
            content1.append("YTTTESTHTML");

        qDebug()<<content1<<"content";

        QFile file(mailaddress+"/Inbox/"+date+".txt");
        if(!file.open(QIODevice::WriteOnly)){   qDebug("open file failed");}

        QTextStream wrtstream(&file);
        if(mail1.getfilename().isEmpty()){
        wrtstream<<"RCPT TO: "<<from<<"\r\n"<<"SUBJECT: "<<subject<<"\r\n"<<"DATE: "<<date<<"\r\n"<<"CONTENT: "<<content1<<"\r\n";
        }
        else{
           wrtstream<<"RCPT TO: "<<from<<"\r\n"<<"SUBJECT: "<<subject<<"\r\n"<<"DATE: "<<date<<"\r\n"<<"CONTENT: "<<content1<<"\r\n"
                   <<"FILENAME: "<<mail1.getfilename().at(0)<<"\r\n"<<"FILE: "<<mail1.getfile().at(0)<<"\r\n";
        }
        //qDebug()<<wrtstream;
        file.close();
    }

}

void  checkmail::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {

            move(event->globalPos() - dragPosition);

            event->accept();

        }

}

void  checkmail::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {

            dragPosition = event->globalPos() - frameGeometry().topLeft();

            event->accept();

        }

}

