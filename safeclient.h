#ifndef SAFECLIENT_H
#define SAFECLIENT_H

#include <QObject>
#include <QLocalServer>
#include <QLocalSocket>
#include <QFileInfo>
#include <QThread>
#include <QEventLoop>
#include <QTimer>
#include <QDir>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QDateTime>

class SafeClient : public QObject
{
    Q_OBJECT
public:
    enum State {
        Offline,
        Unauthorized,
        Idle,
        Sync
    };

    explicit SafeClient(QObject *parent = 0);
    State state(){ return this->m_state; }
    bool auth;
    QString account;
    QString usage;
    QString action;
    void monitor();

private:
    bool stopRequired;
    State m_state;
    QEventLoop *loop;
    QEventLoop *sleep;
    QTimer *timeout;
    QTimer *sleepy;

    void setState(State s){
        if(s != this->m_state) {
            this->m_state = s;
            emit stateChanged();
        }
    }

signals:
    void updateIcon();
    void stateChanged();
    void showLogin();
    void showHome();
    void hideAll();
    void quit();


private slots:
    void handleMessage(const QJsonObject &obj);
    void sendCommand(const QJsonObject &obj);


public slots:
    void logOut();
    void changeDir(const QString &dir);
    void logIn(const QString &login, const QString &password);
    void chdir();
    void stop();

};

#endif // SAFECLIENT_H
