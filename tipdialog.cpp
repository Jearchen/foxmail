#include "tipdialog.h"

tipDialog::tipDialog(QWidget *parent) : QDialog(parent,Qt::FramelessWindowHint)
{
    resize(460,189);

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

    tiplabel = new QLabel(this);
    tiplabel->setGeometry(180,80,120,50);
}

void tipDialog::setQString(QString tip)
{
    tiplabel->setText(tip);
}

void  tipDialog::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {

            move(event->globalPos() - dragPosition);

            event->accept();

        }

}

void  tipDialog::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {

            dragPosition = event->globalPos() - frameGeometry().topLeft();

            event->accept();

        }

}
