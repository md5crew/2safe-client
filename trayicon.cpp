#include "trayicon.h"

TrayIcon::TrayIcon(QObject *parent) :
    QObject(parent)
{
    this->icon = new QSystemTrayIcon(this);
    icon->setContextMenu(this->makeMenu());
    icon->setIcon(QIcon(":/icons/2safe@22x22.png"));
    icon->show();
}

void TrayIcon::setAccount(QString a)
{
    this->m_account = a;
    QAction *action = icon->contextMenu()->actions().at(0);
    if(a.isEmpty()) {
        action->setText("Unauthorized");
        //action->setVisible(false);
    } else {
        action->setText(a);
        //action->setVisible(true);
    }
}

void TrayIcon::setUsage(QString u)
{
    this->m_usage = u;
    QAction *action = icon->contextMenu()->actions().at(1);
    if(u.isEmpty()) {
        action->setText("No info");
        action->setVisible(false);
    } else {
        action->setText(u);
        action->setVisible(true);
    }
}

void TrayIcon::setAction(QString a)
{
    this->m_action = a;
    QAction *action = icon->contextMenu()->actions().at(2);
    if(a.isEmpty()) {
        action->setText("No info");
        action->setVisible(false);
    } else {
        action->setText(a);
        action->setVisible(true);
    }
}

QMenu* TrayIcon::makeMenu()
{
    QMenu *menu = new QMenu();
    QAction *a;

    a = new QAction("Unauthorized", menu);
    a->setEnabled(false);
    menu->addAction(a);
    a = new QAction("No info", menu);
    a->setEnabled(false);
    a->setVisible(false);
    menu->addAction(a);
    a = new QAction("No info", menu);
    a->setEnabled(false);
    a->setVisible(false);
    menu->addAction(a);
    menu->addSeparator();


   // a = new QAction("Open folder", menu);
   // menu->addAction(a);

    a = new QAction("Open website", menu);
    connect(a, &QAction::triggered, [](){
        QDesktopServices::openUrl(QUrl("https://www.2safe.com/web/"));
    });
    menu->addAction(a);

    menu->addSeparator();
    a = new QAction("Switch user", menu);
    connect(a, &QAction::triggered, [&](){
        emit logout();
    });

    menu->addAction(a);
    a = new QAction("Change folder", menu);
    connect(a, &QAction::triggered, [&](){
        emit chdir();
    });
    menu->addAction(a);
    menu->addSeparator();

    a = new QAction("Exit", menu);
    connect(a, &QAction::triggered, [&](){
        emit quit();
    });
    menu->addAction(a);

    return menu;
}

QHash<QString, QAction *> TrayIcon::initActions(QList<QString> labels)
{
    QHash<QString, QAction*> hash;
    foreach (auto l, labels) {
        hash.insert(l, new QAction(l, this));
    }
    return hash;
}

void TrayIcon::showMessage(QString title, QString text)
{
    icon->showMessage(title, text, QSystemTrayIcon::Information, 2000);
}
