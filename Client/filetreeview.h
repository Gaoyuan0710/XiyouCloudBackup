
#ifndef FILETREEVIEW_H
#define FILETREEVIEW_H

#include<QtGui/QWidget>
#include<QTreeView>
#include<QStandardItemModel>
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
    private slots:
	    void MenuRequested(const QPoint &pos);	
        void ActionUpFileDialog();
		void ActionDownFileDialog();
		void LinePathShow(const QString &path);

	private:
	    QTreeView           *treeview;
		QStandardItemModel  *model;

		QMenu               *folderMenu;
		QMenu               *fileMenu;

		QAction             *uploadfile;
		QAction             *downloadfile;

		QFileDialog         *filedialog;
		QLabel              *savelabel;
		QLineEdit           *saveline;
		QPushButton         *savebutton;
};


#endif

