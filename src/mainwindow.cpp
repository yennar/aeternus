#include <QtGui>
#include <QtNetwork>
#include <QtWebKit>
#include "mainwindow.h"

//! [1]

MainWindow::MainWindow(const QUrl& url,const QStringList& configString)
{
	  userName = configString.at(0);
    boardName = configString.at(1);
    passWord = configString.at(2);
    
    progress = 0;
		cur_stage = 0;
		timer_viewpost = new QTimer;
		surfCount = 10;
		
		connect(timer_viewpost, SIGNAL(timeout()), this , SLOT(onTimerViewPost()));

		timer_onboard = new QTimer;
		
		connect(timer_onboard, SIGNAL(timeout()), this , SLOT(onTimerOnBoard()));
//! [1]

    QNetworkProxyFactory::setUseSystemConfiguration(true);

	qsrand(QDateTime::currentDateTime().toTime_t());
//! [2]
    view = new QWebView(this);
    view->load(url);

    connect(view, SIGNAL(titleChanged(QString)), SLOT(adjustTitle()));
    connect(view, SIGNAL(loadProgress(int)), SLOT(setProgress(int)));
    connect(view, SIGNAL(loadFinished(bool)), SLOT(finishLoading(bool)));

    QToolBar *toolBar = addToolBar(tr("Navigation"));
    toolBar->addAction(view->pageAction(QWebPage::Back));
    toolBar->addAction(view->pageAction(QWebPage::Forward));
    toolBar->addAction(view->pageAction(QWebPage::Reload));
    toolBar->addAction(view->pageAction(QWebPage::Stop));

//! [2]

    setCentralWidget(view);
    setUnifiedTitleAndToolBarOnMac(true);
    this->resize(QSize(320,640));
	initLocalServers();
}

void MainWindow::adjustTitle()
{
    if (progress <= 0 || progress >= 100)
        setWindowTitle(view->title());
    else
        setWindowTitle(QString("%1 (%2%)").arg(view->title()).arg(progress));
}

void MainWindow::setProgress(int p)
{
    progress = p;
    adjustTitle();
}
//! [5]

//! [6]
void MainWindow::finishLoading(bool b)
{
	  if (!b) return;
    qDebug()<<"load finished "<<cur_stage;
    

    
    if (cur_stage == 0) //Login Page Finished
    {
    	surfCount = getRandomInt(50,40);
    	PageAction_Login();
    	cur_stage = 1;
    }
    else if (cur_stage == 1) //After Login Page Finished
    {
    	PageAction_GotoTargetBoard();
    	cur_stage = 2;
    }
    else if (cur_stage == 2) //After Board Page Finished
    {
    	if (surfCount-- == 0 ) 
    	{
    		 PageAction_Logout();
    		 cur_stage = 0;
    	}
    	else //Delay some time and execute PageAction_ViewPost();
    	{
			timer_viewpost->setSingleShot(true);
			timer_viewpost->start(getRandomInt(2,2)*1000);
    		cur_stage = 3;
      }
    }  
    else if (cur_stage == 3)
    {
    	//Delay some time and execute PageAction_GotoTargetBoard();
    	timer_onboard->setSingleShot(true);
        timer_onboard->start(getRandomInt(14,5)*1000);
    	cur_stage = 2;
    }
	else if (cur_stage == 10)
	{
		cur_stage = 11;
		PageAction_Logout();
	}
	else if (cur_stage == 11)
	{
		qDebug()<<"force quit";
		this->close();
	}
}

void MainWindow::onTimerViewPost()
{
	if (cur_stage == 10)
	{
		cur_stage = 11;
		PageAction_Logout();
		return;
	}
	PageAction_ViewPost();
}
//! [8]
void MainWindow::onTimerOnBoard()
{
	if (cur_stage == 10)
	{
		cur_stage = 11;
		PageAction_Logout();
		return; 
	}
	 PageAction_GotoTargetBoard();
}

void MainWindow::PageAction_GotoTargetBoard()
{
	view->load(QUrl("http://m.newsmth.net/board/" + boardName));
}


void MainWindow::PageAction_ViewPost()
{
	QStringList articleUrls;
	QWebElementCollection collection = view->page()->mainFrame()->documentElement().findAll("a");
	foreach (QWebElement e, collection) {
		//qDebug()<<e.attribute("href");
		QString s=e.attribute("href");
		if (s.startsWith("/article")) articleUrls.append(s);
	}
	int nCount = articleUrls.size();
	
	int nVal = 1;//getRandomInt(nCount);
	view->load(QUrl("http://m.newsmth.net" + articleUrls.at(nVal)));	
}

void MainWindow::PageAction_Login()
{
    //QString code = "wraper.m_main.sec.id.value=\"stangray\";";
    //view->page()->mainFrame()->evaluateJavaScript(code);
    QWebElement e_id = view->page()->mainFrame()->findFirstElement("input[name=\"id\"]");
    e_id.setAttribute ("value",userName);
    QWebElement p_id = view->page()->mainFrame()->findFirstElement("input[name=\"passwd\"]");
    p_id.setAttribute ("value",passWord);
    QWebElement f_id = view->page()->mainFrame()->findFirstElement("form[method=\"post\"]");
    f_id.setAttribute ("id","mainform");    
    QString code = "document.getElementById(\"mainform\").submit()";
    view->page()->mainFrame()->evaluateJavaScript(code);
}

void MainWindow::PageAction_Logout()
{
    view->load(QUrl("http://m.newsmth.net/user/logout"));
}




int MainWindow::getRandomInt(int scale, int offset)
{
	return (int)((qreal)(qrand ()) * scale /RAND_MAX ) + offset;
}

void MainWindow::initLocalServers()
{
	localServerQuit = new QLocalServer(this);
	QString localHandler = QString("IPC://%1/quit").arg(
		QCoreApplication::applicationPid()
		);
	if (!localServerQuit->listen(localHandler)) {
		close();
		return;
	}
	qDebug()<<"PID: "<<localHandler;
	connect(localServerQuit, SIGNAL(newConnection()), 
		this, SLOT(onLocalServerQuitHandler()));
}

void MainWindow::onLocalServerQuitHandler()
{
	qDebug()<<"onLocalServerQuitHandler";
	cur_stage=10;	
}
