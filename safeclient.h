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
    void stateChanged();
    void quit();

public slots:
    void stop();

};

#endif // SAFECLIENT_H
