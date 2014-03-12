#ifndef TRAYICON_H
#define TRAYICON_H

#include <QObject>
#include <QDebug>
#include <QSystemTrayIcon>
#include <QMenu>

class TrayIcon : public QObject
{
    Q_OBJECT
public:
    explicit TrayIcon(QObject *parent = 0);

private:
    QSystemTrayIcon *icon;

    QString m_account;
    QString m_usage;
    QString m_action;

    QMenu *makeMenu();
    QHash<QString, QAction *> initActions(QList<QString> labels);

signals:
    void quit();

public slots:
    void showMessage(QString title, QString text);
    void setAccount(QString a);
    void setUsage(QString u);
    void setAction(QString a);

};

#endif // TRAYICON_H
