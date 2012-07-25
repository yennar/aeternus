
#include <QtGui>
#include "mainwindow.h"

int main(int argc, char * argv[])
{
    QApplication app(argc, argv);
    QUrl url;
    url = QUrl("http://m.newsmth.net");

    QString boardName = argv[1];
		QString userName = argv[2];
		QString passWord = QInputDialog::getText (NULL,
			QLatin1String("Input Password"),QLatin1String("Input Password"),QLineEdit::Password);

    MainWindow *browser = new MainWindow(url,QStringList()<<userName<<boardName<<passWord);
    #if defined Q_OS_SYMBIAN || defined Q_WS_HILDON || defined Q_WS_MAEMO_5 || defined Q_WS_SIMULATOR
        browser->showMaximized();
    #else
        browser->show();
    #endif
    return app.exec();
}
