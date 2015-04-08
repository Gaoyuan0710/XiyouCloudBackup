#include"mainwindow.h"

#include<QHBoxLayout>
#include<QPushButton>
#include<QLineEdit>
#include<QComboBox>
#include<QRadioButton>
#include<QStackedWidget>
#include<QToolBox>
#include<QToolButton>
#include<QGroupBox>

MainWindow::MainWindow(QMainWindow *parent)
    : QMainWindow(parent)
{
    Titlelabel = new QLabel(tr("云备份"));

//	QLineEdit *lineEdit1 = new QLineEdit;
//	QLineEdit *lineEdit2 = new QLineEdit;

    menulist = new QListWidget;
    menulist->addItem(tr("个人资料"));
    menulist->addItem(tr("我的文件"));
    menulist->addItem(tr("我的好友"));
    menulist->addItem(tr("上传"));
    menulist->addItem(tr("数据监控"));
    menulist->addItem(tr("收到推荐"));
    menulist->addItem(tr("记录"));
    new QListWidgetItem(tr("设置"),menulist);

    QWidget   *firstpage = new QWidget;
//	QWidget   *secondpage= new QWidget;
    QWidget   *threepage = new QWidget;

    stack = new QStackedWidget(this);

    firstpage->setLayout(UserInfo());
    stack->addWidget(firstpage);
//    secondpage->setLayout(MyFile());
    filewidget = new FileWidget;
	stack->addWidget(filewidget);
    threepage->setLayout(MyFriend());
    stack->addWidget(threepage);
    stack->addWidget(new QLabel(tr("上传")));
    stack->addWidget(new QLabel(tr("数据监控")));
    stack->addWidget(new QLabel(tr("收到推荐")));
//  　stack->addWidget(new QLabel(tr("记录")));
//　　stack->addWidget(new QLabel(tr("设置")));

    connect(menulist,SIGNAL(currentRowChanged(int)),stack,SLOT(setCurrentIndex(int)));
    menulist->setCurrentRow(0);

//下部窗口
    QSplitter   *basesplitter = new QSplitter(Qt::Horizontal);
    basesplitter->addWidget(menulist);
    basesplitter->addWidget(stack);
    basesplitter->setStretchFactor(0,1);
    basesplitter->setStretchFactor(1,4);

//主窗口
    mainsplitter = new QSplitter(Qt::Vertical);
    mainsplitter->addWidget(Titlelabel);
    Titlelabel->setAlignment(Qt::AlignCenter);//标签字体居中
    mainsplitter->addWidget(basesplitter);
    mainsplitter->setStretchFactor(1,1);//设置顶部标签不变化

//设置窗口初始大小
    mainsplitter->resize(QSize(800,600));

}

QHBoxLayout*  MainWindow::UserInfo()
{
    QLabel     *infoTitle = new QLabel(tr("个人资料"));
    QLabel     *infoname  = new QLabel(tr("账号："));
    QLabel     *infosex   = new QLabel(tr("性别："));
    QLabel     *infobirth = new QLabel(tr("生日："));

    QLineEdit  *infonameLEdit = new QLineEdit;
    QRadioButton *infoman = new QRadioButton(tr("男"));
    QRadioButton *infowoman = new QRadioButton(tr("女"));
    QComboBox  *yearcbox = new QComboBox();
    yearcbox->addItem(tr("年"));
    QComboBox  *monthcbox = new QComboBox();
    monthcbox->addItem(tr("月"));
    QComboBox  *datacbox = new QComboBox();
    datacbox->addItem(tr("日"));

    QPushButton *infocancel = new QPushButton(tr("撤销"));
    QPushButton *infosave = new QPushButton(tr("保存"));

    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->addWidget(infoTitle,0,2);
    gridLayout->addWidget(infoname,1,0);
    gridLayout->addWidget(infonameLEdit,1,1,1,2);
    gridLayout->addWidget(infosex,2,0);
    gridLayout->addWidget(infoman,2,1,1,2);
    gridLayout->addWidget(infowoman,2,3,1,2);
    gridLayout->addWidget(infobirth,3,0);
    gridLayout->addWidget(yearcbox,3,1);
    gridLayout->addWidget(monthcbox,3,2);
    gridLayout->addWidget(datacbox,3,3);
    gridLayout->addWidget(infocancel,4,1);
    gridLayout->addWidget(infosave,4,3);

    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addLayout(gridLayout);
    leftLayout->addStretch();

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(leftLayout);
    mainLayout->addStretch();

    return mainLayout;
}

/*我的文件窗口*/
/*
QHBoxLayout  *MainWindow::MyFile()
{
	filewidget = new FileWidget;
	QHBoxLayout *mainLayout = new QHBoxLayout;
	mainLayout->addWidget(filewidget);
    return mainLayout;
}
*/

QHBoxLayout*  MainWindow::MyFriend()
{
    QToolBox    *toolbox = new QToolBox(this);
    QToolButton *friend1 = new QToolButton;
    friend1->setText(tr("friend1"));
    friend1->setAutoRaise(TRUE);
    QToolButton *friend2 = new QToolButton;
    friend2->setText(tr("friend2"));
    friend2->setAutoRaise(TRUE);
    QToolButton *friend3 = new QToolButton;
    friend3->setText(tr("friend3"));
    friend3->setAutoRaise(TRUE);
    QPushButton *addfriend = new QPushButton(tr("添加好友"));
    QPushButton *friendstate = new QPushButton(tr("好友动态"));

    QGroupBox *groupBox = new QGroupBox;
    QVBoxLayout *layout = new QVBoxLayout(groupBox);
    layout->setMargin(10);
    layout->setAlignment(Qt::AlignHCenter);
    layout->setAlignment(Qt::AlignTop);
    layout->addWidget(friend1);
    layout->addWidget(friend2);
    layout->addWidget(friend3);

    toolbox->addItem((QWidget*)groupBox,tr("我的好友"));

    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addWidget(toolbox);
    leftLayout->addWidget(addfriend);
    leftLayout->addWidget(friendstate);
    leftLayout->addStretch();

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(leftLayout);
    mainLayout->addStretch();
    mainLayout->setStretch(1,1);

    return mainLayout;
}
