#ifndef MIANWINDOW_H
#define MIANWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QListWidget>
#include <QStackedWidget>
#include <QHBoxLayout>
#include <QSplitter>
#include "filetreeview.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
    public:
        MainWindow(QMainWindow *parent = 0);

        QSplitter                      *mainsplitter;
    private:
        QLabel                         *Titlelabel;
        QListWidget                    *menulist;
        QStackedWidget                 *stack;
		FileWidget                   *filewidget;

//函数声明
    public:
        QHBoxLayout*     UserInfo();
 //       QHBoxLayout*     MyFile();
        QHBoxLayout*     MyFriend();
};

#endif
