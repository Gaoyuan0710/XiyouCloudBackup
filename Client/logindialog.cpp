#include "logindialog.h"
#include "ui_logindialog.h"
#include "replydialog.h"
#include "settingdialog.h"

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    connect(ui->replyButton,SIGNAL(clicked()),this,SLOT(replyButton_clicked()));
    connect(ui->toolButton,SIGNAL(clicked()),this,SLOT(toolButton_clicked()));
    connect(ui->loginButton,SIGNAL(clicked()),this,SLOT(accept()));
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::loginButton_clicked()
{
    accept();
}

void LoginDialog::replyButton_clicked()
{
    ReplyDialog  *replydlg = new ReplyDialog(this);
    replydlg->show();
}

void LoginDialog::toolButton_clicked()
{
    SettingDialog  *settingdlg = new SettingDialog(this);
    settingdlg->show();
}
