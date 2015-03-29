#include "replydialog.h"
#include "ui_replydialog.h"

ReplyDialog::ReplyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReplyDialog)
{
    ui->setupUi(this);
}

ReplyDialog::~ReplyDialog()
{
    delete ui;
}
