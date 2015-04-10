
#ifndef FILETREEVIEW_H
#define FILETREEVIEW_H

#include<QtGui/QWidget>
#include<QTreeView>
#include<QStandardItemModel>
#include<QStandardItem>
#include<QMenu>
#include<QAction>
#include<QLabel>
#include<QLineEdit>
#include<QPushButton>
#include<QFileDialog>

class FileWidget : public QWidget
{
	Q_OBJECT
	public:
		FileWidget(QWidget *parent = 0);

		void CreatMenuAction();
		void CreatMenu();

		void AddFile(QFileInfo  fileinfo);

    private slots:
	    void MenuRequested(const QPoint &pos);	
        void ActionUpFileDialog();
		void ActionDownFileDialog();
		void LinePathShow(const QString &path);

	private:
	    QTreeView           *treeview;
		QStandardItemModel  *model;
		QStandardItem       *itemDocument;
		QStandardItem       *itemPicture;
		QStandardItem       *itemVideo;
		QStandardItem       *itemmusic;
		QStandardItem       *itemother;

		QFileInfo           fileinfo; //文件信息

		QMenu               *folderMenu;//目录菜单
		QMenu               *fileMenu;//文件菜单

		QAction             *uploadfile;//上传
		QAction             *downloadfile;//下载

		QFileDialog         *filedialog;
		QLabel              *savelabel;
		QLineEdit           *saveline;
		QPushButton         *savebutton;
};


#endif

