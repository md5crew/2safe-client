#include "safeclient.h"
#include "safecommon.h"

SafeClient::SafeClient(QObject *parent) :
    QObject(parent),
    m_state(State::Offline),
    stopRequired(false)
{
}

void SafeClient::monitor()
{
    this->loop = new QEventLoop(this);
    this->sleep = new QEventLoop(this);

    this->timeout = new QTimer(this);
    this->timeout->setInterval(5000);
    this->timeout->setSingleShot(true);
    this->timeout->setTimerType(Qt::VeryCoarseTimer);

    this->sleepy = new QTimer(this);
    this->sleepy->setInterval(3000);
    this->sleepy->setSingleShot(true);
    this->sleepy->setTimerType(Qt::VeryCoarseTimer);

    connect(timeout, &QTimer::timeout, [&](){
        qDebug() << "Timed out connection";
        this->loop->exit();
    });

    connect(sleepy, &QTimer::timeout, [&](){
        this->sleep->exit();
    });

    auto client = new QLocalSocket(this);

    connect(client, &QLocalSocket::connected, [&](){
        this->timeout->stop();

        QTextStream stream(client);
        stream << "{\"type\": \"noop\"}";
        stream.flush();
    });

    connect(client, &QLocalSocket::readyRead, [&]() {
        QTextStream stream(client);
        QString info(stream.readAll());
        qDebug() << "Got from daemon:" << info;
        client->disconnectFromServer();
    });

    connect(client, &QLocalSocket::disconnected, [&](){
        qDebug() << "Socket disconnected";
        this->loop->exit();
    });

    qDebug()<<"Socket path: " << (QDir::homePath() + QDir::separator()
                                  + SAFE_DIR + QDir::separator() + SOCKET_FILE);
    while(true) {
        client->connectToServer(QDir::homePath() + QDir::separator()
                                + SAFE_DIR + QDir::separator() + SOCKET_FILE);
        this->timeout->start();
        if(this->stopRequired)
            goto required;

        this->loop->exec();
        this->sleepy->start();
        if(this->stopRequired)
            goto required;

        this->sleep->exec();

required:
        if(this->stopRequired) {
            emit quit();
            return;
        }
    }
}

void SafeClient::stop()
{
    this->stopRequired = true;
    this->timeout->stop();
    this->loop->exit();
    this->sleepy->stop();
    this->sleep->exit();
}
