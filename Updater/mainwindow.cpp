#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setFixedSize(420,90);
    setWindowTitle("Void Clash Updater");

    _mainWidget = new QWidget(this);
    _mainWidget->setFixedSize(420, 90);

    _loadLocalVersion();
    _loadOnlineVersion();
}

void MainWindow::_loadLocalVersion()
{
    QString jsonStr;
    QFile updateFile;

    updateFile.setFileName("./update_log.txt");
    if (!updateFile.open(QIODevice::ReadOnly | QIODevice::Text))
        std::cerr << "update_log.txt is missing." << std::endl;

    jsonStr = updateFile.readAll();
    updateFile.close();

    QJsonDocument doc = QJsonDocument::fromJson(jsonStr.toUtf8());
    QJsonObject obj = doc.object();
    _version = obj.value("version").toString();

    _lVersionLabel = new QLabel("<strong>Local version :</strong>   " + _version, _mainWidget);
    _lVersionLabel->adjustSize();
    _lVersionLabel->move(10, 10);

    _playBtn = new QPushButton("Launch Void Clash Launcher", _mainWidget);
    _playBtn->resize(300, 50);
    _playBtn->move(65, 30);
    QObject::connect(_playBtn, SIGNAL(clicked()), this, SLOT(__launchLauncher()));
}

void MainWindow::_loadOnlineVersion()
{
    QString jsonStr;

//    QNetworkAccessManager manager;
//    _reply = manager.get(QNetworkRequest(QUrl("C:/online_log.txt")));
//    QEventLoop loop;

//    connect(_reply, SIGNAL(finished()), &loop, SLOT(quit()));
//    loop.exec();
//    _version = _reply->readAll();
//    std::cout << _version.toStdString() << std::endl;

    QFile updateFile;
    updateFile.setFileName("./online_log.txt");
    if (!updateFile.open(QIODevice::ReadOnly | QIODevice::Text))
        std::cerr << "online_log.txt is missing." << std::endl;

    jsonStr = updateFile.readAll();
    updateFile.close();

    QJsonDocument doc = QJsonDocument::fromJson(jsonStr.toUtf8());
    QJsonObject obj = doc.object();
    float localVersion = _version.toFloat();
    _version = obj.value("version").toString();
    float onlineVersion = _version.toFloat();


    _oVersionLabel = new QLabel("<strong>Official version :</strong>   " + _version, _mainWidget);
    _oVersionLabel->move(200, 10);

    if (localVersion < onlineVersion)
    {
        //delete _reply;
        _playBtn->setVisible(false);
        _oVersionLabel->setText("<strong style=\"color:red\">" +_oVersionLabel->text() + " -> need update</strong>");

        _updateBtn = new QPushButton("Update Void Clash", _mainWidget);
        _updateBtn->resize(300, 50);
        _updateBtn->move(65, 30);
        QObject::connect(_updateBtn, SIGNAL(clicked()), this, SLOT(__startUpdate()));
    }
    _oVersionLabel->adjustSize();
}


//PUBLIC SLOTS
void MainWindow::__launchLauncher()
{
    QProcess *process = new QProcess(this);
    process->start("./Windows/launcher");
    process->waitForStarted();
    process->waitForFinished();
    QApplication::quit();
}

void MainWindow::__startUpdate()
{
    _updateBtn->setVisible(false);


    _progressBar = new QProgressBar(_mainWidget);
    _progressBar->setValue(0);
    _progressBar->resize(300, 25);
    _progressBar->move(80, 40);
    _progressBar->show();

    QNetworkAccessManager manager;
    _reply = manager.get(QNetworkRequest(QUrl("http://justinbrottes.net/voidclash/gamemod.zip")));

    connect(_reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(__networkError(QNetworkReply::NetworkError)));
    connect(_reply, SIGNAL(finished()), this, SLOT(__updateFinished()));
    connect(_reply, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(__inProgress(qint64, qint64)));
}

void MainWindow::__inProgress(qint64 bReceived, qint64 bTotal)
{
    std::cout << "lol" << std::endl;
    if (bTotal != -1)
    {
        _progressBar->setRange(0, bTotal);
        _progressBar->setValue(bReceived);
    }
}

void MainWindow::__updateFinished()
{
    _reply->deleteLater();
    std::cout << "Update finished" << std::endl;
    _oVersionLabel->setText("<strong style=\"color:green\"><strong>Official version :   </strong>" +_version + " -> updated</strong>");
    _playBtn->setVisible(true);
}

void MainWindow::__networkError(QNetworkReply::NetworkError)
{
    QMessageBox::critical(this, "Erreur", "Erreur lors du chargement. Vérifiez votre connexion internet ou réessayez plus tard <br /><br /> Code de l'erreur : <br /><em>" + _reply->errorString() + "</em>");
        close();
}
