#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QSqlDatabase>
#include <QSqlQuery>

#include <QScreen>
#include <QPixmap>
#include <QBuffer>
#include <QByteArray>
#include <QDateTime>
#include <QSqlError>
#include <QSqlRecord>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);
    engine.load(url);

    QSqlDatabase m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName("./images.db");

    if (!m_db.open())
        qDebug() << "Error: connection with database failed";
    else
        qDebug() << "Database: connection ok";


    assert(QSqlQuery("create table if not exists screenshots ("
                     "   id int primary key, "
                     "   image_data blob not null, "
                     "   image_hash text not null, "
                     "   comparison_percent real not null, "
                     "   date text not null);")
               .exec());

    if (QScreen *screen = QGuiApplication::primaryScreen()) {
        auto pixmap = screen->grabWindow(0);

        QByteArray bytes;
        QBuffer buffer(&bytes);
        buffer.open(QIODevice::WriteOnly);
        pixmap.save(&buffer, "PNG");

        QSqlQuery q;
        q.prepare("insert into screenshots (image_data, image_hash, comparison_percent, date) "
                  "values (:image_data, 'imagehash', 0, :date);");
        q.bindValue(":image_data", bytes);
        QString currentTime = QDateTime::currentDateTime().toString(Qt::ISODate);
        q.bindValue(":date", currentTime);


        if (q.exec())
            qDebug() << "Insert: ok";
        else
            qDebug() << "Error while inserting" << q.lastError().text();
    }

    QSqlQuery q("select * from screenshots;");
    auto record = q.record();

    int dataCol = record.indexOf("image_data");

    while (q.next()) {
        QByteArray dataArr = q.value(dataCol).toByteArray();

        QPixmap pixmap;
        if (pixmap.loadFromData(dataArr)) {
            pixmap.save(filename);
        }
    }

    m_db.close();

    return app.exec();
}
