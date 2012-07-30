
#include <QtGui>
#include "mainwindow.h"

int main(int argc, char * argv[])
{
    QApplication app(argc, argv);
    QUrl url;
    url = QUrl("http://m.newsmth.net");

    QString boardName = argv[1];
	QString userName = argv[2];
	
	QString passWord = argv[3];
	if (passWord.isEmpty())
	{
		passWord = QInputDialog::getText (NULL,
			QLatin1String("Input Password"),QLatin1String("Input Password"),			QLineEdit::Password);
	}
    MainWindow *browser = new MainWindow(url,QStringList()<<userName<<boardName<<passWord);
 
    browser->show();
	
    return app.exec();
}
