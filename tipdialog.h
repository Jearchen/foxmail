#ifndef TIPDIALOG_H
#define TIPDIALOG_H

#include <QDialog>
#include <QtGui>
#include <QLabel>
class tipDialog : public QDialog
{
    Q_OBJECT
public:
    explicit tipDialog(QWidget *parent = 0);
    void setQString(QString tip);
    
signals:
    
public slots:

private:
    QPoint  dragPosition;
    void mouseMoveEvent(QMouseEvent *e);  //重定义鼠标事件
    void mousePressEvent(QMouseEvent *e);

     QLabel *tiplabel;
    
};

#endif // TIPDIALOG_H
