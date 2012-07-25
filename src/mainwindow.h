#include <QtGui>

class QWebView;
QT_BEGIN_NAMESPACE
class QLineEdit;
QT_END_NAMESPACE

//! [1]
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(const QUrl& url, const QStringList& configString);
		
protected slots:


    void adjustTitle();
    void setProgress(int p);
    void finishLoading(bool);



    void AutoLogin();
    void GotoTargetBoard();  
    void AutoViewPost();      
    void onTimeOut();
    void onTimeOut2();
		void LogOut();
		QStringList getArticleURL();

private:
    QWebView *view;
    QLineEdit *locationEdit;
    QAction *rotateAction;
    int progress;
    QStringList articleUrls;
    int cur_stage;
    QTimer *timer;
    QTimer *timer2;
    int surfCount;
    int getRandomInt(int scale,int offset = 0);
    QString userName;
    QString boardName;
    QString passWord;
//! [1]
};
