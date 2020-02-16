#ifndef SETTING_H
#define SETTING_H
#include<QWidget>
#include<QtGui>
#include<QListWidgetItem>
#include<QLabel>
class setting:public QWidget
{
    Q_OBJECT
public:
    setting(QWidget *parent=0);


private slots:
    void contextCreateButton();
    void changeShowAccount(QListWidgetItem* previous, QListWidgetItem* currentitem);
    void deleteaccountslot();
    void contextOkButton();
    void listaccount();

signals:
    void mainwindowcase();

private:
    QListWidget *accounttable;
    QLabel *hosttype;

    QLineEdit *e_mailedit;
    QLineEdit *pwedit;
    QLineEdit *nameedit;
    QLineEdit *pop3edit;
    QLineEdit *pop3portedit;
    QLineEdit *smtpedit;
    QLineEdit *smtpportedit;

    bool removeDirWithContent(const QString &dirName);      //delete file

    QPoint  dragPosition;
    void mouseMoveEvent(QMouseEvent *e);  //重定义鼠标事件
    void mousePressEvent(QMouseEvent *e);
};

#endif // SETTING_H
