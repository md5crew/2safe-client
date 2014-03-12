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

    QQuickView view;
    QObject::connect(&view, SIGNAL(closing(QQuickCloseEvent*)), &cli, SLOT(stop()));
    view.engine()->rootContext()->setContextProperty("homepath", QDir::homePath());

    QObject::connect(&cli, &SafeClient::stateChanged, [&](){
        updateIcon(cli, &icon);
    });
    QObject::connect(&cli, &SafeClient::quit, &a, &QApplication::quit);

//    showLoginDialog(view);
//    showRootDirChooseDialog(view);
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
    icon->setAccount("");
    icon->setUsage("");
    icon->setAction("");
}

