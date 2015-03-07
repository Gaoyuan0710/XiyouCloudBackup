//注册界面

#ifndef REPLYDIALOG_H
#define REPLYDIALOG_H

#include <QDialog>

namespace Ui {
class ReplyDialog;
}

class ReplyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReplyDialog(QWidget *parent = 0);
    ~ReplyDialog();

private:
    Ui::ReplyDialog *ui;
};

#endif // REPLYDIALOG_H
