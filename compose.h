#ifndef COMPOSE_H
#define COMPOSE_H
#include<QWidget>
#include<QtGui>
#include<QDialog>


class compose:public QWidget
{
    Q_OBJECT
public:
    compose(QWidget *parent=0);
    void doletter();
    QLineEdit *Toedit;
    QLineEdit *Subjectedit;
    QTextEdit *letter;
    QListWidget *attachlist;
    QComboBox *from;
    QWidget *attachDialog;

private slots:
    void contextSendButton();
    void contextAttachButton();
    void contextDeleteFile();
    void contextSaveButton();


private:
    void setupUi();

    QLabel *To;

    QLabel *Cc;
    QLineEdit *Ccedit;
    QLabel *Bcc;
    QLineEdit *Bccedit;
    QLabel *SubjectLabel;
    QLabel *tool;   //换字体什么的工具

    QStringList rcpts;      //mail content
    QStringList ccs;
    QStringList bccs;

    QToolButton *send;
    QToolButton *attach;
    QToolButton *hugeattach;
    QToolButton *save;

    QPoint  dragPosition;
    void mouseMoveEvent(QMouseEvent *e);  //重定义鼠标事件
    void mousePressEvent(QMouseEvent *e);

};

#endif // COMPOSE_H
