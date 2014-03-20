#include <QApplication>
#include <QSystemTrayIcon>
#include <QMessageBox>
#include <QQmlEngine>
#include <QQuickView>
#include <QQmlContext>
#include <QDir>

#include <safeclient.h>
#include <trayicon.h>
#include "qtquick2applicationviewer.h"

void updateIcon(const SafeClient& cli, TrayIcon *icon);
void showLoginDialog(QQuickView &view);
void showRootDirChooseDialog(QQuickView &view);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SafeClient cli;

    TrayIcon icon;
    QObject::connect(&icon, &TrayIcon::quit, &cli, &SafeClient::stop);
    QObject::connect(&icon, &TrayIcon::logout, &cli, &SafeClient::logOut);
    QObject::connect(&icon, &TrayIcon::chdir, &cli, &SafeClient::chdir);

    QQuickView view;
    QObject::connect(view.engine(), &QQmlEngine::quit, &view, &QQuickView::close);
    //QObject::connect(&view, SIGNAL(closing(QQuickCloseEvent*)), &cli, SLOT(stop()));
    view.engine()->rootContext()->setContextProperty("homepath", QDir::homePath());

    QObject::connect(&cli, &SafeClient::stateChanged, [&](){
        if (cli.state() == SafeClient::Idle) {
            icon.showMessage("2Safe", "Files have been synced");
        } else if (cli.state() == SafeClient::Sync) {
            icon.showMessage("2Safe", "Begin syncing files");
        } else if (cli.state() == SafeClient::Unauthorized) {
            icon.showMessage("2Safe", "Client deauthorized");
        } else {
            icon.showMessage("2Safe", "Daemon offline");
        }
    });

    QObject::connect(&cli, &SafeClient::updateIcon, [&](){
        updateIcon(cli, &icon);
    });
    QObject::connect(&cli, &SafeClient::showLogin, [&](){
        showLoginDialog(view);
    });
    QObject::connect(&cli, &SafeClient::showHome, [&](){
        showRootDirChooseDialog(view);
    });
    QObject::connect(&cli, &SafeClient::hideAll, [&](){
        view.hide();
    });
    QObject::connect(&cli, &SafeClient::quit, &a, &QApplication::quit);

    cli.monitor();
    return 0;
}

void showLoginDialog(QQuickView& view) {
    view.setTitle("2Safe Client");
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setSource(QUrl::fromLocalFile("qml/login.qml"));
    view.setMinimumWidth(540);
    view.setMinimumHeight(520);
    view.setMaximumWidth(800);
    view.setMaximumHeight(780);
    view.showNormal();
}

void showRootDirChooseDialog(QQuickView &view) {
    view.setTitle("2Safe Client");
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setSource(QUrl::fromLocalFile("qml/rootdir.qml"));
    view.setMinimumWidth(540);
    view.setMinimumHeight(380);
    view.setMaximumWidth(800);
    view.setMaximumHeight(540);
    view.showNormal();
}

void updateIcon(const SafeClient& cli, TrayIcon *icon) {
    icon->setAccount(cli.account);
    icon->setUsage(cli.usage);
    icon->setAction(cli.action);
}

