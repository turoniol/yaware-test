#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "screenshotslist.h"
#include "screenshotsmodel.h"
#include "sqlitescreenshotsdb.h"

#include <QSqlQuery>
#include <QBuffer>
#include <QPixmap>
#include <QScreen>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<ScreenshotsModel>("Screenshots", 1, 0, "ScreenshotsModel");
    qmlRegisterUncreatableType<ScreenshotsList>("ScreenshotsList", 1, 0, "ScreenshotsList",
                                                 "");

    QQmlApplicationEngine engine;

    ScreenshotsList screenshotsList;

    SQLiteScreenshotsDb db("./images.db");

    auto items = db.items();

    qSort(items.begin(), items.end(),
          [](const auto &left, const auto &right) {
              return left.takenTime > right.takenTime;
    });

    for (Screenshot item : items)
        screenshotsList.append(item);

    engine.rootContext()->setContextProperty("screenshotsList", &screenshotsList);

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

//        Screenshot screenshot{};

//        screenshot.comparisonPercentage = 0.0;
//        screenshot.hashData = bytes.toBase64();
//        screenshot.takenTime = QDateTime::currentDateTime();

//        db.insert(screenshot);
//    }

    return app.exec();
}
