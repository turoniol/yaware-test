#include "sqlitescreenshotsdb.h"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlQueryModel>

SQLiteScreenshotsDb::SQLiteScreenshotsDb(SQLiteScreenshotsDb(const QString& dbPath))
    : IScreenshotDb(dbPath)
{
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
    q.bindValue(":image_data", item.rawData);
    q.bindValue(":image_hash", item.rawData);
    q.bindValue(":percentage", item.comparisonPercentage);
    q.bindValue(":date", item.takenTime.toString(Qt::ISODate));


    if (q.exec())
        qDebug() << "The screenshot successfully inserted.";
    else
        qDebug() << "Failed during inserting the screenshot.";
}


Screenshot SQLiteScreenshotsDb::parse(const QSqlRecord &record)
{
    Screenshot result{};

    result.rawData = record.value("image_data").toByteArray();
    result.comparisonPercentage = record.value("comparison_percent").toFloat();
    result.takenTime = record.value("date").toDateTime();

    return result;
}

QString SQLiteScreenshotsDb::dbType() const
{
    return "QSQLITE";
}

