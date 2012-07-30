#include <QtGui>
#include <QtNetwork>
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


	void onTimerViewPost();
	void onTimerOnBoard();


    void PageAction_Login();
    void PageAction_GotoTargetBoard();  
    void PageAction_ViewPost();      
	void PageAction_Logout();

	void onLocalServerQuitHandler();

private:
    QWebView *view;
    QLineEdit *locationEdit;
    QAction *rotateAction;
    int progress;
    
    int cur_stage;
    QTimer *timer_viewpost;
    QTimer *timer_onboard;
    int surfCount;
    int getRandomInt(int scale,int offset = 0);
    QString userName;
    QString boardName;
    QString passWord;

private:
	QLocalServer *localServerQuit;
	QLocalSocket *clientConnection;
	void initLocalServers();
};
