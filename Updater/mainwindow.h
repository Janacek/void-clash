#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QtGui>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QCoreApplication>
#include <QUrl>
#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QProgressBar>
#include <QMessageBox>

#include <QJsonDocument>

#include <iostream>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);

protected:

    QWidget         *_mainWidget;

    QPushButton     *_playBtn;
    QPushButton     *_updateBtn;
    QLabel          *_lVersionLabel;
    QLabel          *_oVersionLabel;
    QString         _version;

    QNetworkReply   *_reply;
    QProgressBar    *_progressBar;
private:
    void _loadLocalVersion();
    void _loadOnlineVersion();

public slots:
    void __launchLauncher();
    void __startUpdate();
    void __inProgress(qint64, qint64);
    void __updateFinished();

    void __networkError(QNetworkReply::NetworkError);
};

#endif // MAINWINDOW_H
