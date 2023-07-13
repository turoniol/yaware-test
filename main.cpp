#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QScreen>
#include <QPixmap>
#include <QBuffer>
#include <QByteArray>
#include "screenshotsmodel.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<ScreenshotsModel>("Screenshots", 1, 0, "ScreenshotsModel");

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);
    engine.load(url);


//    if (QScreen *screen = QGuiApplication::primaryScreen()) {
//        auto pixmap = screen->grabWindow(0);

//        QByteArray bytes;
//        QBuffer buffer(&bytes);
//        buffer.open(QIODevice::WriteOnly);
//        pixmap.save(&buffer, "PNG");

    return app.exec();
}
