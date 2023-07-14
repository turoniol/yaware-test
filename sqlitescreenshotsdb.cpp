#include "sqlitescreenshotsdb.h"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QDebug>

SQLiteScreenshotsDb::SQLiteScreenshotsDb(const QString& dbPath)
{
    open(dbPath);

    assert(QSqlQuery("create table if not exists screenshots ("
                     "   id int primary key, "
                     "   image_data blob not null, "
                     "   image_hash text not null, "
                     "   comparison_percent real not null, "
                     "   date text not null);")
               .exec());
}

QVector<Screenshot> SQLiteScreenshotsDb::items() const
{
    QVector<Screenshot> result;

    QSqlQueryModel model;
    model.setQuery("select * from screenshots;");

    for (int i = 0; i < model.rowCount(); ++i)
        result << parse(model.record(i));

    return result;
}

void SQLiteScreenshotsDb::insertInternal(const Screenshot &item)
{
    QSqlQuery q;
    q.prepare("insert into screenshots (image_data, image_hash, comparison_percent, date) "
              "values (:image_data, :image_hash, :percentage, :date);");
    q.bindValue(":image_data", QString(QByteArray::fromBase64(item.hashData)));
    q.bindValue(":image_hash", QString(item.hashData));
    q.bindValue(":percentage", item.comparisonPercentage);
    q.bindValue(":date",       item.takenTime.toString(Qt::ISODate));


    if (!q.exec())
        qDebug() << q.lastError().text();
}


Screenshot SQLiteScreenshotsDb::parse(const QSqlRecord &record)
{
    Screenshot result{};

    result.hashData             = record.value("image_hash").toByteArray();
    result.comparisonPercentage = record.value("comparison_percent").toFloat();
    result.takenTime            = record.value("date").toDateTime();

    return result;
}

QString SQLiteScreenshotsDb::dbType() const
{
    return "QSQLITE";
}

