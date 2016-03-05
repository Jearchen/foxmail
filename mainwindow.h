#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include<QMainWindow>
#include<QtGui>
#include<QList>
#include <QResizeEvent>
#include <QPainter>
#include <QPaintEvent>
#include <QTableWidget>
#include <QFile>
#include <QtWebKit/QWebFrame>
#include <QtWebKit/QWebView>

class MainWindow:public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void deletethings(QTreeWidgetItem*,int);

private slots:
    void contextMenuButton();
    void contextComposeButton();
    void contextCheckButton();
    void settings();
    void showmail();
    void showmailcontent(QTreeWidgetItem* currentitem, int currentline);
    void replymail(QTreeWidgetItem* currentitem, int currentline);
    void deletemail();

    void iconActivated(QSystemTrayIcon::ActivationReason reason);

protected slots:
    void listaccount();
    void downloadfile();

private:
    void setupUi();
    void setupActions();
    void showAccountSetting();
    void readtxt(QString path, QString box);

    QWidget *widget;
    QToolButton *menu;
    QTreeWidget *accounttable;
    QTreeWidget *mailtable;
   // QWebView *mailcontent;
    QTextBrowser *mailcontent;
    QToolButton *deleteletter;
    ///////////////
    /// \brief download
    QWidget *textraise;
    QPushButton *download;
    QLineEdit *path;
    //////////////

    QPoint  dragPosition;
    void mouseMoveEvent(QMouseEvent *e);  //重定义鼠标事件
    void mousePressEvent(QMouseEvent *e);

    void closeEvent(QCloseEvent *event);

    //退到托盘
    QSystemTrayIcon *trayIcon;
    void createtupanActions();
    void createTrayIcon();
    QAction *quitAction;
    QMenu *trayIconMenu;

    QString filename;
    QString files;

};

#endif // MAINWINDOW_H
