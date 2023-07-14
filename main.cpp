#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "screenshotslist.h"
#include "screenshotsmodel.h"
#include "sqlitescreenshotsdb.h"
#include "controller.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<ScreenshotsModel>("Screenshots", 1, 0, "ScreenshotsModel");
    qmlRegisterUncreatableType<ScreenshotsList>("ScreenshotsList", 1, 0, "ScreenshotsList", "");

    QQmlApplicationEngine engine;

    Controller controller;
    engine.rootContext()->setContextProperty("controller", &controller);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
