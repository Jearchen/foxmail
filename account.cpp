#include "account.h"
#include <QDebug>
#include <QKeyEvent>
#include "smtp.h"
#include <QDir>

account::account(QWidget *parent):QWidget(parent,Qt::FramelessWindowHint)
{

    resize(497,384);

    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(QPixmap("://images/frame4.png")));
    this->setPalette(palette);

    //tip->hide();

    QPushButton *close = new QPushButton(this);
    close->setGeometry(472,6,15,15);
    close->setObjectName(tr("close"));
    connect(close,SIGNAL(clicked()),this,SLOT(close()));

    QLabel *e_mail_address=new QLabel(this);
    e_mail_address->setText(tr("E-mail Address: "));
    e_mail_address->setGeometry(60,130,120,22);
    e_mail_addressedit=new QLineEdit(this);
    e_mail_addressedit->setGeometry(170,133,270,18);
    QLabel *Pwd=new QLabel(this);
    Pwd->setText(tr("Password: "));
    Pwd->setGeometry(88,160,80,22);
    pwdedit=new QLineEdit(this);
    pwdedit->setGeometry(170,163,270,18);
    pwdedit->setEchoMode(QLineEdit::Password);
    manualTool=new QToolButton(this);
    manualTool->setText(tr("Manual..."));
    manualTool->setGeometry(378,193,60,20);

    tip = new QLabel(this);
    tip->setGeometry(170,190,200,20);
    //tip->setText(tr("Verifying......"));
    tip->hide();

    ok = new QPushButton(this);
    ok->setGeometry(357,352,60,20);
    ok->setText(tr("CREATE"));
    ok->setShortcut(Qt::Key_Return);
    ok->setEnabled(false);

    QPushButton *cancel = new QPushButton(this);
    cancel->setGeometry(422,352,60,20);
    cancel->setText(tr("CANCEL"));

    setupUi();

    connect(e_mail_addressedit,SIGNAL(textChanged(const QString &)),this,SLOT(createButtoncase()));
    connect(cancel,SIGNAL(clicked()),this,SLOT(close()));
    connect(manualTool,SIGNAL(clicked()),this,SLOT(manualAction()));
    connect(ok,SIGNAL(clicked()),this,SLOT(createaccount()));

}

void account::manualAction()
{
    if(k==0)
    {
         k=1;
         manualDialog->show();
    }
    else if(k==1)
    {
        manualDialog->hide();
        k=0;
    }
}

void account::setupUi()
{
    manualDialog=new QWidget(this);
    manualDialog->setGeometry(5,213,440,130);
    QLabel *serverTYPE = new QLabel(manualDialog);
    serverTYPE->setText(tr("TYPE: "));
    serverTYPE->setGeometry(110,0,40,20);
    serverTypeButton= new QComboBox(manualDialog);
    serverTypeButton->setGeometry(165,0,70,20);
    serverTypeButton->addItem(tr("POP3"));
    serverTypeButton->addItem(tr("IMAP"));

    pop3add=new QLabel(manualDialog);
    pop3add->setText(tr("POP Server: "));
    pop3add->setGeometry(76,30,80,22);
    pop3edit=new QLineEdit(manualDialog);
    pop3edit->setGeometry(165,33,160,18);
    QLabel *pop3port=new QLabel(manualDialog);
    pop3port->setText(tr("Port: "));
    pop3port->setGeometry(340,30,40,22);
    pop3portedit=new QLineEdit(manualDialog);
    pop3portedit->setGeometry(380,33,50,18);
    pop3portedit->setText(tr("110"));
    QLabel *smtpadd=new QLabel(manualDialog);
    smtpadd->setText(tr("SMTP Server: "));
    smtpadd->setGeometry(69,60,80,22);
    smtpedit=new QLineEdit(manualDialog);
    smtpedit->setGeometry(165,63,160,18);
    QLabel *smtpport=new QLabel(manualDialog);
    smtpport->setText(tr("Port: "));
    smtpport->setGeometry(340,60,40,22);
    smtpportedit=new QLineEdit(manualDialog);
    smtpportedit->setGeometry(380,63,50,18);
    smtpportedit->setText(tr("25"));

    connect(serverTypeButton,SIGNAL(currentIndexChanged(const QString &)),this,SLOT(servercase()));

    manualDialog->hide();
    k=0;
}

void account::servercase()
{
    if(serverTypeButton->currentText().compare("POP3")==0)
    {
        //QMessageBox::information(this,"info",serverTypeButton->currentText());
        pop3portedit->setText(tr("110"));
        pop3add->setText(tr("POP Server: "));
        pop3add->setGeometry(76,30,80,22);
    }
    else
    {
        pop3portedit->setText(tr("143"));
        pop3add->setText(tr("IMAP Server: "));
        pop3add->setGeometry(73,30,80,22);
    }
}

void account::createaccount()
{
    tip->show();
    tip->clear();
    tip->setText(tr("Verifying......"));
    e_mail_addressedit->setEnabled(false);
    pwdedit->setEnabled(false);
    pop3edit->setEnabled(false);
    pop3portedit->setEnabled(false);
    smtpedit->setEnabled(false);
    smtpportedit->setEnabled(false);

    QCoreApplication::processEvents();

    if(e_mail_addressedit->text().contains("@") && e_mail_addressedit->text().contains("."))
    {
        if(accountexit())
        {
            if(k==0)
            {
            judgeaccount();
            }
            checkaccount();
            emit createmail();
            emit mainwindowcase();
        }
        else{   tip->setText(tr("email account exists."));}
     }
    else
    {
        tip->setText(tr("email address error!"));
    }  

    if(tip->text().compare("Verifying......")!=0)
    {e_mail_addressedit->setEnabled(true);
    pwdedit->setEnabled(true);
    pop3edit->setEnabled(true);
    pop3portedit->setEnabled(true);
    smtpedit->setEnabled(true);
    smtpportedit->setEnabled(true);
    }
}

bool account::accountexit()
{
    QFile file("account.txt");

    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
       QString read;
       QStringList str;

       if(file.atEnd()){    return true;}
       while(!file.atEnd())
       {
            read=file.readLine();
            str=read.split(" ");
            if(e_mail_addressedit->text().compare(str.at(0))==0)
            {
                return false;
            }
            else{   return true;break;}
       }
     }
    file.close();

}

void account::checkaccount()
{
    socket=new QTcpSocket(this);
    int portint;
    bool ok1;
    ok1=true;
    portint=smtpportedit->text().toInt(&ok1,10);

    Smtp smtp(smtpedit->text(),portint,e_mail_addressedit->text(),pwdedit->text());

    if(smtp.connectToHost())
    {
        tip->setText(tr("connect!"));

        if(smtp.login()){
            tip->setText(tr("login success"));

            QDir qdir;
            QString dir=e_mail_addressedit->text();
            qdir.mkdir(dir);
            qdir.mkdir(dir+"/sent");
            qdir.mkdir(dir+"/draft");
            qdir.mkdir(dir+"/trash");
            qdir.mkdir(dir+"/Inbox");

            QFile file("account.txt");
            if(!file.open(QIODevice::WriteOnly|QIODevice::Append))
            {
                qDebug()<<"Open file failure!";
            }

                QTextStream txtOutput(&file);

                txtOutput<<e_mail_addressedit->text()<<" ";
                txtOutput<<pwdedit->text()<<" ";
                txtOutput<<e_mail_addressedit->text()<<" ";
                txtOutput<<serverTypeButton->currentText()<<" ";
                txtOutput<<pop3edit->text()<<" ";
                txtOutput<<pop3portedit->text()<<" ";
                txtOutput<<smtpedit->text()<<" ";
                txtOutput<<smtpportedit->text()<<endl;

                file.close();

                tip->setText("create successfully!");
        }
        else
        {
            tip->setText(tr("email address or password error!"));
        }
    }else
    {
        tip->setText(tr("connect failed"));
    }
}

void account::createButtoncase()
{
    if(!e_mail_addressedit->text().isEmpty())
    {
        ok->setEnabled(true);
    }else
    {
        ok->setEnabled(false);
    }
}

void account::judgeaccount()
{
    QString mailtype, pop, smtp, mailpop, mailsmtp;
    mailtype=e_mail_addressedit->text();
    QStringList str;
    str=mailtype.split("@");
    mailtype=str.at(1);
    mailpop = mailtype;
    mailsmtp = mailtype;
    pop =  mailpop.prepend("pop3.");
    smtp = mailsmtp.prepend("smtp.");
    str=mailtype.split(".");
    mailtype=str.at(0);
    qDebug()<<mailtype;

    serverTypeButton->setCurrentIndex(0);
    pop3edit->setText(pop);
    smtpedit->setText(smtp);

    if(mailtype.compare("gmail")==0)
    {
        serverTypeButton->setCurrentIndex(1);
        pop3edit->setText(tr("imap.gmail.com"));
        smtpedit->setText(tr("smtp.gmail.com"));
        smtpportedit->setText(tr("465"));
    }

}

void account::send(QString text)
{
    socket->write(text.toUtf8()+"\r\n");//向服务器发送信息
}

QString account::response()
{
    QString respon;
    if(!socket->waitForReadyRead(30000))
        qDebug("response failed");

    while(socket->canReadLine()){   //接受服务器发回的信息，用while循环是为了避免接受服务器发出的“ ”之类的无内容的回复
          respon=socket->readLine();
          if(respon[3] ==' ')//to avoid response code=" "
              break;
    }
          return respon;
}


void  account::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {

            move(event->globalPos() - dragPosition);

            event->accept();

        }

}

void  account::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {

            dragPosition = event->globalPos() - frameGeometry().topLeft();

            event->accept();

        }

}

