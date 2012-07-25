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
		timer = new QTimer;
		surfCount = 10;
		
		connect(timer, SIGNAL(timeout()), this , SLOT(onTimeOut()));

		timer2 = new QTimer;
		
		connect(timer2, SIGNAL(timeout()), this , SLOT(onTimeOut2()));
//! [1]

    QNetworkProxyFactory::setUseSystemConfiguration(true);

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
    

    
    if (cur_stage == 0)
    {
    	surfCount = getRandomInt(14,10);
    	AutoLogin();
    	//GotoTargetBoard();
    	cur_stage = 1;
    }
    else if (cur_stage == 1)
    {
    	GotoTargetBoard();
    	cur_stage = 2;
    }
    else if (cur_stage == 2)
    {
    	if (surfCount-- == 0 ) 
    	{
    		 LogOut();
    		 cur_stage = 0;
    	}
    	else
    	{
    		articleUrls = getArticleURL();
    		AutoViewPost();
    		cur_stage = 3;
      }
    }  
    else if (cur_stage == 3)
    {
    	
    	timer2->setSingleShot(true);
      timer2->start(getRandomInt(14,5)*1000);
    	cur_stage = 2;
    }      
}
//! [6]

//! [7]
void MainWindow::GotoTargetBoard()
{
    view->load(QUrl("http://m.newsmth.net/board/" + boardName));
}
//! [7]

//! [8]
void MainWindow::onTimeOut()
{
	 int nCount = articleUrls.size();
	 qsrand(QDateTime::currentMSecsSinceEpoch());
	 int nVal = getRandomInt(nCount);
	 view->load(QUrl("http://m.newsmth.net" + articleUrls.at(nVal)));
}
//! [8]
void MainWindow::onTimeOut2()
{
	 GotoTargetBoard();
}

//! [9]
void MainWindow::AutoViewPost()
{
	  timer->setSingleShot(true);
    timer->start(getRandomInt(2,2)*1000);
}

void MainWindow::AutoLogin()
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

void MainWindow::LogOut()
{
    view->load(QUrl("http://m.newsmth.net/user/logout"));
}

QStringList MainWindow::getArticleURL()
{
	QStringList r;
	QWebElementCollection collection = view->page()->mainFrame()->documentElement().findAll("a");
	foreach (QWebElement e, collection) {
    //qDebug()<<e.attribute("href");
    QString s=e.attribute("href");
    if (s.startsWith("/article")) r.append(s);
  }
  return r;
}

int MainWindow::getRandomInt(int scale, int offset)
{
	return (int)((qreal)(qrand ()) * scale /RAND_MAX ) + offset;
}

