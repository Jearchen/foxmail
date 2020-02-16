#include "mainwindow.h"
#include <QDockWidget>
#include "QBitmap"
#include "QPalette"
#include <QDir>
#include <QMenu>
#include <QMouseEvent>
#include <QDebug>
#include "setting.h"
#include "compose.h"
#include "checkmail.h"
#include "account.h"
#include<QFileDialog>
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    resize(1092,640);
    setWindowFlags(Qt::FramelessWindowHint);

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
    QPixmap pixmap("://images/frame.png");
    QPalette palette;
    palette.setBrush(backgroundRole(), QBrush(pixmap));
    setPalette(palette);

    setupUi();
    setupActions();

    QFile file("account.txt");

    if(!file.exists())
    {
        QTextStream fs(&file);
        QString fileContent (fs.readAll());
        if(fileContent.isEmpty())
        {
            this->showAccountSetting();
        }
    }

    if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        qDebug()<<"Open file failure!";
    }
    else
    {
        QTextStream fs(&file);
        QString fileContent (fs.readAll());
        if(fileContent.isEmpty())
        {
            this->showAccountSetting();
        }
    }
    file.close();

    listaccount();
}
void MainWindow::setupActions()
{
    QToolButton *check = new QToolButton(this);
    check->setGeometry(10,30,120,35);
    check->setText("  Check");
    check->setIcon(QPixmap("://images/check.png"));
    check->setIconSize(QPixmap("://images/check.png").size());
    check->setAutoRaise(true);
    check->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    check->setFixedWidth(90);
    check->setFixedHeight(32);

    QToolButton *compose = new QToolButton(this);
    compose->setGeometry(100,30,120,35);
    compose->setText(" Compose");
    compose->setIcon(QPixmap("://images/compose.png"));
    compose->setIconSize(QPixmap("://images/compose.png").size());
    compose->setAutoRaise(true);
    compose->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    compose->setFixedWidth(110);
    compose->setFixedHeight(32);

    deleteletter = new QToolButton(this);
    deleteletter->setText("Delete");
    deleteletter->setGeometry(211,30,120,35);
    deleteletter->setIcon(QPixmap("://images/delete.png"));
    deleteletter->setIconSize(QPixmap("://images/delete.png").size());
    deleteletter->setAutoRaise(true);
    deleteletter->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    deleteletter->setFixedWidth(80);
    deleteletter->setFixedHeight(32);
    deleteletter->setEnabled(false);

    QLineEdit *search = new QLineEdit(this);    //搜索框
    search->setGeometry(700,38,300,20);
    //search->setStyleSheet("QLineEdit{border: 3px solid rgba(0,0,0,0);border-radius:8px}");
    search->setObjectName("search");
    search->hide();



    menu = new QToolButton(this);
    menu->setGeometry(1047,36,22,22);
    QPixmap pixmap4(":/images/menu.png");
    QPixmap fitpixmap4=pixmap4.scaled(256, 256).scaled(48,48,
    Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    menu->setIcon(fitpixmap4);
    menu->setIconSize(fitpixmap4.size());
    menu->setAutoRaise(true);
    menu->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    menu->setFixedWidth(28);
    menu->setFixedHeight(23);

    accounttable = new QTreeWidget(this);
    accounttable->setObjectName("accounttable");
    accounttable->setGeometry(5,66,180,569);
    accounttable->setHeaderHidden(true);
    accounttable->setUpdatesEnabled(true);

    mailtable = new QTreeWidget(this);
    mailtable->setGeometry(185,66,300,569);
    mailtable->setSortingEnabled(true);
    QStringList header;
    header<<"Sort By Date";
    mailtable->setHeaderLabels(header);

    //mailcontent = new QWebView(this);
    textraise = new QWidget(this);
    textraise->setGeometry(485,66,602,569);
    mailcontent = new QTextBrowser(textraise);//!!!!!!!!!!!!!!
    mailcontent->setGeometry(0,0,602,569);
    mailcontent->setObjectName("mailcontent");
    download = new QPushButton(textraise);
    download->setGeometry(500,525,80,40);
    download->setText("download");
    connect(download,SIGNAL(clicked()),this,SLOT(downloadfile()));

    connect(menu,SIGNAL(clicked()),this,SLOT(contextMenuButton()));
    connect(compose,SIGNAL(clicked()),this,SLOT(contextComposeButton()));
    connect(check,SIGNAL(clicked()),this,SLOT(contextCheckButton()));
    connect(accounttable, SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(showmail()));
    connect(mailtable,SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(showmailcontent(QTreeWidgetItem*,int)));
    connect(mailtable,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this,SLOT(replymail(QTreeWidgetItem*,int)));
    connect(deleteletter,SIGNAL(clicked()),this,SLOT(deletemail()));
    connect(this,SIGNAL(deletethings(QTreeWidgetItem*,int)),this,SLOT(showmail()));
}

void MainWindow::setupUi()
{
    QPushButton *close = new QPushButton(this);
    close->setGeometry(1068,6,15,15);
    close->setObjectName("close");
    connect(close,SIGNAL(clicked()),this,SLOT(close()));

  //  QPushButton *maximize = new QPushButton(this);
  // maximize->setGeometry(1040,6,15,15);
  //  maximize->setObjectName("maximize");
  //  connect(maximize,SIGNAL(clicked()),this,SLOT(showMaximized()));

    QPushButton *minimize = new QPushButton(this);
    minimize->setGeometry(1045,6,15,15);
    minimize->setObjectName("minimize");
    connect(minimize,SIGNAL(clicked()),this,SLOT(showMinimized()));

    QIcon icon = QIcon("://images/mail.png");
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(icon);
    QString tip=windowTitle();
    trayIcon->setToolTip(tip);
    createtupanActions();
    createTrayIcon();
    trayIcon->show();
    connect(trayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason){
    switch (reason) {
         case QSystemTrayIcon::Trigger:
         case QSystemTrayIcon::DoubleClick:
                this->show();
               break;
    default:
              break;
            }
}

void MainWindow::createTrayIcon(){
    trayIconMenu=new QMenu(this);
    //trayIconMenu->addAction(setappAction);
    trayIconMenu->addAction(quitAction);
    trayIcon->setContextMenu(trayIconMenu);
}

void MainWindow::createtupanActions(){
    quitAction = new QAction(tr("Exit"), this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void MainWindow::contextMenuButton()
{
    QMenu* contextMenu = new QMenu(this);
    QAction *setting=contextMenu->addAction("Account Setting");
    QAction *exit=contextMenu->addAction("Exit");

    connect(setting,SIGNAL(triggered(bool)),this,SLOT(settings()));
    connect(exit,SIGNAL(triggered(bool)),this,SLOT(close()));
    contextMenu->exec(menu->mapToGlobal(QPoint(0,25)));
    delete contextMenu;
}

void MainWindow::settings()
{
    setting *dialog=new setting(this);
    dialog->setGeometry(230,50,637,549);
    dialog->show();

    connect(dialog, SIGNAL(mainwindowcase()),this, SLOT(listaccount()));
}

void MainWindow::contextComposeButton()
{
    compose *dialog = new compose;
    dialog->show();
}

void MainWindow::contextCheckButton()
{
    if(accounttable->currentItem()->parent() != NULL)
    {
        checkmail *dialog = new checkmail(this);
        dialog->setWindowModality(Qt::ApplicationModal);
        dialog->show();
        dialog->connectpop(accounttable->currentItem()->parent()->text(0));
        connect(dialog, SIGNAL(finishcheck()),this,SLOT(showmail()));
    }
}

void MainWindow::showAccountSetting()
{
    account *dialog = new account(this);
    dialog->setGeometry(310,90,497,384);
    dialog->setWindowModality(Qt::ApplicationModal);
    QCoreApplication::processEvents();
    dialog->show();

    connect(dialog, SIGNAL(mainwindowcase()), this, SLOT(listaccount()));
}

void MainWindow::showmail()
{
    deleteletter->setEnabled(false);
    mailtable->clear();
    if(accounttable->currentItem()->parent() != NULL)
    {
        QString mailaddress = accounttable->currentItem()->parent()->text(0);
        QString box = accounttable->currentItem()->text(0);
        QString path;
        path=mailaddress+"/"+box;

        readtxt(path, box);

    }
    else{   return;}
}

void MainWindow::showmailcontent(QTreeWidgetItem *currentitem, int currentline)
{
        deleteletter->setEnabled(true);
        textraise->show();
        QString mailname = currentitem->text(0);
        QStringList str;
        str = mailname.split("\n");
        mailname = str.at(2);
        qDebug()<<mailname;
        QString mailaddress = accounttable->currentItem()->parent()->text(0);
        QString box = accounttable->currentItem()->text(0);
        QString path;
        path=mailaddress+"/"+box+"/"+mailname+".txt";
        //qDebug()<<path;

        //QString read, to, subject;
        QFile file(path);
        QTextCodec *code=QTextCodec::codecForName("UTF-8"); //!!!!!!!!!!!!!!!!!!!

        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream stream(&file);
            stream.setCodec(code);
           // mailcontent->setContent(0,stream.readAll(),"");
            mailcontent->clear();
            QString content=stream.readAll();

            if(!content.contains("FILENAME:")){
                if(!content.contains("YTTTESTHTML")){
                mailcontent->insertPlainText(content);
                }
                else{
                    content=content.remove("YTTTESTHTML");
                    mailcontent->insertHtml(content);
                }
                filename.clear();
                files.clear();
            }
            else{
                QString mail;
                mail=content.split("FILENAME: ").at(0);
                if(!mail.contains("YTTTESTHTML")){
                mailcontent->insertPlainText(mail);
                }
                else{
                    mail=mail.remove("YTTTESTHTML");
                    mailcontent->insertHtml(mail);
                }
                filename=content.split("FILENAME: ").at(1).split("FILE: ").at(0);
                files=content.split("FILENAME: ").at(1).split("FILE: ").at(1);
                /*if(downpath!=""){
                    QFile dfile(downpath+"/"+filename);
                    qDebug()<<downpath;
                    if(!dfile.open(QIODevice::WriteOnly))
                        qDebug()<<"open file";
                    dfile.write(file.toUtf8().fromBase64(file.toUtf8()));*/



                }


            }


        }



void MainWindow::replymail(QTreeWidgetItem *currentitem, int currentline)
{
    QString mailname = currentitem->text(0);
    QStringList str;
    str = mailname.split("\n");
    mailname = str.at(2);
    //qDebug()<<mailname;
    QString mailaddress = accounttable->currentItem()->parent()->text(0);
    QString box = accounttable->currentItem()->text(0);
    QString path;
    path=mailaddress+"/"+box+"/"+mailname+".txt";

    QString read, to, subject;
    QString content;
    bool attachexit;
    compose *dialog = new compose;
    QFile file(path);
    QTextCodec *code=QTextCodec::codecForName("UTF-8");//!!!!!!!!!!!!

    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream stream(&file);
        stream.setCodec(code);
        read=stream.readLine();
        str=read.split(": ");
        to = str.at(1);
        read=stream.readLine();
        str=read.split(": ");
        subject = str.at(1);
        read=stream.readLine();
        read=stream.readLine();
        str=read.split(": ");
        content = str.at(1);
        if(box.compare("Draft")==0||box.compare("Sent")==0)
        {
            while(!stream.atEnd())
            {
                 read=stream.readLine();

                if(read.split(". ").at(0).compare("ATTACHMENT")!=0){             content.append(read);  attachexit=true;}
                else{      dialog->attachlist->addItem(read.split(" ").at(2));}
             }
        }

    }
    file.close();

    dialog->show();
    if(box.compare("Inbox")==0)
        {
            dialog->Subjectedit->setText("Re:"+subject);
            QStringList str;
            str =to.split("<");
            to = str.at(1);
            str = to.split(">");
            to = str.at(0);
            dialog->Toedit->setText(to);
        }
        else{      dialog->Subjectedit->setText(subject);   dialog->Toedit->setText(to);}

    int row;
    row = accounttable->currentColumn();
    qDebug()<<row;
    dialog->from->setCurrentIndex(row);
    if(box.compare("Draft")==0){        dialog->letter->setText(content);}
    if(attachexit==true){    dialog->attachDialog->show();}


}

void MainWindow::readtxt(QString path, QString box)
{
    QDir *dir=new QDir(path);
    QStringList filter;
    filter<<"*.txt";
    dir->setNameFilters(filter);
    QList<QFileInfo> *fileInfo=new QList<QFileInfo>(dir->entryInfoList(filter));
    int i;
    QString read, to, subject, date;
    QStringList str;

    for(i=0;i<fileInfo->count();i++)
    {
        QFile file(fileInfo->at(i).filePath());
        bool attachexit=false;
        QTextCodec *code=QTextCodec::codecForName("UTF-8");//!!!!!!!!!!!!!!!!!!

        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
                QTextStream stream(&file);
                stream.setCodec(code);

                read=stream.readLine();
                str=read.split(": ");
                to = str.at(1);
                read=stream.readLine();
                str=read.split(": ");
                subject = str.at(1);
                read=stream.readLine();
                str=read.split(": ");
                date= str.at(1);
                qDebug()<<date;
                read=stream.readLine();
                while(!stream.atEnd())
                {
                    read=stream.readLine();
                    if(read.split(". ").at(0).compare("ATTACHMENT")==0){     attachexit = true;  break; }
                    if(read.contains("FILENAME")){	attachexit = true;  break;}
                }
         }
        file.close();

        QTreeWidgetItem *item=new QTreeWidgetItem(mailtable,QStringList(to+"\n"+subject+"\n"+date));
        if(attachexit){     item->setIcon(0,QIcon("://images/attach.png"));}
    }

}

void MainWindow::deletemail()
{
    QString mailname = mailtable->currentItem()->text(0);
    QStringList str;
    str = mailname.split("\n");
    mailname = str.at(2);
    QString mailaddress = accounttable->currentItem()->parent()->text(0);
    QString box = accounttable->currentItem()->text(0);
    QString path;
    path=mailaddress+"/"+box+"/"+mailname+".txt";

    if(box.compare("Trash")!=0)
    {
        QFile file(path);
        QTextCodec *code=QTextCodec::codecForName("UTF-8");
        QString content;
        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream stream(&file);
            stream.setCodec(code);
            content=stream.readAll();
        }
        file.close();

        QDir dir;
        dir.remove(path);

        path=mailaddress+"/trash/"+mailname+".txt";
        QFile afile(path);
        if(!afile.open(QIODevice::WriteOnly))
            qDebug("open file failed");

        QTextStream wrtstream(&afile);
        wrtstream<<content;
        afile.close();
    }
    else
    {
        QDir dir;
        dir.remove(path);
    }
    emit deletethings(accounttable->currentItem(), accounttable->currentColumn());
}

void MainWindow::listaccount()
{
    accounttable->clear();
    QFile file("account.txt");
    QString read;
    QStringList str;
    QTreeWidgetItem *accountitem[30];
    QTreeWidgetItem *child;
    QTreeWidgetItem *child1;
    QTreeWidgetItem *child2;
    QTreeWidgetItem *child3;
    QStringList columItemList,columItemList1,columItemList2,columItemList3;
    int i =0;
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
       while(!file.atEnd())
       {
            read=file.readLine();
            str=read.split(" ");
            accountitem[i] = new QTreeWidgetItem(accounttable, QStringList(str.at(2)));
            columItemList<<"Inbox";
            columItemList1<<"Draft";
            columItemList2<<"Sent";
            columItemList3<<"Trash";

            child  = new QTreeWidgetItem(columItemList);
            child1 = new QTreeWidgetItem(columItemList1);
            child2 = new QTreeWidgetItem(columItemList2);
            child3 = new QTreeWidgetItem(columItemList3);
            accountitem[i]->addChild(child);
            accountitem[i]->addChild(child1);
            accountitem[i]->addChild(child2);
            accountitem[i]->addChild(child3);

            i=i+1;
       }
    }
    file.close();
}

MainWindow::~MainWindow()
{

}

void  MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {

            move(event->globalPos() - dragPosition);

            event->accept();

        }

}

void  MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {

            dragPosition = event->globalPos() - frameGeometry().topLeft();

            event->accept();

        }

}

void MainWindow::closeEvent(QCloseEvent *event){
   // writeSettings();
    if (this->trayIcon->isVisible())
        {
          hide();
           event->ignore();
        }
}

void MainWindow::downloadfile()
{
    QByteArray str;
     //QString fileName(tr("ok.txt")) ;
     QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                      "/home",
                                                   QFileDialog::ShowDirsOnly
                                                      | QFileDialog::DontResolveSymlinks);

       // QDir d;
       // d.mkpath(dir);//可以不用，因为路径已经有了，就不用mk了
     QFile file(dir+"/"+filename);
         if(!file.open(QFile::WriteOnly))
             qDebug()<<"open failed";
         str=str.fromBase64(files.remove("\r\n").toUtf8());
         file.write(str);
         file.close();
}

