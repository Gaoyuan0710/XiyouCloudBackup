#include "logindialog.h"
#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	QTextCodec::setCodecForTr(QTextCodec::codecForLocale()); //设置编码
    MainWindow  client;
    LoginDialog  login;

    if(login.exec()==LoginDialog::Accepted)
    {
        client.mainsplitter->show();
        return a.exec();
    }
    else
        return 0;
}
