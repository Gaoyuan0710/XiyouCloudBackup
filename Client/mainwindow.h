#ifndef MIANWINDOW_H
#define MIANWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QListWidget>
#include <QStackedWidget>
#include <QHBoxLayout>
#include <QSplitter>

class MainWindow : public QMainWindow
{
    Q_OBJECT
    public:
        MainWindow(QMainWindow *parent = 0);

        QSplitter                      *mainsplitter;
    private:
        QLabel                         *Titlelabel;
        QListWidget                 *menulist;
        QStackedWidget         *stack;

//函数声明
    private:
        QHBoxLayout*  UserInfo();
        QWidget*      MyFile();
        QHBoxLayout*  MyFriend();
};

#endif
