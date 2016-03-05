#include "compose.h"
#include "smtp.h"
#include "tipdialog.h"

compose::compose(QWidget *parent):QWidget(parent,Qt::FramelessWindowHint)
{
    resize(1092,640);

    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(QPixmap("://images/frame.png")));
    this->setPalette(palette);

    QPushButton *close = new QPushButton(this);
    close->setGeometry(1067,6,15,15);
    close->setObjectName("close");
    connect(close,SIGNAL(clicked()),this,SLOT(close()));

    from = new QComboBox(this);
    from->setGeometry(850,6,150,20);
    from->setObjectName("from");
    QFile file("account.txt");
    QString read;
    QStringList str;
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
       while(!file.atEnd())
       {
            read=file.readLine();
            str=read.split(" ");
            from->addItem(str.at(0));
       }
    }
    file.close();

    setupUi();
    doletter();
}

void compose::setupUi()
{
    send = new QToolButton(this);
    send->setText("Send");
    send->setGeometry(10,30,122,35);
    send->setIcon(QPixmap("://images/send.png"));
    send->setIconSize(QPixmap("://images/send.png").size());
    send->setAutoRaise(true);
    send->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    send->setFixedWidth(80);
    send->setFixedHeight(32);

    save = new QToolButton(this);
    save->setText("Save");
    save->setGeometry(90,30,122,35);
    save->setIcon(QPixmap("://images/save.png"));
    save->setIconSize(QPixmap("://images/save.png").size());
    save->setAutoRaise(true);
    save->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    save->setFixedWidth(80);
    save->setFixedHeight(32);

    attach = new QToolButton(this);
    attach->setText("Insert  Attach");
    attach->setGeometry(170,30,122,35);
    attach->setIcon(QPixmap("://images/attach.png"));
    attach->setIconSize(QPixmap("://images/attach.png").size());
    attach->setAutoRaise(true);
    attach->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    attach->setFixedWidth(122);
    attach->setFixedHeight(32);

    hugeattach = new QToolButton(this);
    hugeattach->setText("Huge  Attach");
    hugeattach->setGeometry(290,30,122,35);
    hugeattach->setIcon(QPixmap("://images/hugeattach.png"));
    hugeattach->setIconSize(QPixmap("://images/hugeattach.png").size());
    hugeattach->setAutoRaise(true);
    hugeattach->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    hugeattach->setFixedWidth(122);
    hugeattach->setFixedHeight(32);
    hugeattach->hide();

    //password->setEchoMode(QLineEdit::Password);//使密码框显示为“....."而不是数字
    connect(send,SIGNAL(clicked()),this,SLOT(contextSendButton()));
    connect(attach,SIGNAL(clicked()),this,SLOT(contextAttachButton()));
    connect(save,SIGNAL(clicked()),this,SLOT(contextSaveButton()));
}

void compose::doletter()
{
    To=new QLabel(this);
    To->setText("To: ");
    To->setGeometry(45,90,22,22);
    Toedit=new QLineEdit(this);
    Toedit->setGeometry(70,90,1000,25);
    //Toedit->returnPressed();
    Cc=new QLabel(this);
    Cc->setText("Cc: ");
    Cc->setGeometry(45,122,22,22);
    Ccedit=new QLineEdit(this);
    Ccedit->setGeometry(70,122,1000,25);
    Bcc=new QLabel(this);
    Bcc->setText("Bcc: ");
    Bcc->setGeometry(41,154,22,22);
    Bccedit=new QLineEdit(this);
    Bccedit->setGeometry(70,154,1000,25);
    SubjectLabel=new QLabel(this);
    SubjectLabel->setText("Subject: ");
    SubjectLabel->setGeometry(21,186,60,22);
    Subjectedit=new QLineEdit(this);
    Subjectedit->setGeometry(70,186,1000,25);

    tool=new QLabel(this);
    tool->setGeometry(40,218,312,48);
    tool->setPixmap(QPixmap("://images/compose_edit_normal.png"));
    tool->hide();

    letter = new QTextEdit(this);
    letter->setGeometry(7,258,1080,377);

    attachDialog= new QWidget(this);
    attachDialog->setGeometry(567,495,520,140);
    attachDialog->setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(QPixmap("://images/attach_img.png")));
    attachDialog->setPalette(palette);
    attachlist = new QListWidget(attachDialog);
    attachlist->setGeometry(20,30,480,70);
    QLabel *attachtxt = new QLabel(attachDialog);
    attachtxt->setGeometry(20,5,100,20);
    attachtxt->setText("Attach");
    QPushButton *attachdelete  = new QPushButton(attachDialog);
    attachdelete->setGeometry(20,105,60,20);
    attachdelete->setText(tr("Remove"));
    attachDialog->hide();

    connect(attachdelete,SIGNAL(clicked()),this,SLOT(contextDeleteFile()));
}


void compose::contextSendButton()
{
    QString str=Toedit->text();
    rcpts=str.split(";");    //邮件接受者地址以";"分隔
    ccs=Ccedit->text().split(";");
    bccs=Bccedit->text().split(";");
    QStringList to=rcpts+ccs+bccs;

    //QString host=ui->host->text();
    //int port=25;
    QString host, password, sender;
    int port;
    QString user=from->currentText();
    QString portstr;
    QFile file("account.txt");

    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
       QString read;
       QStringList str;

       while(!file.atEnd())
       {
            read=file.readLine();
            str=read.split(" ");
            if(user.compare(str.at(0))==0)
            {
                sender=str.at(0);
                //Ccedit->setText(sender);
                password=str.at(1);
                //Bccedit->setText(password);
                host = str.at(6);
                //qDebug()<<host<<"host";
                portstr=str.at(7);
                //qDebug()<<portstr<<"port";
                break;
            }
       }
     }
    file.close();
    bool ok1;
    ok1=true;
    port=portstr.toInt(&ok1,10);
    //qDebug()<<port;
    //QString password=ui->password->text();
    //QString sender=Toedit->text();
    QString subject=Subjectedit->text();
    QString content=letter->toPlainText();         //接收ui中输入的内容

   MailContent email(sender,rcpts,ccs,bccs,subject,content);

    for (int i = 0; i < attachlist->count(); ++i)
    {
        email.addAttachment(new Attachment(new QFile(attachlist->item(i)->text())));
    }

    Smtp smtp(host,port,user,password,sender,to);
    smtp.connectToHost();
    smtp.login();

    tipDialog *tipdialog = new tipDialog(this);
    tipdialog->setWindowModality(Qt::ApplicationModal);
    tipdialog->setQString("Sending....");
    tipdialog->show();

    QCoreApplication::processEvents();

    if(smtp.sendMail(email))
    {
        tipdialog->setQString("Send Successfully");
        smtp.saveSendMail(email, "sent");
    }else
    {
        tipdialog->setQString("Send Failed");
        smtp.saveSendMail(email, "draft");
    }

    smtp.quit();


}

void compose::contextAttachButton()
{

    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFiles);
    if (dialog.exec())
    {
        attachlist->addItems(dialog.selectedFiles());
    }
    attachDialog->show();

    if(attachlist->count()==0)
    {
        attachDialog->hide();
    }
   // attachDialog->show();
}

void compose::contextDeleteFile()
{
    attachlist->takeItem(attachlist->currentRow());
    if(attachlist->count()==0)
    {
        attachDialog->hide();
    }
}

void compose::contextSaveButton()
{
    QString From=from->currentText();
    QString To=Toedit->text();
    QString Subject=Subjectedit->text();
    QString content=letter->toPlainText();
    QStringList attachments;
    for(int i=0;i< attachlist->count();++i){
        attachments.append(attachlist->item(i)->text());
    }

    QDateTime dt;
    QTime time;
    QDate date;
    dt.setTime(time.currentTime());
    dt.setDate(date.currentDate());
    QString currentDate = dt.toString("yyyy-MM-dd-hh-mm-ss");
    qDebug()<<currentDate;

    QFile file(From+"/draft/"+currentDate+".txt");
    if(!file.open(QIODevice::WriteOnly))
        qDebug("open file failed");

    file.write("RCPT TO: "+To.toUtf8()+"\r\n");
    file.write("SUBJECT: "+Subject.toLocal8Bit()+"\r\n");
    file.write("DATE: "+currentDate.toUtf8()+"\r\n");
    file.write("CONTENT: "+content.toLocal8Bit()+"\r\n");


    for(int i=0;i<attachments.count();i++){
        file.write("ATTACHMENT. "+QString::number(i,10).toUtf8()+". "+attachments.at(i).toUtf8()+"\r\n");
    }
    file.close();
}

void  compose::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {

            move(event->globalPos() - dragPosition);

            event->accept();

        }

}

void  compose::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {

            dragPosition = event->globalPos() - frameGeometry().topLeft();

            event->accept();

        }

}
