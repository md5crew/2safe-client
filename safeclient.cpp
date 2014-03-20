#include "safeclient.h"
#include "safecommon.h"

SafeClient::SafeClient(QObject *parent) :
    QObject(parent),
    auth(false),
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
        this->setState(Offline);
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
        QString data(stream.readAll());

        /* JSON parsing */
        QJsonParseError jsonError;
        QJsonDocument jsonMessage = QJsonDocument::fromJson(data.toLatin1(), &jsonError);
        if (jsonError.error) {
            qWarning() << "JSON error:" << jsonError.errorString();
            return;
        } else if (!jsonMessage.isObject()) {
            qWarning() << "Not an object:" << jsonMessage;
            return;
        }

        if(this->state() == Offline) {
            this->setState(Unauthorized);
        }

        QJsonObject message = jsonMessage.object();
        QString type = message["type"].toString();
        if(type == "queue") {
            QJsonArray messages = message["messages"].toArray();
            foreach(QJsonValue v, messages) {
                handleMessage(v.toObject());
            }
        }

        client->disconnectFromServer();
    });

    connect(client, &QLocalSocket::disconnected, [&](){
        this->loop->exit();
    });

    qDebug() << "Socket path: " << (QDir::homePath() + QDir::separator()
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

void SafeClient::logOut()
{
    /*
    QJsonObject obj;
    obj.insert("type", QString("action"));
    obj.insert("verb", QString("logout"));
    sendCommand(obj);
    */
    showLogin();
}

void SafeClient::changeDir(const QString &dir)
{
    QJsonObject obj;
    QJsonObject args;
    args.insert("dir", QString(dir));
    obj.insert("type", QString("action"));
    obj.insert("verb", QString("chdir"));
    obj.insert("args", args);
    sendCommand(obj);
}

void SafeClient::logIn(const QString &login, const QString &password)
{
    QJsonObject obj;
    QJsonObject args;
    args.insert("login", QString(login));
    args.insert("password", QString(password));
    obj.insert("type", QString("action"));
    obj.insert("verb", QString("auth"));
    obj.insert("args", args);
    sendCommand(obj);
}

void SafeClient::handleMessage(const QJsonObject &obj)
{
    QString type = obj["type"].toString();
    if(type == "event") {
        QString category = obj["category"].toString();
        QJsonObject values = obj["values"].toObject();
        if(category == "disk_quota") {
            double used = values["used_bytes"].toDouble();
            double total = values["total_bytes"].toDouble();
            this->usage = "Used " + QString::number(used / MB, 'f', 1)
                    + "MB of " + QString::number(total / MB, 'f', 1) + "MB";
            emit updateIcon();
        } else if (category == "sync") {
            ulong count = (quint64)values["count"].toDouble();
            uint timestamp = (uint)values["timestamp"].toDouble();

            if(count > 0) {
                this->action = "Syncing " + QString::number(count) + (count > 1 ? " objects" : " object");
                this->updateIcon();
                this->setState(Sync);
            } else {
                this->action = "Last sync time: " + QDateTime::fromTime_t(timestamp).toString();
                this->updateIcon();
                this->setState(Idle);
            }
        } else if (category == "auth") {
            bool auth = values["authorized"].isBool();
            QString login = values["login"].toString();
            if(auth) {
                this->auth = true;
                this->account = login;
                this->updateIcon();
                if(this->state() == Offline ||
                        this->state() == Unauthorized) {
                    this->setState(Idle);
                }
            } else {
                this->auth = false;
                this->account = "Unauthorized";
                this->updateIcon();
                this->setState(Unauthorized);
            }
        } else {
            qDebug() << "Daemon sent event of unknwon type" << obj;
        }
    }
}

void SafeClient::sendCommand(const QJsonObject &obj)
{
    QEventLoop loop;
    QTimer timeout;
    timeout.setInterval(5000);
    timeout.setSingleShot(true);
    timeout.setTimerType(Qt::VeryCoarseTimer);

    connect(&timeout, &QTimer::timeout, [&](){
        qDebug() << "Timed out connection (sendCommand)";
        loop.exit();
    });

    auto client = new QLocalSocket(this);
    connect(client, &QLocalSocket::connected, [&](){
        this->timeout->stop();
        QTextStream stream(client);
        stream << QJsonDocument(obj).toJson();
        stream.flush();
    });
    connect(client, &QLocalSocket::disconnected, [&](){
        loop.exit();
    });
    client->connectToServer(QDir::homePath() + QDir::separator()
                            + SAFE_DIR + QDir::separator() + SOCKET_FILE);
    loop.exec();
}

void SafeClient::chdir()
{
    showHome();
}

void SafeClient::stop()
{
    this->stopRequired = true;
    this->timeout->stop();
    this->loop->exit();
    this->sleepy->stop();
    this->sleep->exit();
}
